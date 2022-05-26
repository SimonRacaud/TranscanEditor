import abc
from typing import Sequence

from src.model import OCRConfig, OCRPage

class IOpticalCharacterRecognition(metaclass=abc.ABCMeta):    
    @classmethod
    def __subclasshook__(cls, subclass):
        return (hasattr(subclass, 'process_batch') and 
                callable(subclass.process_batch) and 
                hasattr(subclass, '__init__') and 
                callable(subclass.__init__) and 
                hasattr(subclass, 'process_img') and 
                callable(subclass.process_img) or 
                NotImplemented)
    
    @abc.abstractmethod
    def __init__(cls, config: OCRConfig):
        """ Init OCR """
        raise NotImplementedError

    @abc.abstractmethod
    def process_batch(cls, img_path_list: Sequence[str]) -> Sequence[OCRPage]:
        """ Process a batch of image and return a list of text and bouncing boxes """
        raise NotImplementedError

    @abc.abstractmethod
    def process_img(cls, img_path: str) -> OCRPage:
        """ Process an image and return a list of text and bouncing boxes """
        raise NotImplementedError