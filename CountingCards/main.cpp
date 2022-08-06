//
//  main.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "shoe.hpp"
#include "bank.hpp"
#include "dealer.hpp"
//#include "hand.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define MAX_RELOAD 1000

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
    
    cout << "\nEnter number of Players between 1 and 4  \n";
    int numPlayers = 0;
    cin >> numPlayers;
    while(!cin || (numPlayers < 1 || numPlayers > 4))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number of players between 1 and 4 \n";
        cin >> numPlayers;
    }
    
    int totalFunds = 0;    //track all funds added for end profit/loss
    // Starting player balance
    int funds = 0;
    cout << "\nEnter starting player funds from 100 to 1,000 \n";
    cin >> funds;
    while(!cin || funds < 100 || funds > 1000)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 100 and 1,000 \n";
        cin >> funds;
    }
    totalFunds += funds;
    
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
    
    // Try-Catch statements I probably won't use
                                                            /* Try,Catch
    // allocate new shoe, bank, and dealer on heap
    Shoe* shoe = nullptr;
    try {
        shoe = new Shoe(numDecks, cutPoint);
    } catch (bad_alloc& ba) {
        cout << "Error: new shoe allocation failed\n";
        return 1;
    }
    Bank* bank = nullptr;
    try {
        bank = new Bank(funds);
    } catch (bad_alloc& ba) {
        cout << "Error: new bank allocation failed\n";
        return 1;
    }
    Dealer* dealer = nullptr;
    try {
        dealer = new Dealer(1);
    } catch (bad_alloc& ba) {
        cout << "Error: new dealer allocation failed\n";
        return 1;
    }
                                        End Try,Catch        */
    
    Shoe* shoe = new (std::nothrow) Shoe(numDecks, cutPoint);
    if(shoe==nullptr)
    {
        cout << "shoe ptr did not allocate correctly \n";
        return 1;
    }
    Bank* bank = new Bank(funds);
    // TODO: get number of players from user, put it in Dealer constructor
    Dealer* dealer = new Dealer(numPlayers);        // 1 for 1 player
    
    // Hands are dealt
    int handContinues = 0;      // -1 = quit, 0 = hand done, 1 = hand ongoing
    handContinues = dealer->dealHands(shoe, bank, bet);
    
    cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
    cout << "True count (ratio) is "<< shoe->getTrueCount() << endl;       //TODO: remove this
    // while handOngoing >=0
    while(handContinues>-1) // while quit hasn't been requested
    {
        if(handContinues==1)    // hand is ongoing, call action
        {
            handContinues = dealer->action(shoe, bank);
            cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
            cout << "True count (ratio) is "<<shoe->getTrueCount() << endl;  //TODO: remove this
        }
        else   //handContinues==0, Do comuterAction, then Do dealer action, then start new hand
        {
            if(numPlayers > 1 && !dealer->hasBlackjack())
            {
                cout << "\nPerforming computer hand actions \n";
                dealer->computerAction(shoe);
                cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
                cout << "True count (ratio) is "<<shoe->getTrueCount() << endl;  //TODO: remove this
            }
            cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
            handContinues = dealer->dealerAction(shoe, bank);
            if(handContinues<0) break;      // 'Q' has been input to quit. Jump to end and final print stmt
            cout << "\nNEW HAND \n";
            //TODO: put new shoe check here
            
            // check if player has funds
            if(bank->getBalance() == 0)
            {
                cout << "\nYOU ARE OUT OF FUNDS! \n";
                cout << "\nEnter amount to reload (up to $1,000) | 'q' to quit! \n";
                char reload_str[10];
                if(reload_str[0] == 'q' || reload_str[0]=='Q') break;
                while(!cin || reload_str[0] < '0' || reload_str[0] > '9' )
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter number to reload or 'q' to quit \n";
                    cin >> reload_str;
                }
                int reload = (int) stol(reload_str);
                if (reload > MAX_RELOAD) reload = MAX_RELOAD;
                bank->addFunds(reload);
                totalFunds += reload;
            }
            if(shoe->endOfShoe())   // Shoe is finished. Start a new one    //FIXME: not working
            {
                cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
                cout << "True count (ratio) is "<<shoe->getTrueCount() << endl;  //TODO: remove this
                cout << "Time to start a new shoe with "<<numDecks<<" decks \n\n";
                delete shoe;
                shoe = new Shoe(numDecks, cutPoint);    //TODO: check don't need 'Shoe shoe = ... '
            }
            cout << "\nPLEASE ENTER NEW BET  |  'q' to quit. \n";
            cin >> bet_str;
            if(bet_str[0] == 'q' || bet_str[0] == 'Q') break;
            while(!cin || bet_str[0] < '0' || bet_str[0] > '9')
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
                cin >> bet_str;
                if(bet_str[0] == 'q' || bet_str[0] == 'Q') break;
            }
            bet = (int) stol(bet_str);
            handContinues = dealer->dealHands(shoe, bank, bet);
            cout << "Running count is "<<shoe->getCount() << endl;  //TODO: remove this
            cout << "True count (ratio) is "<<shoe->getTrueCount() << endl;  //TODO: remove this
        }
    }
    int net = bank->getBalance() - totalFunds;
    /// EXIT SEQUENCE
    cout << "\nThanks for Playing!! \n";
    cout << "_____________________________ \n \n";
    cout << "| TOTAL FUNDS ADDED    : $"<< totalFunds <<" \n";
    cout << "| BANKROLL             : $"<< bank->getBalance() <<" \n";
    cout << "----------------------------- \n";
    (net >= 0) ?
    cout << "| NET WIN/LOSS         : $"<< net <<" \n\n":
    cout << "| NET WIN/LOSS         : -$"<< net*-1 <<" \n\n";
    
    // Unallocate heap memory
    delete shoe;
    delete dealer;
    delete bank;
    
    return 0;
}



/*
cout << "_____________________________ \n \n";
cout << "| BANKROLL     : $"<< bank.getBalance() <<" \n";
cout << "| CURRENT BET  : $"<< bet << "  \n";
cout << "----------------------------- \n \n";                  */
