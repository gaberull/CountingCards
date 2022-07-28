//
//  shoe.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "shoe.hpp"
#include <vector>
#include <unordered_map>

class Shoe
{
private:
    int _numDecks = 1;
    int _cardsRemaining = 0;
    std::unordered_map<int, uint8_t> deck;
    std::vector<uint8_t> decks;
    Shoe();     // private default constructor prevents it from being called
    
public:
    Shoe(int numDecks);
    void shuffle();
    
    
};

Shoe::Shoe(int numDecks): _numDecks(numDecks)
{
    _cardsRemaining = numDecks * 52;
    decks = std::vector<uint8_t>(_cardsRemaining, 0);
    uint8_t cardMask = 0x10;
    uint8_t suitMask = 0x01;
    for(int i=0; i<_cardsRemaining; i++)
    {
        decks[i] |= cardMask;
        decks[i] |= suitMask;
        
    }
    
    
}
/**
 @brief shuffles the vector of cards in place using Fisher Yates algorithm in O(n) time with O(1) extra space
 */
void Shoe::shuffle()
{
    
}
