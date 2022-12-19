# import argparse
from flask import Flask, request
from dotenv import load_dotenv

from src.model.apiModel import OCRInput, RenderInput
from src.model.model import OCRPage, TranslatorService
from src.utility.create_block_cluster import create_block_cluster
from src.utility.exception import InvalidJson, InternalError, InvalidRequest, APIError
from src.utility.check import check_argument

from src.FileManager import FileManager
from src.OCRManager import OCRManager
from src.ImageCleaner import ImageCleaner
from src.TranslatorManager import TranslatorManager
from src.TextEditor import TextEditor

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

@app.route("/translate", methods=['POST'])
def process_translation():
    """
        Input: OCRPage structure, TranslationService
        Output: OCRPage strcture
    """
    try:
        inputJson = request.json
        check_argument(inputJson, ["page", "translationService"])
        pageJson = inputJson["page"]
        translation_service = inputJson["translationService"]
        if type(translation_service) is not str:
            raise InvalidJson("Invalid translation service value")
        translation_service = TranslatorService[translation_service]
        input = OCRPage.deserialize(pageJson)
    except APIError as err:
        raise err
    except Exception as err:
        print("Error {}".format(err))
        raise InvalidJson("Invalid body")
    try:
        translator = TranslatorManager(translation_service, src_lang='en', dest_lang='fr')
        input = translator.translate_page(input)
    except Exception as err:
        print("Error translator: {}".format(err))
        raise InternalError("The translator failed to process.")
    return input.serialize()

@app.route("/render", methods=['POST'])
def process_render():
    """
        Input: OCRPage structure, Editing config
        Output: Rendered image
    """
    try:
        inputJson = request.json
        input = RenderInput.deserialize(inputJson)
    except APIError as err:
        raise err
    except Exception as err:
        print("Error {}".format(err))
        raise InvalidJson("Invalid body")
    # Read cleaned image
    try:
        image_clean = FileManager.load_image(input.page.clean_path)
    except Exception as err:
        print("Error loading image: {}".format(err))
        raise InvalidJson("Unable to load cleaned image")
    # Read source image
    try:
        image_src = FileManager.load_image(input.page.src_path)
    except Exception as err:
        print("Error loading image: {}".format(err))
        raise InvalidJson("Unable to load source image")
    # Render result
    image_final = TextEditor.process_img(input.config, input.page.clusters, image_clean, image_src)
    # Save result
    try:
        output_directory = FileManager.path_from_filepath(input.page.render_path)
        filename = FileManager.filename_from_filepath(FileManager.add_file_suffix(input.page.src_path, "_render"))
        input.page.render_path = FileManager.save_image(image_final, filename, output_directory)
    except Exception as err:
        print("Error save result: {}".format(err))
        raise InternalError("Fail to save image")
    return input.page.serialize()

### Error management

@app.errorhandler(APIError)
def handle_invalid_body(error: APIError):
    return error.to_dict(), error.status_code

###

if __name__ == '__main__':
    load_dotenv() # Load env variables
    app.run(debug=True, port=5000, threaded=True)
    # args = parser.parse_args()
