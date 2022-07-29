//
//  hand.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//

#pragma once
#include <cstdlib>
#include <vector>
#include <unordered_map>

class Hand
{
    std::vector<char> cardArray;
    std::vector<char> suitArray;
    std::unordered_map<char, int> valueMap; // valueMap doesn't include Ace
    std::unordered_map<uint8_t, char> cardMap;
    std::unordered_map<uint8_t, char> suitMap;
    int numCards = 0;
    bool blackjack = false;
    bool splittable = false;
    int _value = 0;
    
    
public:
    Hand();
    Hand(uint8_t card1, uint8_t card2);
    std::string getHand();
    int hit(Shoe shoe);
    int getValue();
    bool isBlackjack();
};
