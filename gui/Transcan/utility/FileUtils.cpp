#include "FileUtils.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <iostream>

#include "include/env_config.h"

FileUtils::FileUtils()
{

}

bool FileUtils::checkImgFilePath(QString const& filepath)
{
    if (!QFile::exists(filepath)) {
        return false; // File doesn't exist
    }
    QFileInfo fi(filepath);
    if (!SUPPORTED_EXTENSION_SUFFIX.contains(fi.suffix())) {
        return false; // invalid extension
    }
    return true;
}

bool FileUtils::checkDirExist(QString const& dirpath)
{
    QDir dir(dirpath);

    return dir.exists();
}

bool FileUtils::copyToDestination(QString const& srcFilePath, QString const &destDir)
{
    QString destFile = destDir;

    if (*(destFile.end() - 1) != '/') {
        destFile += "/";
    }
    destFile += QFileInfo(srcFilePath).fileName();
    return QFile::copy(srcFilePath, destFile);
}

QString FileUtils::mergeFilepath(QString const &path, QString &file)
{
    if (path.endsWith('/')) {
        return path + file;
    }
    return path + '/' + file;
}
void FileUtils::createProjectDirectory(shared_ptr<ProjectConfig> config)
{
    try {
        QString projectDirName = config->projectName;
        projectDirName = projectDirName.replace(' ', '_');
        projectDirName = projectDirName.removeIf([](const QChar c) {
           return !(c.isLetterOrNumber() || c == '_' || c == '-' || c == '.');
        });
        if (projectDirName.isEmpty()) {
            std::invalid_argument("FileUtils::createProjectDirectory : invalid empty project name");
        }

        QDir dir(config->destPath);
        if (dir.exists()) {
            qDebug() << "FileUtils::createProjectDirectory : create project directory => " << config->destPath;
            if (dir.exists(projectDirName)) {
                // The project directory already exist
                if (dir.exists(projectDirName + "/source")
                        && dir.exists(projectDirName + "/clean")
                        && dir.exists(projectDirName + "/result")) {
                    QDir previousProjectDir = dir;
                    previousProjectDir.cd(projectDirName);
                    if (!previousProjectDir.removeRecursively()) {
                        throw std::runtime_error("FileUtils::createProjectDirectory : unable to reset old project directory");
                    }
                } else {
                    throw std::runtime_error("FileUtils::createProjectDirectory : directory already exist");
                }
            }
            if (!dir.mkpath(projectDirName)) {
                throw std::runtime_error("FileUtils::createProjectDirectory : unable to create project directory");
            }
            bool success = dir.mkdir(projectDirName+"/source")
                    && dir.mkdir(projectDirName+"/clean")
                    && dir.mkdir(projectDirName+"/result");
            config->destPath = FileUtils::mergeFilepath(config->destPath, projectDirName);
            if (!success) {
                throw std::runtime_error("FileUtils::createProjectDirectory : unable to create project sub-directories");
            }
        } else {
            qDebug() << "Destination directory : " << config->destPath;
            throw std::runtime_error("FileUtils::createProjectDirectory : destination directory doesn't exist.");
        }
    } catch (std::exception const &err) {
        qDebug("Exception (FileUtils) : %s", err.what());
        throw std::runtime_error("FileUtils::createProjectDirectory Error : " + std::string(err.what()));
    }
}

vector<OCRPage> FileUtils::loadPagesFromPath(QString const &path)
{
    vector<OCRPage> pages;

    try {
        QDir dir(path);
        QStringList filter;
        QFileInfoList filelistinfo;

        // Filter image files
        for (size_t i = 0; i < (size_t)SUPPORTED_EXTENSION.size(); i++) {
            filter << SUPPORTED_EXTENSION.at(i);
        }
        dir.setNameFilters(filter);
        // Load images
        filelistinfo = dir.entryInfoList();
        foreach (const QFileInfo &fileinfo, filelistinfo) {
            QString imageFile = fileinfo.absoluteFilePath();
            OCRPage page;
            page.imagePath = imageFile;
            pages.push_back(page);
        }
    } catch (std::exception const &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        throw std::runtime_error("Unable to access source directory.");
    }
    return pages;
}

bool FileUtils::exportSourceImage(QImage const &imageObj, QString const &projectPath,
                            Directory const target, OCRPage &page)
{
    auto fileInfo = QFileInfo((page.imagePath.isEmpty()) ? page.sourceImagePath : page.imagePath);
    QString targetPath;
    QString dirPath = projectPath;
    const QString ext = ".png";

    switch (target) {
        case Directory::SOURCE:
        dirPath += "/source/";
            break;
        case Directory::CLEAN:
        dirPath += "/clean/";
            break;
        case Directory::RESULT:
        dirPath += "/result/";
            break;
    }
    targetPath = dirPath + fileInfo.baseName() + ext;
    switch (target) {
        case Directory::SOURCE:
        page.sourceImagePath = targetPath;
            break;
        case Directory::CLEAN:
        page.cleanImagePath = targetPath;
            break;
        case Directory::RESULT:
        page.renderImagePath = targetPath;
            break;
    }
    return imageObj.save(dirPath + fileInfo.baseName() + ext, "png");
}

bool FileUtils::initProjectSourceImages(vector<OCRPage> &pages, QString const &projectSrcDir)
{
    QList<QPair<OCRPage&, std::shared_ptr<QPixmap>>> pageFileList;
    size_t maxImgWidth = 0;

    for (OCRPage &page : pages) {
        std::shared_ptr<QPixmap> img = std::make_shared<QPixmap>();
        QString path = page.imagePath;
        bool loaded = img->load(path);
        if (!loaded) { // Failed to load image
            throw std::invalid_argument("FileUtils::initProjectSourceImages Failed to load image "+path.toStdString());
        }
        maxImgWidth = qMax(maxImgWidth, (size_t)img->width());
        pageFileList.append(QPair<OCRPage&, std::shared_ptr<QPixmap>>(page, img));
    }
    // Scale images and add them to the UI (scene):
    for (QPair<OCRPage&, std::shared_ptr<QPixmap>> &pair : pageFileList) {
        OCRPage &page = pair.first;
        std::shared_ptr<QPixmap> img = pair.second;

        *img = img->scaledToWidth(maxImgWidth);
        bool ok = FileUtils::exportSourceImage(img->toImage(), projectSrcDir, Directory::SOURCE, page);
        if (!ok) {
            return false; // An error occured
        }
    }
    return true;
}
