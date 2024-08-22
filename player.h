#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "deck.h"
#include <QString>
#include <iomanip>

struct Player{
    int HP;
    int maxHP;
    std::vector<Effect> effects;

    int actions;

    int cardBonus[5];

    CardType lastType=GREY;
    int combo=0;


    DeckBuilder deckBuilder;

    float playerDifficulty=1.0;



    Deck mainDeck;
    Deck handDeck;
    Deck discardDeck;
    Deck banishedDeck;
    Deck voidedDeck;



    //temporary status:
    int physicalDamage;
    int magicDamage;
    int physicalShield;
    int magicShield;
    int physicalArmor;
    int magicArmor;
    int physicalPenetration;
    int magicPenetration;
    int spikes;
    int heal;
    int blind;
    int manaSurge;
    int manaDrain;
    int lifeDrain;
    int deepWounds;
    int elusive;


    int mark;
    int stun;
    int taunt;
    int immortal;
    int invulnerable;


    Player(){
        HP=1;
        actions=3;
        for(int i=0;i<5;i++)
            cardBonus[i]=0;
    }

    void reset(std::string deckFile, std::string playerFile){
        handDeck.revealed=true;
        mainDeck.revealed=false;
        discardDeck.revealed=false;
        banishedDeck.revealed=false;
        clearAppliedEffects();
        mainDeck.clear();

        loadPlayer(playerFile);
        mainDeck.loadDeck(deckFile);
        if(mainDeck.size()<4)
            mainDeck=Deck::create(deckBuilder, playerDifficulty, 5+playerDifficulty);

        refillHand();
    }

    void refillHand(){
        Deck::shuffle(mainDeck);

        while(handDeck.size()<4){
            if(mainDeck.size()==0){
                Deck::shuffle(discardDeck);
                while(discardDeck.size()!=0)
                    Deck::draw(discardDeck, mainDeck);
            }
            Deck::draw(mainDeck, handDeck);
        }
    }

    void discardHand(){
        while(handDeck.size()!=0){
            Deck::draw(handDeck, discardDeck);
        }
    }

    void joinDeck(){
        while(discardDeck.size()!=0)
            Deck::draw(discardDeck, mainDeck);
        while(handDeck.size()!=0)
            Deck::draw(handDeck, mainDeck);
        while(banishedDeck.size()!=0)
            Deck::draw(banishedDeck, mainDeck);
        voidedDeck.clear();

        for(int i=0;i<mainDeck.size();)
            if(mainDeck.cards.at(i)->voided){
                mainDeck.cards.erase(mainDeck.cards.begin()+i);
                continue;
            }
            else{
                i++;
            }
    }

    void setHP(int HP){
        this->HP=HP;
        if(this->HP>maxHP)
            this->HP=maxHP;
    }

    void changeHP(int HP){
        this->HP+=HP;
        if(this->HP>maxHP)
            this->HP=maxHP;
    }

    bool dead(){
        return HP<=0;
    }

    void loadPlayer(std::string file){

        std::ifstream inputFile(file);

        if (!inputFile.is_open()) {
            return;
        }

        int i=0;

        deckBuilder.rEffectType.clear();
        deckBuilder.rEffectDuration.clear();
        deckBuilder.rNumEffects.clear();
        deckBuilder.rCost.clear();
        deckBuilder.rType.clear();
        deckBuilder.effectStrenght.clear();

        std::string line;

        while (std::getline(inputFile, line)) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);


                if (key == "CB"){
                    cardBonus[i]=std::stoi(value);
                    i++;
                }

                if (key == "HP"){
                    HP=std::stoi(value);
                }

                if (key == "MHP"){
                    maxHP=std::stoi(value);
                }

                if (key == "rEffectType"){
                    deckBuilder.rEffectType.push_back(std::stoi(value));
                }

                if (key == "rEffectDuration"){
                    deckBuilder.rEffectDuration.push_back(std::stoi(value));
                }

                if (key == "rNumEffects"){
                    deckBuilder.rNumEffects.push_back(std::stoi(value));
                }

                if (key == "rCost"){
                    deckBuilder.rCost.push_back(std::stoi(value));
                }

                if (key == "rType"){
                    deckBuilder.rType.push_back(std::stoi(value));
                }

                if (key == "effectStrenght"){
                    deckBuilder.effectStrenght.push_back(std::stoi(value));
                }
                if (key == "Dif"){
                    playerDifficulty=(float)std::stoi(value)/1000;
                }
            }
        }

        inputFile.close();
    }

    void savePlayer(std::string file){


        std::ofstream outputFile;

        outputFile.open(file);

        if (!outputFile.is_open()) {
            return;
        }

        for(int i=0;i<5;i++)
                outputFile<<"CB: "<<cardBonus[i]<<"\n";
        outputFile<<"HP: "<<HP<<"\n";
        outputFile<<"MHP: "<<maxHP<<"\n";
        outputFile<<"Dif: "<<playerDifficulty*1000<<"\n";

        for(int i=0;i<16;i++)
            outputFile<<"rEffectType: "<<deckBuilder.rEffectType.at(i)<<"\n";
        for(int i=0;i<5;i++)
            outputFile<<"rEffectDuration: "<<deckBuilder.rEffectDuration.at(i)<<"\n";
        for(int i=0;i<4;i++)
            outputFile<<"rNumEffects: "<<deckBuilder.rNumEffects.at(i)<<"\n";
        for(int i=0;i<4;i++)
            outputFile<<"rCost: "<<deckBuilder.rCost.at(i)<<"\n";
        for(int i=0;i<5;i++)
            outputFile<<"rType: "<<deckBuilder.rType.at(i)<<"\n";
        for(int i=0;i<16;i++)
            outputFile<<"effectStrenght: "<<deckBuilder.effectStrenght.at(i)<<"\n";


        outputFile.close();
    }

    bool playCard(Player *otherPlayer, int i){
        return playCard(otherPlayer, handDeck.cards.at(i));
    }

    bool playCard(Player *otherPlayer, Card *card){


        if(card->cost>actions){
            return false;
        }

        actions-=card->cost;

        for(int p=0;p<card->effects.size();p++)
            playEffect(otherPlayer,card->effects.at(p));

        if(card->type==lastType){
            if(combo<3)
                combo++;
        }
        else{
            combo=1;
            lastType=card->type;
        }

        for(int i=0;i<5;i++){
            if(i==(int)card->type){
                cardBonus[i]+=combo;
            }
            else{
                if(cardBonus[i]>0)
                    cardBonus[i]-=1;
            }
        }

        if(card->banished)
            Deck::draw(handDeck, banishedDeck, card->ID);
        else if(card->voided)
            Deck::draw(handDeck, voidedDeck, card->ID);
        else
            Deck::draw(handDeck, discardDeck, card->ID);

        return true;
    }

    void playEffect(Player *otherPlayer, Effect effect){
        if(effect.target==PLAYER)
            this->effects.push_back(effect);
        else
            otherPlayer->effects.push_back(effect);
    }

    void text(){
        for(int i=0;i<this->effects.size();i++)
            std::cout<<this->effects.at(i).text()<<std::endl;
    }

    int printHP(Canvas *canvas, int x, int y){
        canvas->write(x, y, Qt::AlignCenter, std::to_string(HP), 5, 5, 0);
        return 0;
    }

    int printActions(Canvas *canvas, int x, int y){
        canvas->write(x, y, Qt::AlignCenter, std::to_string(actions), 5, 5, 0);
        return 0;
    }

    int printBonus(Canvas *canvas, int x, int y){
        for(int i=1;i<5;i++){
            Texture t=Texture(QPixmap("data/bonus1.png"));
            t.print(canvas, x, y+i*100, Qt::AlignCenter, 2, -1);
            std::ostringstream stream;
            stream<<std::setprecision(2)<<(float)cardBonus[i]/2<<" %";
            std::string str=stream.str();
            canvas->write(x, y+i*100, Qt::AlignCenter, str, 4, 2, 0);
        }
        return 0;
    }


    int printEffects(Canvas *canvas, int x, int y, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){
        for(int i=0;i<effects.size();i++)
            effects.at(i).print(canvas, x+i*300, y, align, scale, ID);
        return 0;
    }

    void clearAppliedEffects(){
        physicalDamage = 0;
        magicDamage = 0;
        physicalShield = 0;
        magicShield = 0;
        physicalArmor = 0;
        magicArmor = 0;
        physicalPenetration = 0;
        magicPenetration = 0;

        spikes = 0;
        heal = 0;

        blind = 0;
        manaSurge = 0;
        manaDrain = 0;

        lifeDrain = 0;
        deepWounds = 0;

        elusive = 0;


        mark = false;
        stun = false;
        taunt = false;
        immortal = false;
        invulnerable = false;
    }

    static void resolveEffects(Player &p1, Player &p2){

        if(!p1.stun)
            if(!p2.invulnerable){
                float penRatio=0;
                float armorRatio=0;

                int preMitDmgDealt=0;

                //damage
                int physicalDmgDealt=p1.physicalDamage;
                float physicalPenetration=p1.physicalPenetration/5;
                float physicalArmor=p2.physicalArmor/5;
                penRatio=10/(10+physicalPenetration);
                armorRatio=physicalArmor/(5+physicalPenetration);
                physicalDmgDealt=physicalDmgDealt/(penRatio+armorRatio);

                if(p1.mark)
                    physicalDmgDealt*=2;

                preMitDmgDealt+=physicalDmgDealt;
                physicalDmgDealt-=p2.physicalShield;
                if(physicalDmgDealt<0)
                    physicalDmgDealt=0;



                int magicDmgDealt=p1.magicDamage;
                float magicPenetration=p1.magicPenetration/5;
                float magicArmor=p2.magicArmor/5;
                penRatio=10/(10+magicPenetration);
                armorRatio=magicArmor/(5+magicPenetration);
                magicDmgDealt=magicDmgDealt/(penRatio+armorRatio);

                if(p1.mark)
                    magicDmgDealt*=2;

                preMitDmgDealt+=magicDmgDealt;

                magicDmgDealt-=p2.magicShield;
                if(magicDmgDealt<0)
                    magicDmgDealt=0;

                if(rand()%100<100*100/(100+p2.blind)){
                    p2.changeHP(-physicalDmgDealt-magicDmgDealt);
                    int lifeDrained=(preMitDmgDealt)*p1.lifeDrain/(p1.lifeDrain+50)*(20+p2.deepWounds)/(20+20*p2.deepWounds);
                    p1.changeHP(lifeDrained);

                    if(!p1.invulnerable){
                        if(physicalDmgDealt+magicDmgDealt>0)
                            p1.changeHP(-p2.spikes);
                    }
                }
            }

        int lifeHealed=p1.heal*20/(20-p2.deepWounds);
        p1.changeHP(lifeHealed);

        //immortal
        if(p1.immortal)
            if(p1.HP<=0)
                p1.setHP(1);

        if(p2.immortal)
            if(p2.HP<=0)
                p2.setHP(1);

        for(int i=0;i<p1.elusive;i++){
            Card *card=new Card();
            card->cost=1;
            card->voided=true;
            card->back=Texture(QPixmap("data/back2.png"));
            card->cover=Texture(QPixmap("data/cover0.png"));
            card->type=GREY;
            p2.discardDeck.addCard(card);
        }

    }

    void applyEffects(){
        for(int i=0;i<effects.size();i++){
            applyEffect(effects.at(i));
        }
        for(int i=effects.size()-1;i>=0;i--){
            if(effects.at(i).duration<=0){
                effects.erase(effects.begin()+i);
            }
        }
    }

    void applyEffect(Effect &effect){

        switch (effect.effectType) {

        case PHYSICAL_SHIELD:
            physicalShield+=effect.intensity*(1+cardBonus[2]*0.005);
            break;
        case MAGIC_SHIELD:
            magicShield+=effect.intensity*(1+cardBonus[2]*0.005);
            break;
        case PHYSICAL_DAMAGE:
            physicalDamage+=effect.intensity*(1+cardBonus[1]*0.005);
            break;
        case MAGIC_DAMAGE:
            magicDamage+=effect.intensity*(1+cardBonus[1]*0.005);
            break;
        case PHYSICAL_ARMOR:
            physicalArmor+=effect.intensity*(1+cardBonus[2]*0.005);
            break;
        case MAGIC_ARMOR:
            magicArmor+=effect.intensity*(1+cardBonus[2]*0.005);
            break;
        case PHYSICAL_PENETRATION:
            physicalPenetration+=effect.intensity*(1+cardBonus[1]*0.005);
            break;
        case MAGIC_PENETRATION:
            magicPenetration+=effect.intensity*(1+cardBonus[1]*0.005);
            break;
        case SPIKES:
            spikes+=effect.intensity*(1+cardBonus[3]*0.005);
            break;
        case HEAL:
            heal+=effect.intensity*(1+cardBonus[4]*0.005);
            break;
        case BLIND:
            blind+=effect.intensity*(1+cardBonus[3]*0.005);
            break;
        case MANA_SURGE:
            manaSurge+=effect.intensity*(1+cardBonus[3]*0.005);
            break;
        case MANA_DRAIN:
            manaDrain+=effect.intensity*(1+cardBonus[3]*0.005);
            break;
        case LIFE_DRAIN:
            lifeDrain+=effect.intensity*(1+cardBonus[4]*0.005);
            break;
        case DEEP_WOUNDS:
            deepWounds+=effect.intensity*(1+cardBonus[4]*0.005);
            break;
        case ELUSIVE:
            elusive+=effect.intensity*(1+cardBonus[3]*0.005);
            break;

        case MARK:
            mark=true;
            break;
        case STUN:
            stun=true;
            break;
        case TAUNT:
            taunt=true;
            break;
        case IMMORTAL:
            immortal=true;
            break;
        case INVULNERABLE:
            invulnerable=true;
            break;

        default:
            break;
        }

        effect.duration--;
    }

};

#endif // PLAYER_H
