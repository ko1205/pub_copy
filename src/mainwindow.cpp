#include "mainwindow.h"
#include <QFileDialog>

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

    QLabel *showPatheLabel = new QLabel("Show Path");
    showPathEdit = new QLineEdit();
    selectPathButton = new QPushButton("...");
    selectPathButton->setFixedWidth(30);
    setShowPathButton = new QPushButton("Set");
    selectProjectCombobox = new QComboBox();
    selectProjectCombobox->addItem("<Select Project>");
    selectProjectCombobox->addItem("test");
    selectProjectCombobox->setCurrentText("test");
    QHBoxLayout *topLineLayout = new QHBoxLayout();

    shotListView = new QListView();
    QeueListView = new QTableWidget();
    QeueListView->setColumnCount(3);
    QeueListView->setRowCount(10);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText("shot");

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText("image");

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText("script");

    QeueListView->setHorizontalHeaderItem(0,item0);
    QeueListView->setHorizontalHeaderItem(1,item1);
    QeueListView->setHorizontalHeaderItem(2,item2);

    QSplitter *ViewSplitter = new QSplitter();

    topLineLayout->addWidget(showPatheLabel);
    topLineLayout->addWidget(showPathEdit);
    topLineLayout->addWidget(selectPathButton);
    topLineLayout->addWidget(setShowPathButton);
    topLineLayout->addWidget(selectProjectCombobox);

    ViewSplitter->addWidget(shotListView);
    ViewSplitter->addWidget(QeueListView);
    QList<int> sizes;
    sizes << 150 << 500;
    ViewSplitter->setSizes(sizes);

    QHBoxLayout *targetPathLayout = new QHBoxLayout();
    QLabel *targetPahtLabel = new QLabel("target Path");
    targetPathEdit = new QLineEdit();
    selectTargetPathButton = new QPushButton("...");
    selectTargetPathButton->setFixedWidth(30);

    targetPathLayout->addWidget(targetPahtLabel);
    targetPathLayout->addWidget(targetPathEdit);
    targetPathLayout->addWidget(selectTargetPathButton);

    copyButton = new QPushButton("Copy");

    baselayout->addLayout(topLineLayout);
    baselayout->addWidget(ViewSplitter);
    baselayout->addLayout(targetPathLayout);
    baselayout->addWidget(copyButton);

    centralWidget->setLayout(baselayout);
    setCentralWidget(centralWidget);

    connect(selectPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedShowPath()));
    connect(selectTargetPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedTargetPath()));
    connect(this,SIGNAL(folderSelected(QLineEdit*)),SLOT(selectDiractory(QLineEdit*)));
}

void MainWindow::clickedShowPath()
{
    emit folderSelected(showPathEdit);
}

void MainWindow::clickedTargetPath()
{
    emit folderSelected(targetPathEdit);
}

void MainWindow::selectDiractory(QLineEdit *edit)
{
    QString folderName = QFileDialog::getExistingDirectory(this,"Select Forder",QDir::home().path());
    folderName = QDir::toNativeSeparators(folderName);
    if(folderName !=""){
        edit->setText(folderName);
    }
}
