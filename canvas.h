#ifndef CANVAS_H
#define CANVAS_H

#include "util.h"
#include "entity.h"
#include <QPainter>
#include <iostream>

struct Canvas
{
    QPainter *painter;
    Coord coord;
    Dim dim;
    float zoom;
    Coord offset;

    std::vector<QRect> elements;
    std::vector<int> IDs;

    void setPainter(QPainter *painter){
        this->painter=painter;
        elements.clear();
        IDs.clear();
    }

    Coord click(int x, int y){
        //point in canvas
        int cx=x-(coord.x+dim.width/2)+offset.x;
        int cy=y-(coord.y+dim.height/2)+offset.y;
        //distance from center of canvas to cursor
        int zx=cx/zoom;
        int zy=cy/zoom;
        return {zx, zy};
    }

    int clickElement(int x, int y){
        for(int i=elements.size()-1;i>=0;i--)
            if(elements.at(i).x()<x&&elements.at(i).x()+elements.at(i).width()>x&&
                elements.at(i).y()<y&&elements.at(i).y()+elements.at(i).height()>y)
                return IDs.at(i);
        return -1;
    }

    bool clickCanvas(int x, int y){
        return x>coord.x&&x<coord.x+dim.width&&y>coord.y&&y<coord.y+dim.height;
    }

    void border(){
        painter->drawLine(coord.x, coord.y, coord.x+dim.width, coord.y);
        painter->drawLine(coord.x, coord.y+dim.height, coord.x+dim.width, coord.y+dim.height);
        painter->drawLine(coord.x, coord.y, coord.x, coord.y+dim.height);
        painter->drawLine(coord.x+dim.width, coord.y, coord.x+dim.width, coord.y+dim.height);
    }

    void write(int x, int y, Qt::Alignment align, std::string text, int textSize, int scale=1, int margin=0){
        QRect rect=QRect(x-textSize*100, y-textSize*100, textSize*200, textSize*200);
        write(rect, align, text, textSize, scale, margin);
    }

    void write(QRect rect, Qt::Alignment align, std::string text, int textSize, int scale=1, int margin=0){
        rect.setRect(rect.x()+margin, rect.y()+margin, rect.width()-margin*2, rect.height()-margin*2);
        //point in canvas
        int px=rect.x()*zoom+coord.x+dim.width/2-offset.x;
        int py=rect.y()*zoom+coord.y+dim.height/2-offset.y;
        int zx=rect.width()*zoom;
        int zy=rect.height()*zoom;

        //QFontMetricsF fontMetrics(painter->font());
        //QRectF textRect = fontMetrics.boundingRect(QRectF(), Qt::TextWordWrap, text);
        //qreal textHeight = textRect.height();
        //QRect textRect = painter.boundingRect(targetRect, Qt::AlignCenter, text);

        QRect rect2=QRect(px, py, zx, zy);

        QFont font("Arial", textSize*scale);
        painter->setFont(font);

        QTextOption to;
        to.setWrapMode(QTextOption::WordWrap);
        to.setAlignment(align);

        painter->drawText(rect2, QString::fromStdString(text), to);
    }

    int print(int x, int y, QPixmap *pmap, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){

        QPixmap *pmap2=new QPixmap();
        *pmap2=*pmap;



        //point in canvas
        float cx=x*zoom+coord.x+dim.width/2-offset.x;
        float cy=y*zoom+coord.y+dim.height/2-offset.y;

        //distance from center to edges of zoomed image
        float zx=(pmap2->width()/2)*zoom*scale;
        float zy=(pmap2->height()/2)*zoom*scale;

        //where to start drawing the zoomed image
        float px=cx-zx;
        float py=cy-zy;

        QSize newSize = pmap2->size() * zoom * scale;
        *pmap2 = pmap2->scaled(newSize, Qt::KeepAspectRatio, Qt::FastTransformation);

        //limits
        float lleft=px;
        float lright=px+pmap2->width();
        float lup=py;
        float ldown=py+pmap2->height();

        //cut
        float cleft=0;
        float cright=0;
        float cup=0;
        float cdown=0;

        if(lleft<coord.x)
            cleft=coord.x-lleft;

        if(lright>coord.x+dim.width)
            cright=lright-(coord.x+dim.width);

        if(lup<coord.y)
            cup=coord.y-lup;

        if(ldown>coord.y+dim.height)
            cdown=ldown-(coord.y+dim.height);

        if(cleft+cright>=pmap2->width()||cup+cdown>=pmap2->height())
            return-1;

        QRect cutRect(cleft, cup, -cleft-cright+pmap2->width(), -cup-cdown+pmap2->height());
        *pmap2 = pmap2->copy(cutRect);

        float pointX=px+cleft;
        float pointY=py+cup;

        /*
        if(align&Qt::AlignLeft){
            pointX+=0;
        }
        if(align&Qt::AlignRight){
            pointX+=-pmap2->width();
        }
        if(align&Qt::AlignHCenter){
            pointX+=-pmap2->width()/2;
        }
        if(align&Qt::AlignTop){
            pointY+=0;
        }
        if(align&Qt::AlignBottom){
            pointY+=-pmap2->height();
        }
        if(align&Qt::AlignVCenter){
            pointY+=-pmap2->height()/2;
        }
*/


        painter->drawPixmap(pointX, pointY, *pmap2);



        QRect element(pointX, pointY, pmap2->width(), pmap2->height());
        elements.push_back(element);
        IDs.push_back(ID);

        delete pmap2;

        return ID;
    }

};

#endif // CANVAS_H
