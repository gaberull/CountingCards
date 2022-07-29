//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//


#include <iostream>
#include "shoe.hpp"
#include "hand.hpp"
#include "bank.hpp"
#include <iostream>
using namespace std;

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    Dealer();
public:
    Dealer(int numPlayers);
    std::vector<Hand> dealHands(Shoe shoe, Bank playerBank);
    int hit(int bet, Hand playerHand, Shoe shoe, Bank playerBank);
};

Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    handArray = std::vector<Hand>(numPlayers);
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
            if(lastRound)
            {
                std::cout << "This will be the last hand on this shoe \n";
            }
        }
                            
    }
    
    // TODO: add functionality for multiple computer players
    //compareHands(handArray[0], handArray[_numPlayers]);
    
    return handArray;
    
}
  
// TODO: FINISH this and change up function signature probably
int Dealer::hit(int bet, Hand playerHand, Shoe shoe, Bank playerBank)
{
    cout << "Dealer::hit() - Hitting player's hand \n";
    int newValue = playerHand.hit(shoe);
    if(newValue < 0)
    {
        "Dealer:: Player busts. You have lost your bet \n";
        playerBank.removeFunds(bet);
        return 0;
        
    }
    else if(playerHand.isBlackjack())
    {
        "Dealer:: BLACKJACK!! Blackjack pays 3:2 \n";
        playerBank.addFunds(bet*3/2);
        return 2;
    }
    
    //TODO: finish
    
    return -1;
    
    
}
                                                        
