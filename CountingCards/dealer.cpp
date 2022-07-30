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
    int dealHands(Shoe shoe, Bank playerBank, int bet);
    int action(Shoe shoe, Bank playerBank, int bet);
    int hitPlayer(Shoe shoe);
    int hitDealer(Shoe shoe, Bank playerBank);
};
                                                     

Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    handArray = std::vector<Hand>(numPlayers);
}

/**
    @returns 1 if hand is ongoing
             0 if hand is finished
            - 1 if something has gone wrong
 */
int Dealer::dealHands(Shoe shoe, Bank playerBank, int bet)
{
    (*this).handArray = std::vector<Hand>();
    bool lastRound = false;
    bool blackjack = false;
    bool dealerBlackjack = false;
    Hand* dealerHand = nullptr;
    // vector of player hands. User will be zero, dealer will be _numPlayers
    
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        Hand currHand = Hand(shoe.dealCard(), shoe.dealCard());
        handArray.push_back(currHand);
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
            // TODO: overloaded operator= not working
            dealerHand = new Hand(currHand);
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
        cout << "Dealer Has:       " << dealerHand->getHand() << "\n";
        cout << "That lucky buffoon has BLACKJACK.. Got us this time.. \n\n";
        playerBank.removeFunds(bet);
        cout << "_____________________________ \n \n";
        cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        
        cout << "\nInput 'c' to continue \n";
        char temp;
        cin >> temp;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
            cin >> temp;
        }
        delete dealerHand;
        return 0;
    }
    if(blackjack)
    {
        
        if(dealerBlackjack)
        {
            cout << "OH NO!!!! You have BLACKJACK, BUT so does the dealer! :(  This round is a push! \n";
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            
            cout << "Input 'c' to continue \n";
            char temp;
            cin >> temp;
            while(!cin || (temp != 'c' && temp != 'C'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
                cin >> temp;
            }
            
            delete dealerHand;
            return 0;
        }
        else
        {
            cout << "****   CONGRATS!!!! You have a BLACKJACK!! It pays 3:2!!   ****\n\n\n";
            playerBank.payBlackjack(bet);
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            cout << "Input 'c' to continue \n";
            char temp;
            cin >> temp;
            while(!cin || (temp != 'c' && temp != 'C'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
                cin >> temp;
            }
            delete dealerHand;
            return 0;
            
        }
    }
    delete dealerHand;
    return 1;
    
}
                                
/**
 Dealer offers menu of option and then performs then
 @returns 0 if hand is done, 1 if not done
 */
int Dealer::action(Shoe shoe, Bank playerBank, int bet)
{
    int handOngoing = 1;
    char action = 'q';
    // List menu of options for the player
    cout << "**  What action would you like to take?  **\n\n";
    cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
    cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n\n";
    cin >> action;
    while(!cin || (action != 'h' && action != 'p' && action != 's' && action != 'd' && action != 'm' && action != 'c' && action != 'r' && action != 'x'))   //TODO: add handling capital letters
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. \n";
        cout << "**  What action would you like to take?  **\n\n";
        cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
        cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n\n";
        cin >> action;
    }
    
    switch (action) {
        case 'h':   //hitPlayer(int bet, Shoe shoe, Bank playerBank)
            cout << "\nPlayer Hits! \n";
            handOngoing = hitPlayer(shoe);
            if(!handOngoing)   // player busts
            {
                playerBank.removeFunds(bet);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                cout << "\nInput 'c' to continue \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
                    cin >> temp;
                }
                return 0;
            }
            else    // still alive after player hit
            {
                cout << "**  What action would you like to take?  **\n\n";
                cout << "||  'h' - hit           |  'p' - stand pat    | \n";
                //TODO: finish this
            }
            break;
        case 'p':   //Stand pat
            cout << "\nPlayer Stands Pat \n";
            break;
        case 's':   //Player splits a pair. Must double bet or add remainder of stack
            cout << "\nPlayer Splits \n";
            break;
        case 'd':   // Double down
            cout << "\nPlayer Doubles!!  \n";
            break;
        case 'm':   // Double down
            cout << "\nPlayer Requests a strategy hint\n";
            break;
        case 'c':   // Double down
            cout << "\nPlayer Requests the current count of the deck  \n";
            break;
        case 'r':   // Double down
            cout << "\nPlayer Requests a list of the rules  \n";
            break;
        case 'x':   // Double down
            cout << "\nPlayer Surrenders  :-(  \n";
            
            return 0;
            break;
            
        default:
            break;
    }
    
    
    return 1;   //TODO: 1== not done
}
  
// TODO: LEFT OFF HERE on 7/29 at 8:18 PM. FINISH this and fix this up
// returns 0 if player busts. 1 if not
//
// hit() is for player only
//
int Dealer::hitPlayer(Shoe shoe)
{
    Hand player = handArray[0];
    Hand dealer = handArray[_numPlayers];
    cout << "Dealer::hit() - Hitting player's hand \n";
    int newPlayerVal = player.hit(shoe);
    if(newPlayerVal < 0)
    {
        cout << "\nOh NO!! You busted.. You have lost your bet \n";
        //playerBank.removeFunds(bet);  // wrong spot
        // Dealer shows hand but it is over
        // TODO: change this when I add in more computer players. Hand won't be displayed yet
        cout << "Dealer Had:       " << dealer.getHand() << "\n\n"; // TODO: check that Hand dealer works (persists)
        
        cout << "Input 'c' to continue \n";
        char temp;
        cin >> temp;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
            cin >> temp;
        }
        
        return 0;
    }
    else
    {
        cout << "Player has "<< player.getHand() << " for a value of " << newPlayerVal <<" \n";
        cout << "Input 'c' to continue \n";
        char temp;
        cin >> temp;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
            cin >> temp;
        }
        return 1;
    }
    return 1;
}
                                                        

/**     display bankroll
 
 cout << "_____________________________ \n \n";
 cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
 cout << "----------------------------- \n \n";
 
 */
