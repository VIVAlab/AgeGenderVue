#include "facetrackingwindow.h"

FaceTrackingWindow::FaceTrackingWindow(QWidget *parent, DemoManagerWindow *windowManager) :
        QWidget(parent)
{
    recording = false;
    stop = false;
    lock_writter=false;

    teenMales = 0;
    teenFemales = 0;
    adultMales = 0;
    adultFemales = 0;
    seniorMales = 0;
    seniorFemales = 0;

    detections = 0;

    //Open the webcam. The number represents the webcam ID, 0 is the default.
    capture.open(0);



    if(! capture.isOpened())
    {
        throw "No capture";
    }


    std::cout<<mkdir("Database")<<std::endl;


    frameToShow = NULL;

    number=0;


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


    recordButton = new QPushButton("Start recording");


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

    stats_layout->addWidget(recordButton);
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
    QTimer *timer_write = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(timer_tick_m()));
    connect(timer_write, SIGNAL(timeout()), this, SLOT(timer_tick_w()));
    timer->start(20);
    timer_write->start(5000);

    connect(this, SIGNAL(faceDetected(DetectionInformation)), windowManager, SLOT(face_detected(DetectionInformation)));
    connect(recordButton, SIGNAL(clicked()), this, SLOT(toggle_recording()));

    videoOutput = NULL;

}

void FaceTrackingWindow::toggle_recording()
{
    if (recording)
    {
        recordButton->setText("Start recording");
        recording = false;
        delete videoOutput;
    }
    else
    {

        recordButton->setText("Stop recording");
        videoOutput = new VideoWriter();
        videoOutput->open("Recording.avi",
                          CV_FOURCC('M', 'J', 'P', 'G'),
                          15,
                          Size(640, 480)
                          );

        if (!videoOutput->isOpened())
        {
            throw "Could not open the output video for write";
        }

        recording = true;
    }
}

FaceTrackingWindow::~FaceTrackingWindow()
{
    //CLOSE the video file.  called by destructor:

    delete objectTracker;
    delete frameToShow;
    capture.release();
}

void FaceTrackingWindow::frame_flip(Mat &frame){

    Point center = Point( frame.cols/2, frame.rows/2 );
    double angle = 90.0;
    double scale = 1;

    Mat rot_mat = getRotationMatrix2D( center, angle, scale );
    Mat rot_mat2 = getRotationMatrix2D( center, -1.f*angle, scale );
    warpAffine( frame, frame, rot_mat, frame.size());
    flip(frame,frame,0);
    warpAffine( frame, frame, rot_mat2, frame.size());
}



void FaceTrackingWindow::timer_tick_m()
{
    //read next frame if any
    if(!capture.read(frame))
    {
        cout << "Could not read frame" << endl;
        return;
    }
    frame_flip(frame);
    cv::Mat result = faceTracker.track(frame);
    vector<DetectionInformation> bestv = faceTracker.getBestDetection_m();
    push_back_db();
    for(int i=0;i<bestv.size();i++){
            DetectionInformation best=bestv[i];

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


        }
    }

    if (recording)
    {
        (*videoOutput).write(result);
    }
    stats_widget->setFixedHeight(this->height()*0.15);



    cv::cvtColor(result, convertFrame,cv::COLOR_BGR2RGB);

    delete frameToShow;
    frameToShow = NULL;
    frameToShow =  new QImage((uchar*)convertFrame.data, convertFrame.cols, convertFrame.rows, QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(*frameToShow));

    imageLabel->show();

}

void FaceTrackingWindow::timer_tick_w()
{
    if(write_db() && !lock_writter){
        db_stack.clear();
    }
}
void FaceTrackingWindow::push_back_db()
{
    if(!lock_writter){
        vector<TrackedFace> temp;
        for(int i=0;i<faceTracker.objectTracker->trackedFaces.size();i++){
            temp.push_back(*faceTracker.objectTracker->trackedFaces[i]);
        }
        db_stack.push_back(temp);
    }

}
bool FaceTrackingWindow::write_db()
{

    ostringstream convert;   // stream used for the conversion
    convert << number;
    string name="datafile_"+convert.str();
    lock_writter=true;
    FILE * file = fopen (("Database//"+name+".csv").c_str(),"w");
    for(int i=0;i<db_stack.size();i++){


        for(int j=0;j<db_stack[i].size();j++){
            if(j==0)
                fprintf(file, "%s,",db_stack[i][0].time_app.c_str());
            fprintf(file, "P%d,%s,%s,",j,
                    (db_stack[i].at(j).getCurrentGender()?"Male":"Female"),
                    (db_stack[i].at(j).getCurrentAge()==1?"Teen":
                     (db_stack[i].at(j).getCurrentAge()==2?"Adult":"Senior")));
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    number++;
    number=(number%10);
    fclose(file);
    lock_writter=false;
    return true;
}
