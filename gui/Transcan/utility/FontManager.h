#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QFontDatabase>

class FontManager
{
public:
    FontManager();

    static FontManager &getInstance(); // singleton

    QStringList regularFamilies() const;
    QStringList sfxFamilies() const;
    QStringList families() const;

private:
    void loadFont(QList<QString> const &fileList, QList<QString> &dest);

private:
    QList<QString> _regularFont;
    QList<QString> _sfxFont;

    static std::unique_ptr<FontManager> _instance;
};

#endif // FONTMANAGER_H
