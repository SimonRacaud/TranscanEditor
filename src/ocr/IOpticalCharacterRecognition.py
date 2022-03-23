import abc
from typing import Sequence

from src.model import OCRConfig, OCRPage

class IOpticalCharacterRecognition(metaclass=abc.ABCMeta):    
    @classmethod
    def __subclasshook__(cls, subclass):
        return (hasattr(subclass, 'process_batch') and 
                callable(subclass.process_batch) and 
                hasattr(subclass, 'setup') and 
                callable(subclass.setup) and 
                hasattr(subclass, 'process_img') and 
                callable(subclass.process_img) or 
                NotImplemented)
    
    @abc.abstractclassmethod
    def setup(cls, config: OCRConfig):
        """ Init OCR """
        raise NotImplementedError

    @abc.abstractclassmethod
    def process_batch(cls, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        raise NotImplementedError

    @abc.abstractclassmethod
    def process_img(cls, img_path: str) -> OCRPage:
        """ Process an image and return a list of text and bouncing boxes """
        raise NotImplementedError