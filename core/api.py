# import argparse
from flask import Flask, request
from dotenv import load_dotenv

from src.model.apiModel import OCRInput
from src.model.model import OCRPage
from src.utility.create_block_cluster import create_block_cluster
from src.utility.exception import InvalidJson, InternalError, InvalidRequest, APIError

from src.FileManager import FileManager
from src.OCRManager import OCRManager
from src.ImageCleaner import ImageCleaner

app = Flask(__name__)

# def str2bool(v):
#     """ prompt reply to boolean """
#     return v.lower() in ("yes", "y", "true", "t", "1")

# parser = argparse.ArgumentParser(description='ScanTranslator')
# parser.add_argument('-i', '--input_folder', required=True, type=str, help='folder path to input images')
# parser.add_argument('--gpu', default=False, type=str2bool, help='Enable CUDA (default: use the CPU)')
# parser.add_argument('-o', '--ouput_folder', default='./result/', type=str, help='folder path to output images')
# parser.add_argument('--ocr', default=OCRService.LOCAL_CRAFTTESSERACT, type=OCRService, choices=list(OCRService), help='OCR Service name')
# parser.add_argument('--trad', default=TranslatorService.GOOGLE, type=TranslatorService, choices=list(TranslatorService), help='Translation service name')

@app.route("/")
def root():
    return {"I'm a teapot": "The server refuses to brew coffee because it is, permanently, a teapot."}, 418

@app.route("/ocr", methods=['POST'])
def ocr():
    """
        Input: Config struture + Source Image
        Output: OCRPage structure anager
    """
    inputJson = request.json
    try:
        input = OCRInput.deserialize(inputJson)
        print("\nConfiguration: {}\n".format(input))
    except InvalidJson as err:
        raise err
    except Exception as err:
        print("Error: ", err)
        raise InvalidJson("Invalid JSON input")
    #
    try:
        ocrManager = OCRManager(input.ocr_config)
    except Exception as err:
        print("Error: ", err)
        raise InvalidJson("ocrService value not supported")
    print("OCR : process image " + input.ocr_config.src_img_path)
    try:
        page, image = ocrManager.process_img(input.ocr_config.src_img_path)
    except Exception as err:
        print("Error: ", err)
        raise InternalError("Fail to process page")
    try:
        page = create_block_cluster(page, 
            input.ocr_config.box_cluster_search_range, 
            input.ocr_config.box_cluster_search_step)
    except Exception as err:
        print("Error: ", err)
        raise InternalError("Fail to generate block clusters")
    try:
        page.clean_path = input.ocr_config.output_folder # Save output location
        page.render_path = input.ocr_config.output_folder
        pageOut = page.serialize()
        pageOut['index'] = input.index
    except Exception as err:
        print("Error: ", err)
        raise InternalError("Fail to serialise output JSON")
    return pageOut

@app.route("/clean", methods=['POST'])
def process_clean():
    """
        Input: OCRPage structure
        Output: Cleaned image 
    """
    try:
        inputJson = request.json
        input = OCRPage.deserialize(inputJson)
    except Exception as err:
        print("Error {}".format(err))
        raise InvalidJson("Invalid body")
    try:
        src_image = FileManager.load_image(input.src_path)
    except Exception as err:
        print("Error loading image: {}".format(err))
        raise InvalidJson("Unable to load source image")
    try:
        image_clean = ImageCleaner.process(src_image, input.blocks)
    except Exception as err:
        print("Error processing image: {}".format(err))
        raise InternalError("An error occured when processing the image")
    try:
        output_directory = FileManager.path_from_filepath(input.clean_path)
        filename = FileManager.filename_from_filepath(FileManager.add_file_suffix(input.src_path, "_clean"))
        input.clean_path = FileManager.save_image(image_clean, filename, output_directory)
    except Exception as err:
        print("Error save result: {}".format(err))
        raise InternalError("Fail to save image")
    return input.serialize()

# @app.route("/translate", methods=['POST'])
# def process_translation():
#     """
#         Input: OCRPage structure
#         Output: OCRPage strcture
#     """
#     # translator = TranslatorManager(config.translation_service, src_lang='en', dest_lang='fr')
#     # translator.translate_page(page)
#     pass

# @app.route("/render", methods=['POST'])
# def process_render():
#     """
#         Input: OCRPage structure
#         Output: Rendered image
#     """
#     #FileManager.setup(config)    
#     # image_final = TextEditor.process_img(config, page.clusters, image_clean, page.image)
#     # FileManager.save_image(image_final, page.image_path, config)
#     pass

### Error management

@app.errorhandler(APIError)
def handle_invalid_body(error: APIError):
    return error.to_dict(), error.status_code

###

if __name__ == '__main__':
    load_dotenv() # Load env variables
    app.run(debug=True, port=5000)
    # args = parser.parse_args()
