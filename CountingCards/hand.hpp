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
    uint8_t _card1;
    uint8_t _card2;
    int numCards = 0;
    bool blackjack = false;
    bool splittable = false;
    int _value = 0;
    
public:
    Hand();
    Hand(const Hand& diffHand); // Copy constructor
    Hand(uint8_t card1, uint8_t card2);
    std::string getHand();
    std::string displayOne();
    int hit(Shoe& shoe);
    std::vector<Hand> split(Shoe& shoe);
    int getValue();
    bool isBlackjack();
    bool isSplittable();
    int getNumCards();
    // ~Hand() Destructor
    Hand operator= (Hand& diffHand);    // overload equals operator
};

                                                
