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
    cout << "Enter number of decks between 1 and 8 to play! \n";
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
    cout << "Choose point where shoe is reshuffled (cut card location)\n";
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
    cout << "Enter starting player funds from 100 to 1,000 \n";
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
    cout << "Ready to Play??? Input bet to play (\"100\") to start or 'q' to quit \n";
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
    
    cout << "Here we go!!!!!!!! Good Luck! \n";
    cout << "--------------- \n";
    cout << "PLAYER WAGERS "<< bet << endl;
    cout << "--------------- \n";
    Shoe shoe(numDecks, cutPoint);
    Bank bank(funds);
    //TODO: Get number of players from user and add functionality for multiple
    // just one player for now
    Dealer dealer(1);
    dealer.dealHands(shoe, funds);
    cout << "What action would you like to take? \n";
    cout << "'h' to HIT, 's' to split, 'd' to double, '2' to double down, 'r' for rules \n";
    cout << "'m' for Strategy Hint, 'c' to get current running count \n";
    char action;
    cin >> action;
    
    
    return 0;
}
