//
//  shoe.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once
#include <vector>
#include <cstdlib>

class Shoe
{
private:
    int _numDecks = 1;
    int _cardsRemaining = 0;
    int _cutPoint = 52;
    int _count = 0;
    bool _endOfShoe = false;
    std::vector<uint8_t> fullShoe;
         // private default constructor prevents it from being called
    
public:
    Shoe();
    Shoe(int numDecks, int cutPoint);
    void shuffle();
    uint8_t dealCard();
    bool shoeFinished();
};

                                                        
