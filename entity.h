#ifndef ENTITY_H
#define ENTITY_H
#include "deckBuilder.h"

struct Entity{

    int ID;

    Entity(){
        ID=Entity::newEntity();
    }

    static int newEntity(){
        static int IdCount=1;
        return IdCount++;
    }
};


#endif // ENTITY_H
