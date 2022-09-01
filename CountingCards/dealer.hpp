//
//  dealer.hpp
//  CountingCards
//
//  Copyright (c) 2022 Gabe Scott
//

#pragma once
#include "hand.hpp"
#include "bank.hpp"
#include "shoe.hpp"

class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;                // The user's array of hands (only 1 unless split)
    std::vector<Hand> patHands;                 // The user's array of hands standing pat (only 1 or 0 unless split)
    std::vector<Hand> otherPlayers;             // Computer players' array of hands
    std::vector<Hand> otherPats;                // Computer players' array of hands already standing pat
    Hand* dealerHand;                           // Dealer's hand (pointer)
    //bool dealerBlackjack = false;
    Dealer();                                   // Default constructor - private so it can't be called
    
    //friend std::ostream& operator<<(std::ostream& s, const Dealer& dealer);
    
public:
    explicit Dealer(int numPlayers);                    // Contructor - using explicit to deny implicit conversion (char to int)
    bool hasBlackjack();                                // returns  whether or not this dealer's hand is blackjack
    int dealHands(Shoe* shoe, Bank* playerBank, int bet);       // Initial hand dealing to user and AI players, dealer
    int testDealHands(Bank* playerBank);                    // for testing purposes - choose hand & dealer hand
    int action(Shoe* shoe, Bank* playerBank, char action ='a', bool test=false); 
    int computerAction(Shoe* shoe, bool printAction=false);   // Perform action on computer players' hands, print action first time only
    int dealerAction(Shoe* shoe, Bank* playerBank);     // Perform dealer actions after all players' hand actions done
    int hitPlayer(Hand& player, Shoe* shoe);            // calls hit() and adds text output
    int testHitPlayer(Hand& player);                    // for testing. choose hit card
    char correctAction(Hand& player, Hand* dealer, int count=0, bool print=true); // returns correct action for any hands vs dealer card shown
    std::string addCommas(int num);
    //std::string printCard(char first, char sec);
    ~Dealer();                                          // Destructor
};
