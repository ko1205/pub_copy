#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QSplitter>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QListView>
#include <QListWidget>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <QDialog>
#include <QProgressBar>
#include <QDir>
#include <QSettings>
#include <QRadioButton>
#include <QGroupBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void folderSelected(QLineEdit *edit);


private:
    void createMenus();
    void createCentralWidget();
    void setQueueList();
    void insertQueue(QListWidgetItem *item);
    void removeQueue(QListWidgetItem *item);
    int queueFileCount();
    void fileCopy(QDir *targetDir);

    QStringList searchShotfolder(QString project);

    QMenu *fileMenu;
    QString showPath;
    QLineEdit *showPathEdit;
    QPushButton *selectPathButton;
    QPushButton *setShowPathButton;
    QComboBox *selectProjectCombobox;

    QLineEdit *targetPathEdit;
    QPushButton *selectTargetPathButton;

    QComboBox *typeCombobox;
    QListWidget *shotListView;
    QTableWidget *queueListView;

    QPushButton *copyButton;

    QLabel *statusMessage;
    QProgressBar *progressBar;
    QSettings *setting;

private slots:
    void clickedShowPath();
    void clickedTargetPath();
    void selectDiractory(QLineEdit *edit);
    void setShowPath();
    void projectChange();
    void checkShotList(QListWidgetItem *item);
    void startCopy();


};

class SelectMultiDialog : public QDialog
{
    Q_OBJECT
public:
    SelectMultiDialog(QWidget *parent=0);
    ~SelectMultiDialog();
    void setDialog(QStringList images,QStringList scripts,QTableWidget *queueList);

private:
    QListWidget *imageList;
    QListWidget *scriptList;
    QPushButton *selectButton;
    QTableWidget *queueTable;

private slots:
    void selectSetQueue();

};

#endif // MAINWINDOW_H
