//
//  shoe.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once
#include <vector>
#include <unordered_map>
#include <cstdlib>


class Shoe
{
    int _numDecks = 1;
    int _cardsRemaining = 0;
    int _cutPoint = 52;
    int _count = 0;
    int _aceCount = 0;
    bool _endOfShoe = false;
    std::vector<uint8_t> fullShoe;
         // private default constructor prevents it from being called
    Shoe();
    
public:
    Shoe(int numDecks, int cutPoint);
    void shuffle();             //TODO: remove if not going to use. Probably won't
    uint8_t dealCard();
    bool endOfShoe();
    int getCount();
    int getAceCount();
    ~Shoe();
};

                                                        
