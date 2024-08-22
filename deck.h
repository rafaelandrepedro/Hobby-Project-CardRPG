#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <fstream>

struct Deck{
    std::vector<Card*> cards;

    bool revealed;

    Deck(){
        this->revealed=false;
    }

    void clear(){
        cards.clear();
    }

    void reveal(bool rev){
        this->revealed=rev;
        for(int i=0;i<cards.size();i++){
            cards.at(i)->revealed=rev;
        }
    }

    Card* addCard(Texture cover, Texture back){
        Card *card=new Card();
        card->cover=cover;
        card->back=back;
        addCard(card);
        return card;
    }

    void addCard(Card *card){
        card->revealed=this->revealed;
        cards.push_back(card);
    }

    void removeCard(int i){
        cards.erase(cards.begin()+i);
    }

    void print(Canvas *canvas, int x, int y, int spreadX=0, int spreadY=0, Qt::Alignment align=Qt::AlignCenter, float scale=1, int ID=-1){
        for(int i=0;i<cards.size();i++){
            cards.at(i)->print(canvas, x+i*spreadX-spreadX*(cards.size()-1)/2, y-i*spreadY-spreadY*(cards.size()-1)/2, align, scale, cards.at(i)->ID);
        }
    }

    int size(){
        return cards.size();
    }

    void loadDeck(std::string file){

        cards.clear();

        std::string name="";
        int size=0;

        std::ifstream inputFile(file);

        if (!inputFile.is_open()) {
            return;
        }

        std::string line;

        Card card;
        Effect ef;
        ef.target=PLAYER;
        card.revealed=true;
        card.cover=Texture(QPixmap("data/c.png"));
        card.voided=false;
        card.banished=false;

        while (std::getline(inputFile, line)) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 2);


                if (key == "Duration"){
                    ef.duration=std::stoi(value);
                }

                if (key == "Intensity"){
                    ef.intensity=std::stoi(value);
                }

                if (key == "Type"){
                    ef.effectType=(EffectType)std::stoi(value);
                }

                if (key == "Target"){
                    if(value=="PLAYER")
                        ef.target=PLAYER;
                    if(value=="FOE")
                        ef.target=FOE;
                }

                if (key == "Effect"){
                    card.effects.push_back(ef);
                    ef.target=PLAYER;
                }

                if (key == "Cost"){
                    card.cost=std::stoi(value);
                }

                if (key == "Voided"){
                    card.voided=true;
                }

                if (key == "Banished"){
                    card.banished=true;
                }

                if (key == "Ctype"){
                    int type=std::stoi(value);
                    card.cover=Texture(QPixmap("data/cover"+QString::number(type)+".png"));
                    card.type=(CardType)type;
                }

                if (key == "Card"){
                    card.back=Texture(QPixmap("data/back2.png"));
                    cards.push_back(card.copy());
                    card.effects.clear();
                    card.cover=Texture(QPixmap("data/c.png"));
                    card.voided=false;
                    card.banished=false;
                }

                if (key == "Deck"){
                    return;
                }
            }
        }

        inputFile.close();
    }

    void saveDeck(std::string file){


        std::ofstream outputFile;

        outputFile.open(file);

        if (!outputFile.is_open()) {
            return;
        }

        for(int i=0;i<cards.size();i++){
            for(int j=0;j<cards.at(i)->effects.size();j++){

                outputFile<<"Duration: "<<cards.at(i)->effects.at(j).duration<<"\n";
                outputFile<<"Intensity: "<<cards.at(i)->effects.at(j).intensity<<"\n";
                outputFile<<"Type: "<<(int)(cards.at(i)->effects.at(j).effectType)<<"\n";
                if(cards.at(i)->effects.at(j).target==PLAYER)
                    outputFile<<"Target: PLAYER\n";
                if(cards.at(i)->effects.at(j).target==FOE)
                    outputFile<<"Target: FOE\n";
                outputFile<<"Effect: Effect\n";
            }
            outputFile<<"Cost: "<<cards.at(i)->cost<<"\n";
            outputFile<<"Ctype: "<<(int)cards.at(i)->type<<"\n";
            if(cards.at(i)->voided)
                outputFile<<"Voided: Void\n";
            if(cards.at(i)->banished)
                outputFile<<"Banished: Banish\n";
            outputFile<<"Card: Card\n";
        }
        outputFile<<"Deck: Deck\n";

        outputFile.close();
    }

    static Deck create(DeckBuilder deckBuilder, float difficulty=1.0, float numCards=10){
        Deck deck;
        for(int i=0;i<numCards;i++){
            deck.cards.push_back(Card::create(deckBuilder, difficulty));
        }
        return deck;
    }

    static void draw(Deck &d1, Deck &d2){
        d2.addCard(d1.cards.front());
        d1.removeCard(0);
    }

    static void drawPos(Deck &d1, Deck &d2, int pos){
        d2.addCard(d1.cards.at(pos));
        d1.removeCard(pos);
    }

    static void draw(Deck &d1, Deck &d2, int ID){
        for(int i=0;i<d1.size();i++)
            if(ID==d1.cards.at(i)->ID){
                d2.addCard(d1.cards.at(i));
                d1.removeCard(i);
            }
    }

    static void shuffle(Deck &d1){
        for(int i=0;i<d1.size()*d1.size();i++){
            int r=rand()%d1.size();
            d1.addCard(d1.cards.at(r));
            d1.removeCard(r);
        }
    }
};

#endif // DECK_H
