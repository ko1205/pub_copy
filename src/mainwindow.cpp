#include "mainwindow.h"
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>
#include "common.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    createCentralWidget();
    setQueueList();

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
    connect(copyButton,SIGNAL(clicked(bool)),this,SLOT(startCopy()));

///////////////////////////////////////////////////////////////////

//    QFile file("C:/Temp/copy_test/abc.txt");
//    file.copy("C:/Temp/copy_test2/abc_copy.txt");
//    QDir dir;
//    dir.mkpath("C:/Temp/copy_test/test/test/test");
//    subCopy("C:/Temp/copy_test/","C:/Temp/copy_test2");
//    int fileCount = entryFileCount("Z:\\show\\baolin\\seq\\shot\\shot_0036\\render\\pub\\images\\shot_0036_render_v02_w01");
//    QString test;
//    QMessageBox::information(this,"",test.setNum(fileCount),QMessageBox::Yes);
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
//    queueListView->setColumnCount(3);
//    queueListView->setRowCount(10);

//    QTableWidgetItem *item0 = new QTableWidgetItem;
//    item0->setText("shot");

//    QTableWidgetItem *item1 = new QTableWidgetItem;
//    item1->setText("image");

//    QTableWidgetItem *item2 = new QTableWidgetItem;
//    item2->setText("script");

//    queueListView->setHorizontalHeaderItem(0,item0);
//    queueListView->setHorizontalHeaderItem(1,item1);
//    queueListView->setHorizontalHeaderItem(2,item2);

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
    QLabel *targetPahtLabel = new QLabel("Target Path");
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

void MainWindow::setQueueList()
{
        queueListView->setColumnCount(4);
        queueListView->setRowCount(0);
//        queueListView->setSortingEnabled(1);
        queueListView->sortByColumn(0,Qt::AscendingOrder);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText("shot");

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setText("image");

        QTableWidgetItem *item2 = new QTableWidgetItem;
        item2->setText("script");

        QTableWidgetItem *item3 = new QTableWidgetItem;
        item3->setText("Path");

        queueListView->setHorizontalHeaderItem(0,item0);
        queueListView->setHorizontalHeaderItem(1,item1);
        queueListView->setHorizontalHeaderItem(2,item2);
        queueListView->setHorizontalHeaderItem(3,item3);

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
//        queueListView->setRowCount(list.size());
        for(int i = 0; i < list.size();i++)
        {
//            QTableWidgetItem *item = new QTableWidgetItem();
            QDir projectDir = dir.absoluteFilePath(list.at(i));
            QDir seqDir = projectDir.filePath("seq");
            if(seqDir.exists()){
//                item->setText(projectDir.absolutePath());
//                queueListView->setItem(i,0,item);
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
        int queueListRowCount = queueListView->rowCount();
        for(int i=0;i<queueListRowCount;i++)
        {
            queueListView->removeRow(0);
        }

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
        insertQueue(item);
//        QMessageBox::information(this,"check",name,QMessageBox::Yes);
//        QAbstractItemModel *model = queueListView->model();
//        queueListView->sortByColumn(0,Qt::AscendingOrder);
//        model->removeRow(3);
//        queueListView->findItems()
//        queueListView->row();

    }else{
        disconnect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        item->setBackgroundColor(QColor(Qt::white));
        connect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        removeQueue(item);
//        QMessageBox::information(this,"uncheck",name,QMessageBox::Yes);
    }

}

void MainWindow::startCopy()
{
    if(targetPathEdit->text()=="")
    {
        QMessageBox::warning(this,"Warning","Set Target Path!!!",QMessageBox::Yes);
    }else{
        QDir targetPathDir = targetPathEdit->text();
        if(!targetPathDir.exists()){
            QMessageBox::warning(this,"Warning","invisibla Path!!",QMessageBox::Yes);
        }else if(queueListView->rowCount()==0){
            QMessageBox::warning(this,"Warning","Empty QueueList",QMessageBox::Yes);
        }else{
            int fileCount = queueFileCount();
            QString fileCountString;
//            QMessageBox::information(this,"",fileCountString.setNum(100),QMessageBox::Yes);
        }
    }
}

void MainWindow::insertQueue(QListWidgetItem *item)
{
    QString shot = item->data(Qt::DisplayRole).toString();
    QString path = item->data(Qt::UserRole+1).toString();
    QDir pathDir = path;
    QDir imagePathDir = pathDir.filePath("render/pub/images");
    QDir scriptPathDir = pathDir.filePath("render/pub/script");

    QStringList imagesList = imagePathDir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    imagesList.removeOne("_tmp");
    imagesList.removeOne("temp");
    imagesList.removeOne(".vrayThumbs");
//    if(1 < imagesList.count())
//    {
//        for(int i=0;i<imagesList.count(); i++)
//        {
//            QMessageBox::information(this,"",imagesList.at(i),QMessageBox::Yes);
//        }
//    }

    QStringList nukeFilter;
    nukeFilter<< "*.nk";
    QStringList scriptList = scriptPathDir.entryList(nukeFilter,QDir::Files);
//    if(1 < scriptList.count())
//    {
//        for(int i=0;i<scriptList.count(); i++)
//        {
//            QMessageBox::information(this,"",scriptList.at(i),QMessageBox::Yes);
//        }
//    }
    queueListView->setRowCount(queueListView->rowCount()+1);
//    QMessageBox::information(this,"",pathDir.absolutePath(),QMessageBox::Yes);
    if( 1 == imagesList.count() && 1 == scriptList.count())
    {

        QTableWidgetItem *scriptItem = new QTableWidgetItem();
        QTableWidgetItem *imagesItem = new QTableWidgetItem();

        scriptItem->setText(scriptList.constFirst());
        imagesItem->setText(imagesList.constFirst());

        queueListView->setItem(queueListView->rowCount()-1,1,imagesItem);
        queueListView->setItem(queueListView->rowCount()-1,2,scriptItem);


    }else{
        SelectMultiDialog dialog;
        dialog.setDialog(imagesList,scriptList,queueListView);
        dialog.exec();

    }
    QTableWidgetItem *shotNameItem = new QTableWidgetItem();
    QTableWidgetItem *pathItem = new QTableWidgetItem();
    shotNameItem->setText(shot);
    pathItem->setText(path);
    queueListView->setItem(queueListView->rowCount()-1,0,shotNameItem);
    queueListView->setItem(queueListView->rowCount()-1,3,pathItem);
    queueListView->sortByColumn(0,Qt::AscendingOrder);
}

void MainWindow::removeQueue(QListWidgetItem *item)
{
    QString shot = item->data(Qt::DisplayRole).toString();
    QList<QTableWidgetItem *> queueItmes = queueListView->findItems(shot,Qt::MatchExactly);
    int row = queueListView->row(queueItmes.constFirst());
    queueListView->model()->removeRow(row);
}

int MainWindow::queueFileCount()
{
    int fileCount = 100;
    for(int i =0;i<queueListView->rowCount();i++)
    {
        QTableWidgetItem *pathItem =  queueListView->item(i,3);
        QTableWidgetItem *imagesItem =  queueListView->item(i,1);
        QDir pathDir = pathItem->data(Qt::DisplayRole).toString();
        QDir imagesDir = pathDir.filePath("render/pub/images");
        imagesDir = imagesDir.filePath(imagesItem->data(Qt::DisplayRole).toString());

        if(imagesDir.exists()){
            QMessageBox::information(this,"",imagesDir.absolutePath(),QMessageBox::Yes);
            QStringList imagesFileList = imagesDir.entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
            fileCount = imagesFileList.size();
            QString fileCountString;
//            QMessageBox::information(this,"",fileCountString.setNum(fileCount),QMessageBox::Yes);
            for(int m =0; m < imagesFileList.size();m++)
            {
                QMessageBox::information(this,"",fileCountString.setNum(fileCount),QMessageBox::Yes);
            }
        }

    }
    return fileCount;
}

SelectMultiDialog::SelectMultiDialog(QWidget *parent)
    :QDialog(parent)
{
    imageList = new QListWidget();
    scriptList = new QListWidget();
    selectButton = new QPushButton("Select OK");
    QVBoxLayout *baseLayout = new QVBoxLayout();
    QSplitter *viewerSplitter = new QSplitter(Qt::Vertical);
    viewerSplitter->addWidget(imageList);
    viewerSplitter->addWidget(scriptList);
    baseLayout->addWidget(viewerSplitter);
    baseLayout->addWidget(selectButton);
    setLayout(baseLayout);
}

SelectMultiDialog::~SelectMultiDialog()
{

}

void SelectMultiDialog::setDialog(QStringList images, QStringList scripts,QTableWidget *queueList)
{
    queueTable = queueList;
    for(int i = 0;i<images.count();i++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(images.at(i));
        imageList->addItem(item);
    }
    imageList->sortItems(Qt::DescendingOrder);
    imageList->setCurrentRow(0,QItemSelectionModel::Select);

    for(int i = 0;i<scripts.count();i++)
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(scripts.at(i));
        scriptList->addItem(item);
    }
    scriptList->sortItems(Qt::DescendingOrder);
    scriptList->setCurrentRow(0,QItemSelectionModel::Select);
    connect(selectButton,SIGNAL(clicked(bool)),this,SLOT(selectSetQueue()));
}

void SelectMultiDialog::selectSetQueue()
{
    QTableWidgetItem *scriptItem = new QTableWidgetItem();
    QTableWidgetItem *imagesItem = new QTableWidgetItem();


    imagesItem->setText(imageList->currentItem()->data(Qt::DisplayRole).toString());
    scriptItem->setText(scriptList->currentItem()->data(Qt::DisplayRole).toString());

    queueTable->setItem(queueTable->rowCount()-1,1,imagesItem);
    queueTable->setItem(queueTable->rowCount()-1,2,scriptItem);
    this->done(0);
}
