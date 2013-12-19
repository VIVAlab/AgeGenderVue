#include "facetrackingwindow.h"

FaceTrackingWindow::FaceTrackingWindow(QWidget *parent, DemoManagerWindow *windowManager) :
    QWidget(parent)
{

   stop = false;

   teenMales = 0;
   teenFemales = 0;
   adultMales = 0;
   adultFemales = 0;
   seniorMales = 0;
   seniorFemales = 0;

   detections = 0;


   //Open the webcam. The number represents the webcam ID, 0 is the default.
   capture.open(1);


   //cv::namedWindow("Output", cv::WINDOW_NORMAL);
   //cv::setWindowProperty("Output", cv::WND_PROP_FULLSCREEN, cv::WINDOW_AUTOSIZE);

   //cv::resizeWindow("Output", 1920, 1080);


   if(! capture.isOpened())
   {
       throw "No capture";
   }


  frameToShow = NULL;




  imageLabel = new QLabel();
  imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
  imageLabel->setScaledContents(true);


  layout = new QVBoxLayout(this);
  layout->addWidget(imageLabel);


  stats_widget = new QWidget();
  stats_layout = new QHBoxLayout(stats_widget);
  stats_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
  layout->addWidget(stats_widget);






  this->setLayout(layout);


  teen_male_label = new QLabel("Teen males : 0");
  teen_female_label = new QLabel("Teen females : 0");
  adult_male_label = new QLabel("Adult males : 0");
  adult_female_label = new QLabel("Adult females : 0");
  senior_male_label = new QLabel("Senior males : 0");
  senior_female_label = new QLabel("Senior females : 0");
  total_detection_label = new QLabel("Detections : 0");


  teen_male_label->setFont(QFont( "Arial", 18));
  teen_female_label->setFont(QFont( "Arial", 18));
  adult_male_label->setFont(QFont( "Arial", 18));
  adult_female_label->setFont(QFont( "Arial", 18));
  senior_male_label->setFont(QFont( "Arial", 18));
  senior_female_label->setFont(QFont( "Arial", 18));
  total_detection_label->setFont(QFont( "Arial", 24, QFont::Bold));


  teen_stats_layout = new QVBoxLayout();
  adult_stats_layout = new QVBoxLayout();
  senior_stats_layout = new QVBoxLayout();

  stats_layout->addWidget(total_detection_label);


  stats_layout->addLayout(teen_stats_layout);
  stats_layout->addLayout(adult_stats_layout);
  stats_layout->addLayout(senior_stats_layout);

  teen_stats_layout->addWidget(teen_male_label);
  teen_stats_layout->addWidget(teen_female_label);
  adult_stats_layout->addWidget(adult_male_label);
  adult_stats_layout->addWidget(adult_female_label);
  senior_stats_layout->addWidget(senior_male_label);
  senior_stats_layout->addWidget(senior_female_label);


  faceTracker.init(true, true);

  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(timer_tick()));

  timer->start(40);

  connect(this, SIGNAL(faceDetected(DetectionInformation)), windowManager, SLOT(face_detected(DetectionInformation)));



}


FaceTrackingWindow::~FaceTrackingWindow()
{
   //CLOSE the video file.  called by destructor:

    delete objectTracker;
    delete frameToShow;
    capture.release();
}

void FaceTrackingWindow::timer_tick()
{
    //read next frame if any
    if(!capture.read(frame))
    {
        cout << "Could not read frame" << endl;
        return;
    }

    cv::Mat result = faceTracker.track(frame);

    DetectionInformation best = faceTracker.getBestDetection();

    //Good detection
    if (best.getAge() != -1)
    {
        detections++;
        if (best.getGender() && best.getAge() == 1)
        {
            teenMales++;
        }
        else if (best.getGender() && best.getAge() == 3)
        {
            seniorMales++;
        }
        else if (!best.getGender() && best.getAge() == 3)
        {
            seniorFemales++;
        }
        else if (!best.getGender() && best.getAge() == 1)
        {
            teenFemales++;
        }
        else if (best.getGender())
        {
            adultMales++;
        }
        else if (!best.getGender())
        {
            adultFemales++;
        }

        stringstream sstm;
        sstm << "Teen males : " << teenMales;
        teen_male_label->setText(sstm.str().c_str());


        stringstream sstf;
        sstf << "Teen females : " << teenFemales;
        teen_female_label->setText(sstf.str().c_str());

        stringstream ssam;
        ssam << "Adult males : " << adultMales;
        adult_male_label->setText(ssam.str().c_str());

        stringstream ssaf;
        ssaf << "Adult females : " << adultFemales;
        adult_female_label->setText(ssaf.str().c_str());

        stringstream sssm;
        sssm << "Senior males : " << seniorMales;
        senior_male_label->setText(sssm.str().c_str());

        stringstream sssf;
        sssf << "Senior females : " << seniorFemales;
        senior_female_label->setText(sssf.str().c_str());


        stringstream ds;
        ds << "Detections : " << detections;
        total_detection_label->setText(ds.str().c_str());


        emit faceDetected(best);
    }


    stats_widget->setFixedHeight(this->height()*0.15);

    cv::cvtColor(result, convertFrame,cv::COLOR_BGR2RGB);

    delete frameToShow;
    frameToShow = NULL;
    frameToShow =  new QImage((uchar*)convertFrame.data, convertFrame.cols, convertFrame.rows, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(*frameToShow));

    imageLabel->show();


}


//

