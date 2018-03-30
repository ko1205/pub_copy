#include "copythread.h"
#include <QMessageBox>
#include "common.h"
#include <QDate>

CopyThread::CopyThread(QObject *parent)
    :QThread(parent)
{
//    QMessageBox::information(0,"","OK Thread",QMessageBox::Yes);
}

CopyThread::~CopyThread()
{

}

void CopyThread::setThread(QTableWidget *tableWidget, QProgressBar *progressbar, bool allcopy, QComboBox *combobox, QDir path)
{
    queueListView = tableWidget;
    progressBar = progressbar;
    allCopy = allcopy;
    typeCombobox = combobox;
    targetPathDir = path;
    isCancel=false;

}

void CopyThread::run()
{
    int fileCount = queueFileCount();
    progressBar->setRange(0,fileCount);
//    progressBar->setValue(0);

    fileCopy(&targetPathDir);
    emit copyfinish(isCancel);
}

int CopyThread::queueFileCount()
{
    QString image;
    QString script;
    switch (typeCombobox->currentIndex()) {
    case 0:
        image = "render/pub/images";
        script= "render/pub/script";
        break;
    case 1:
        image = "fx/images";
        script= "fx/precomp";
        break;
    }
    int fileCount = 0;
    for(int i =0;i<queueListView->rowCount();i++)
    {
        if(isCancel)
        {
            return 0;
        }
        QTableWidgetItem *pathItem =  queueListView->item(i,3);

        QTableWidgetItem *imagesItem =  queueListView->item(i,1);
        QDir pathDir = pathItem->data(Qt::DisplayRole).toString();
        QDir imagesDir = pathDir.filePath(image);
        imagesDir = imagesDir.filePath(imagesItem->data(Qt::DisplayRole).toString());

        if(imagesDir.exists()){
//            QMessageBox::information(this,"",imagesDir.absolutePath(),QMessageBox::Yes);
//            QString fileCountString;
            fileCount += entryFileCount(imagesDir.absolutePath());
//            QMessageBox::information(this,"",fileCountString.setNum(fileCount),QMessageBox::Yes);

        }
        if(allCopy){
            QTableWidgetItem *scriptsItem = queueListView->item(i,2);
            QDir scriptsPathDir = pathDir.filePath(script);
            QDir scriptsFileDir = scriptsPathDir.filePath(scriptsItem->data(Qt::DisplayRole).toString());
            QFileInfo scriptFile(scriptsFileDir.absolutePath());
            if(scriptFile.exists())
            {
                fileCount++;
            }
            QString movFileName = scriptFile.baseName();
            movFileName = movFileName + ".mov";
            QDir movFileDir = scriptsFileDir = scriptsPathDir.filePath(movFileName);
            QFileInfo movFile(movFileDir.absolutePath());
            if(movFile.exists())
            {
                fileCount++;
    //            QMessageBox::information(this,"",movFileDir.absolutePath(),QMessageBox::Yes);
            }
        }

    }
    return fileCount;
}

void CopyThread::fileCopy(QDir *targetDir)
{
    copyedCount = 0;
    QString image;
    QString script;
    QString target;
    switch (typeCombobox->currentIndex()) {
    case 0:
        image = "render/pub/images";
        script= "render/pub/script";
        target= "3D";
        break;
    case 1:
        image = "fx/images";
        script= "fx/precomp";
        target= "fx";
        break;
    }

    targetDir->mkdir("seq");
    targetDir->cd("seq");
    for(int i =0;i<queueListView->rowCount();i++)
    {
        if(isCancel)
        {
            return;
        }
        QTableWidgetItem *pathItem =  queueListView->item(i,3);
        QTableWidgetItem *imagesItem =  queueListView->item(i,1);

        QString path = pathItem->data(Qt::DisplayRole).toString();
        QStringList pathSplit= path.split("/");

        QString fullTargetPath = targetDir->absolutePath()+"/"+pathSplit[pathSplit.size()-2]+"/"+pathSplit[pathSplit.size()-1]+"/comp/dev/src/"+target;
        targetDir->mkpath(fullTargetPath);
        QDir fullTargetDir(fullTargetPath);
        fullTargetDir.mkdir("../../wip");

        QDir pathDir = path;
        QDir imagesDir = pathDir.filePath(image);
        imagesDir = imagesDir.filePath(imagesItem->data(Qt::DisplayRole).toString());
        fullTargetDir.mkdir("images");
        fullTargetDir.mkdir("script");
        fullTargetDir.cd("images");
        if(imagesDir.exists()){
            QString dateStr="";

            if(typeCombobox->currentData(Qt::DisplayRole).toString()=="FX"){
                 dateStr = QDate::currentDate().toString("yyMMdd");
                 fullTargetDir.mkdir(dateStr);
                 dateStr = "/"+dateStr;
            }
            subCopy(imagesDir.absolutePath(),fullTargetDir.absolutePath()+dateStr,progressBar);
        }


        if(allCopy){
            QTableWidgetItem *scriptsItem = queueListView->item(i,2);
            QDir scriptsPathDir = pathDir.filePath(script);
            QDir scriptsFileDir = scriptsPathDir.filePath(scriptsItem->data(Qt::DisplayRole).toString());
            QFileInfo scriptFile(scriptsFileDir.absolutePath());
            fullTargetDir.cd("../script");
            if(scriptFile.exists()){
                subCopy(scriptFile.absoluteFilePath(),fullTargetDir.absolutePath(),progressBar);

                QString movFileName = scriptFile.baseName();
                movFileName = movFileName + ".mov";
                QDir movFileDir = scriptsFileDir = scriptsPathDir.filePath(movFileName);
                QFileInfo movFile(movFileDir.absolutePath());
                if(movFile.exists())
                {
                    subCopy(movFile.absoluteFilePath(),fullTargetDir.absolutePath(),progressBar);
                }
            }
        }
    }
}

void CopyThread::subCopy(const QString &src, const QString &destPath, QProgressBar *progressBar)
{
    if(isCancel)
    {
        return;
    }
    QFileInfo fileInfo(src);
    QDir destPathDIr(destPath);
    if(fileInfo.isFile()){
        QFile file(fileInfo.filePath());
        file.copy(destPathDIr.filePath(fileInfo.fileName()));
        copyedCount++;
        emit copyCount(copyedCount);
//        progressBar->setValue(progressBar->value()+1);
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
            subCopy(newSrcPath,newDestPath,progressBar);
        }
    }
}

void CopyThread::cancel()
{
    isCancel = true;
}
