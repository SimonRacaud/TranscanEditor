from dataclasses import dataclass

##
### Parse arguments
##

# def str2bool(v):
#     """ prompt reply to boolean """
#     return v.lower() in ("yes", "y", "true", "t", "1")

# parser = argparse.ArgumentParser(description='CRAFT Text Detection')
# parser.add_argument('--trained_model', default='weights/craft_mlt_25k.pth', type=str, help='pretrained model')
# parser.add_argument('--text_threshold', default=0.7, type=float, help='text confidence threshold')
# parser.add_argument('--low_text', default=0.4, type=float, help='text low-bound score')
# parser.add_argument('--link_threshold', default=0.4, type=float, help='link confidence threshold')
# parser.add_argument('--cuda', default=False, type=str2bool, help='Use cuda for inference')
# parser.add_argument('--canvas_size', default=1280, type=int, help='image size for inference')
# parser.add_argument('--mag_ratio', default=1.5, type=float, help='image magnification ratio')
# parser.add_argument('--poly', default=False, action='store_true', help='enable polygon type')
# parser.add_argument('--show_time', default=False, action='store_true', help='show processing time')
# parser.add_argument('--test_folder', default='/data/', type=str, help='folder path to input images')
# parser.add_argument('--refine', default=False, action='store_true', help='enable link refiner')
# parser.add_argument('--refiner_model', default='weights/craft_refiner_CTW1500.pth', type=str, help='pretrained refiner model')
# args = parser.parse_args()

@dataclass
class Config:
    trained_model = 'craft/weights/craft_mlt_25k.pth'
    text_threshold = 0.7 # 0.7
    low_text = 0.4 # 0.4
    link_threshold = 0.4
    cuda = False
    canvas_size = 1300 # 1280
    mag_ratio = 1.5
    poly = False
    show_time = True
    test_folder = '../../dataset/Chapter_99'
    refine = True
    refiner_model = 'craft/weights/craft_refiner_CTW1500.pth'
    result_folder = './result/'