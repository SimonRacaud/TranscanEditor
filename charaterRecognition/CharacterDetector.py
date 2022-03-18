"""  
Copyright (c) 2019-present NAVER Corp.
MIT License
"""

# -*- coding: utf-8 -*-
import os
import time
from numpy import ndarray

import torch
import torch.backends.cudnn as cudnn
from torch.autograd import Variable

import cv2
import numpy as np
import craft.craft_utils as craft_utils
import craft.imgproc as imgproc
import craft.file_utils as file_utils

from craft.craft import CRAFT
from craft.refinenet import RefineNet

from collections import OrderedDict
from CraftConfig import Config

class CharacterDetector:
    @classmethod
    def process(cls, args: Config, image_list: list):
        """ Load test images in folder """
        # load net
        net = CRAFT()     # initialize

        print('Loading weights from checkpoint (' + args.trained_model + ')')
        if args.cuda:
            net.load_state_dict(cls.__copyStateDict(torch.load(args.trained_model)))
        else:
            net.load_state_dict(cls.__copyStateDict(torch.load(args.trained_model, map_location='cpu')))

        if args.cuda:
            net = net.cuda()
            net = torch.nn.DataParallel(net)
            cudnn.benchmark = False

        net.eval() # Enable eval mode

        # LinkRefiner : fusion between boxes
        refine_net = None
        if args.refine:
            refine_net = RefineNet()
            print('Loading weights of refiner from checkpoint (' + args.refiner_model + ')')
            if args.cuda:
                refine_net.load_state_dict(cls.__copyStateDict(torch.load(args.refiner_model)))
                refine_net = refine_net.cuda()
                refine_net = torch.nn.DataParallel(refine_net)
            else:
                refine_net.load_state_dict(cls.__copyStateDict(torch.load(args.refiner_model, map_location='cpu')))

            refine_net.eval()
            args.poly = True

        t = time.time()

        # load data
        for k, image_path in enumerate(image_list):
            print("Test image {:d}/{:d}: {:s}".format(k+1, len(image_list), image_path), end='\r')
            image: ndarray = imgproc.loadImage(image_path)

            bboxes, polys, score_text = cls.__eval_image(net, image, args, refine_net)

            # save score text
            #filename, file_ext = os.path.splitext(os.path.basename(image_path))
            #mask_file = result_folder + "/res_" + filename + '_mask.jpg'
            #cv2.imwrite(mask_file, score_text)

            file_utils.saveResult(image_path, image[:,:,::-1], polys, dirname=result_folder)
        print("elapsed time : {}s".format(time.time() - t))

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
        config: Config,
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
        x = torch.from_numpy(x).permute(2, 0, 1)    # [h, w, c] to [c, h, w]
        x = Variable(x.unsqueeze(0))                # [c, h, w] to [b, c, h, w]
        if config.cuda:
            x = x.cuda()

        # forward pass
        with torch.no_grad():
            y, feature = net(x)

        # make score and link map
        score_text = y[0,:,:,0].cpu().data.numpy()
        score_link = y[0,:,:,1].cpu().data.numpy()

        # refine link
        if refine_net is not None:
            with torch.no_grad():
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
###

if __name__ == '__main__':
    result_folder = './result/'
    if not os.path.isdir(result_folder):
        os.mkdir(result_folder)

    config = Config()
    image_path_list, _, _ = file_utils.get_files(config.test_folder)

    detector = CharacterDetector()
    detector.process(config, image_path_list)
