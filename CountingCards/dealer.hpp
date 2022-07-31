//
//  dealer.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once
#include "hand.hpp"
#include "bank.hpp"

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    std::vector<Hand> otherPlayers;
    Hand* dealerHand;
    Dealer();
public:
    Dealer(int numPlayers);
    int dealHands(Shoe shoe, Bank playerBank, int bet);
    int action(Shoe shoe, Bank playerBank, int bet, Hand playerHand);
    int hitPlayer(Shoe shoe);
    int hitDealer(Shoe shoe);
    Hand splitHand(Hand hand, Shoe shoe);
    ~Dealer();
    
    friend std::ostream& operator<<(std::ostream& s, const Dealer& dealer);
};
                                        



