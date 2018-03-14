#ifndef COMMON_H
#define COMMON_H

#include <QDir>
#include <QFile>
#include <QProgressBar>

void subCopy(const QString &src,const QString &destPath,QProgressBar *progressBar);
int entryFileCount(const QString &srcPath);

#endif // COMMON_H
