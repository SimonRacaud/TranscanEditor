import argparse
from dotenv import load_dotenv

from src.OCRManager import OCRManager
from src.FileManager import FileManager
from src.TextEditor import TextEditor
from src.model import AppConfig, CraftConfig, OCRService
from src.ImageCleaner import ImageCleaner
from src.utility.create_block_cluster import create_block_cluster

from src.utility.draw_bouncing_box import draw_bouncing_box, draw_cluster_box
from src.utility.show_debug import show_debug

def str2bool(v):
    """ prompt reply to boolean """
    return v.lower() in ("yes", "y", "true", "t", "1")

parser = argparse.ArgumentParser(description='ScanTranslator')
parser.add_argument('-i', '--input_folder', required=True, type=str, help='folder path to input images')
parser.add_argument('--gpu', default=False, type=str2bool, help='Enable CUDA (default: use the CPU)')
parser.add_argument('-o', '--ouput_folder', default='./result/', type=str, help='folder path to output images')
parser.add_argument('--ocr', default=OCRService.LOCAL_CRAFTTESSERACT, type=OCRService, choices=list(OCRService), help='OCR Service name')
args = parser.parse_args()

if __name__ == '__main__':
    load_dotenv() # Load env variables
    # Config
    craftConfig = CraftConfig()
    craftConfig.cuda = args.gpu
    config = AppConfig(args.input_folder, ocr_service=args.ocr)
    config.output_folder = args.ouput_folder
    config.ocr_config.craft_config = craftConfig
    # Init
    FileManager.setup(config)    
    ocrManager = OCRManager(config)
    # Process
    image_path_list = FileManager.get_files(config.input_folder)
    for file_path in image_path_list:
        try:
            print("## OCR : process", file_path)
            page = ocrManager.process_img(file_path)

            page = create_block_cluster(page, config)

            print("## Clean image")
            image_clean = ImageCleaner.process(page.image, page.blocks)

            print("## Insert text")
            image_final = TextEditor.process_img(config, page.clusters, image_clean, page.image)

            ## DEBUG : add bouncing boxes
            # draw_bouncing_box(image_final, page.blocks)
            # draw_cluster_box(image_final, page.clusters)

            print("## Save result")
            FileManager.save_image(image_final, page.image_path, config)
        except BaseException as err:
            print("Error: Fail to process image", file_path, err)