//
//  dealer.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once
#include "hand.hpp"
#include "bank.hpp"

#include "shoe.hpp"
//using namespace std;

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    std::vector<Hand> patHands;
    
    std::vector<Hand> otherPlayers;
    std::vector<Hand> otherPats;
    Hand* dealerHand;
    Dealer();
public:
    // to deny implicit conversion (cahr to int) using "explicit"
    explicit Dealer(int numPlayers);
    
    int dealHands(Shoe* shoe, Bank* playerBank, int bet);
    int action(Shoe* shoe, Bank* playerBank, char action ='a'); // int bet=0,  was in there.
    int hitPlayer(Hand& player, Shoe* shoe);
    
    void playAIHands(Shoe* shoe);
    int dealerAction(Shoe* shoe, Bank* playerBank);
    char correctAction(Hand& player, Hand* dealer, int count=0);      // returns correct action for any hands vs dealer hand

    ~Dealer();
    
    //friend std::ostream& operator<<(std::ostream& s, const Dealer& dealer);
};
                                        



