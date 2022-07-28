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
    Dealer();
public:
    Dealer(int numPlayers): _numPlayers(numPlayers) { }
    void dealHands(Shoe shoe);
    
};
