#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>

class FileUtils
{
public:
    FileUtils();

    /**
     * @brief checkImgFilePath : check if the file exist and its extension
     * @return
     */
    static bool checkImgFilePath(QString const& filepath);

    /**
     * @brief checkDirExist : check if a directory exist
     * @param dirpath
     * @return
     */
    static bool checkDirExist(QString const& dirpath);

    /**
     * @brief copyToDestination
     *  Copy image at srcFilePath to destDir directory
     * @param srcFilePath
     * @param destDir
     * @return
     */
    static bool copyToDestination(QString const& srcFilePath, QString const &destDir);
};

#endif // FILEUTILS_H
