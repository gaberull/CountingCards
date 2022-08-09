//
//  hand.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//  Copyright (c) 2022 Gabe Scott   -   MIT License
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
    
    //TODO: make these maps static
    std::unordered_map<uint8_t, char> cardMap;
    std::unordered_map<char, int> valueMap;
    std::unordered_map<uint8_t, char> suitMap;
    
    uint8_t _card1;
    uint8_t _card2;
    int numCards = 0;
    bool blackjack = false;
    bool splittable = false;
    bool isPat = false;
    int _value = 0;
    int handBet = 0;
    bool soft = false;
    
    friend std::ostream& operator<<(std::ostream& s, const Hand& hand);   // overload << operator
    
public:
    Hand();                                 // Default constructor
    Hand(const Hand& diffHand);             // Copy constructor
    Hand(uint8_t card1, uint8_t card2, int bet=0);     // 3 parameter constructor
    std::string getHand() const;            // get str of hand
    std::string displayOne();               // Function to display one card for Dealer
    char getFirstCard();                    // Returns hand's first card (dealer's shown card)
    char getSecondCard();                   // Returns hand's second card
    int hit(Shoe* shoe);                    // hit hand - get one card
    Hand split(Shoe* shoe);                 // split a pair (only have 2 cards) TODO: need & for reference (Hand&) ?
    int getValue();                         // get integer value of hand
    bool isBlackjack();                     // Blackjack - i.e. AsJs
    bool isSplittable();                    // Two cards are a pair - splittable
    int getNumCards();                      // get number of cards
    void setPat(const bool pat);            // set whether or not hand can receive more cards
    bool getPat();                          // get whether or not hand can receive more cards
    //int doubleHand(Shoe* shoe, int bet=0);
    int getBet();                           // get bet on this hand
    void setBet(const int bet);             // set new bet on this hand
    bool isSoft();                          // returns whether or not the hand is soft
    
    Hand& operator= (const Hand& diffHand); // overload = operator. returns *this for chaining purposes
    ~Hand();                                //Destructor
};
