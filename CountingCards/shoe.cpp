//
//  shoe.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "shoe.hpp"
#include <vector>

class Shoe
{
private:
    int _numDecks = 1;
    int _cardsRemaining = 0;
    std::vector<uint8_t> fullShoe;
    Shoe();     // private default constructor prevents it from being called
    
public:
    Shoe(int numDecks);
    void shuffle();
    
    
};

/**
 @brief Constructor
 @param numDecks - number of decks in the shoe
 */
Shoe::Shoe(int numDecks): _numDecks(numDecks)
{
    _cardsRemaining = numDecks * 52;
    fullShoe = std::vector<uint8_t>(_cardsRemaining, 0);
    // build a deck in map suit by suit
    for(int i=0; i<52; i++)
    {
        
    }
    uint8_t cardMask = 0x10;
    uint8_t suitMask = 0x01;
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
        fullShoe[i] &= 0x00;
        fullShoe[i] |= cardMask;
        fullShoe[i] |= suitMask;
            
    }
    
    
}
/**
 @brief shuffles the vector of cards in place using Fisher Yates algorithm in O(n) time with O(1) extra space
 */
void Shoe::shuffle()
{
    
}
