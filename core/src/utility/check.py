
from typing import Sequence

from src.utility.exception import InvalidJson

def check_argument(dictionary: dict, key_list: Sequence[str]):
    for key in key_list:
        if not key in dictionary.keys():
            raise InvalidJson('Missing key ' + key + ' in body')