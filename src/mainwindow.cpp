#include "mainwindow.h"
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    createCentralWidget();

    statusMessage = new QLabel("Set Show forder");

    QProgressBar *progressBar = new QProgressBar();
    progressBar->setRange(0,100);
    progressBar->setValue(50);
    statusBar()->addWidget(statusMessage);
    statusBar()->addWidget(progressBar);
    connect(selectPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedShowPath()));
    connect(selectTargetPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedTargetPath()));
    connect(this,SIGNAL(folderSelected(QLineEdit*)),SLOT(selectDiractory(QLineEdit*)));
    connect(setShowPathButton,SIGNAL(clicked(bool)),this,SLOT(setShowPath()));
    connect(selectProjectCombobox,SIGNAL(currentTextChanged(QString)),this,SLOT(projectChange(QString)));
    connect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
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

    shotListView = new QListWidget();
    queueListView = new QTableWidget();
    queueListView->setColumnCount(3);
    queueListView->setRowCount(10);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText("shot");

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText("image");

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText("script");

    queueListView->setHorizontalHeaderItem(0,item0);
    queueListView->setHorizontalHeaderItem(1,item1);
    queueListView->setHorizontalHeaderItem(2,item2);

    QSplitter *ViewSplitter = new QSplitter();

    topLineLayout->addWidget(showPatheLabel);
    topLineLayout->addWidget(showPathEdit);
    topLineLayout->addWidget(selectPathButton);
    topLineLayout->addWidget(setShowPathButton);
    topLineLayout->addWidget(selectProjectCombobox);

    ViewSplitter->addWidget(shotListView);
    ViewSplitter->addWidget(queueListView);
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

void MainWindow::setShowPath()
{
    QStringList list;
    showPath = showPathEdit->text();
    if(showPath!=""){
        selectProjectCombobox->clear();
        selectProjectCombobox->addItem("<Select Project>");
        QDir dir = showPath;
        list = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        queueListView->setRowCount(list.size());
        for(int i = 0; i < list.size();i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            QDir projectDir = dir.absoluteFilePath(list.at(i));
            QDir seqDir = projectDir.filePath("seq");
            if(seqDir.exists()){
                item->setText(projectDir.absolutePath());
                queueListView->setItem(i,0,item);
                selectProjectCombobox->addItem(list.at(i));
            }
        }
        if(selectProjectCombobox->count()!=1){
            showPathEdit->setEnabled(false);
            statusMessage->setText("Select Project");
            selectProjectCombobox->setEnabled(true);

        }else{
            QMessageBox::warning(this,"warning","No has Project folder",QMessageBox::Yes);
            selectProjectCombobox->setEnabled(false);
        }

    }else{
        QMessageBox::warning(this,"warning","select show folder",QMessageBox::Yes);
        selectProjectCombobox->setEnabled(false);
    }

}

QStringList MainWindow::searchShotfolder(QString project)
{
    QStringList shotList;
    QDir showDir = showPath;
    QDir projectDir = showDir.filePath(project);
    projectDir = projectDir.filePath("seq");
    QStringList seqList = projectDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    for(int i = 0; i < seqList.size(); i++)
    {
        QDir seqDir = projectDir.filePath(seqList.at(i));
        QStringList shotDirList = seqDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
        for(int n = 0; n < shotDirList.size(); n++)
        {
            QDir shotDir = seqDir.filePath(shotDirList.at(n));
            QDir renderDir = shotDir.filePath("render");
            if(renderDir.exists())
            {
               shotList.append(shotDir.absolutePath());
            }
        }
//        shotList.append(shotDirList);
    }
    return shotList;
}

void MainWindow::projectChange(QString project)
{
    QDir path = showPath;
    path = path.filePath(project);
    if(path.exists()){
        shotListView->clear();

        QStringList shotList = searchShotfolder(project);
        for(int i =0;i < shotList.size(); i++)
        {
            QDir shotDir = shotList.at(i);
            QListWidgetItem *item = new QListWidgetItem(shotDir.dirName());
            item->setData(Qt::UserRole+1,shotDir.absolutePath());
            item->setFlags(item->flags()|Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            shotListView->addItem(item);
        }
    }
    statusMessage->setText("Check Shot");
}

void MainWindow::checkShotList(QListWidgetItem *item)
{
    QString name = item->data(Qt::UserRole+1).toString();
    if(item->checkState())
    {
        disconnect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        item->setBackgroundColor(QColor(Qt::lightGray));
        connect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        QMessageBox::information(this,"check",name,QMessageBox::Yes);
    }else{
        disconnect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        item->setBackgroundColor(QColor(Qt::white));
        connect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        QMessageBox::information(this,"uncheck",name,QMessageBox::Yes);
    }

}
