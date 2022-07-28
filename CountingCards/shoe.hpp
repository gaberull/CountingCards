//
//  shoe.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once

#include <vector>
class Shoe
{
private:
    int _numDecks = 1;
    int _cardsRemaining = 0;
    int _cutPoint;
    int _count = 0;
    std::vector<uint8_t> fullShoe;
    Shoe();     // private default constructor prevents it from being called
    
public:
    Shoe(int numDecks, int cutPoint);
    void shuffle();
    std::pair<uint8_t, bool> dispenseCard();
};
