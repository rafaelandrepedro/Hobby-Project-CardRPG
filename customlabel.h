#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

#include "game.h"







class CustomLabel : public QLabel
{
    Q_OBJECT

public:

    Game game;
    QTimer *timer;

    CustomLabel(QWidget* parent = nullptr) : QLabel(parent) {
        setMouseTracking(true);

        // Create a QTimer instance
        timer = new QTimer(this);

        // Connect the timeout signal of the timer to your custom slot
        connect(timer, &QTimer::timeout, this, &CustomLabel::onTimeout);

        // Set the interval for the timer (in milliseconds)
        timer->start(20);


    }



protected:

    void onTimeout()
    {
        //std::cout<<"timer"<<std::endl;
    }



    void paintEvent(QPaintEvent* event) override {
        //parentWidget()->resize(340, 340);
        QLabel::paintEvent(event);

        // Now, you can draw additional content on top of the image.
        QPainter painter(this);




        game.print(&painter);



        update();
    }

    void mousePressEvent(QMouseEvent* event) override {
        std::cout<<"Click "<<event->x()<<" "<<event->y()<<std::endl;
        game.mousePressEvent(event);

        if(game.activity==0)
            close();

        update();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {

    }

    void mouseMoveEvent(QMouseEvent* event) override {

        update();
    }

    void wheelEvent(QWheelEvent *event) override{

    }

    //void resizeEvent(QResizeEvent* event) override {

    //}
};



#endif // CUSTOMLABEL_H
