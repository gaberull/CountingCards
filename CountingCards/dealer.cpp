//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//


#include "shoe.hpp"
#include "hand.hpp"
#include "bank.hpp"
#include <iostream>

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    Dealer();
public:
    Dealer(int numPlayers);
    std::vector<Hand> dealHands(Shoe shoe, Bank playerBank);
    int compareHands(Hand player, Hand dealer);
    
};
Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    handArray = std::vector<Hand>(_numPlayers+1);
}

std::vector<Hand> Dealer::dealHands(Shoe shoe, Bank playerBank)
{
    handArray = std::vector<Hand>(_numPlayers+1);
    bool lastRound = false;
    // vector of player hands. User will be zero, dealer will be _numPlayers
    
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        Hand currHand = Hand(shoe.dealCard(), shoe.dealCard());
        handArray[i] = currHand;
        // we are on the user
        if(i == 0)
        {
            std::cout << "You have " << currHand.getHand() << std::endl;
            
        }
        // if we are on dealer
        else if(i == _numPlayers)
        {
            std::cout << "Dealer has " << currHand.getHand() << std::endl;
            lastRound = shoe.shoeFinished();
        }
                            
    }
    return handArray;
}

int Dealer::compareHands(Hand player, Hand dealer)
{
    
}
