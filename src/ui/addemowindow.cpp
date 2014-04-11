#include "addemowindow.h"

AdDemoWindow::AdDemoWindow(QWidget *parent) :
    QWidget(parent)
{

    layout = new QHBoxLayout(this);

    image = new QPixmap("ads//adult_male.jpg");
    imageLabel = new QLabel(this);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
    imageLabel->setScaledContents(true);
    imageLabel->setGeometry(100,100,image->width(),image->height());

    layout->addWidget(imageLabel);

}


 void AdDemoWindow::change_ad(std::string filename)
 {
    std::string concatedname = "ads/" + filename;
    imageLabel->setPixmap(QPixmap(concatedname.c_str()));
    imageLabel->show();

 }
