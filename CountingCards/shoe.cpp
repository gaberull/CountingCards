//
//  shoe.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "shoe.hpp"
#include <vector>
#include <iostream>
#include <time.h>
using namespace std;
                                                             
/**
    Default constructor.
    Should be private and unable to be called, hence commented out.
 */
Shoe::Shoe()
{
                                        /*
    int _numDecks = 1;
    int _cardsRemaining = 0;
    int _cutPoint = 52;
    int _count = 0;
    bool _endOfShoe = false;
    fullShoe = std::vector<uint8_t>(52, 0x00);
                                         */
}
/**
 @brief Constructor
 @param numDecks - number of decks in the shoe
 */
Shoe::Shoe(int numDecks, int cutPoint): _numDecks(numDecks)
{
    
    
    _cardsRemaining = numDecks * 52;
    fullShoe = std::vector<uint8_t>(_cardsRemaining);
    
    uint8_t cardMask = 0x00;
    uint8_t suitMask = 0x00;
    for(int i=0; i<_cardsRemaining; i++)
    {
        // Card switch statement
        switch (i%13) {
            case 0:
                cardMask = 0x10;    //Ace
                break;
            case 1:
                cardMask = 0x20;    // Two
                break;
            case 2:
                cardMask = 0x30;    // Three
                break;
            case 3:
                cardMask = 0x40;    // Four
                break;
            case 4:
                cardMask = 0x50;    // five
                break;
            case 5:
                cardMask = 0x60;    // six
                break;
            case 6:
                cardMask = 0x70;    // seven
                break;
            case 7:
                cardMask = 0x80;    // eight
                break;
            case 8:
                cardMask = 0x90;    // nine
                break;
            case 9:
                cardMask = 0xA0;    // Ten
                break;
            case 10:
                cardMask = 0xB0;    // Jack
                break;
            case 11:
                cardMask = 0xC0;    // Queen
                break;
            case 12:
                cardMask = 0xD0;    // King
                break;
        };
        
        // Suit switch statement
        switch (i%4) {
            case 0:
                suitMask = 0x01;    // Spades
                break;
            case 1:
                suitMask = 0x02;    // Clubs
                break;
            case 2:
                suitMask = 0x03;    // Hearts
                break;
            case 3:
                suitMask = 0x04;    // Diamonds
                break;
        };
        
        // set card and suit
        fullShoe[i] = 0x00;
        fullShoe[i] |= cardMask;
        fullShoe[i] |= suitMask;
    }
    // set point at which shoe will be done with
    switch (cutPoint) {
        case 0:
            _cutPoint = 0;
            break;
        case 1:
            _cutPoint = _cardsRemaining / 10;
            break;
        case 2:
            _cutPoint = _cardsRemaining / 4;
            break;
        case 3:
            _cutPoint = _cardsRemaining / 2;
            break;
    }
    
}
/**
 @brief shuffles the vector of cards in place using Fisher Yates algorithm in O(n) time with O(1) extra space
        It really just resets the final card and resets the shoe, since the actual cards don't really get shuffled. They are pulled randomly
 */
void Shoe::shuffle()    //TODO: possibly remove this. Not using it.
{
    _cardsRemaining = 52 * _numDecks;
    _count = 0;
    _endOfShoe = false;
    cout << "Where would you like to place the cut card? \n";
    char c;
    cout << "Input 0 to play all of the shoe, 1 to play 90%, 2 to play 75%, and 3 to play 50% \n";
    cin >> c;
    while(!cin || c < '0' || c > '3')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter 0 to play all of the shoe, 1 to play 90%, 2 to play 75%, and 3 to play 50% \n";
        cin >> c;
    }
    switch (c) {
        case 0:
            _cutPoint = 0;
            break;
        case 1:
            _cutPoint = _cardsRemaining / 10;
            break;
        case 2:
            _cutPoint = _cardsRemaining / 4;
            break;
        case 3:
            _cutPoint = _cardsRemaining / 2;
            break;
            
        default:
            break;
    }
}

/**
 @brief pulls one card from shoe using shuffle algorithm
 */
uint8_t Shoe::dealCard()
{
    srand((unsigned int)std::time(0));
    int i = rand() % _cardsRemaining;   // c4 in 167, 415 is d4
    uint8_t ret = fullShoe[i];
    swap(fullShoe[i], fullShoe[_cardsRemaining-1]);
    _cardsRemaining--;
    
    
    if(_cardsRemaining <= _cutPoint)
    {
        _endOfShoe = true;
    }
    return ret;
    
}

/**
 @returns bool - if the shoe needs to be restarted or not
 */
bool Shoe::shoeFinished()
{
    return _endOfShoe;
}

/**
    Destructor
 */
Shoe::~Shoe()
{
    //cout << "Shoe Destructor called\n";
}
