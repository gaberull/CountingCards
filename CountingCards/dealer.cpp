//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//


#include "shoe.hpp"
#include "card.hpp"
#include <iostream>

class Dealer
{
private:
    int _numPlayers;
    Dealer();
public:
    Dealer(int numPlayers): _numPlayers(numPlayers) { }
    void dealHands(Shoe shoe);
    
};

void Dealer::dealHands(Shoe shoe)
{
    for(int i=0; i<_numPlayers+1; i++)
    {
        std::pair<uint8_t,bool> hand = std::make_pair(0xFF, 0);
        hand = shoe.dispenseHand();
        string finalHand = Card.getHand(hand.first);
        if(i == _numPlayers)
        {
            std::cout << "Dealer has " << Card.getHand(hand.first) << endl;
        }
    }
}
