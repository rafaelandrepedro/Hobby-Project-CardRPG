#ifndef EFFECT_H
#define EFFECT_H
#include <string>
#include "texture.h"
#include "randomizer.h"


static float rEffectType[]={200,80,
                           180,50,
                           140,60,
                           80,30,
                           7,10,8,
                           4,2,
                           2,5,
                           10};
static float rEffectDuration[]={0,100, 20, 2, 1};

static float effectStrenght[]={1, 1.1,
                                       1, 1.25,
                                       0.8, 0.9,
                                       0.6, 0.75,
                                       0.25, 0.35, 0.15,
                                       0.01, 0.005,
                                       0.60, 0.60,
                                       0.008};





enum EffectType{
    //over time
    PHYSICAL_SHIELD=0,
    MAGIC_SHIELD,
    PHYSICAL_DAMAGE,
    MAGIC_DAMAGE,
    PHYSICAL_ARMOR,
    MAGIC_ARMOR,
    PHYSICAL_PENETRATION,
    MAGIC_PENETRATION,
    SPIKES,
    HEAL,
    BLIND,
    MANA_SURGE,
    MANA_DRAIN,
    LIFE_DRAIN,
    DEEP_WOUNDS,
    ELUSIVE,

    //over time bool
    MARK=100,
    STUN,
    TAUNT,
    IMMORTAL,
    INVULNERABLE
};

enum Target{
    PLAYER,
    FOE
};

const std::string effectTypeNames[2][25] = {{
        "Physical Shield",
        "Magic Shield",
        "Physical Damage",
        "Magic Damage",
        "Physical Armor",
        "Magic Armor",
        "Physical Penetration",
        "Magic Penetration",
        "Spikes",
        "Heal",
        "Blind",
        "Mana Surge",
        "Mana Drain",
        "Life Drain",
        "Deep Wounds",
        "Elusive"
    },{
        "Mark",
        "Stun",
        "Taunt",
        "Immortal",
        "Invulnerable"
    }};

struct Effect{

    std::string name;
    EffectType effectType;
    int intensity;
    int duration;
    Target target;

    Effect(){
        name="Effect";
        duration=0;
    }

    std::string text(){
        std::string str;
        if(target==PLAYER)
            if((int)effectType<100)
                str="Grant you "+std::to_string(intensity)+" "+effectTypeNames[0][effectType]+" for "+std::to_string(duration)+" turns. ";
            else
                str="Grant you "+effectTypeNames[1][effectType-100]+" for "+std::to_string(duration)+" turns. ";
        else
            if((int)effectType<100)
                str="Grant the foe "+std::to_string(intensity)+" "+effectTypeNames[0][effectType]+" for "+std::to_string(duration)+" turns. ";
            else
                str="Grant the foe "+effectTypeNames[1][effectType-100]+" for "+std::to_string(duration)+" turns. ";
        return str;
    }

    int print(Canvas *canvas, int x, int y, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1, int layout=0){
        Texture t;
        switch(effectType){
        case PHYSICAL_DAMAGE:
            t=Texture(QPixmap("data/e1.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MAGIC_DAMAGE:
            t=Texture(QPixmap("data/e2.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case PHYSICAL_SHIELD:
            t=Texture(QPixmap("data/e3.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MAGIC_SHIELD:
            t=Texture(QPixmap("data/e4.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case PHYSICAL_ARMOR:
            t=Texture(QPixmap("data/e5.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MAGIC_ARMOR:
            t=Texture(QPixmap("data/e6.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case PHYSICAL_PENETRATION:
            t=Texture(QPixmap("data/e7.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MAGIC_PENETRATION:
            t=Texture(QPixmap("data/e8.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case SPIKES:
            t=Texture(QPixmap("data/e9.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case HEAL:
            t=Texture(QPixmap("data/e10.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case BLIND:
            t=Texture(QPixmap("data/e11.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MANA_SURGE:
            t=Texture(QPixmap("data/e12.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case MANA_DRAIN:
            t=Texture(QPixmap("data/e13.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case LIFE_DRAIN:
            t=Texture(QPixmap("data/e14.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case DEEP_WOUNDS:
            t=Texture(QPixmap("data/e15.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        case ELUSIVE:
            t=Texture(QPixmap("data/e16.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
            break;

        default:
            break;
        }

        if(target==FOE){
            t=Texture(QPixmap("data/red.png"));
            ID=t.print(canvas, x, y, align, scale, ID);
        }

        if(layout==0){
            int offsetX=t.pmap.width()/2*0.80*scale;
            int offsetY=t.pmap.height()/2*0.80*scale;

            t=Texture(QPixmap("data/n2.png"));
            ID=t.print(canvas, x+offsetX, y+offsetY, align, scale/2, ID);
            QRect rect=QRect(x+offsetX-50, y+offsetY+-50, 100, 100);
            canvas->write(x+offsetX, y+offsetY, Qt::AlignCenter, std::to_string(intensity), scale/2, scale, 0);

            t=Texture(QPixmap("data/n1.png"));
            ID=t.print(canvas, x+offsetX, y-offsetY, align, scale/2, ID);
            QRect rect2=QRect(x+offsetX-50, y-offsetY-50, 100, 100);
            canvas->write(x+offsetX, y-offsetY, Qt::AlignCenter, std::to_string(duration), scale/2, scale, 0);
        }
        else{
            int offsetX1=t.pmap.width()/2*2*scale;
            int offsetX2=t.pmap.width()/2*4*scale;

            t=Texture(QPixmap("data/n2.png"));
            ID=t.print(canvas, x+offsetX1, y, align, scale, ID);
            QRect rect1=QRect(x+offsetX1-50, y-50, 100, 100);
            canvas->write(x+offsetX1, y, Qt::AlignCenter, std::to_string(intensity), scale*4, scale, 0);

            t=Texture(QPixmap("data/n1.png"));
            ID=t.print(canvas, x+offsetX2, y, align, scale, ID);
            QRect rect2=QRect(x+offsetX2-50, y-50, 100, 100);
            canvas->write(x+offsetX2, y, Qt::AlignCenter, std::to_string(duration), scale*4, scale, 0);
        }
        return ID;
    }



    static Effect create(DeckBuilder deckBuilder, float points=1.0){
        Effect ef;
        ef.effectType=(EffectType)(Randomizer::Srand(deckBuilder.rEffectType));
        ef.duration=Randomizer::Srand(deckBuilder.rEffectDuration);
        ef.intensity=(rand()%5+15) * abs(points) * deckBuilder.effectStrenght[ef.effectType]/1000;

        if(!ef.intensity){
            ef.intensity=1;
            ef.duration=1;
        }

        if(points>0)
            ef.target=PLAYER;
        else
            ef.target=FOE;

        return ef;
    }


};

#endif // EFFECT_H
