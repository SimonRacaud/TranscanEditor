#include "FontManager.h"

#include <QDebug>
#include "include/resource_font.h"

std::unique_ptr<FontManager> FontManager::_instance = nullptr;

FontManager::FontManager()
{
    // Load fonts from resources
    this->loadFont(RES_FONT_REG, _regularFont);
    this->loadFont(RES_FONT_SFX, _sfxFont);
}

FontManager &FontManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = std::make_unique<FontManager>();
    }
    return *_instance.get();
}

QStringList FontManager::regularFamilies() const
{
    return QStringList::fromList(_regularFont);
}

QStringList FontManager::sfxFamilies() const
{
    return QStringList::fromList(_sfxFont);
}

QStringList FontManager::families() const
{
    return QFontDatabase::families();
}

/** PRIVATE **/

void FontManager::loadFont(QList<QString> const &fileList, QList<QString> &dest)
{
    for (QString const &font : fileList) {
        int id = QFontDatabase::addApplicationFont(font);
        if (id == -1) {
            qDebug() << "FontManager: unable to load font " << font;
            std::runtime_error("FontManager fail to load font");
        }
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        if (!dest.contains(family)) // We can load bold, italic, ... versions of the font.
            dest.push_back(family);
    }
}
