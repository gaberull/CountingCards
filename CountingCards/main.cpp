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
    char bet_str[10];
    cout << "\nBLACKJACK TIME!!!!! \n";
    cout << "\nPlace your bet! (enter a number i.e. \"155\") or 'q' to quit \n";
    cin >> bet_str;
    if(bet_str[0] == 'q' || bet_str[0] == 'Q') return 0;
    while(!cin || bet_str[0] < '0' || bet_str[0] > '9')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
        cin >> bet_str;
        if(bet_str[0] == 'q' || bet_str[0] == 'Q') return 0;
    }
    int bet = 0;
    
    bet = (int) stol(bet_str);
    //cout << "bet_int is " << bet_int << "\n";
    Shoe shoe(numDecks, cutPoint);
    Bank bank(funds);
    // just one player for now
    Dealer dealer(1);
    
    // Hands are dealt
    int handContinues = 1;
    // while handOngoing >=0
    while(handContinues>=0) // while quit hasn't been requested
    {
        // double check this bit. may never be 1 returned from Dealer::action()
        if(handContinues==1)    // hand is ongoing, call action
        {
            dealer.action(shoe, bank, bet);
        }
        else   //handContinues==0, start new hand
        {
            cout << "\nNEW HAND \n";
            cout << "\nWhat is your wager friend? 'q' to quit \n";
            cin >> bet_str;
            if(bet_str[0] == 'q' || bet_str[0] == 'Q') return 0;
            while(!cin || bet_str[0] < '0' || bet_str[0] > '9')
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
                cin >> bet_str;
                if(bet_str[0] == 'q' || bet_str[0] == 'Q') return 0;
            }
            bet = (int) stol(bet_str);
            dealer.action(shoe, bank, bet);
        }
    }
    
    cout << "\nThanks for Playing!! \n";
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< bank.getBalance() <<" \n";
    cout << "----------------------------- \n \n";
    
    return 0;
    
}



/*
cout << "\n HERE WE GOOO!!!!  Good Luck! \n";
cout << "_____________________________ \n \n";
cout << "| BANKROLL     : $"<< bank.getBalance() <<" \n";
cout << "| CURRENT BET  : $"<< bet << "  \n";
cout << "----------------------------- \n \n";                  */
