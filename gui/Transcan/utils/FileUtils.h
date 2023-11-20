#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QList>
#include <QPixmap>

#include "model/Page.h"

class FileUtils
{
public:
    FileUtils();

    enum Directory {
        SOURCE,
        CLEAN,
        RESULT
    };

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

    static QString mergeFilepath(QString const &path, QString &file);
    /**
     * @brief createProjectDirectory : create project directory and sub-directories
     * @param config : project configuration variables
     */
    static void createProjectDirectory(shared_ptr<ProjectConfig> config);
    /**
     * @brief exportImageList : write images to projectPath/
     * @param list
     * @return
     */
    static bool exportSourceImage(QImage const &imageObj, QString const &projectPath,
                                             Directory const target, OCRPage &page);

    /**
     * @brief FileUtils::loadPagesFromPath : fetch image files in <path> directory
     * and return their path compiled into a vector of OCRPage
     * @param path : path of a local directory
     * @throw runtime_error
     * @return
     */
    static vector<OCRPage> loadPagesFromPath(QString const &path);

    /**
     * @brief initProjectSourceImages : Resize and move original images to local project directory
     * @param pages : Project pages (will be modifed)
     * @param projectSrcDir : Project source directory path
     * @return
     */
    static bool initProjectSourceImages(vector<OCRPage> &pages, QString const &projectSrcDir);
};

#endif // FILEUTILS_H
