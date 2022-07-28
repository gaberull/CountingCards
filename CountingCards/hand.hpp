//
//  hand.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//

#pragma once
#include <cstdlib>
#include <vector>

class Hand
{
    std::vector<char> cardArray;
    std::vector<char> suitArray;
    bool blackjack = false;
    bool soft = false;
    int _value = 0;
    Hand();
public:
    Hand(uint8_t card1, uint8_t card2);
    std::string getHand();
    std::pair<int,int> hit();
};
