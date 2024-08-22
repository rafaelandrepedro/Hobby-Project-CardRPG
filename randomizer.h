#ifndef RANDOMIZER_H
#define RANDOMIZER_H
#include <vector>
#include <cstdlib>



struct Randomizer{
    std::vector<int> values;
    std::vector<int> weights;

    void add(int value, int weight){
        values.push_back(value);
        weights.push_back(weight);
    }
    int random(){
        int sumWeight=0;
        for(int i=0;i<weights.size();i++)
            sumWeight+=weights.at(i);
        if(sumWeight!=0){
            int randVal=rand()%sumWeight;
            sumWeight=0;
            for(int i=0;i<weights.size();i++){
                sumWeight+=weights.at(i);
                if(sumWeight>randVal)
                    return values.at(i);
            }
        }
        return -1;
    }

    static int Srand(std::vector<float> weights){
        float sumWeight=0;
        for(int i=0;i<weights.size();i++)
            sumWeight+=weights.at(i);
        if(sumWeight!=0){
            float randVal=((float)rand()/(float)rand()*sumWeight);
            while(randVal>sumWeight)
                randVal-=sumWeight;
            sumWeight=0;
            for(int i=0;i<weights.size();i++){
                sumWeight+=weights.at(i);
                if(sumWeight>randVal)
                    return i;
            }
        }
        return -1;
    }
};
#endif // RANDOMIZER_H
