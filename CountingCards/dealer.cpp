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
    std::vector<Hand> dealHands(Shoe shoe, Bank playerBank, int bet);
    int hit(int bet, Shoe shoe, Bank playerBank);
};
                                                     

Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    handArray = std::vector<Hand>(numPlayers);
}

std::vector<Hand> Dealer::dealHands(Shoe shoe, Bank playerBank, int bet)
{
    
    handArray = std::vector<Hand>(_numPlayers+1);
    bool lastRound = false;
    bool blackjack = false;
    bool dealerBlackjack = false;
    // vector of player hands. User will be zero, dealer will be _numPlayers
    
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        Hand currHand = Hand(shoe.dealCard(), shoe.dealCard());
        handArray[i] = currHand;
        // we are on the user
        if(i == 0)
        {
            std::cout << "Your cards     :     " << currHand.getHand() << std::endl;
            blackjack = currHand.isBlackjack();
            
        }
        // if we are on dealer
        else if(i == _numPlayers)
        {
            std::cout << "Dealer shows   :     " << currHand.displayOne() << "\n \n";
            dealerBlackjack = currHand.isBlackjack();
            lastRound = shoe.shoeFinished();
            if(lastRound)
            {
                std::cout << "This will be the last hand on this shoe \n";
            }
        }
                            
    }
    if(dealerBlackjack && !blackjack)
    {
        cout << "Dealer Has:       " << handArray[_numPlayers].getHand() << "\n";
        cout << "Unfortunately, they have BLACKJACK.. They win this round.. \n \n";
        playerBank.removeFunds(bet);
    }
    if(blackjack)
    {
        if(handArray[_numPlayers].isBlackjack())
        {
            cout << "OH NO!!!! You have BLACKJACK, BUT so does the dealer! :(  This round is a push! \n";
        }
        else
        {
            cout << "****   CONGRATS!!!! You have a BLACKJACK!! It pays 3:2!!   ****\n\n";
            //Pay out 3:2 to playerBank
            playerBank.payBlackjack(bet);
        }
    }
    
    return handArray;
    
}
  
// TODO: LEFT OFF HERE on 7/29 at 8:18 PM. FINISH this and fix this up
//
//
int Dealer::hit(int bet, Shoe shoe, Bank playerBank)
{
    Hand player = handArray[0];
    Hand dealer = handArray[_numPlayers];
    cout << "Dealer::hit() - Hitting player's hand \n";
    int newPlayerVal = player.hit(shoe);
    if(newPlayerVal < 0)
    {
        cout << "Dealer:: Oh NO!! You busted.. You have lost your bet \n";
        playerBank.removeFunds(bet);
        return 0;
        
    }
    
    int newDealerVal = dealer.hit(shoe);
    if(newDealerVal < 0)
    {
        cout << "Dealer:: Dealer busted!! You have won your bet \n";
        playerBank.addFunds(bet);
        return 0;
    }
    
    if(newPlayerVal == newDealerVal)
    {
        cout << "We have a tie! This hand is a push!! \n";
        
    }
    
    //TODO: finish this
    
    return -1;
    
    
}
                                                        
