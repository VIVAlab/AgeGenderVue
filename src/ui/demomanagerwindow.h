#ifndef DEMOMANAGERWINDOW_H
#define DEMOMANAGERWINDOW_H

class DemoManagerWindow;

#include <QtWidgets/QWidget>
#include <QtGui>

#include "facetrackingwindow.h"
#include "addemowindow.h"
#include "detectioninformation.h"


class DemoManagerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DemoManagerWindow(QWidget *parent = 0);
    ~DemoManagerWindow();

private:


    QHBoxLayout* layout;
    QVBoxLayout* facetrackingLayout;
    QVBoxLayout* adDemoLayout;

    QPushButton* faceTrackingButton;
    FaceTrackingWindow* facetracking_window;
    QLabel* faceTrackingDescription;

    QPushButton* adDemoButton;
    AdDemoWindow* adDemoWindow;
    QLabel* adDemoDescription;

signals:
    
public slots:
    void launch_tracking_demo();
    void launch_ad_demo();

    void face_detected(DetectionInformation detectedFace);
};

#endif // DEMOMANAGERWINDOW_H
