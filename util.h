#ifndef UTIL_H
#define UTIL_H

#include <QtMultimedia>
#include <QMediaPlayer>
#include <QString>

struct Coord{
    int x;  //coordenada X
    int y;  //coordenada Y

    static bool compare(const Coord& c1, const Coord& c2){
        if(c1.y==c2.y)
            return c1.x<c2.x;
        else
            return c1.y<c2.y;
    }
};

struct Dim{
    Dim(){
        this->width=1;
        this->height=1;
    }
    Dim(int width, int height){
        this->width=width;
        this->height=height;
    }

    int width;  //coordenada X
    int height;  //coordenada Y
};



enum Ori{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void play(QString filename);





#endif // UTIL_H
