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
    
    //TODO: Get number of players from user and add functionality for multiple
    // just one player for now
    Dealer dealer(1);
    int dealt = dealer.dealHands(shoe, funds, bet_int);
    if(dealt)
    {
        cout << "**  What action would you like to take?  **\n\n";
        cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
        cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n\n";
        char action;
        cin >> action;
    }
    
    
    
    return 0;
}
