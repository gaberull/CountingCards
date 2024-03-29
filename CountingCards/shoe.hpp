//
//  shoe.hpp
//  CountingCards
//
//  Copyright (c) 2023 Gabe Scott
//

#pragma once
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <cstdint>

class Shoe
{
    int _numDecks = 1;
    int _cardsRemaining = 0;    // number cards remaining in shoe
    int _cutPoint = 52;         // point at which shoe will be restarted
    int _count = 0;             // shoe running count | hi vs low cards
    int _aceCount = 0;          // number of aces seen this shoe
    bool _endOfShoe = false;    // last round for current shoe
    std::vector<uint8_t> fullShoe;  // vector of all cards in shoe
    Shoe();                     // private def constructor | won't be called
    
public:
    Shoe(int numDecks, int cutPoint);
    uint8_t dealCard();         // deal single card from shoe
    bool endOfShoe();           // return if shoe will be restarted at end of round
    int getCount();             // get running count of deck
    float getTrueCount();       // get running count of deck / num decks remaining
    int getAceCount();          // get running count of aces dealt
    int getCardsLeft();         // get num cards left in shoe
    //std::string printCountStr();
    ~Shoe();                    // shoe destructor
};
