import argparse

from src.FileManager import FileManager
from src.TextEditor import TextEditor
from src.model import AppConfig, CraftConfig
from src.ImageCleaner import ImageCleaner
from src.ocr.default.OCRCraftTesseract import OCRCraftTesseract

from src.utility.draw_bouncing_box import draw_bouncing_box
from src.utility.show_debug import show_debug

def str2bool(v):
    """ prompt reply to boolean """
    return v.lower() in ("yes", "y", "true", "t", "1")

parser = argparse.ArgumentParser(description='ScanTranslator')
parser.add_argument('-i', '--input_folder', required=True, type=str, help='folder path to input images')
parser.add_argument('--gpu', default=False, type=str2bool, help='Enable CUDA (default: use the CPU)')
parser.add_argument('-o', '--ouput_folder', default='./result/', type=str, help='folder path to output images')
args = parser.parse_args()

if __name__ == '__main__':
    craftConfig = CraftConfig()
    craftConfig.cuda = args.gpu
    config = AppConfig(args.input_folder)
    config.output_folder = args.ouput_folder
    config.ocr_config.craft_config = craftConfig

    FileManager.setup(config)    
    OCRCraftTesseract.setup(config.ocr_config)

    image_path_list = FileManager.get_files(config.input_folder)

    for file_path in image_path_list:
        print("## OCR : process", file_path)
        page = OCRCraftTesseract.process_img(file_path)
        print("## Clean image")
        image_clean = ImageCleaner.process(page.image, page.blocks)

        print("## Insert text")
        image_final = TextEditor.process_img(config, page.blocks, image_clean, page.image)

        ## DEBUG : add bouncing boxes
        # draw_bouncing_box(image_final, bboxes)

        print("## Save result")
        FileManager.save_image(image_final, page.image_path, config)

        # break # DEBUG