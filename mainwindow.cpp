#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    data_lock = new QMutex();
    //Create the graphics scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,790,490);

    //Set the scene on the graphics view
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    openCamera();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::openCamera(){
    //if(capturer != nullptr){
        // if a thread is already running, stop it
        //capturer->setRunning(false);
        //disconnect(capturer, &RecordThread::frameCaptured, this, &MainWindow::updateFrame);
        //disconnect(capturer, &RecordThread::fpsChanged, this, &MainWindow::updateFPS);
        //disconnect(capturer, &RecordThread::videoSaved, this, &MainWindow::appendSavedVideo);
        //connect(capturer, &RecordThread::finished, capturer, &RecordThread::deleteLater);
    //}
    // I am using my second camera whose Index is 2.  Usually, the
    // Index of the first camera is 0.
    int camID = 0;
    capturer = new RecordThread(camID, data_lock);
    connect(capturer, &RecordThread::frameCaptured, this, &MainWindow::updateFrame);
    //connect(capturer, &RecordThread::fpsChanged, this, &MainWindow::updateFPS);
    //connect(capturer, &RecordThread::videoSaved, this, &MainWindow::appendSavedVideo);
    capturer->start();
    //mainStatusLabel->setText(QString("Capturing Camera %1").arg(camID));
    //monitorCheckBox->setCheckState(Qt::Unchecked);
    //recordButton->setText("Record");
    //recordButton->setEnabled(true);
}

void MainWindow::updateFrame(cv::Mat *mat){
    data_lock->lock();
    currentFrame = *mat;
    data_lock->unlock();

    QImage frame(
        currentFrame.data,
        currentFrame.cols,
        currentFrame.rows,
        currentFrame.step,
        QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(frame);

    scene->clear();
    ui->graphicsView->resetMatrix();
    scene->addPixmap(image);
    scene->update();
    scene->setSceneRect(image.rect());

}
