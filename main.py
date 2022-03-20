import argparse

from src.FileManager import FileManager
from src.TextEditor import TextEditor
from src.model import AppConfig, CraftConfig
from src.charaterRecognition.CharacterDetector import CharacterDetector
from src.ImageCleaner import ImageCleaner
from src.TextExtractor import TextExtractor

from src.utility.draw_bouncing_box import draw_bouncing_box

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
    config.craft_config = craftConfig

    FileManager.setup(config)    

    image_path_list = FileManager.get_files(config.input_folder)

    print("## Text detector")
    for image, bboxes, image_path in CharacterDetector.process(config.craft_config, image_path_list):
        print("## Character extraction")
        textBlockList = TextExtractor.character_extraction(image, bboxes)

        print("## Clean image")
        image_clean = ImageCleaner.process(image, textBlockList)
        print("## Insert text")
        image_final = TextEditor.process(textBlockList, image_clean)

        ## DEBUG : add bouncing boxes
        # draw_bouncing_box(image_final, bboxes)

        print("## Save result")
        FileManager.save_image(image_final, image_path, config)

