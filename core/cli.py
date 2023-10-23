#cython: language_level=3

import argparse
import sys
from dotenv import load_dotenv

from src.OCRManager import OCRManager
from src.FileManager import FileManager
from src.model.model import OCRConfig, CraftConfig, OCRService, TranslatorService, EditConfig
from src.ImageCleaner import ImageCleaner
from src.utility.create_block_cluster import create_block_cluster
from src.TranslatorManager import TranslatorManager
from src.utility.exception import InternalError
from src.TextEditor import TextEditor

def str2bool(v):
    """ prompt reply to boolean """
    return v.lower() in ("yes", "y", "true", "t", "1")

parser = argparse.ArgumentParser(description='ScanTranslator')
parser.add_argument('-i', '--input_image', required=True, type=str, help='path to input images')
parser.add_argument('--gpu', default=False, type=str2bool, help='Enable CUDA (default: use the CPU)')
parser.add_argument('-o', '--ouput_folder', default='./result/', type=str, help='folder path to output images')
parser.add_argument('--ocr', default=OCRService.LOCAL_CRAFTTESSERACT, type=OCRService, choices=list(OCRService), help='OCR Service name')
parser.add_argument('--trad', default=TranslatorService.GOOGLE, type=TranslatorService, choices=list(TranslatorService), help='Translation service name')

def process_ocr(config):
    try:
        ocrManager = OCRManager(config)
    except BaseException as err:
        print("Program setup failure.", err)
        sys.exit(1)
    # Process
    try:
        print(f"\n\tOCR page... {config.src_img_path}")
        page, image = ocrManager.process_img(config.src_img_path)
    except BaseException as err:
        print("Error3: ", err)
        raise InternalError("Fail to process page")

    try:
        print(f"\tOCR, create block clusters...")
        page = create_block_cluster(page, 
            config.box_cluster_search_range, 
            config.box_cluster_search_step,
            config.style)
    except BaseException as err:
        print("Error4: ", err)
        raise InternalError("Fail to generate block clusters")
    
    try:
        page.clean_path = config.output_folder # Save output location
        page.render_path = config.output_folder
        #pageOut = page.serialize()
        #pageOut['index'] = 0
    except BaseException as err:
        print("Error5: ", err)
        raise InternalError("Fail to serialise output JSON")
    return page

def process_clean(page):
    try:
        src_image = FileManager.load_image(page.src_path)
    except BaseException as err:
        print("Error loading image: {}".format(err))
        raise InternalError("Unable to load source image")
    try:
        print(f"\n\tClean page... {config.src_img_path}")
        image_clean = ImageCleaner.process(src_image, page.clusters)
    except BaseException as err:
        print("Error processing image: {}".format(err))
        raise InternalError("An error occured when processing the image")
    try:
        output_directory = FileManager.path_from_filepath(page.clean_path)
        filename = FileManager.filename_from_filepath(FileManager.add_file_suffix(page.src_path, "_clean"))
        page.clean_path = FileManager.save_image(image_clean, filename, output_directory)
        print(f"\tClean... saved as {page.clean_path}")
    except BaseException as err:
        print("Error save result: {}".format(err))
        raise InternalError("Fail to save image")
    return page

def process_translate(page):
    translation_service = args.trad
    try:
        print(f"\n\tStart translation : {translation_service}")
        translator = TranslatorManager(translation_service, src_lang='en', dest_lang='fr')
        page = translator.translate_page(page)
    except BaseException as err:
        print("Error translator: {}".format(err))
        raise InternalError("The translator failed to process.")
    return page

def process_render(page):
    editConfig = EditConfig()
    try:
        image_clean = FileManager.load_image(page.clean_path)
    except BaseException as err:
        print("Error loading image: {}".format(err))
        raise InternalError("Unable to load cleaned image")
    # Read source image
    try:
        image_src = FileManager.load_image(page.src_path)
    except BaseException as err:
        print("Error loading image: {}".format(err))
        raise InternalError("Unable to load source image")
    # Render result
    image_final = TextEditor.process_img(editConfig, page.clusters, image_clean, image_src)
    # Save result
    try:
        output_directory = FileManager.path_from_filepath(page.render_path)
        filename = FileManager.filename_from_filepath(FileManager.add_file_suffix(page.src_path, "_render"))
        page.render_path = FileManager.save_image(image_final, filename, output_directory)
    except BaseException as err:
        print("Error save result: {}".format(err))
        raise InternalError("Fail to save image")

    print("\nDone !")
    print(f"see {page.render_path}")


if __name__ == '__main__':
    load_dotenv() # Load env variables
    args = parser.parse_args()
    # Config
    craftConfig = CraftConfig()
    craftConfig.cuda = args.gpu
    config = OCRConfig(args.input_image, args.ouput_folder, args.ocr)
    config.craft_config = craftConfig

    page = process_ocr(config)
    page = process_clean(page)
    page = process_translate(page)
    process_render(page)