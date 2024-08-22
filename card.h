#ifndef CARD_H
#define CARD_H
#include "texture.h"
#include "entity.h"
#include "effect.h"
#include "randomizer.h"

static float rNumEffects[]={0,6,4,2};
static float rCost[]={1,100,10,1};
static float rType[]={2,1,1,1,1};

enum CardType{
    GREY,
    RED,
    YELLOW,
    BLUE,
    GREEN
};

struct Card:public Entity{
    Texture cover;
    Texture back;

    bool revealed;

    std::vector<Effect> effects;

    int cost;

    bool banished;
    bool voided;

    CardType type;





    Card(){
        this->revealed=true;
        cost=1;
        type=GREY;
        banished=false;
        voided=false;
    }



    Card* copy(){
        Card *card=new Card();
        card->cover=cover;
        card->back=back;
        card->cost=cost;
        card->type=type;
        card->banished=banished;
        card->voided=voided;
        for(int i=0;i<effects.size();i++)
            card->effects.push_back(effects.at(i));
        return card;
    }

    void setEffect(Effect effect){
        this->effects.push_back(effect);
    }

    void text(){
        for(int i=0;i<this->effects.size();i++)
            std::cout<<this->effects.at(i).text()<<std::endl;
    }

    int print(Canvas *canvas, int x, int y, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){
        if(revealed){
            ID=cover.print(canvas, x, y, align, scale, ID);
            Dim dim=cover.dimension();
            dim.width*=scale;
            dim.height*=scale;
            QRect rect=QRect(x-dim.width/2*0.9, y-dim.height/2*0.9, dim.width*0.9, dim.height*0.9);
            std::string str="";


            int offsetX=dim.width/2*1.0;
            int offsetY=dim.height/2*1.0;

            Texture t=Texture(QPixmap("data/n3.png"));
            ID=t.print(canvas, x-offsetX, y-offsetY, align, scale, ID);
            canvas->write(x-offsetX, y-offsetY, Qt::AlignCenter, std::to_string(cost), scale, scale*2, 0);


            for(int i=0;i<effects.size();i++){
                str=" "+effects.at(i).text();
                float textSize=2.5*scale;
                effects.at(i).print(canvas, rect.x()+25*textSize, rect.y()+35*textSize, align, scale*3/4, ID, 1);
                canvas->write(rect, Qt::AlignTop|Qt::AlignLeft, str, textSize, scale, 10);
                rect.setTop(rect.top()+50*textSize);
            }

            if(banished&&!voided){
                str=" Banished";
                float textSize=2.5*scale;
                canvas->write(rect, Qt::AlignTop|Qt::AlignLeft, str, textSize, scale, 10);
            }

            if(voided){
                str=" Voided";
                float textSize=2.5*scale;
                canvas->write(rect, Qt::AlignTop|Qt::AlignLeft, str, textSize, scale, 10);
            }

        }
        else
            back.print(canvas, x, y, align, scale, ID);

        return ID;
    }

    int printEffects(Canvas *canvas, int x, int y, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){
        for(int i=0;i<effects.size();i++)
            effects.at(i).print(canvas, x, y-i*250, align, scale, ID);
        return ID;
    }

    static Card* create(DeckBuilder deckBuilder, float difficulty=1.0){
        Effect ef;
        Card* card=new Card();

        card->back=Texture(QPixmap("data/back2.png"));

        int type=Randomizer::Srand(deckBuilder.rType);
        card->type=(CardType)type;
        card->cover=Texture(QPixmap("data/cover"+QString::number(type)+".png"));
        if(type!=0)
            difficulty*=1.0;

        int cost=Randomizer::Srand(deckBuilder.rCost);
        card->cost=cost;

        if(rand()%10==0){
            difficulty*=1.15;
            card->banished=true;
        }

        int numEffects=Randomizer::Srand(deckBuilder.rNumEffects);



        if(numEffects>1&&rand()%5==0&&difficulty>0){
            ef=Effect::create(deckBuilder, -difficulty/numEffects*(cost+0.1));
            card->setEffect(ef);
            for(int j=1;j<numEffects;j++){
                ef=Effect::create(deckBuilder, (difficulty/numEffects+difficulty/(numEffects*(numEffects-1)))*(cost+0.1));
                card->setEffect(ef);
            }
        }
        else{
            for(int j=0;j<numEffects;j++){
                ef=Effect::create(deckBuilder, difficulty/numEffects*(cost+0.1));
                card->setEffect(ef);
            }
        }
        return card;
    }

};

#endif // CARD_H
