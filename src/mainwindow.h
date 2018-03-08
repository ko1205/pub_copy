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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void folderSelected(QLineEdit *edit);


private:
    void createMenus();
    void createCentralWidget();
    void setQueueList();
    void insertQueue(QListWidgetItem *item);
    void removeQueue(QListWidgetItem *item);

    QStringList searchShotfolder(QString project);

    QMenu *fileMenu;
    QString showPath;
    QLineEdit *showPathEdit;
    QPushButton *selectPathButton;
    QPushButton *setShowPathButton;
    QComboBox *selectProjectCombobox;

    QLineEdit *targetPathEdit;
    QPushButton *selectTargetPathButton;


    QListWidget *shotListView;
    QTableWidget *queueListView;

    QPushButton *copyButton;

    QLabel *statusMessage;

private slots:
    void clickedShowPath();
    void clickedTargetPath();
    void selectDiractory(QLineEdit *edit);
    void setShowPath();
    void projectChange(QString project);
    void checkShotList(QListWidgetItem *item);


};

class SelectMultiDialog : public QDialog
{
    Q_OBJECT
public:
    SelectMultiDialog(QWidget *parent=0);
    ~SelectMultiDialog();

private:

};

#endif // MAINWINDOW_H
