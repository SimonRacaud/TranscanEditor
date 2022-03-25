# -*- coding: utf-8 -*-

CYR_TO_LAT = {
    # ucase
    u'А':u'A',
    u'Б':u'B',
    u'В':u'B',
    u'Г':u'R',
    u'Д':u'D',
    u'Ђ':u'Đ',
    u'Е':u'E',
    u'Ж':u'X',
    u'З':u'3',
    u'И':u'N',
    u'Ј':u'J',
    u'К':u'K',
    u'Л':u'n',
    u'М':u'M',
    u'Н':u'H',
    u'Њ':u'H',
    u'О':u'O',
    u'П':u'M',
    u'Р':u'P',
    u'С':u'C',
    u'Т':u'T',
    u'У':u'y',
    u'Х':u'X',
    u'Ц':u'U',
    u'Ч':u'y',
    u'Џ':u'U',
    u'Ш':u'W',
    # locase
    u'а':u'a',
    u'б':u'o',
    u'в':u'b',
    u'г':u'r',
    u'е':u'e',
    u'ж':u'x',
    u'з':u'3',
    u'и':u'n',
    u'ј':u'j',
    u'к':u'k',
    u'л':u'n',
    u'м':u'm',
    u'н':u'H',
    u'њ':u'h',
    u'о':u'o',
    u'п':u'n',
    u'р':u'p',
    u'с':u'c',
    u'т':u'T',
    u'у':u'y',
    u'х':u'x',
    u'ц':u'u',
    u'ч':u'y',
    u'џ':u'u',
    u'ш':u'w',
}

def cyrillic_to_latin(text):
    result = ''
    for c in text:
        char = CYR_TO_LAT.get(c)
        if char != None:
            result += char
        else:
            result += c
    return result