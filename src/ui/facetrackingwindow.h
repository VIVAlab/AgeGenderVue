#ifndef FACETRACKINGWINDOW_H
#define FACETRACKINGWINDOW_H


class FaceTrackingWindow;

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QTimer>
#include <QtWidgets/QLabel>
#include <QtWidgets/QsizePolicy>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <iostream>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <direct.h>

#include "objecttracker.h"
#include "utils.h"

#include "facetracker.h"
#include "demomanagerwindow.h"


using namespace std;
using namespace cv;

class FaceTrackingWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FaceTrackingWindow(QWidget *parent = 0, DemoManagerWindow *windowManager = 0);
    ~FaceTrackingWindow();


    QImage* frameToShow;
    QLabel* imageLabel;


    Mat frame; //current video frame
    Mat convertFrame;

    bool stop;


    VideoCapture capture;


    ObjectTracker* objectTracker;



    FaceTracker faceTracker;

signals:
    
public slots:
    

private slots:
    void timer_tick_m();
    void timer_tick_w();

    void toggle_recording();

signals:
    void faceDetected(DetectionInformation detectedFace);

private:
    QVBoxLayout* layout;
    QHBoxLayout* stats_layout;
    QWidget* stats_widget;

    QVBoxLayout* teen_stats_layout;
    QVBoxLayout* adult_stats_layout;
    QVBoxLayout* senior_stats_layout;

    QLabel* teen_male_label;
    QLabel* teen_female_label;
    QLabel* adult_male_label;
    QLabel* adult_female_label;
    QLabel* senior_male_label;
    QLabel* senior_female_label;

    QLabel* total_detection_label;

    QPushButton* recordButton;

    int teenMales;
    int teenFemales;
    int adultMales;
    int adultFemales;
    int seniorMales;
    int seniorFemales;
    int detections;
    int number;

    bool recording;
    bool lock_writter;

    VideoWriter* videoOutput;

    vector<vector <TrackedFace> >  db_stack;

    void push_back_db();
    void frame_flip(Mat &frame);
    bool write_db();

};

#endif // FACETRACKINGWINDOW_H