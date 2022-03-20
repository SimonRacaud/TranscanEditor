from dataclasses import dataclass
import string
import cv2

@dataclass
class CraftConfig:
    cuda = False            # Use cuda for inference
    trained_model = './weights/craft_mlt_25k.pth' # pretrained model
    text_threshold = 0.7    # text confidence threshold
    low_text = 0.4          # text low-bound score
    link_threshold = 0.4    # link confidence threshold
    canvas_size = 1300      # image size for inference
    mag_ratio = 1.5         # image magnification ratio
    poly = False            # enable polygon type
    show_time = True        # show processing time
    refine = True           # enable link refiner
    refiner_model = './weights/craft_refiner_CTW1500.pth' # pretrained refiner model

@dataclass
class AppConfig:
    input_folder: string
    output_folder = './result/'
    craft_config: CraftConfig = CraftConfig()

@dataclass
class TextBlock:
    str: string
    position: any # (int, int)
    font_scale: int
    bbox: any # (int, int, int, int)
    pivot: any # (int, int)
    angle: float
    size: any # (int, int)
    color: any = (0, 0, 255)
    font: int = cv2.FONT_HERSHEY_SIMPLEX
    thickness: int = 2