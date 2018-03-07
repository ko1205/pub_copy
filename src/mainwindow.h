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
#include <QTableWidget>

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

    QMenu *fileMenu;
    QString *showPath;
    QLineEdit *showPathEdit;
    QPushButton *selectPathButton;
    QPushButton *setShowPathButton;
    QComboBox *selectProjectCombobox;

    QLineEdit *targetPathEdit;
    QPushButton *selectTargetPathButton;


    QListView *shotListView;
    QTableWidget *QeueListView;

    QPushButton *copyButton;

private slots:
    void clickedShowPath();
    void clickedTargetPath();
    void selectDiractory(QLineEdit *edit);


};

#endif // MAINWINDOW_H
