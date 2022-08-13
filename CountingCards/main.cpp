//
//  main.cpp
//  CountingCards
//
//  Copyright (c) 2022 Gabe Scott
//

#include "shoe.hpp"
#include "bank.hpp"
#include "dealer.hpp"
#include <thread>   // for sleep_for(duration)
#include <chrono>   // for sleep_for(duration)
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#define MAX_RELOAD 1000 // if changed, change in dealer.cpp too

using namespace std;

int main(int argc, const char * argv[])
{
    cout << "     _____\n";
    cout << "    |A .  | _____\n";
    cout << "    | /.\\ ||A ^  | _____\n";
    cout << "    |(_._)|| / \\ ||A _  | _____\n";
    cout << "    |  |  || \\ / || ( ) ||A_ _ |\n";
    cout << "    |____V||  .  ||(_'_)||( v )|\n";
    cout << "           |____V||  |  || \\ / |\n";
    cout << "                  |____V||  .  |\n";
    cout << "                         |____V|\n\n";

    cout << "::::::::::::::::::::::::::::::::::::" << endl;
    cout << "::                                ::" << endl;
    cout << "::          LET'S   PLAY          ::" << endl;
    cout << "::                                ::" << endl;
    cout << "::       SOME    BLACKJACK!       ::" << endl;
    cout << "::                                ::" << endl;
    cout << "::::::::::::::::::::::::::::::::::::" << endl;
    
    cout << "\n\nHow many decks do you want to use?  (1 - 8)\n";
    int numDecks = 0;
    bool test = false;
    cin >> numDecks;
    while(!cin || numDecks < 1 || numDecks > 8)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number of decks between 1 and 8 \n";
        cin >> numDecks;
    }
    
    cout << "\n\n";
    cout << "How many players? (1 - 4)\n";  //9 is test mode !!!!!!!!!!!!!!!!!!!!!!!!!!
    int numPlayers = 0;
    cin >> numPlayers;
    if(numPlayers == 9)
    {
        test = true;
        numPlayers = 1;
    }
    while(!cin || (numPlayers < 1 || numPlayers > 4))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number of players between 1 and 4 \n";
        cin >> numPlayers;
        if(numPlayers==9)
        {
            test = true;
            numPlayers = 1;
            break;
        }
    }
    
    int cutPoint = -1;
    cout << "\n";
    cout << "\nHow much of the shoe will we play?  (before re-shuffle)\n";
    cout << "0 - 100%  |  1 - 90%  |  2 - 75% |  3 - 50% \n";
    cin >> cutPoint;
    while(!cin || cutPoint < 0 || cutPoint > 3)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 0 and 3 \n";
        cout << "Enter 0 to play full shoe, 1 for 90%, 2 for 75%, 3 for 50% \n";
        cin >> cutPoint;
    }
    
    int totalFunds = 0;    //track all funds added for end profit/loss
    
    int funds = 0;  // Starting player balance
    cout << "\n\n";
    cout << "How Much Money Will you Put On Table? (100 - " << MAX_RELOAD<< ")\n";
    cin >> funds;
    while(!cin || funds < 100 || funds > MAX_RELOAD)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 100 and 1,000 \n";
        cin >> funds;
    }
    totalFunds += funds;
    
    int bet = 0;
    if(!test)
    {
        // Create shoe and player bank, and pass them to dealer in constructor
        char bet_str[10];
        cout << "\n";
        cout << "\nEnter Bet  |  'q' to quit " << endl;
        //cout << "~~~~~   Enter Your Bet | 'q' to quit   ~~~~~" << endl;
        //cout << "\n";
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
    }
    
    // Try-Catch statements that I probably won't use
    // TODO: remove these
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
    Dealer* dealer = new Dealer(numPlayers);        // 1 for 1 player
    
    //
    //TODO: add prompt to add more funds at start of new hand
    // TODO: putting in wrong bet value twice breaks it - from M testing 8/8/22
    //
    
    // Hands are dealt  --------------------------------------------------------
    int handContinues = 0;   // -1 == quit, 0 == hand done, 1 == hand ongoing
    
    if(test) handContinues = dealer->testDealHands(bank);
    else handContinues = dealer->dealHands(shoe, bank, bet);
    
    
    while(handContinues>-1) // while quit hasn't been requested
    {
        if(handContinues==1)    // hand is ongoing, call action (for user)
        {
            if(test) handContinues = dealer->action(shoe, bank, 'a', true);
            else handContinues = dealer->action(shoe, bank);
            
        }
        else   //handContinues==0, Do computerAction, then Do dealer action, then start new hand
        {
            if(numPlayers > 1 && !dealer->hasBlackjack())
            {
                cout << "\n";
                cout << "    <<<<     Computer Action     >>>>" << endl;
                dealer->computerAction(shoe);
            }
            handContinues = dealer->dealerAction(shoe, bank);
            
            // add on funds - will be 0 unless set in Dealer::action()
            int addOn = bank->getAddNextHand();
            if(addOn>0)
            {
                bank->addFunds(addOn);
                bank->setAddNextHand(0);
                cout << "\n\n";
                cout << "Adding $" << addOn << " to bankroll " <<endl;
                cout << "-----------------------------  \n";
                cout << "| BANKROLL     : $"<< bank->getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                totalFunds += addOn;
                std::chrono::seconds duration(2);
                std::this_thread::sleep_for(duration);
            }

            if(handContinues<0) break;      // 'Q' has been input to quit (-1). Jump to end and final print stmt
            
            // check if player has funds
            if(bank->getBalance() == 0)
            {
                cout << "\nYOU ARE OUT OF FUNDS! \n";
                cout << "\nEnter amount to reload (up to $" << MAX_RELOAD << ") | 'q' to quit! \n";
                char reload_str[10];
                cin >> reload_str;
                reload_str[0] = toupper(reload_str[0]);
                if(reload_str[0]=='Q') break;
                while(!cin || reload_str[0] <= '0' || reload_str[0] > '9' ) //TODO: handle each digit being number
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter number to reload or 'q' to quit \n";
                    cin >> reload_str;
                    reload_str[0] = toupper(reload_str[0]);
                    if(reload_str[0]=='Q') break;
                }
                int reload = (int) stol(reload_str);
                if (reload > MAX_RELOAD)
                {
                    reload = MAX_RELOAD;
                    cout << "your reload was too much. You get a reload of $" << MAX_RELOAD <<endl;
                    cout << "\n";
                    
                }
                bank->addFunds(reload);
                totalFunds += reload;
                int net = bank->getBalance() - totalFunds;
                cout << "_____________________________ \n\n";
                cout << "| TOTAL FUNDS ADDED    : $"<< totalFunds <<" \n";
                cout << "| BANKROLL             : $"<< bank->getBalance() <<" \n";
                cout << "----------------------------- \n";
                (net >= 0) ?
                cout << "| NET PROFIT/LOSS         : $"<< net <<" \n\n":
                cout << "| NET PROFIT/LOSS         : -$"<< net*-1 <<" \n\n";
            }
            if(shoe->endOfShoe())   // Shoe is finished. Start a new one
            {
                cout << "Time to start a new shoe with " << numDecks<< " decks \n\n";
                delete shoe;
                shoe = new Shoe(numDecks, cutPoint);
            }
            cout <<"\n";
            cout << "    <<<   NEW HAND   >>>" << endl;
            cout << "\n";
            if(!test)
            {
                cout << "\nEnter Bet  |  'q' to quit " << endl;
                string new_bet_str = "";
                cin >> new_bet_str;
                new_bet_str[0] = toupper(new_bet_str[0]);
                if(new_bet_str[0] == 'Q') break;
                while(!cin || new_bet_str[0] < '0' || new_bet_str[0] > '9')
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
                    cin >> new_bet_str;
                    new_bet_str[0] = toupper(new_bet_str[0]);
                    if(new_bet_str[0] == 'Q') break;
                }
                bet = (int) stol(new_bet_str);
            }
            (!test) ?
            handContinues = dealer->dealHands(shoe, bank, bet):
            handContinues = dealer->testDealHands(bank);
        }
    }
    int net = bank->getBalance() - totalFunds;
    cout << "\n\n\n\n";
    cout << "      _____\n";
    cout << "     |A .  | _____\n";
    cout << "     | /.\\ ||A ^  | _____\n";
    cout << "     |(_._)|| / \\ ||A _  | _____\n";
    cout << "     |  |  || \\ / || ( ) ||A_ _ |\n";
    cout << "     |____V||  .  ||(_'_)||( v )|\n";
    cout << "            |____V||  |  || \\ / |\n";
    cout << "                   |____V||  .  |\n";
    cout << "                          |____V|\n\n\n";

    // EXIT SEQUENCE
    cout << "\nThanks for Playing!! \n";
    cout << "_____________________________ \n\n";
    cout << "| TOTAL FUNDS ADDED    : $"<< totalFunds <<" \n";
    cout << "| BANKROLL             : $"<< bank->getBalance() <<" \n";
    cout << "----------------------------- \n";
    (net >= 0) ?
    cout << "| NET WIN/LOSS         : $"<< net <<" \n\n":
    cout << "| NET WIN/LOSS         : -$"<< net*-1 <<" \n\n";
    
    // De-allocate heap memory
    delete shoe;
    delete dealer;
    delete bank;
    
    return 0;
}
