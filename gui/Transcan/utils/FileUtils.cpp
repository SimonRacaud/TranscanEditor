#include "FileUtils.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "include/environment.h"

FileUtils::FileUtils()
{

}

bool FileUtils::checkImgFilePath(QString const& filepath)
{
    if (!QFile::exists(filepath)) {
        return false; // File doesn't exist
    }
    QFileInfo fi(filepath);
    if (!SUPPORTED_EXTENSION.contains(fi.suffix())) {
        return false; // invalid extension
    }
    return true;
}

bool FileUtils::checkDirExist(QString const& dirpath)
{
    QDir dir(dirpath);

    return dir.exists();
}
