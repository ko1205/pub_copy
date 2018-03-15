#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QProgressBar>
#include <QTableWidget>
#include <QComboBox>
#include <QDir>

class CopyThread : public QThread
{
    Q_OBJECT
public:
    CopyThread(QObject *parent = 0);
    ~CopyThread();

    void setThread(QTableWidget *tableWidget,QProgressBar *progressbar,bool allcopy,QComboBox *combobox,QDir path);

public slots:
    void cancel();

signals:
    void copyCount(int);
    void copyfinish(bool);

private:
    void run();
    int queueFileCount();
    void fileCopy(QDir *targetDir);
    void subCopy(const QString &src, const QString &destPath, QProgressBar *progressBar);


    QProgressBar *progressBar;
    QTableWidget *queueListView;
    QComboBox *typeCombobox;
    QDir targetPathDir;

    bool allCopy;
    int copyedCount;
    bool isCancel;
};

#endif // COPYTHREAD_H
