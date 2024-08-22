#ifndef DECKBUILDER_H
#define DECKBUILDER_H
#include <vector>

struct DeckBuilder{
    std::vector<float> rEffectType={200,80,
                             180,50,
                             140,60,
                             80,30,
                             7,10,8,
                             4,2,
                             2,5,
                             10};
    std::vector<float> rEffectDuration={0,100, 20, 2, 1};
    std::vector<float> rNumEffects={0,6,4,2};
    std::vector<float> rCost={1,100,10,1};
    std::vector<float> rType={2,1,1,1,1};
    std::vector<float> effectStrenght={1, 1.1,
                                     1, 1.25,
                                     0.8, 0.9,
                                     0.6, 0.75,
                                     0.25, 0.35, 0.15,
                                     0.01, 0.005,
                                     0.60, 0.60,
                                     0.008};
};

#endif // DECKBUILDER_H
