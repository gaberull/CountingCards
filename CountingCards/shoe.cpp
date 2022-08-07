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
 @brief
    Default constructor. Should be private and unable
    to be called, hence why it's commented out.
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
 @brief
    returns number of cards remaining in shoe
 */
int Shoe::getCardsRemaining()
{
    return _cardsRemaining;
}

/**
 @brief
    Shoe Constructor
 @param numDecks
    number of decks in the shoe
 @param cutPoint
    The point in the deck (card index) at which the shoe needs to be reset
 */
Shoe::Shoe(int numDecks, int cutPoint): _numDecks(numDecks)
{
    //_numDecks = numDecks; // int function header
    _cardsRemaining = numDecks * 52;
    _count = 0;
    _aceCount = 0;
    _endOfShoe = false;
    fullShoe = vector<uint8_t>(_cardsRemaining, 0x00);
    
    // set point at which shoe will be finished
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
        default:    // default case won't be hit
            break;
    }
    
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
            default:    // default case won't be hit
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
            default:    // default case won't be hit
                break;
        };
        // set card and suit
        fullShoe[i] = 0x00;
        fullShoe[i] |= cardMask;
        fullShoe[i] |= suitMask;
    }
}

/**
 @brief
    pulls one card from shoe using shuffle algorithm. Each
    pull is random. Sets _endOfShoe once _cardRemaining
    == _cutPoint.
 @returns uint8_t
    unsigned 8 bit int. first 4 bits are card, last 4 are suit
 */
uint8_t Shoe::dealCard()
{
    
    //cout << "-- cards remaining == " << getCardsRemaining() << "\n";  // debug line
    
    // seed random number generator
    srand((unsigned int)std::time(0));
    int i = 0;
    if(_cardsRemaining <= 0)  // No cards left. will repeat a couple cards
    {
        i = rand() % (_numDecks * 52);  // If we run out of cards, grab random from whole used shoe
        _count = 0;
        _aceCount = 0;
    }
    else
    {
        i = rand() % _cardsRemaining;
    }
       // c4 in 167, 415 is d4
    uint8_t ret = fullShoe[i];
    // keeping count of the deck. High cards vs low cards
    switch(ret>>4)
    {
        case 0x01:
        {
            _aceCount++;
            _count--;
            break;
        }
        case 0x02:
        {
            _count++;
            break;
        }
        case 0x03:
        {
            _count++;
            break;
        }
        case 0x04:
        {
            _count++;
            break;
        }
        case 0x05:
        {
            _count++;
            break;
        }
        case 0x06:
        {
            _count++;
            break;
        }
        case 0x0A:
        {
            _count--;
            break;
        }
        case 0x0B:
        {
            _count--;
            break;
        }
        case 0x0C:
        {
            _count--;
            break;
        }
        case 0x0D:
        {
            _count--;
            break;
        }
    }
    // swap card with end of deck (shuffle on each card draw)
    if (_cardsRemaining > 0)
    {
        swap(fullShoe[i], fullShoe[_cardsRemaining-1]);
    }
    else
    {
        _count = 0;
        _aceCount = 0;
    }
    _cardsRemaining--;
    
    if(_cardsRemaining <= _cutPoint)
    {
        _endOfShoe = true;
    }
    return ret;
}

/**
 @brief
    get whether or not this is the final round in shoe before it
    needs to be reset.
 @returns bool
    true    -   the shoe needs to be restarted
    false   -   the shoe doesn't need to be restarted
 */
bool Shoe::endOfShoe()
{
    return _endOfShoe;
}

/**
 @brief
    Get count of the deck
 @returns int
    running count of the deck
 */
int Shoe::getCount()
{
    if(this->_cardsRemaining <= 0)
    {
        return 0;
    }
    return _count;
}

/**
 @brief
    takes running running count and divides by number of decks
    to get the "True count" ratio
 @return float
    running count / number of decks remaining
 */
float Shoe::getTrueCount()
{
    float ret = 0.00;
    float decksLeft = 0.0;
    if(_cardsRemaining <= 0)
    {
        return 0.00;
    }

    decksLeft = _cardsRemaining/52.0;
    ret = _count / decksLeft;
    // round to 2 decimals
    ret = (int)(ret * 100 + .5);
    ret /= 100;
    return ret;
}

/**
 @brief Get the count of aces we have seen
 @return int    number of aces that have been dealt thus far
 */
int Shoe::getAceCount()
{
    return _aceCount;
}

int Shoe::getCardsLeft()
{
    return _cardsRemaining;
}
/**
 @brief Destructor for Shoe
 */
Shoe::~Shoe()
{
    //cout << "Shoe Destructor called\n";
}
