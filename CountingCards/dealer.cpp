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
    Hand* dealerHand;
    Dealer();
public:
    Dealer(int numPlayers);
    int dealHands(Shoe shoe, Bank playerBank, int bet);
    int action(Shoe shoe, Bank playerBank, int bet, char action='a');
    int hitPlayer(Shoe shoe);
    int hitDealer(Shoe shoe);
    ~Dealer();
};
                                                     
/**
 constructor to be used
 */
Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    handArray = std::vector<Hand>(numPlayers);
    dealerHand = new Hand();
}

/**
    @returns 1 if hand is ongoing
             0 if hand is finished
            - 1 to quit game
 */
int Dealer::dealHands(Shoe shoe, Bank playerBank, int bet)
{
    (*this).handArray = std::vector<Hand>();
    bool lastRound = false;
    bool blackjack = false;
    bool dealerBlackjack = false;
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
 can input an action character. if action parameter is the default 'a', will request action character, otherwise skips that
 @returns 0 if hand is done, 1 if hand continues, -1 to quit program
 
    this function does the paying out to player and removing of lost bets
 */
int Dealer::action(Shoe shoe, Bank playerBank, int bet, char action)    // TODO: maybe remove this action default value
{
    cout << "\n     Good Luck!!  \n\n";
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
    cout << "| CURRENT BET  : $"<< bet << "  \n";
    cout << "----------------------------- \n \n";
    
    
    int player = handArray[0].getValue();
    int dealer = handArray[_numPlayers].getValue();
    if(action =='a')
    {
        // List menu of options for the player
        cout << "**  What action would you like to take?  **\n\n";
        cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
        cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n";
        cout << "    'q' or 'Q' to quit\n\n";
        cin >> action;
        while(!cin || (action != 'h' && action != 'p' && action != 's' && action != 'd' && action != 'm' && action != 'c' && action != 'r' && action != 'x' && action != 'q'))   //TODO: add handling capital letters
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. \n";
            cout << "**  What action would you like to take?  **\n\n";
            cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
            cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n\n";
            cin >> action;
        }
    }
    
    switch (action) {
        case 'Q':{   // Double down
            cout << "\nPlayer Requests to Quit. Thanks for Playing!!\n\n";
            return -1;
            break;
        }
        case 'q': {  // Double down
            cout << "\nPlayer Requests to Quit. Thanks for Playing!!\n\n";
            return -1;
            break;
        }
        case 'h': {  //hitPlayer(int bet, Shoe shoe, Bank playerBank)
            cout << "\nPLAYER WILL HIT! \n";
            player = hitPlayer(shoe);
            if(player<0)   // player busts
            {
                playerBank.removeFunds(bet);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                    cin >> temp;
                }
                if(temp=='c' || temp=='C') return 0;
                if(temp=='q' || temp=='Q') return -1;
            }
            else    // still alive after player hit
            {
                return Dealer::action(shoe, playerBank, bet);  // return 1; ??
            }
            break;
        }
        case 'p': {  //Stand pat
            cout << "\nPlayer Chooses to Stand Pat with score of "<< player << " \n\n";
            dealer = hitDealer(shoe);
            if(dealer < 0)  // dealer busts
            {
                playerBank.addFunds(bet);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                    cin >> temp;
                }
                if(temp=='q' || temp=='Q') return -1;
                
            }
            else    // dealer did not bust
            {
                cout << "\n Action is Done. \n";
                cout << "\nPlayer has a score of "<< player << " \n\n";
                cout << "\nDealer has a score of "<< dealer << " \n\n";
                if(player < dealer)
                {
                    cout << "\nDealer wins :-( \n";
                    cout << "\nYou lost your bet of  $"<< bet << "\n\n";
                    playerBank.removeFunds(bet);
                }
                else if(player > dealer)
                {
                    cout << "\nYOU WIN!!! :)  \n";
                    cout << "\nYou won  $"<< bet << "\n\n";
                    playerBank.addFunds(bet);
                }
                else // draw
                {
                    cout << "\nHAND IS A DRAW \n";
                    cout << "\nGood luck on the next one\n\n";
                }
                
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                    cin >> temp;
                }
                if(temp=='q' || temp=='Q') return -1;
                return 0;
            }
               // this hand is finished
            break;
        }
        case 's': {  //Player splits a pair. Must double bet or add remainder of stack
            cout << "\nPlayer Splits \n";
            Hand hand = handArray[0];
            if(!hand.isSplittable())
            {
                cout << "\nThis hand cannot be split. Please choose another action \n";
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                    cin >> temp;
                }
                if(temp=='q' || temp=='Q') return -1;
                
                return Dealer::action(shoe, playerBank, bet);  // ??
            }
            else    //TODO: Write code to split a hand
            {
                //
            }
            
            break;
        }
        case 'd': {// Double down
            cout << "Player chooses to DOUBLE!! \n";
            cout << "Player gets one additional card and doubles bet \n";
            int remaining_funds = playerBank.getBalance()-bet;
            if(remaining_funds < bet)
            {
                cout << "Player only has $" << remaining_funds << " to bet\n";
                bet += remaining_funds;
            }
            else
            {
                bet *= 2;
            }
            player = hitPlayer(shoe);
            if(player<0)   // player busts
            {
                playerBank.removeFunds(bet);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                    cin >> temp;
                }
                if(temp=='c' || temp=='C') return 0;
                if(temp=='q' || temp=='Q') return -1;
            }
            else    // still alive after player doubled
            {
                cout << "\nPlayer has a score of "<< player << " \n\n";
                
                cout << "\nInput 'c' to continue or 'q' to quit \n";
                char temp;
                cin >> temp;
                while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                    cin >> temp;
                }
                if(temp=='q' || temp=='Q') return -1;
                
                return 1;   // dealer has yet to act
            }
            break;
        }
        case 'm': {  // Get basic strategy Hint
            cout << "\nPlayer Requests a strategy hint \n";
            cout << "\nYou have a score of "<< player << " \n\n";
            cout << "\nAgainst the dealer's score of "<< dealer << " \n\n";
            break;
        }
        case 'c': {  // Current count of Deck
            cout << "\nPlayer Requests the current count of the deck  \n";
            break;
        }
        case 'r': {  // List Rules
            cout << "\nPlayer Requests a list of the rules  \n";
            break;
        }
        case 'x': {  // Surrender
            cout << "\nPlayer Surrenders  :-(  \n";
            
            break;
        }
            
        default: {
            break;
        }
    }
    
    
    return 1;   //TODO: 1== not done
}
  
// TODO: LEFT OFF HERE on 7/29 at 8:18 PM. FINISH this and fix this up
// returns -1 if player busts. player's hands value otherwise
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
        
        return newPlayerVal;
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
        return newPlayerVal;
    }
    return newPlayerVal;
}

/**
 This function will hit the Dealer until they bust or have a good enough score to stand pat.
 @returns -1 if dealer busts   ||     else score of the dealer's hand
 */
int Dealer::hitDealer(Shoe shoe)
{
    Hand dealer = handArray[_numPlayers];
    int dealerScore = dealer.getValue();
    cout << "Dealer Has   " << dealer.getHand() << "   initially \n\n";
    cout << "Dealer's hand has a value of " << dealerScore <<" \n\n";
    int numHits = 0;
    
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealer.hit(shoe);
        numHits++;
        cout << "Dealer Has   " << dealer.getHand() << "    after hitting "<< numHits <<" times \n\n";
        if(dealerScore > 0)
        {
            cout << "Dealer's hand has a score of " << dealer.getValue() << "  \n\n";
        }
        else
        {
            cout << "Dealer BUSTS!!!! WOOOOOOT! Enjoy that money \n";
        }
        
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
    }
    return dealerScore;
}
    
Dealer::~Dealer()
{
    cout << "Dealer destructor being called \n";
    delete dealerHand;
}

/**     display bankroll
 
 cout << "_____________________________ \n \n";
 cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
 cout << "----------------------------- \n \n";
 
 */
