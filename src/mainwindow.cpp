#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QHeaderView>
#include "common.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();
    createCentralWidget();
    setQueueList();
    resize(800,500);

    statusMessage = new QLabel("Set Show forder");

    progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignCenter);
    cancelButton =new QPushButton("Cancel");
//    cancelButton->setEnabled(false);
//    progressBar->setRange(0,300);
//    progressBar->setValue(50);
    statusBar()->addWidget(statusMessage);
    statusBar()->addWidget(progressBar);
    statusBar()->addWidget(cancelButton);
    connect(selectPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedShowPath()));
    connect(selectTargetPathButton,SIGNAL(clicked(bool)),this,SLOT(clickedTargetPath()));
    connect(this,SIGNAL(folderSelected(QLineEdit*)),SLOT(selectDiractory(QLineEdit*)));
    connect(setShowPathButton,SIGNAL(clicked(bool)),this,SLOT(setShowPath()));
    connect(selectProjectCombobox,SIGNAL(currentTextChanged(QString)),this,SLOT(projectChange()));
    connect(typeCombobox,SIGNAL(currentTextChanged(QString)),this,SLOT(projectChange()));
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    setting->setValue("path",showPath);
    setting->setValue("type",typeCombobox->currentText());
    event->accept();
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
//    selectProjectCombobox->addItem("test");
//    selectProjectCombobox->setCurrentText("test");
    QHBoxLayout *topLineLayout = new QHBoxLayout();

    typeCombobox = new QComboBox();
    typeCombobox->addItem("Render");
    typeCombobox->addItem("FX");
//    typeCombobox->setEnabled(false);

    shotListView = new QListWidget();
    queueListView = new QTableWidget();

    QVBoxLayout *LeftLayout = new QVBoxLayout();
    LeftLayout->addWidget(typeCombobox);
    LeftLayout->addWidget(shotListView);
    QWidget *LeftWidget = new QWidget();
    LeftWidget->setLayout(LeftLayout);
    LeftLayout->setMargin(0);

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

    ViewSplitter->addWidget(LeftWidget);
    ViewSplitter->addWidget(queueListView);
    QList<int> sizes;
    sizes << 150 << 800;
    ViewSplitter->setSizes(sizes);

    QHBoxLayout *targetPathLayout = new QHBoxLayout();
    selectCopyGroup = new QGroupBox("Select Copy");

//    selectCopyGroup->setEnabled(false);

    QVBoxLayout *selectCopyLayout = new QVBoxLayout();
    allCopyButton = new QRadioButton("All Copy");
    imageCopyButton = new QRadioButton("Images Copy");
    selectCopyLayout->addWidget(allCopyButton);
    selectCopyLayout->addWidget(imageCopyButton);
    selectCopyGroup->setLayout(selectCopyLayout);

    connect(allCopyButton,SIGNAL(toggled(bool)),this,SLOT(toggledAllCopy(bool)));
    connect(imageCopyButton,SIGNAL(toggled(bool)),this,SLOT(toggledImageCopy(bool)));

    allCopyButton->toggle();

    QLabel *targetPahtLabel = new QLabel("Target Path");
    targetPathEdit = new QLineEdit();
    selectTargetPathButton = new QPushButton("...");
    selectTargetPathButton->setFixedWidth(30);

    targetPathLayout->addWidget(selectCopyGroup);
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

    setting = new QSettings("Pub_copy.ini",QSettings::IniFormat);
    showPathEdit->setText(setting->value("path").toString());
    showPath = setting->value("path").toString();
    typeCombobox->setCurrentText(setting->value("type").toString());
    setWindowTitle("Pub Copy v1.0 Bata");


}

void MainWindow::setQueueList()
{
    QStringList tableLabels;
    tableLabels << "shot" << "images" << "script" << "path";
    queueListView->setColumnCount(tableLabels.size());
    queueListView->setHorizontalHeaderLabels(tableLabels);
//    queueListView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    queueListView->horizontalHeader()->setStretchLastSection(true);
    queueListView->setColumnWidth(0,80);
    queueListView->setColumnWidth(1,180);
    queueListView->setColumnWidth(2,180);
//        queueListView->setRowCount(0);
//        queueListView->setSortingEnabled(1);
//        queueListView->sortByColumn(0,Qt::AscendingOrder);

//        QTableWidgetItem *item0 = new QTableWidgetItem;
//        item0->setText("shot");

//        QTableWidgetItem *item1 = new QTableWidgetItem;
//        item1->setText("image");

//        QTableWidgetItem *item2 = new QTableWidgetItem;
//        item2->setText("script");

//        QTableWidgetItem *item3 = new QTableWidgetItem;
//        item3->setText("Path");

//        queueListView->setHorizontalHeaderItem(0,item0);
//        queueListView->setHorizontalHeaderItem(1,item1);
//        queueListView->setHorizontalHeaderItem(2,item2);
//        queueListView->setHorizontalHeaderItem(3,item3);

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
            QDir renderDir = shotDir.filePath(typeCombobox->currentText());
            if(renderDir.exists())
            {
               shotList.append(shotDir.absolutePath());
            }
        }
//        shotList.append(shotDirList);
    }
    return shotList;
}

void MainWindow::projectChange()
{
    QDir path = showPath;
    QString project = selectProjectCombobox->currentText();
    path = path.filePath(project);

    shotListView->clear();
    int queueListRowCount = queueListView->rowCount();
    for(int i=0;i<queueListRowCount;i++)
    {
        queueListView->removeRow(0);
    }


    if(path.exists()){
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
        statusMessage->setText("Check Shot");
    }

}

void MainWindow::checkShotList(QListWidgetItem *item)
{
    QString name = item->data(Qt::UserRole+1).toString();
    if(item->checkState())
    {
        disconnect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        item->setBackgroundColor(QColor(Qt::lightGray));
        connect(shotListView,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(checkShotList(QListWidgetItem*)));
        if(!insertQueue(item)){
            item->setCheckState(Qt::Unchecked);
        }
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
            copyThread = new CopyThread(this);
            connect(copyThread,SIGNAL(copyCount(int)),this,SLOT(copyedCount(int)));
            connect(copyThread,SIGNAL(finished()),this,SLOT(finishCopy()));
            connect(cancelButton,SIGNAL(clicked(bool)),copyThread,SLOT(cancel()));
            connect(copyThread,SIGNAL(copyfinish(bool)),this,SLOT(isCancel(bool)));
            copyThread->setThread(queueListView,progressBar,allCopy,typeCombobox,targetPathDir);
            copyThread->start();

            m_isCancel = false;
            copyButton->setEnabled(false);
            selectCopyGroup->setEnabled(false);
            targetPathEdit->setEnabled(false);
            shotListView->setEnabled(false);
            queueListView->setEnabled(false);
            selectPathButton->setEnabled(false);
            selectTargetPathButton->setEnabled(false);
            setShowPathButton->setEnabled(false);
            typeCombobox->setEnabled(false);
            selectProjectCombobox->setEnabled(false);

//            int fileCount = queueFileCount();
//            QString fileCountString;
//            QMessageBox::information(this,"",fileCountString.setNum(fileCount),QMessageBox::Yes);
//            progressBar->setRange(0,fileCount);
//            progressBar->setValue(0);
//            fileCopy(&targetPathDir);
//            QMessageBox::information(this,"Finish","Copy Finish",QMessageBox::Yes);
        }
    }
}

void MainWindow::fileCopy(QDir *targetDir)
{
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

//            subCopy(imagesDir.absolutePath(),fullTargetDir.absolutePath(),progressBar);
        }


        if(allCopy){
            QTableWidgetItem *scriptsItem = queueListView->item(i,2);
            QDir scriptsPathDir = pathDir.filePath(script);
            QDir scriptsFileDir = scriptsPathDir.filePath(scriptsItem->data(Qt::DisplayRole).toString());
            QFileInfo scriptFile(scriptsFileDir.absolutePath());
            fullTargetDir.cd("../script");
            if(scriptFile.exists()){
//                subCopy(scriptFile.absoluteFilePath(),fullTargetDir.absolutePath(),progressBar);

                QString movFileName = scriptFile.baseName();
                movFileName = movFileName + ".mov";
                QDir movFileDir = scriptsFileDir = scriptsPathDir.filePath(movFileName);
                QFileInfo movFile(movFileDir.absolutePath());
                if(movFile.exists())
                {
//                    subCopy(movFile.absoluteFilePath(),fullTargetDir.absolutePath(),progressBar);
                }
            }
        }
    }
}

bool MainWindow::insertQueue(QListWidgetItem *item)
{
    QString image;
    QString script;
    switch (typeCombobox->currentIndex()) {
    case 0:
        image = "render/pub/images";
        script= "render/pub/script";
        break;
    case 1:
        image = "fx/pub/images";
        script= "fx/pub/precomp";
        break;
    }

    QString shot = item->data(Qt::DisplayRole).toString();
    QString path = item->data(Qt::UserRole+1).toString();
    QDir pathDir = path;
    QDir imagePathDir = pathDir.filePath(image);
    QDir scriptPathDir = pathDir.filePath(script);

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
    if(0 == imagesList.count() && 0 == scriptList.count()){
        QMessageBox::information(this,"","Empty Folder",QMessageBox::Yes);
        return false;

    }else{
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

        return true;
    }
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

void MainWindow::toggledAllCopy(bool on)
{
    if(on){
        allCopy = true;
//        QMessageBox::information(this,"","toggled All Copy",QMessageBox::Yes);
    }

}

void MainWindow::toggledImageCopy(bool on)
{
    if(on){
        allCopy = false;
//        QMessageBox::information(this,"","toggled image Copy",QMessageBox::Yes);
    }

}

void MainWindow::copyedCount(int copyCount)
{
    progressBar->setValue(copyCount);
}

void MainWindow::isCancel(bool on)
{
    m_isCancel = on;
}

void MainWindow::finishCopy()
{
//    disconnect(copyThread,SIGNAL(copyCount(int)),this,SLOT(copyedCount(int)));
//    disconnect(copyThread,SIGNAL(finished()),this,SLOT(finishCopy()));
//    disconnect(cancelButton,SIGNAL(clicked(bool)),copyThread,SLOT(cancel()));
//    disconnect(copyThread,SIGNAL(copyfinish(bool)),this,SLOT(isCancel(bool)));

    copyButton->setEnabled(true);
    selectCopyGroup->setEnabled(true);
    targetPathEdit->setEnabled(true);
    shotListView->setEnabled(true);
    queueListView->setEnabled(true);
    selectPathButton->setEnabled(true);
    selectTargetPathButton->setEnabled(true);
    setShowPathButton->setEnabled(true);
    typeCombobox->setEnabled(true);
    selectProjectCombobox->setEnabled(true);

    if(m_isCancel){
        QMessageBox::information(this,"Cancel","Copy Cancel",QMessageBox::Yes);
    }else{
        QMessageBox::information(this,"Finish","Copy Finish",QMessageBox::Yes);
    }
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

