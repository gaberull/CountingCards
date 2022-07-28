//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "dealer.hpp"
class Dealer
{
private:
    int _numPlayers;
    Dealer();
public:
    Dealer(int numPlayers): _numPlayers(numPlayers) { }
    void dealHands();
    
};

void Dealer::dealHands()
{
    for(int i=0; i<_numPlayers+1; i++)
    {
        
    }
}
