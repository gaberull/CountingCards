//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//


#include "shoe.hpp"
#include "hand.hpp"
#include <iostream>

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    Dealer();
public:
    Dealer(int numPlayers): _numPlayers(numPlayers) { }
    void dealHands(Shoe shoe);
    int compareHands(Hand player, Hand dealer);
    
};

void Dealer::dealHands(Shoe shoe)
{
    for(int i=0; i<_numPlayers+1; i++)
    {
        std::pair<uint8_t,bool> hand = std::make_pair(0xFF, 0);
        hand = shoe.dispenseHand();
        std::string handString = Card.getHand(hand.first);
        if(i == _numPlayers)
        {
            std::cout << "Dealer has " << handString << std::endl;
        }
        else if(i == 0)
        {
            std::cout << "You have " << handString << std::endl;
        }
        else
        {
            std::cout << "Player " << i+1 << " has " << handString << std::endl;
        }
    }
}

int Dealer::compareHands(Hand player, Hand dealer)
{
    
}
