#ifndef GAME_H
#define GAME_H
#include "texture.h"
#include "util.h"
#include "canvas.h"
#include "deck.h"
#include "player.h"

#include <thread>
#include <chrono>





struct Game{
    int activity;

    Canvas canvas;
    Coord cursor;

    Deck rewardDeck;

    Player player;
    Player foe;




    Game(){
        reset();
    }

    void reset(){
        activity=1;

        player.reset("data/scripts/playerDeck1.txt", "data/scripts/player1.txt");
        foe.reset("data/scripts/foeDeck1.txt", "data/scripts/foe1.txt");

        foe.actions=3;
        while(foe.actions!=0){
            if(!foe.playCard(&player, 0))
                Deck::drawPos(foe.handDeck, foe.discardDeck, 0);
            if(foe.actions!=0)
                foe.refillHand();
        }
        foe.actions=3;
        foe.discardHand();
        foe.refillHand();
    }

    void print(QPainter *painter){

        canvas.setPainter(painter);
        canvas.coord={10, 10};
        canvas.dim={1500, 800};
        canvas.offset={0, 0};
        canvas.zoom=0.25;
        canvas.border();

        //ACTIVITY #1 //game
        if(activity==1){
            player.mainDeck.print(&canvas, -2400, 300, 10, -20, Qt::AlignCenter, 2, Entity::newEntity());
            player.handDeck.print(&canvas, 0, 300, 900, 0, Qt::AlignCenter, 2, Entity::newEntity());
            player.discardDeck.print(&canvas, 2400, 300, 10, -20, Qt::AlignCenter, 2, Entity::newEntity());

            foe.mainDeck.print(&canvas, 1600, -1400, 1, -2, Qt::AlignCenter, 0.5, Entity::newEntity());
            foe.handDeck.reveal(false);
            foe.handDeck.print(&canvas, 2000, -1400, 100, 0, Qt::AlignCenter, 0.5, Entity::newEntity());
            foe.discardDeck.print(&canvas, 2400, -1400, 1, -2, Qt::AlignCenter, 0.5, Entity::newEntity());

            player.printEffects(&canvas, -2500, -500, Qt::AlignCenter, 4, Entity::newEntity());

            foe.printEffects(&canvas, -2500, -900, Qt::AlignCenter, 4, Entity::newEntity());

            player.printHP(&canvas, -2800, -700);
            foe.printHP(&canvas, 2800, -700);

            player.printBonus(&canvas, -2800, -1600);
            foe.printBonus(&canvas, 2800, -1600);

            player.printActions(&canvas, -2800, -550);
            foe.printActions(&canvas, 2800, -550);


        }
        //ACTIVITY #2 //reward
        if(activity==2){
            Texture p1=Texture(QPixmap("data/op1.png"));
            Texture p2=Texture(QPixmap("data/op2.png"));
            Texture p3=Texture(QPixmap("data/op3.png"));
            Texture p4=Texture(QPixmap("data/op4.png"));
            p1.print(&canvas, -1500, 0, Qt::AlignCenter, 2, 1);
            p2.print(&canvas, -500, 0, Qt::AlignCenter, 2, 2);
            p3.print(&canvas, 500, 0, Qt::AlignCenter, 2, 3);
            p4.print(&canvas, 1500, 0, Qt::AlignCenter, 2, 4);
        }
        //ACTIVITY #3 //add
        if(activity==3){
            rewardDeck.print(&canvas, 0, 0, 1200, 0, Qt::AlignCenter, 2, Entity::newEntity());
            Texture p1=Texture(QPixmap("data/o1.png"));
            p1.print(&canvas, 0, -1000, Qt::AlignCenter, 2, -2);
        }
        //ACTIVITY #4 //remove
        if(activity==4){
            player.mainDeck.print(&canvas, 0, 0, 750*7/player.mainDeck.size(), 0, Qt::AlignCenter, 2, Entity::newEntity());
        }
        //ACTIVITY #5 //copy
        if(activity==5){
            player.mainDeck.print(&canvas, 0, 0, 750*7/player.mainDeck.size(), 0, Qt::AlignCenter, 2, Entity::newEntity());
        }
        //ACTIVITY #6 //bonus
        if(activity==6){
            Texture p1=Texture(QPixmap("data/op1b.png"));
            Texture p2=Texture(QPixmap("data/op2b.png"));
            Texture p3=Texture(QPixmap("data/op3b.png"));
            p1.print(&canvas, -1000, 0, Qt::AlignCenter, 2, 1);
            p2.print(&canvas, 0, 0, Qt::AlignCenter, 2, 2);
            p3.print(&canvas, 1000, 0, Qt::AlignCenter, 2, 3);
        }

    }

    void mousePressEvent(QMouseEvent* event){
        int x, y;
        x = event->pos().x();
        y = event->pos().y();


        if(canvas.clickCanvas(x, y)){

            int elementID=canvas.clickElement(x, y);

            std::cout<<elementID<<std::endl;
            //ACTIVITY #1 //game
            if(activity==1){
                for(int i=0;i<player.handDeck.cards.size();i++){
                    if(elementID==player.handDeck.cards.at(i)->ID){

                        player.playCard(&foe, i);

                        if(player.actions!=0)
                            player.refillHand();

                        if(player.actions==0){
                            player.actions=3;
                            player.discardHand();
                            player.refillHand();

                            player.applyEffects();
                            foe.applyEffects();
                            player.actions+=player.manaSurge-foe.manaDrain;
                            foe.actions+=foe.manaSurge-player.manaDrain;
                            Player::resolveEffects(player, foe);
                            Player::resolveEffects(foe, player);
                            player.clearAppliedEffects();
                            foe.clearAppliedEffects();

                            std::cout<<"HP: "<<player.HP<<std::endl;
                            std::cout<<"HP: "<<foe.HP<<std::endl;
                            if(player.HP<=0){
                                reset();
                                return;
                            }
                            if(foe.HP<=0){
                                player.joinDeck();
                                foe.joinDeck();

                                player.playerDifficulty+=0.05;
                                foe.playerDifficulty+=0.05;

                                if(rand()%2)
                                    foe.mainDeck.removeCard(rand()%foe.mainDeck.size());
                                foe.mainDeck.addCard(Card::create(foe.deckBuilder, foe.playerDifficulty));

                                foe.maxHP+=10;
                                foe.setHP(foe.maxHP);
                                player.changeHP(20);

                                player.savePlayer("data/scripts/player1.txt");
                                foe.savePlayer("data/scripts/foe1.txt");

                                activity=2;
                                return;
                            }

                            while(foe.actions!=0){
                                if(!foe.playCard(&player, 0))
                                    Deck::drawPos(foe.handDeck, foe.discardDeck, 0);
                                if(foe.actions!=0)
                                    foe.refillHand();
                            }
                            foe.actions=3;
                            foe.discardHand();
                            foe.refillHand();
                        }



                        break;
                    }
                }


                return;
            }
            //ACTIVITY #2 //reward
            if(activity==2){
                if(elementID==1){//add card
                    activity=3;

                    for(int rewardCardNum=0;rewardCardNum<3;rewardCardNum++)
                        rewardDeck.addCard(Card::create(player.deckBuilder, player.playerDifficulty));
                    rewardDeck.reveal(true);
                }
                if(elementID==2){//delete card
                    activity=4;
                    player.mainDeck.reveal(true);
                }
                if(elementID==3){//duplicate card
                    activity=5;
                    player.mainDeck.reveal(true);
                }
                if(elementID==4){//upgrade card
                    activity=6;
                }
                return;
            }
            //ACTIVITY #3 //add
            if(activity==3){
                static int rerolls=0;
                for(int i=0;i<rewardDeck.cards.size();i++)
                    if(elementID==rewardDeck.cards.at(i)->ID){

                        player.mainDeck.addCard(rewardDeck.cards.at(i));
                        rewardDeck.clear();

                        player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                        foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                        rewardDeck.reveal(false);
                        rerolls=0;
                        reset();
                        break;
                    }
                if(elementID==-2){
                    rewardDeck.clear();
                    rerolls++;
                    float dif=(float)(foe.mainDeck.size()-1-(float)rerolls/3)/8;
                    for(int rewardCardNum=0;rewardCardNum<3;rewardCardNum++)
                        rewardDeck.addCard(Card::create(player.deckBuilder, dif));
                }

                return;
            }
            //ACTIVITY #4 //remove
            if(activity==4){
                for(int i=0;i<player.mainDeck.cards.size();i++)
                    if(elementID==player.mainDeck.cards.at(i)->ID){

                        player.mainDeck.removeCard(i);


                        static int cardsRemoved=3;
                        cardsRemoved--;

                        if(cardsRemoved==0){
                            cardsRemoved=3;

                            player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                            foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                            player.mainDeck.reveal(false);
                            reset();
                        }


                        break;
                    }
                return;
            }
            //ACTIVITY #5 //copy
            if(activity==5){
                for(int i=0;i<player.mainDeck.cards.size();i++)
                    if(elementID==player.mainDeck.cards.at(i)->ID){

                        player.mainDeck.addCard(player.mainDeck.cards.at(i)->copy());

                        player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                        foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                        player.mainDeck.reveal(false);
                        reset();
                        break;
                    }
                return;
            }
            //ACTIVITY #6 //bonus
            if(activity==6){
                if(elementID==1){//more status
                    for(int u=0;u<player.mainDeck.size();u++)
                        for(int v=0;v<player.mainDeck.cards.at(u)->effects.size();v++){
                            player.mainDeck.cards.at(u)->effects.at(v).intensity*=1.08;
                        }
                    player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                    foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                    reset();
                }
                if(elementID==2){//more duration
                    for(int u=0;u<player.mainDeck.size();u++)
                        for(int v=0;v<player.mainDeck.cards.at(u)->effects.size();v++){
                            if(player.mainDeck.cards.at(u)->effects.at(v).intensity>1){
                                int newIntensity=player.mainDeck.cards.at(u)->effects.at(v).intensity/2;
                                player.mainDeck.cards.at(u)->effects.at(v).duration*=(float)(player.mainDeck.cards.at(u)->effects.at(v).intensity)/(float)(newIntensity);
                                player.mainDeck.cards.at(u)->effects.at(v).intensity=newIntensity;
                            }
                            player.mainDeck.cards.at(u)->effects.at(v).intensity*=1.1;
                        }
                    player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                    foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                    reset();
                }
                if(elementID==3){//less duration
                    for(int u=0;u<player.mainDeck.size();u++)
                        for(int v=0;v<player.mainDeck.cards.at(u)->effects.size();v++){
                            if(player.mainDeck.cards.at(u)->effects.at(v).duration>1){
                                int newDuration=player.mainDeck.cards.at(u)->effects.at(v).duration-1;
                                player.mainDeck.cards.at(u)->effects.at(v).intensity*=(float)(player.mainDeck.cards.at(u)->effects.at(v).duration)/(float)(newDuration);
                                player.mainDeck.cards.at(u)->effects.at(v).duration=newDuration;
                            }
                            player.mainDeck.cards.at(u)->effects.at(v).intensity*=1.06;
                        }
                    player.mainDeck.saveDeck("data/scripts/playerDeck1.txt");
                    foe.mainDeck.saveDeck("data/scripts/foeDeck1.txt");
                    reset();
                }
                return;
            }
        }
    }


};

#endif // GAME_H
