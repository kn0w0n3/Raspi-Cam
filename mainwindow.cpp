#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : cameras) {
        qDebug() << "Info: " << cameraInfo.deviceName();
        //if (cameraInfo.deviceName() == "mycamera")
            //camera = new QCamera(cameraInfo);
    }

    camera = new QCamera;

    viewFinder = new QCameraViewfinder();
    setCentralWidget(viewFinder);
    viewFinder->show();

    camera->setViewfinder(viewFinder);

    //imageCapture = new QCameraImageCapture(camera);

    //camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();


    recorder = new QMediaRecorder(camera);
    QVideoEncoderSettings videoSettings;
    videoSettings.setCodec("video/mpeg2");
    videoSettings.setResolution(640, 480);
    recorder->setVideoSettings(videoSettings);
    //recorder->setContainerFormat("mp4");

    QAudioEncoderSettings audioSettings;
    //audioSettings.setCodec("audio/amr");
    //audioSettings.setQuality(QMultimedia::HighQuality);

    recorder->setAudioSettings(audioSettings);

    //recorder->setOutputLocation("/home/pi/Videos");
    recorder->setOutputLocation(QUrl::fromLocalFile("/home/pi/Desktop/v.mp2"));
    recorder->record();


}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    recorder->stop();
}
