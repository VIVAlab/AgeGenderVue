#ifndef ADDEMOWINDOW_H
#define ADDEMOWINDOW_H

class AdDemoWindow;

#include <QWidget>
#include <QtGui>

#include "demomanagerwindow.h"


class AdDemoWindow : public QWidget
{
    Q_OBJECT
public:
    AdDemoWindow(QWidget *parent);
    
signals:
    
public slots:
    void change_ad(std::string filename);

private:
    QLabel* imageLabel;
    QPixmap* image;

    QHBoxLayout* layout;
    
};

#endif // ADDEMOWINDOW_H
