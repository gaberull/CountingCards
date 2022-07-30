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
    Dealer();
public:
    Dealer(int numPlayers);
    int dealHands(Shoe shoe, Bank playerBank, int bet);
    int action(Shoe shoe, Bank playerBank, int bet);
    int hitPlayer(Shoe shoe);
    int hitDealer(Shoe shoe);
};
                                        



