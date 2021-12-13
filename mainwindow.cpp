#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    data_lock = new QMutex();
    this->setFixedSize(945,602);
    //Create the graphics scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,650,490);

    //Set the scene for the graphics view
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    openCamera();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::openCamera(){
    int camID = 0;
    capturer = new RecordThread(camID, data_lock);
    connect(capturer, &RecordThread::frameCaptured, this, &MainWindow::updateFrame);
    capturer->start();
}

void MainWindow::updateFrame(cv::Mat *mat){
    //data_lock->lock();
    currentFrame = *mat;
    //data_lock->unlock();

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

void MainWindow::on_recordButton_clicked(){
    if(isRecording == false){
        isRecording = true;
        ui->recordButton->setText("Stop");
        capturer->setVideoSavingStatus(RecordThread::STARTING);
        capturer->setRunning(true);
    }
    else if(isRecording == true){
        isRecording = false;
        ui->recordButton->setText("Record");
        capturer->setVideoSavingStatus(RecordThread::STOPPING);
        capturer->setRunning(false);
        //delete capturer;
        openCamera();
    }
}
