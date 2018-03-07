#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    createCentralWidget();
//    QHBoxLayout *layouts = new QHBoxLayout;
//    QWidget *widget = new QWidget;

    QLabel *test = new QLabel("Set Show forder");
//    layouts->addWidget(test);
//    widget->setLayout(layouts);
//    setCentralWidget(widget);
//    statusBar()->showMessage("Set Show forder");
    statusBar()->addWidget(test);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu = menuBar()->addMenu(tr("&Edit"));
}

void MainWindow::createCentralWidget()
{
    QWidget *centralWidget = new QWidget();
    QVBoxLayout *baselayout = new QVBoxLayout();
    QSplitter *sp = new QSplitter();
    QLabel *showPatheLabel = new QLabel("Show Path");
    showPathEdit = new QLineEdit();
    selectPathButton = new QPushButton("...");
    setShowPathButton = new QPushButton("Set");
    QHBoxLayout *topLineLayout = new QHBoxLayout();

    topLineLayout->addWidget(showPatheLabel);
    topLineLayout->addWidget(showPathEdit);
    topLineLayout->addWidget(selectPathButton);
    topLineLayout->addWidget(setShowPathButton);

    baselayout->addLayout(topLineLayout);
    centralWidget->setLayout(baselayout);
    setCentralWidget(centralWidget);

}
