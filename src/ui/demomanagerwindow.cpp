#include "demomanagerwindow.h"


DemoManagerWindow::DemoManagerWindow(QWidget *parent) :
    QWidget(parent)
{

     faceTrackingButton = new QPushButton("Launch tracking demo");
     adDemoButton = new QPushButton("Launch ad demo");


     layout = new QHBoxLayout(this);
     facetrackingLayout = new QVBoxLayout();
     adDemoLayout = new QVBoxLayout();
     layout->addLayout(facetrackingLayout);
     layout->addLayout(adDemoLayout);


     faceTrackingDescription = new QLabel("Demonstrates real-time face tracking and age+gender recognition.");
     adDemoDescription = new QLabel("Shows a proof of concept of targeted adds.");

     facetrackingLayout->addWidget(faceTrackingButton);
     facetrackingLayout->addWidget(faceTrackingDescription);

     adDemoLayout->addWidget(adDemoButton);
     adDemoLayout->addWidget(adDemoDescription);

     connect(faceTrackingButton, SIGNAL(clicked()), this, SLOT(launch_tracking_demo()));
     connect(adDemoButton, SIGNAL(clicked()), this, SLOT(launch_ad_demo()));



     facetracking_window = new FaceTrackingWindow(NULL, this);
     adDemoWindow = new AdDemoWindow(NULL);



  //   this->show();
}


DemoManagerWindow::~DemoManagerWindow()
{
    delete faceTrackingButton;
    delete facetracking_window;

    delete adDemoButton;
    delete adDemoWindow;
}


void DemoManagerWindow::launch_tracking_demo()
{
    facetracking_window->show();
}

void DemoManagerWindow::launch_ad_demo()
{
    adDemoWindow->show();
}

void DemoManagerWindow::face_detected(DetectionInformation detectedFace)
{

    if (detectedFace.getGender() && detectedFace.getAge() == 1)
        adDemoWindow->change_ad("teen_male.jpg");
    else if (detectedFace.getGender() && detectedFace.getAge() == 3)
        adDemoWindow->change_ad("senior_male.jpg");
    else if (!detectedFace.getGender() && detectedFace.getAge() == 3)
        adDemoWindow->change_ad("senior_female.jpg");
    else if (!detectedFace.getGender() && detectedFace.getAge() == 1)
        adDemoWindow->change_ad("teen_female.jpg");
    else if (detectedFace.getGender())
        adDemoWindow->change_ad("adult_male.jpg");
    else if (!detectedFace.getGender())
        adDemoWindow->change_ad("adult_female.jpg");



}
