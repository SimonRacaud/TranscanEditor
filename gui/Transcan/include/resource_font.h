#ifndef RESOURCE_FONT_H
#define RESOURCE_FONT_H

#include "include/environment.h"

#include <QList>
#include <QString>

static const QList<QString> RES_FONT_REG = {
    FONT_DIR_REG"augie.ttf",
    FONT_DIR_REG"Felt Regular.ttf",
    FONT_DIR_REG"mangat.ttf",
    FONT_DIR_REG"mangatb.ttf",
    FONT_DIR_REG"mangati.ttf",
    FONT_DIR_REG"wildwordsbold.ttf",
    //FONT_DIR_REG"wildwordsbolditalic.TTF", // Italic not supported yet
    //FONT_DIR_REG"wildwordsitalic.TTF",
    FONT_DIR_REG"wildwordsroman.TTF",
};
static const QList<QString> RES_FONT_SFX = {
    FONT_DIR_SFX"another.ttf",
    FONT_DIR_SFX"bigfish.ttf",
    FONT_DIR_SFX"TrashHand.TTF",
    FONT_DIR_SFX"Vnhltfap.ttf",
};

#endif // RESOURCE_FONT_H
