#ifndef TEXTURE_H
#define TEXTURE_H

#include <QPixmap>
#include <QPainter>
#include <vector>
#include <fstream>
#include "util.h"
#include "canvas.h"

struct Texture{
    QPixmap pmap;
    std::string name;
    int offX=0;
    int offY=0;

    Texture(){}

    Texture(QPixmap pmap){
        this->pmap=pmap;
    }

    void scale(float scale){
        QSize newSize = pmap.size() * scale;
        pmap = pmap.scaled(newSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    }

    int print(Canvas *canvas, int x, int y, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){
        return canvas->print(x, y, &pmap, align, scale, ID);
    }

    Dim dimension(){
        return {pmap.width(), pmap.height()};
    }

};

struct TextureList{
    std::vector<Texture> textures;

    void loadTextures(std::string file){
        int sizeX=0;
        int sizeY=0;
        int offX=0;
        int offY=0;

        std::ifstream inputFile(file);

        if (!inputFile.is_open()) {
            return;
        }

        std::string line;

        while (std::getline(inputFile, line)) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);

                if (key == "SizeX"){
                    sizeX = std::stoi(value);
                }
                if (key == "SizeY"){
                    sizeY =  std::stoi(value);
                }
                if (key == "OffX"){
                    offX =  std::stoi(value);
                }
                if (key == "OffY"){
                    offY =  std::stoi(value);
                }
                if (key == "Done"){
                    break;
                }
                if (key == "Texture"){
                    QPixmap pmap=QPixmap("data/textures/"+QString::fromStdString(value)+".png");
                    int rows=pmap.height()/(sizeY+offY*2);
                    int columns=pmap.width()/(sizeX+offX*2);

                    for(int i=0;i<columns;i++){
                        for(int j=0;j<rows;j++){
                            Texture texture;
                            texture.pmap=pmap.copy(i*(sizeX+offX*2), j*(sizeY+offY*2), sizeX+offX*2, sizeY+offY*2);
                            texture.name=value+"_"+std::to_string(i)+"_"+std::to_string(j);
                            texture.offX=offX;
                            texture.offY=offY;
                            textures.push_back(texture);
                        }
                    }
                }
            }
        }

        inputFile.close();
    }

    Texture* selectTexture(std::string name){
        for(int i=0;i<textures.size();i++)
            if(textures.at(i).name==name)
                return &(textures.at(i));
        return nullptr;
    }

    Texture* selectTexture(int ord){
        if(ord<textures.size())
            return &(textures.at(ord));
        else
            return nullptr;
    }




};
/*
struct AnimatedTextureFrame{
    Texture *texture;
    Coord coord;
    //float orientation;

    AnimatedTextureFrame(){
        coord={0, 0};
        //orientation=0;
    }

    void print(Canvas *canvas, int x, int y, float scale){
        texture->print(canvas, x+coord.x, y+coord.y, scale);
    }
};

struct AnimatedTexture{
    std::vector<AnimatedTextureFrame> animatedTextureFrames;
    Coord coord;

    AnimatedTexture(){}

    void addFrame(AnimatedTextureFrame animatedTextureFrame){
        animatedTextureFrames.push_back(animatedTextureFrame);
    }

    void print(Canvas *canvas, int x, int y, float scale, int frame){
        animatedTextureFrames.at(frame%animatedTextureFrames.size()).print(canvas, x+coord.x, y+coord.y, scale);
    }

};
*/

#endif // TEXTURE_H
