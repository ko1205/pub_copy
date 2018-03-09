#include "common.h"
#include <QMessageBox>

void subCopy(const QString &src, const QString &destPath)
{
    QFileInfo fileInfo(src);
    QDir destPathDIr(destPath);
    if(fileInfo.isFile()){
        QFile file(fileInfo.filePath());
        file.copy(destPathDIr.filePath(fileInfo.fileName()));
    }else if(fileInfo.isDir()){
        QDir srcDir(src);
        destPathDIr.mkdir(srcDir.dirName());
        QFileInfoList fileList = srcDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
        for(int i = 0; i < fileList.size();i++)
        {
            QString newDestPath;
            QString newSrcPath;
            newDestPath = destPathDIr.filePath(srcDir.dirName());
            newSrcPath = fileList.at(i).absoluteFilePath();
            subCopy(newSrcPath,newDestPath);
        }
    }
}

int entryFileCount(const QString &srcPath)
{
    QFileInfo fileInfo(srcPath);
    QDir srcDir(srcPath);
    int count = 0;
    if(fileInfo.isFile()){
        count++;
    }else if(fileInfo.isDir()){
        QFileInfoList list = srcDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot);
        for(int i=0;i<list.size();i++)
        {
            QString newPath = list.at(i).absoluteFilePath();
            count += entryFileCount(newPath);
        }
    }
    return count;
}
