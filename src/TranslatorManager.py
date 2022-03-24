from lib2to3.pytree import Base
import os
from deep_translator import (GoogleTranslator,
                             MicrosoftTranslator,
                             PonsTranslator,
                             LingueeTranslator,
                             LibreTranslator,
                             MyMemoryTranslator,
                             YandexTranslator,
                             PapagoTranslator,
                             DeeplTranslator)

from src.model import OCRPage, TranslatorService

class TranslatorManager:
    def __init__(self, service: TranslatorService) -> None:
        self.service = service
        self.trad_class_init = {
            TranslatorService.GOOGLE: self.__init_google, 
            TranslatorService.MICROSOFT: self.__init_microsoft,
            TranslatorService.DEEPL: self.__init_deepl,
            TranslatorService.LINGUEE: self.__init_linguee,
            TranslatorService.LIBRE: self.__init_libre,
            TranslatorService.MYMEMORY: self.__init_mymemory,
            TranslatorService.PONS: self.__init_pons,
            TranslatorService.YANDEX: self.__init_yandex,
            TranslatorService.PAPAGO: self.__init_papago
        }
        self.setup(service)
    
    def setup(self, service: TranslatorService, src_lang: str = 'en', dest_lang: str = 'fr'):
        self.translate_func = lambda text : self.translator.translate(text)
        self.src_lang = src_lang
        self.dest_lang = dest_lang
        
        self.translator = self.trad_class_init[service]()
    
    def get_supported_langages(self):
        return self.translator.get_supported_languages(as_dict=True)

    def translate_page(self, page: OCRPage):
        try:
            for block in page.clusters:
                block.translation = self.translate_sentence(block.sentence) or block.sentence
        except BaseException as err:
            raise RuntimeError('TranslatorManager: an error occured.', err)
        return page

    def translate_sentence(self, text: str):
        return self.translate_func(text)
    
    @staticmethod
    def __get_api_key(var_name: str):
        try:
            return os.environ[var_name]
        except:
            print('Error: (TranslatorManager): '+var_name+' env variable not found. Api key required to use the service.')
    
    def __init_google(self):
        return GoogleTranslator(source=self.src_lang, target=self.dest_lang)
    
    def __init_microsoft(self):
        api_key = TranslatorManager.__get_api_key('TRAD_KEY_MICROSOFT')
        return MicrosoftTranslator(api_key=api_key, source=self.src_lang, target=self.dest_lang)

    def __init_deepl(self):
        api_key = TranslatorManager.__get_api_key('TRAD_KEY_DEEPL')
        use_free = os.getenv('TRAD_DEEPL_USE_FREE', 'True').lower() in ('true', '1', 't')
        return DeeplTranslator(api_key=api_key, source=self.src_lang, target=self.dest_lang, use_free_api=use_free)
    
    def __init_linguee(self):
        return LingueeTranslator(source=self.src_lang, target=self.dest_lang)

    def __init_libre(self):
        base_url = os.environ.get('TRAD_LIBRE_BASE_URL')
        api_key = os.environ.get('TRAD_KEY_LIBRE')
        return LibreTranslator(source=self.src_lang, target=self.dest_lang, base_url=base_url, api_key=api_key)
    
    def __init_mymemory(self):
        return MyMemoryTranslator(source=self.src_lang, target=self.dest_lang)

    def __init_pons(self):
        return PonsTranslator(source=self.src_lang, target=self.dest_lang)

    def __init_yandex(self):
        api_key = TranslatorManager.__get_api_key('TRAD_KEY_YANDEX')
        self.translate_func = lambda text : self.translator.translate(source=self.src_lang, target=self.dest_lang, text=text)
        return YandexTranslator(api_key)
    
    def __init_papago(self):
        try:
            client_id = os.environ['TRAD_CLIENT_ID_PAPAGO']
        except:
            print('Error: (TranslatorManager): TRAD_CLIENT_ID_PAPAGO env variable not found. Api client id required to use the service.')
        api_key = TranslatorManager.__get_api_key('TRAD_KEY_PAPAGO')
        return PapagoTranslator(client_id=client_id, secret_key=api_key, source=self.src_lang, target=self.dest_lang)
