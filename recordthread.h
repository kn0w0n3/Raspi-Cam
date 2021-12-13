#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QString>
#include <QMutex>
#include <QThread>
#include <QTime>
#include <QtConcurrent>
#include <QDebug>
#include "utilities.h"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video/background_segm.hpp"

using namespace std;

class RecordThread : public QThread{
    Q_OBJECT
public:
    RecordThread(int camera, QMutex *lock);
    RecordThread(QString videoPath, QMutex *lock);
   ~RecordThread() override;
    void setRunning(bool run) {running = run;}
    void startCalcFPS() {fps_calculating = true;}
    enum VideoSavingStatus {STARTING, STARTED, STOPPING, STOPPED};

    void setVideoSavingStatus(VideoSavingStatus status) {video_saving_status = status;}
    void setMotionDetectingStatus(bool status){
        motion_detecting_status = status;
        motion_detected = false;
        if(video_saving_status != STOPPED) video_saving_status = STOPPING;
    }

protected:
    void run() override;

signals:
    void frameCaptured(cv::Mat *data);
    void fpsChanged(double fps);
    void videoSaved(QString name);

private:
    void calculateFPS(cv::VideoCapture &cap);
    void startSavingVideo(cv::Mat &firstFrame);
    void stopSavingVideo();
    void motionDetect(cv::Mat &frame);

private:
    bool running;
    int cameraID;
    QString videoPath;
    QMutex *data_lock;
    cv::Mat frame;

    // FPS calculating
    bool fps_calculating;
    double fps;

    // video saving
    double frame_width, frame_height;
    VideoSavingStatus video_saving_status;
    QString saved_video_name;
    cv::VideoWriter *video_writer;

    // motion analysis
    bool motion_detecting_status;
    bool motion_detected;
    cv::Ptr<cv::BackgroundSubtractorMOG2> segmentor;
};

#endif // RECORDTHREAD_H
