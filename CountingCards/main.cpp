//
//  main.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "shoe.hpp"
#include "bank.hpp"
#include "dealer.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
    cout << "Let's Play some Blackjack!! \n";
    cout << "\nEnter number of decks between 1 and 8 to play! \n";
    int numDecks = 0;
    cin >> numDecks;
    while(!cin || numDecks < 1 || numDecks > 8)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number of decks between 1 and 8 \n";
        cin >> numDecks;
    }
    
    int cutPoint = -1;
    cout << "\nChoose point where shoe is reshuffled (cut card location)\n";
    cout << "Enter 0 to play full shoe, 1 for 90%, 2 for 75%, 3 for 50% \n";
    cin >> cutPoint;
    while(!cin || cutPoint < 0 || cutPoint > 3)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 0 and 3 \n";
        cout << "Enter 0 to play full shoe, 1 for 90%, 2 for 75%, 3 for 50% \n";
        cin >> cutPoint;
    }
    
    // Starting player balance
    int funds = -1;
    cout << "\nEnter starting player funds from 100 to 1,000 \n";
    cin >> funds;
    while(!cin || funds < 100 || funds > 1000)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 100 and 1,000 \n";
        cin >> funds;
    }
    // Debug info
    //cout << "number of decks is " << numDecks << endl;
    //cout << "shuffle mark is  " << cutPoint << endl;
    
    // Create shoe and player bank, and pass them to dealer in constructor
    char bet[10];
    cout << "\nReady to Play??? Input bet to play (i.e. \"155\") to start or 'q' to quit \n";
    cin >> bet;
    if(bet[0] == 'q' || bet[0] == 'Q') return 0;
    while(!cin || bet[0] < '0' || bet[0] > '9')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
        cin >> bet;
        if(bet[0] == 'q' || bet[0] == 'Q') return 0;
    }
    int bet_int = 0;
    
    bet_int = (int) stol(bet);
    //cout << "bet_int is " << bet_int << "\n";
    Shoe shoe(numDecks, cutPoint);
    Bank bank(funds);
    
    cout << "\n HERE WE GOOO!!!!  Good Luck! \n";
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< bank.getBalance() <<" \n";
    cout << "| CURRENT BET  : $"<< bet << "  \n";
    cout << "----------------------------- \n \n";
    
    //TODO: move all of below actions to Dealer::action()
    // just one player for now
    Dealer dealer(1);
    // Hands are dealt
    int handOngoing = 0;
    handOngoing = dealer.dealHands(shoe, funds, bet_int); // returns 1 if hand still going, 0 if over (dealer BJ)
    char action = 'q';
    if(handOngoing) // hand is not done yet (dealer didn't bust)
    {
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
        
    }
    
    switch (action) {
        case 'h':   //hitPlayer(int bet, Shoe shoe, Bank playerBank)
            cout << "\nPlayer Hits! \n";
            handOngoing = dealer.hitPlayer(shoe);
            if(!handOngoing)   // player busts
            {
                bank.removeFunds(bet_int);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< bank.getBalance() <<" \n";
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
            break;
            
        default:
            break;
    }
    
    
    
    
    return 0;
}
