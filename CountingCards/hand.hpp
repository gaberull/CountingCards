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
#include "shoe.hpp"

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
    Hand();                                 // Default constructor
    Hand(const Hand& diffHand);             // Copy constructor
    Hand(uint8_t card1, uint8_t card2);     // 2 parameter constructor
    std::string getHand();                  // get str of hand
    std::string displayOne();               // Function to display one card for Dealer
    int hit(Shoe* shoe);                    // hit hand - get one card
    std::vector<Hand> split(Shoe* shoe);    // split a pair (only have 2 cards)
    int getValue();                         // get integer value of hand
    bool isBlackjack();                     // Blackjack - i.e. AsJs
    bool isSplittable();                    // Two cards are a pair - splittable
    int getNumCards();                      // get number of cards
    
    Hand& operator= (Hand& diffHand);       // overload = operator. returns *this for chaining purposes
    ~Hand();                                //Destructor
    
    friend std::ostream& operator<<(std::ostream& s, const Hand& hand);   // overload << operator
};

                                                
// friend ostream& operator<<(ostream& s, const Hand& hand);
