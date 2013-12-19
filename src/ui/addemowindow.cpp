#include "addemowindow.h"

AdDemoWindow::AdDemoWindow(QWidget *parent) :
    QWidget(parent)
{

    layout = new QHBoxLayout(this);

    image = QPixmap("ads/adult_male.jpg");
    imageLabel = new QLabel();
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding );
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(image.scaled(imageLabel->width(), imageLabel->height(), Qt::KeepAspectRatio));

    layout->addWidget(imageLabel);

}


 void AdDemoWindow::change_ad(std::string filename)
 {
    std::string concatedname = "ads/" + filename;
    imageLabel->setPixmap(QPixmap(concatedname.c_str()));
    imageLabel->show();

 }
