//
//  dealer.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    Dealer();
public:
    Dealer(int numPlayers);
    std::vector<Hand> dealHands(Shoe shoe, Bank playerBank);
    //int compareHands(Hand player, Hand dealer);
    
};


