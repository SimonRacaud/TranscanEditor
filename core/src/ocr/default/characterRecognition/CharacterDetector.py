#cython: language_level=3
# -*- coding: utf-8 -*-
import time
from collections import OrderedDict

from numpy import ndarray
import numpy as np
from torch import nn, load, from_numpy, no_grad
import torch.backends.cudnn as cudnn
from torch.autograd import Variable
import cv2

import src.ocr.default.characterRecognition.craft.craft_utils as craft_utils
import src.ocr.default.characterRecognition.craft.imgproc as imgproc

from src.ocr.default.characterRecognition.craft.craft import CRAFT
from src.ocr.default.characterRecognition.craft.refinenet import RefineNet

from src.model.model import CraftConfig

class CharacterDetector:

    @classmethod
    def setup(cls, args: CraftConfig):
         # load net
        cls.net = CRAFT()     # initialize

        print('Loading weights from checkpoint (' + args.trained_model + ')')
        if args.cuda:
            cls.net.load_state_dict(cls.__copyStateDict(load(args.trained_model)))
        else:
            cls.net.load_state_dict(cls.__copyStateDict(load(args.trained_model, map_location='cpu')))

        if args.cuda:
            cls.net = cls.net.cuda()
            cls.net = nn.DataParallel(cls.net)
            cudnn.benchmark = False

        cls.net.eval() # Enable eval mode

        # LinkRefiner : fusion between boxes
        cls.refine_net = None
        if args.refine:
            cls.refine_net = RefineNet()
            print('Loading weights of refiner from checkpoint (' + args.refiner_model + ')')
            if args.cuda:
                cls.refine_net.load_state_dict(cls.__copyStateDict(load(args.refiner_model)))
                cls.refine_net = cls.refine_net.cuda()
                cls.refine_net = nn.DataParallel(cls.refine_net)
            else:
                cls.refine_net.load_state_dict(cls.__copyStateDict(load(args.refiner_model, map_location='cpu')))

            cls.refine_net.eval()
            args.poly = True
        return args


    @classmethod
    def process(cls, args: CraftConfig, image_list: list):
        """ Load test images in folder """
        t = time.time()
        # load data : process result
        for k, image_path in enumerate(image_list):
            print("Process image {:d}/{:d}: {:s}".format(k+1, len(image_list), image_path), end='\r')
            yield cls.process_one(args, image_path)
        print("elapsed time : {}s".format(time.time() - t))
    
    @classmethod
    def process_one(cls, args: CraftConfig, image_path: str):
        image: ndarray = imgproc.loadImage(image_path)

        bboxes, polys, score_text = cls.__eval_image(cls.net, image, args, cls.refine_net)

        image = image[:,:,::-1]
        image = np.array(image)

        """ save score text """
        #filename, file_ext = os.path.splitext(os.path.basename(image_path))
        #mask_file = result_folder + "/res_" + filename + '_mask.jpg'
        #cv2.imwrite(mask_file, score_text)
        bboxes = cls.__convert_bboxes(bboxes)
        return image, bboxes, image_path

    @staticmethod
    def __convert_bboxes(bboxes): 
        result = []
        for bbox in bboxes:
            poly = np.array(bbox).astype(np.int32)
            result.append(poly)
        return result

    @staticmethod
    def __copyStateDict(state_dict):
        """
        Clone the state dict 
        """
        if list(state_dict.keys())[0].startswith("module"):
            start_idx = 1
        else:
            start_idx = 0
        new_state_dict = OrderedDict()
        for k, v in state_dict.items():
            name = ".".join(k.split(".")[start_idx:])
            new_state_dict[name] = v
        return new_state_dict

    @staticmethod
    def __eval_image(
        net: CRAFT,
        image: ndarray,
        config: CraftConfig,
        refine_net=None):
        """
            Execute craft AI network
        """
        t0 = time.time()

        # resize image
        img_resized, target_ratio, size_heatmap = imgproc.resize_aspect_ratio(image, config.canvas_size, interpolation=cv2.INTER_LINEAR, mag_ratio=config.mag_ratio)
        ratio_h = ratio_w = 1 / target_ratio

        # preprocessing
        x = imgproc.normalizeMeanVariance(img_resized)
        x = from_numpy(x).permute(2, 0, 1)    # [h, w, c] to [c, h, w]
        x = Variable(x.unsqueeze(0))                # [c, h, w] to [b, c, h, w]
        if config.cuda:
            x = x.cuda()

        # forward pass
        with no_grad():
            y, feature = net(x)

        # make score and link map
        score_text = y[0,:,:,0].cpu().data.numpy()
        score_link = y[0,:,:,1].cpu().data.numpy()

        # refine link
        if refine_net is not None:
            with no_grad():
                y_refiner = refine_net(y, feature)
            score_link = y_refiner[0,:,:,0].cpu().data.numpy()

        t0 = time.time() - t0
        t1 = time.time()

        # Post-processing
        boxes, polys = craft_utils.getDetBoxes(
            score_text, 
            score_link, 
            config.text_threshold, 
            config.link_threshold, 
            config.low_text, 
            config.poly)

        # coordinate adjustment
        boxes = craft_utils.adjustResultCoordinates(boxes, ratio_w, ratio_h)
        polys = craft_utils.adjustResultCoordinates(polys, ratio_w, ratio_h)
        for k in range(len(polys)):
            if polys[k] is None: polys[k] = boxes[k]

        t1 = time.time() - t1

        # render results (optional)
        render_img = score_text.copy()
        render_img = np.hstack((render_img, score_link))
        ret_score_text = imgproc.cvt2HeatmapImg(render_img)

        if config.show_time : print("\ninfer/postproc time : {:.3f}/{:.3f}".format(t0, t1))

        return boxes, polys, ret_score_text
