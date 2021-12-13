#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"
#include <QGraphicsScene>
#include "recordthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openCamera();
    void updateFrame(cv::Mat*);
    void on_recordButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    cv::Mat currentFrame;

    // for capture thread
    QMutex *data_lock;
    RecordThread *capturer;
    bool isRecording = false;
};

#endif // MAINWINDOW_H
