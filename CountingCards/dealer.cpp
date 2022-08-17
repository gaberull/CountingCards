//
//  dealer.cpp
//  CountingCards
//
//  Copyright (c) 2022 Gabe Scott
//

#include <thread>   // for sleep_for(duration)
#include <chrono>   // for sleep_for(duration)
#include <iostream>
#include "dealer.hpp"
#include "shoe.hpp"
#include "hand.hpp"
#include "bank.hpp"
#include <iostream>

#define MAX_RELOAD 1000

using namespace std;

/**
 @brief
    only Dealer constructor that will be used
 @param numPlayers
    the number of players in current game
 @property
    allocates memory on heap for dealerHand pointer
    using "new" keyword
 */
Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    otherPlayers = std::vector<Hand>();
    handArray = std::vector<Hand>();
    dealerHand = new Hand();
    //dealerBlackjack = false;
}

/**
 @brief
    get whether or not dealer has a blackjack
 @return bool
    true    -   the dealer has blackjack
    false   -    the dealer doesn't have blackjack
 */
bool Dealer::hasBlackjack()
{
    return dealerHand->isBlackjack();
}

/**
 @brief
    sets all hand for all _numPlayers. removes bet from playerBank
    at start of hand. Bet is then added to hand in Hand() constructor
 @returns   int
    1   -   hand is ongoing
    0   -   hand is finished
    -1  -   quit game
 */
int Dealer::dealHands(Shoe* shoe, Bank* playerBank, int bet)    //FIXME: 8/8/22 - not showing dealer other card when everybody busts
{
    if(playerBank->getBalance() < bet)      // Check that bet amount is in playerBank
    {
        cout << "\n";
        cout <<"Uh Oh... \n";
        char bet_str[11];
        cout << "You bet more than you have. Enter bet from 1 to "<<playerBank->getBalance()<< endl;
        cin >> bet_str;
        bet_str[0] = toupper(bet_str[0]);
        if(bet_str[0] == 'Q') return -1;
        while(!cin || bet_str[0] < '0' || bet_str[0] > '9' )
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input \n";
            cout << "Enter Bet  |  'q' to quit  " << endl;
            cin >> bet_str;
            bet_str[0] = toupper(bet_str[0]);
            if(bet_str[0] == 'Q') return -1;
        }
        int bet = (int) stol(bet_str);
        return dealHands(shoe, playerBank, bet);
    }
    delete dealerHand;
    handArray = std::vector<Hand>();
    otherPlayers = std::vector<Hand>();
    
    playerBank->removeFunds(bet);
    bool blackjack = false;
    
    cout << "\n";
    cout << "    <<<   Dealing Cards   >>>" << endl;
    
    Hand playerHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
    blackjack = playerHand.isBlackjack();
    handArray.push_back(playerHand);
    dealerHand = new Hand(shoe->dealCard(), shoe->dealCard());
    if(_numPlayers > 1 || dealerHand->isBlackjack() || playerHand.isBlackjack()) // don't display hands if only 1 player, will show in action(). Gotta do blackjack payouts if any blackjacks
    {
        std::cout << "\nYour cards         :    " << playerHand.getHand() << "\n\n";
        std::chrono::seconds duration(2);
        std::this_thread::sleep_for(duration);
    }
    
    // vector of player hands. User will be zero, dealer will be _numPlayers
    for(int i=0; i<_numPlayers-1; i++)
    {
        // putting bet=0 on computer hands
        Hand AIHand = Hand(shoe->dealCard(), shoe->dealCard());
        std::cout << "Player "<<_numPlayers - i<< " has       :    " << AIHand.getHand() << std::endl; // FIXME: print ordering doesn't match computerAction()
        if(AIHand.isBlackjack() && !dealerHand->isBlackjack())
        {
            cout << "Player  has BLACKJACK!! Gets paid 3:2 \n\n";
        }
        else
        {
            otherPlayers.push_back(AIHand);
        }
        std::chrono::seconds duration(2);
        std::this_thread::sleep_for(duration);
    }
    
    if(_numPlayers > 1 || dealerHand->isBlackjack() || playerHand.isBlackjack())  // don't display hands if only 1 player, will show in action()
    {
        
        std::cout << "\nDealer shows       :    " << dealerHand->displayOne() << "\n \n";
        std::chrono::seconds duration(2);
        std::this_thread::sleep_for(duration);
        
        if(dealerHand->isBlackjack())
        {
            cout << "Uh Ohhh..." << endl;
            std::chrono::seconds duration(2);
            std::this_thread::sleep_for(duration);
            cout << "Dealer has blackjack... " << endl;
        }
    }
    if(shoe->endOfShoe())
    {
        std::cout << "<<<<  Final Hand of Shoe  >>>>" << endl;
        //std::cout << "This will be the last hand on this shoe \n";
    }
    //TODO: maybe display computer players here in order of back to front of otherPlayers vector
    if(!blackjack && dealerHand->isBlackjack())
    {
        handArray.pop_back();
        cout << "Dealer Has         :    " << dealerHand->getHand() << "\n";
        cout << "\n** Got us this time..  **\n\n";   //FIXME: too much displayed when dealer wins here heads up
        cout << "-----------------------------  \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        
        // skip continue sequence showing twice when only 1 player and dealer has bj
        if(_numPlayers == 1)
        {
            //std::chrono::seconds duration(2);
            //std::this_thread::sleep_for(duration);
            return 0;
        }
        
        //               Continue sequence                /////////////////////////////////////////////////
        cout << "\n";
        //cout << "\n<< <<   'c' to Continue  | 'q' to Quit   >> >>" << endl;
        cout << "\n'c' to Continue  | 'q' to Quit " << endl;
        
        char temp;
        cin >> temp;
        temp = toupper(temp);
        while(!cin || (temp != 'C' && temp != 'Q'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> temp;
            temp = toupper(temp);
        }
        if(temp == 'Q') return -1;
        //            End of  Continue sequence             ////////////////////////////////////////////
        return 0;
    }
    if(blackjack)
    {
        if(dealerHand->isBlackjack())   // Hand is a PUSH
        {
            handArray.pop_back();
            playerBank->addFunds(bet);   // put funds back in that we took out at start for bet
            cout << "You have BLACKJACK, BUT so does the dealer! :( \nThis round is a push! \n";
            cout << "\n";
            cout << "Dealer Has         :    " << dealerHand->getHand() << "\n\n";
            cout << "-----------------------------  \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            if(_numPlayers == 1)
            {
                //std::chrono::seconds duration(2);
                //std::this_thread::sleep_for(duration);
                return 0;
            }

            //               Continue sequence                /////////////////////////////////////////////////
            cout << "\n";
            cout << "\n'c' to Continue  | 'q' to Quit  " << endl;
            char temp;
            cin >> temp;
            temp = toupper(temp);
            while(!cin || (temp != 'C' && temp != 'Q'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                cin >> temp;
                temp = toupper(temp);
            }
            if(temp=='Q') return -1;
            //              End of  Continue sequence             ////////////////////////////////////////////
            return 0;
        }
        else    // Player beats dealer with a blackjack
        {
            playerBank->addFunds(bet);
            playerBank->payBlackjack(bet);
            int win = bet * 3/2;
            string win_str = std::to_string(win);
            //cout << "****   You hit a BLACKJACK!!!  You Win $" << win_str << "  ****\n\n";
            cout << "*** You hit a BLACKJACK!!! ***\n";
            cout << "\n";

            cout << "        _____  _____\n";
            cout << "       |A .  ||J _  |\n";
            cout << "       | /.\\ || ( ) |\n";
            cout << "       |(_._)||(_'_)|\n";
            cout << "       |  |  ||  |  |\n";
            cout << "       |____A||____J|\n";
            cout << "\n";
            cout << "You WIN $"<< win_str << "!!!!      \n";
            cout << "\n";
            
            
            handArray.pop_back();
            if(_numPlayers==1) cout << "Dealer Had         :    " << dealerHand->getHand() << endl; //TODO: check this on multiplayer
            cout << "\n";
            cout << "-----------------------------  \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            // skip double 'c' to continue sequence when 1 player (other in dealerAction() )
            if(_numPlayers == 1)
            {
                return 0;
            }
            
            //////////////////////////////                 Continue sequence                /////////////////////////////////////////////////
            //cout << "\nInput 'c' to continue or 'q' to quit \n";
            cout << "\n";
            //cout << "~~~~~  'c' - Continue | 'q' - Quit   ~~~~~" << endl;
            //cout << "\n<< <<   'c' to Continue  | 'q' to Quit   >> >>" << endl;
            cout << "\n'c' to Continue  | 'q' to Quit  " << endl;
            char temp;
            cin >> temp;
            temp = toupper(temp);
            while(!cin || (temp != 'C' && temp != 'Q'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                cin >> temp;
                temp = toupper(temp);
            }
            if(temp=='Q') return -1;
            //////////////////////////////               End of  Continue sequence             ////////////////////////////////////////////
            return 0;
        }
    }
    return 1;   // 1 means hand is not over. funds have already been removed from playerBank
}

/**
 @brief
    Only for testing purposes. Choose hands of user and dealer.
 @discussion
    set players hand and set dealer's hand. Only for 1 player vs dealer for now
 @returns int
    0   -   hand is finished           dealer bj, player bj, or both
    1   -   hand is ongoing
    -1  -   quit game
 */
int Dealer::testDealHands(Bank* playerBank)
{
    unordered_map<char, uint8_t> revCardMap =
    {
        {'A',0x11},{'2', 0x21}, {'3',0x31},{'4',0x41}, {'5',0x51},{'6',0x61}, {'7',0x71}, {'8',0x81}, {'9',0x91}, {'T',0xA1}, {'J',0xB1}, {'Q',0xC1},{'K',0xD1}
    };
    
    cout << "Test mode: \n" << endl;
    char p_first, p_sec, d_first, d_sec;
    cout << "Enter player first card" << endl;
    cin >> p_first;
    p_first = toupper(p_first);
    cout << "Enter player second card " << endl;
    cin >> p_sec;
    p_sec = toupper(p_sec);
    cout << "Enter dealer first card " << endl;
    cin >> d_first;
    d_first = toupper(d_first);
    cout << "Enter dealer second card " << endl;
    cin >> d_sec;
    d_sec = toupper(d_sec);
    cout << "Enter bet. No more than $" << playerBank->getBalance() << endl;
    int bet = -1;
    cin >>bet;
    
    while(!cin || bet < 0 || bet > playerBank->getBalance())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter a number between 1 and " << playerBank->getBalance() << endl;
        cin >> bet;
    }
    playerBank->removeFunds(bet);
    
    handArray = std::vector<Hand>();
    Hand player(revCardMap[p_first], revCardMap[p_sec], bet);
    handArray.push_back(player);
    delete dealerHand;
    dealerHand = new Hand(revCardMap[d_first], revCardMap[d_sec], bet);
    
    //    Conditions /////////////////////
    // player bj        dealer bj
    // player bj        dealer not bj
    // player not bj    dealer bj
    // player not bj    dealer not bj
    
    if(player.isBlackjack() && dealerHand->isBlackjack())
    {
        handArray.pop_back();
        cout << "\n";
        cout << "\n";
        cout << "Player Has         :    " << player.getHand() << endl;
        cout << "Dealer Shows       :    " << dealerHand->displayOne() << endl;
        cout << "Uh Ohhh..." << endl;
        std::chrono::seconds duration(2);
        std::this_thread::sleep_for(duration);
        cout << "Dealer Has         :    " << dealerHand->getHand() << endl;
        cout << "\n";
        cout << "You have BLACKJACK, BUT so does the dealer! :( \n";
        cout << "Hand is a push. No win or loss of funds" << endl;
        cout << "\n";
        cout << "-----------------------------  \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        playerBank->addFunds(bet);   // put funds back in that we took out at start for bet
                                                                                        /*
        cout << "'c' to Continue  | 'q' to Quit " << endl;
        char temp;
        cin >> temp;
        temp = toupper(temp);
        if(temp == 'Q') return -1;
        while(!cin || (temp != 'C') )
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> temp;
            temp = toupper(temp);
            if(temp=='Q') return -1;
        }
                                                                                         */
        return 0;
    }
    else if(player.isBlackjack() && !dealerHand->isBlackjack())
    {
        handArray.pop_back();
        playerBank->addFunds(bet);
        playerBank->payBlackjack(bet);
        int win = bet * 3/2;
        string win_str = std::to_string(win);
        cout << "Player Has         :    " << player.getHand() << endl;
        cout << "Dealer Shows       :    " << dealerHand->displayOne() << endl;
        cout << "\n";
        std::chrono::seconds duration(2);
        std::this_thread::sleep_for(duration);
        cout << "*** You hit a BLACKJACK!!! ***\n";
        cout << "\n";

        cout << "        _____  _____\n";
        cout << "       |A .  ||J _  |\n";
        cout << "       | /.\\ || ( ) |\n";
        cout << "       |(_._)||(_'_)|\n";
        cout << "       |  |  ||  |  |\n";
        cout << "       |____A||____J|\n";
        cout << "\n";

        cout << "You WIN $"<< win_str << "!!!!      \n";
        cout << "\n";
        cout << "Dealer Had         :    " << dealerHand->getHand() << endl;
        
        cout << "\n";
        cout << "-----------------------------  \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
                                                                        /*
        cout << "'c' to Continue  | 'q' to Quit " << endl;
        char temp;
        cin >> temp;
        temp = toupper(temp);
        if(temp=='Q') return -1;
        while(!cin || (temp != 'C') )
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> temp;
            temp = toupper(temp);
            if(temp=='Q') return -1;
        }
                                                                         */
        return 0;
    }
    else if(!player.isBlackjack() && dealerHand->isBlackjack())
    {
        std::chrono::seconds duration(2);
        
        handArray.pop_back();
        cout << "Player Has         :    " << player.getHand() << endl;
        cout << "Dealer Shows       :    " << dealerHand->displayOne() << endl;
        cout << "\n";
        cout << "Uh Ohhh..." << endl;
        std::this_thread::sleep_for(duration);
        cout << "Dealer Has         :    " << dealerHand->getHand() << endl;
        cout << "For blackjack :( " << endl;
        cout << "\n";
        cout << "You LOST your bet of $" << player.getBet() << "    >:-(" << endl;
        //cout << "\n";
        cout << "-----------------------------  \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
                                                                                        
        return 0;
    }
    else    //(!player->isBlackjack() && !dealerHand->isBlackjack()
    {
        cout << "\n";
        cout << "Player Has         :    " << player.getHand() << endl;
        cout << "Dealer Shows       :    " << dealerHand->displayOne() << endl;
        cout << "\n";
        
        cout << "'c' to Continue  | 'q' to Quit " << endl;
        char in = 'a';
        cin >> in;
        in = toupper(in);
        while(!cin || (in != 'C' && in != 'Q') )
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n";
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> in;
            in = toupper(in);
        }
        if(in == 'Q') return -1;
    }
    return 1;
}

/**
 @brief
    Dealer offers menu of options and then performs it. Can
    input a pre-determined action character.
 @discussion
    The user can input an action character. if action
    parameter is the default 'a', will request action character,
    otherwise skips that
 
 @param shoe (Shoe)
    the shoe containing all the decks that the game is being played with
 @param playerBank (Bank)
    the user's current balance of funds, and functions to update that balance
 @param action (char)
    this char is set to default value='a'   in the function declaration, and may not end up being used
 
 @property  case 'q', 'Q'
    quit the program
 @property  case 'h'
    hit the hand
 @property  case 'p'
    stand pat with hand
 @property  case 's'
    split hand and turn into 2 hands to be played
 @property  case 'd'
    double hand
 @property  case 'm'
    get optimal play (strategy) for hand vs dealer's currently shown card
 @property  case 'c'
    get current count of deck. Prints running count and true count (ratio of running count to # decks remaining)
 @property  case 'r'
    get a list of the rules of blackjack
 
 @discussion
    adds the main functionality of the gameplay. It covers all actions that the player can
    take. Possible actions include the above listed properties. Hands are popped off the
    back of handArray at end of action. Specifically before a 0 is returned bet is removed
    from bank in dealHands(), and then again in 'd' - double and 's' - split (double can
    add less than bet, split can't)
 
 @return int
    0   -   hand is done
    1   -   hand continues
    -1  -   quit program
 */
int Dealer::action(Shoe* shoe, Bank* playerBank, char action, bool test)
{
    
    if(handArray.size() == 0)
    {
        return 0;
    }
    action = toupper(action);
    Hand playerHand = handArray.back();
    handArray.pop_back();
    
    int player = playerHand.getValue();
    if(action =='A')
    {
        cout << "\n------------------------------------\n";
        cout << "Your Cards         :    "<< playerHand.getHand() << " \n";
        cout << "Dealer shows       :    " << dealerHand->displayOne() << "\n";
        cout << "------------------------------------\n";
        // List menu of options for the player
        cout << "\n***  Please choose the action you would like to take  ***\n";
        cout << "---------------------------------------------------------------------------\n";
        cout << "||  'h' - Hit            |  'p' - Stand Pat      |  's' - Split          ||\n";
        cout << "||  'd' - Double Down    |  'm' - Strategy Hint  |  'c' - Get Count      ||\n";
        cout << "||  'r' - List Rules     |  'b' - Reload Bank    |  'q' or 'Q' to Quit   ||\n";
        cout << "---------------------------------------------------------------------------\n\n";
        
        cin >> action;
        action = toupper(action);
        while(!cin || (action != 'H' && action != 'P' && action != 'S' && action != 'D' && action != 'M' && action != 'C' && action != 'R' && action != 'B' && action != 'Q' && action != 'X'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. \n";
            cout << "**  What action would you like to take?  **\n\n";
            cout << "---------------------------------------------------------------------------\n";
            cout << "||  'h' - Hit            |  'p' - Stand Pat      |  's' - Split          ||\n";
            cout << "||  'd' - Double Down    |  'm' - Strategy Hint  |  'c' - Get Count      ||\n";
            cout << "||  'r' - List Rules     |  'b' - Reload Bank    |  'q' or 'Q' to Quit   ||\n";
            cout << "---------------------------------------------------------------------------\n\n";
            cin >> action;
            action = toupper(action);
        }
    }
    switch (action) {
        case 'Q':{   // Quit
            cout << "\nPlayer Requests to Quit. \n\n";
            return -1;
            break;
        }
            // Hit the Hand
        case 'H': {
            cout << "\n";
            cout << "    <<<   Hitting Hand   >>>" << endl;
            cout << "\n";
            
            if(test) player = testHitPlayer(playerHand);
            else player = hitPlayer(playerHand, shoe);
            if(player<0)   // player busts
            {
                cout << "-----------------------------  \n";
                cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
                cout << "----------------------------- \n \n";
            }
            else
            {
                handArray.push_back(playerHand);
            }
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            break;
        }
        
            // Stand Pat
        case 'P': {
            cout << "\nPlayer Stands with :    "<< playerHand.getHand() << " \n";
            cout << "for a value of     "<< playerHand.getValue() << " \n\n";
            playerHand.setPat(true);
            patHands.push_back(playerHand);
            
            cout << "\n";
            cout << "'c' to Continue  | 'q' to Quit  " << endl;
            char temp = 'a';
            cin >> temp;
            temp = toupper(temp);
            while(!cin || (temp!= 'C' && temp != 'Q') )
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                cin >> temp;
                temp = toupper(temp);
            }
            if(temp=='Q') return -1;
            
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            break;
        }
            // Split the hand - must be a pair. won't return a 0 or 1. Only calls action again.
        case 'S': {
            cout << "\n";
            cout << "    <<<   Player Splits   >>>" << endl;
            cout << "\n";
            //Must double bet OR add remainder of bankroll to bet
            if(!playerHand.isSplittable())
            {
                cout << "\n** This hand cannot be split. Please choose another action **\n";
                std::chrono::seconds duration(2);
                std::this_thread::sleep_for(duration);
                handArray.push_back(playerHand);
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);  // ??
            }
            else    // split the hand
            {
                int bet = playerHand.getBet();
                if(playerBank->getBalance() < bet)
                {
                    cout << "Not enough funds to split. Choose another action \n";
                    cout << "\n'c' to Continue  | 'q' to Quit " << endl;
                    char temp;
                    cin >> temp;
                    temp = toupper(temp);
                    while(!cin || (temp != 'C' && temp != 'Q'))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Wrong Input. Enter 'c' to continue. 'q' to quit\n";
                        cin >> temp;
                        temp = toupper(temp);
                        //if(temp=='Q') return -1;
                    }
                    if(temp == 'Q') return -1;
                    handArray.push_back(playerHand);
                    if(test) return Dealer::action(shoe, playerBank, 'a', true);
                    return Dealer::action(shoe, playerBank);
                }
                // subtract bet again from bank. Betting 2x original bank now
                playerBank->removeFunds(playerHand.getBet());
                Hand newHand = playerHand.split(shoe);    // this will change playerHand and create newHand
                // new split hands
                cout << "\nHand 1             :    " << playerHand.getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                cout << "\nHand 2             :    " << newHand.getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                
                bool bjflag = false;    // flag for blackjack hit - for only one pause
                handArray.push_back(playerHand);
                if(playerHand.isBlackjack())
                {
                    bjflag = true;
                    int bet = playerHand.getBet();
                    string bet_str = to_string(bet * 3/2);
                    cout << "\n***  Hand 1 is blackjack!!  ***\n";
                    cout << "\n";

                    cout << "        _____  _____\n";
                    cout << "       |A .  ||J _  |\n";
                    cout << "       | /.\\ || ( ) |\n";
                    cout << "       |(_._)||(_'_)|\n";
                    cout << "       |  |  ||  |  |\n";
                    cout << "       |____A||____J|\n";
                    cout << "\n";
                    cout << "You WIN $"<< bet_str << "!!!!      \n";
                    cout << "\n";
                    
                    playerBank->addFunds(playerHand.getBet());
                    playerBank->payBlackjack(bet);
                    handArray.pop_back();
                }
                handArray.push_back(newHand);
                if(newHand.isBlackjack())
                {
                    bjflag = true;
                    int bet = newHand.getBet();
                    string bet_str = to_string(bet * 3/2);
                    cout << "\n***  Hand 2 is blackjack!!  ***\n";
                    cout << "\n";

                    cout << "        _____  _____\n";
                    cout << "       |A .  ||J _  |\n";
                    cout << "       | /.\\ || ( ) |\n";
                    cout << "       |(_._)||(_'_)|\n";
                    cout << "       |  |  ||  |  |\n";
                    cout << "       |____A||____J|\n";
                    cout << "\n";
                    cout << "You WIN $"<< bet_str << "!!!!      \n";
                    cout << "\n\n";
                    playerBank->addFunds(newHand.getBet());
                    playerBank->payBlackjack(bet);
                    handArray.pop_back();
                }
                // If either new hand was a blackjack, pause for 2 seconds
                if(bjflag)
                {
                    std::chrono::seconds duration(2);
                    std::this_thread::sleep_for(duration);
                }
                cout << "\n";
                cout << "\n'c' to Continue  | 'q' to Quit " << endl;
                char temp;
                cin >> temp;
                temp = toupper(temp);
                while(!cin || (temp != 'C' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                    cin >> temp;
                    temp = toupper(temp);
                }
                if(temp=='Q') return -1;
                
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);
            }
            break;
        }
            // Double down
        case 'D': {
            //cout << "Player chooses to DOUBLE!! \n";
            cout << "\n";
            cout << "    <<<   Player Doubles   >>>" << endl;
            cout << "\n";
            if(playerHand.getNumCards() > 2)
            {
                cout << "\n** You can only double down on a hand with exactly two cards.**\n";
                cout << "** Please choose another action. **\n";
                handArray.push_back(playerHand);
                // pause for user to take in action
                std::chrono::seconds duration(3);
                std::this_thread::sleep_for(duration);
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);
            }
            cout << "Player gets one additional card and doubles bet \n";
            
            int newBet = playerHand.getBet();// = playerHand.getBet();
            if(playerBank->getBalance() == 0)
            {
                cout << "You have no more money. Just hit instead. Doubling is a losing play now\n";
                cout << "Choose hit from the menu \n";
                handArray.push_back(playerHand);
                // pause for user to take in action
                std::chrono::seconds duration(3);
                std::this_thread::sleep_for(duration);
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);
            }
            else if(playerBank->getBalance() < newBet)
            {
                cout << "Player doesn't have enough $ to double bet." << endl;
                cout << "\n";
                cout << "Player adds $" << playerBank->getBalance() << " to bet!" << endl;;
                newBet = playerBank->getBalance();
            }
            playerBank->removeFunds(newBet);
            
            int oldbet = playerHand.getBet();
            playerHand.setBet(oldbet + newBet);
            cout << "-----------------------------  \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "| CURRENT BET  : $"<< playerHand.getBet() << "  \n";
            cout << "----------------------------- \n \n";
            
            // add one and only one card to player's hand. Then it is pat. or bust.
            
            cout << "\n";
            cout << "*  Doubling Hand  *" << endl;
            if(test) player = testHitPlayer(playerHand);
            else player = hitPlayer(playerHand, shoe);
            playerHand.setPat(true);
            
            if(player<0)   // player busts
            {
                cout << "-----------------------------  \n";
                cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                if(_numPlayers > 1) //FIXME: check this condition
                {
                    cout << "\n";
                    cout << "\n'c' to Continue  | 'q' to Quit " << endl;
                    char temp = 'a';
                    cin >> temp;
                    temp = toupper(temp);
                    while(!cin || (temp != 'C' && temp != 'Q') )
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                        cin >> temp;
                        temp = toupper(temp);
                        //if(temp =='Q') return -1;
                    }
                    if(temp=='Q') return -1;
                }
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);
            }
            else    // hand still alive after double
            {
                patHands.push_back(playerHand);
                cout << "\n'c' to Continue  | 'q' to Quit " << endl;
                char temp = 'a';
                cin >> temp;
                temp = toupper(temp);
                while(!cin || (temp != 'C' && temp != 'Q'))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wrong Input. Enter 'C' or 'c' to continue. 'q' or 'Q' to quit\n";
                    cin >> temp;
                    temp = toupper(temp);
                }
                if(temp=='Q') return -1;
                
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);   // hand finished
            }
            break;
        }
            // Get strategy Hint:
            // - basic strategy
            // - count adjusted strategy
        case 'M': {
            cout << "\n";
            cout << "    <<<  Player requests strategy hint  >>>" << endl;
            cout << "\n\n";
            
            cout << "Your hand is         :    " << playerHand.getHand() << "\n";
            cout << "Against Dealer's     :    " << dealerHand->displayOne() << "\n";
            cout << "Your hand value     " << playerHand.getValue() << "\n";
            this->correctAction(playerHand, dealerHand);
            handArray.push_back(playerHand);
            cout << "\n***  Please choose the action you would like to take  ***\n";   
            char temp ='e';
            cin >> temp;
            temp = toupper(temp);
            if(temp=='Q') return -1;    //FIXME:
            if(!cin || (temp != 'H' && temp != 'P' && temp != 'S' && temp != 'D' && temp != 'M' && temp != 'C' && temp != 'R' && temp != 'X' && temp != 'B'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nWrong Input. \n";
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);    // show menu again
            }
            if(test) return Dealer::action(shoe, playerBank, temp, true);
            return Dealer::action(shoe, playerBank, temp);  // don't show menu again - should still be visible above
            break;
        }
            // Get Current count of Deck
        case 'C': {
            cout << "\n";
            cout << "    <<<  Player requests deck count  >>>" << endl;
            cout << "\n\n";
            cout << "The Running Count is "<< shoe->getCount() << "\n";
            cout << "True count (ratio) is "<< shoe->getTrueCount() << "\n"; 
            std::chrono::seconds duration(2);
            std::this_thread::sleep_for(duration);
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
            
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            break;
        }
            // List Rules
        case 'R': {
            
            cout << "\n";
            cout << "    <<<  Player requests game rules  >>>" << endl;
            cout << "\n\n";
            cout << "Some Terminology:\n\n";
            cout <<"|Blackjack| - A two card hand that contains exactly one ace, and one hand counting for 10 points (King, Queen, Jack, or Ten). This pays out at 3:2\n";
            
            cout <<"\n|Dealer| - The person distributing hands to all the players and him/herself\n";
            cout <<"\n|Bust, busted| - When a hand goes over 21 and is now dead\n";
            cout <<"\n|Soft Hand| - A hand that contains at least one ace still counting for 11 points. Since Aces can count for either 1 or 11 points, this hand has no chance of busting by taking a card\n";
            cout <<"\n|Hard, hard hand| - A hand that does not have any aces counting for 11 in it. It can either contain aces or not\n";
            cout <<"\n|Shoe| - the box that holds the deck(s) that the cards are being distributed from.\n";
            cout <<"\n|Hit, hitting| - Taking a card from the shoe\n";
            cout <<"\n|Double, doubling down| - The action of doubling the bet on a hand and taking just one card. This hand's action is now complete\n";
            cout <<"\n|Split, splitting hand| - The action of splitting a pair in a hand into two hands, with each hand then receiving one card. The original bet is doubled and then split between the two hands\n";
            cout <<"\n|Standing, standing pat| - when a player has not busted, and will not be taking any more cards. The action for that hand is complete\n";
            cout <<"\n|Count| - The positive or negative value that is calculated by adding one for high cards, and subtracting one for low cards. A higher count is better for the player, because a deck full of high cards works in the player's favor, due to a higher payout when hitting blackjack\n";
            
            cout << "\n Other Info: \n";
            cout << "* Dealer hits until he has 17 or better (stands on soft 17s) \n";
            cout << "* The running count is calculated as follows:  \n";
            cout << "   - high cards add 1 to count  \n";
            cout << "   - low cards subtract 1 from count  \n";
            cout << "   - The true count is ratio running count to decks remaining  \n";
            cout << "   - The higher the count, the better  \n";
            cout << "   - True count is used to adjust bet sizing and specific hand actions\n"; //TODO: implement this
            
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
            
            cout << "\n";
            cout << "\n'c' to Continue  | 'q' to Quit " << endl;
            char temp;
            cin >> temp;
            temp = toupper(temp);
            while(!cin || (temp != 'C' && temp != 'Q'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
                cin >> temp;
                temp = toupper(temp);
            }
            if(temp=='Q') return -1;
            
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            break;
        }
            // Surrender
        case 'X': {       //TODO: maybe just get rid of surrender altogether
            cout << "\nPlayer Surrenders  :-(  \n";
            cout << "Surrendering has not yet been implemented as of 8/7/22. \n";
            cout << "I likely will just end up removing it from this game altogether - Gabe \n";
            cout << "Please select another option (after 3 second delay)\n";
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
            
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            break;
        }
           // reload (B)ank next hand
        case 'B':{
            cout << "    <<<   Reload Bankroll   >>>" << endl;
            cout << "\n";
            cout << "\n";   //TODO: check this spacing. check +getAdd
            if(playerBank->getBalance()+playerHand.getBet()+playerBank->getAddNextHand() > (MAX_RELOAD*3/4) )
            {
                cout << "Sorry, you have too much money to reload! " << endl;
                cout << "Gotta keep it fun! " << endl;
                std::chrono::seconds duration(3);
                std::this_thread::sleep_for(duration);
                handArray.push_back(playerHand);
                
                if(test) return Dealer::action(shoe, playerBank, 'a', true);
                return Dealer::action(shoe, playerBank);
            }
            cout << "Enter amount to reload  (after this hand ends)" << endl;
            
            int temp;
            cin >> temp;
            while(!cin || temp < 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Wrong Input. Enter positive number" << endl;
                cin >> temp;
            }
            
            // entered amount is too much
            if(temp+playerHand.getBet()+playerBank->getBalance()+playerBank->getAddNextHand() > (MAX_RELOAD*3/4))
            {
                int currAdd = playerBank->getAddNextHand();
                int addOn = MAX_RELOAD*3/4 - playerHand.getBet() - playerBank->getBalance() - playerBank->getAddNextHand();
                if(addOn > 0)
                {
                    cout << "\n";
                    cout << "You are broke!! We will give you a marker to " << endl;
                    cout << "bring your bankroll up to $" << (MAX_RELOAD *3/4) <<endl;
                    cout << "\n";
                    cout << "* Adding $" << addOn + currAdd << " to your bankroll next hand! *" << endl;
                }
                else    //addOn == 0   - the player tried to add on twice in a row
                {
                    cout << "\n";
                    cout << "Nice try... No more monies for you!";
                    cout << "\n";
                    cout << "\n";
                }
                
                playerBank->setAddNextHand(currAdd + addOn);
            }
            else    // entered amount is good
            {
                int addOn = playerBank->getAddNextHand() + temp;
                cout << "\n";
                cout << "You will add on $" << addOn << " next hand" <<endl;
                playerBank->setAddNextHand(addOn);
            }
            
            std::chrono::seconds duration(2);
            std::this_thread::sleep_for(duration);
            handArray.push_back(playerHand);
            
            if(test) return Dealer::action(shoe, playerBank, 'a', true);
            return Dealer::action(shoe, playerBank);
            
            break;
        }
            // default case will never execute
        default: {
            break;
        }
    }
    return 1;   // should never be hit
}

/**
 @brief
    Completes the computer  hands' actions (hitting, standing, splitting). Goes
    until there are no more computer hands to act on and all hands are either
    bust or in the vector of hands standing pat called otherPats.
 @param shoe
    the Shoe object that holds the decks
 @return int
    0   -   Hand is finished. otherPlayers vector is empty. No more hands to play
    1   -  (Won't ever be returned) Hand isn't finished
 */
int Dealer::computerAction(Shoe* shoe)    //TODO: Double check this for loop logic
{
    if(otherPlayers.empty())
    {
        return 0;
    }
    Hand curr = otherPlayers.back();
    otherPlayers.pop_back();
    char action = this->correctAction(curr, dealerHand, 0, false);
    std::chrono::seconds duration(2);       // set pause duration
    switch (action) {
            // Hit
        case 'h':
        {
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player hits \n";
            std::this_thread::sleep_for(duration);
            curr.hit(shoe);
            //cout << "\nAfter hit, computer player has " << curr.getHand() <<"\n";
            //std::this_thread::sleep_for(duration);
            
            if(curr.getValue() < 0) // bust
            {
                cout << "\nComp Player has    :    " << curr.getHand() << "\n";
                cout << "Comp Player has busted \n";
            }
            else
            {
                otherPlayers.push_back(curr);
            }
            return computerAction(shoe);
            break;
        }
            // Stand pat
        case 'p':
        {
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player Stands Pat \n";
            otherPats.push_back(curr);
            std::this_thread::sleep_for(duration);
            // computerPat()
            return computerAction(shoe);
            break;
        }
            // Split
        case 's':
        {
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player splits \n";
            
            std::this_thread::sleep_for(duration);  // set pause duration
            Hand newHand = curr.split(shoe);    // this will change playerHand and create newHand
            otherPlayers.push_back(curr);
            otherPlayers.push_back(newHand);
            cout << "\nHand 1             :    "<< otherPlayers[otherPlayers.size()-1].getHand() <<"\n";
            cout << "\nHand 2             :    "<< otherPlayers[otherPlayers.size()-2].getHand() <<"\n";
            return computerAction(shoe);
            break;
        }
            // Double
        case 'd':
        {
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player doubles \n";   // actually just hitting once
            std::this_thread::sleep_for(duration);
            curr.hit(shoe);
            //cout << "\nAfter Double, computer player has " << curr.getHand() <<"\n";
            std::this_thread::sleep_for(duration);
            cout << "\nComp Player has    :    " << curr.getHand() << " after doubling\n";
            std::this_thread::sleep_for(duration);
            if(curr.getValue() < 0)
            {
                cout << "\nComp Player has busted \n";
            }
            else
            {
                otherPats.push_back(curr);
            }
            return computerAction(shoe);
            break;
        }
            //default will never execute
        default:
        {
            break;
        }
    }
    return 1;
}

/**
 @brief
    This function will hit the Dealer until they bust or have a good enough
    score (17 or better) to stand pat. Called after user hands and computer
    hands have completed hand actions
 @returns   int
    0   -   hand is done
    -1  -   quit game
 */
int Dealer::dealerAction(Shoe* shoe, Bank* playerBank)
{

    //
    // if there are none of this player's hands to play, or AI hands to play
    //
    if((patHands.size()==0 && otherPats.size()==0) || dealerHand->isBlackjack())
    {
        cout << "\n";
        cout << "    <<<   Action is Done   >>>" << endl;
        cout << "\n";
        cout << "\n'c' to Continue  | 'q' to Quit " << endl;
        char temp;
        cin >> temp;
        if(temp=='q' || temp=='Q') return -1;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> temp;
            if(temp=='q' || temp=='Q') return -1;
        }
        return 0;
    }
    // Dealer acts
    cout << "\n";
    cout << "    <<<    Dealer Acts    >>>" << endl;
    
    int dealerScore = dealerHand->getValue();
    cout << "\nDealer Has         :    " << dealerHand->getHand() << "      initially \n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);        // Pause so player can count cards
    
    int numHits = 0;
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealerHand->hit(shoe);
        numHits++;
        cout << "Dealer Has         :    " << dealerHand->getHand() << "     after hitting "<< numHits <<" time(s) \n";
        std::this_thread::sleep_for(duration);
    }
    if(dealerScore > 0)    // dealer not bust
    {
        cout << "Dealer action is finished \n\n";
    }
    else    // dealer is bust
    {
        cout << "Dealer BUSTS!!!!  \n";
    }
    cout << "\n";
    cout << "    <<<   Hand Showdown   >>>" << endl;
    (dealerHand->getValue() > 0) ?
    cout << "\nDEALER Has         :    " << dealerHand->getHand() << "\n\n":
    cout <<"\nDealer is BUST \n";
    
    int numYourHands = (int)patHands.size();
    for(int i=1; i<=numYourHands; i++)
    {
        Hand playerHand = patHands.back();
        patHands.pop_back();
        (numYourHands<=1) ?
        cout << "\nYour hand has      :    " << playerHand.getValue() << "\n":
        cout << "Your hand number " << i << " has " << playerHand.getValue() << "\n";
        
        (dealerScore<0) ?
        cout << "Against Dealer's Busted Hand \n":
        cout << "Against Dealer's   :    " << dealerScore << "\n";
        if(_numPlayers==1) cout << "\n";                            //FIXME: check here
        
        if(playerHand.getValue() < dealerScore)  // player loses
        {
            cout << "YOU LOST your bet of $" << playerHand.getBet() << "    >:-("<< endl;
            if(_numPlayers  > 1) cout << "\n";
        }
        else if(playerHand.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Hand is a Push. You win and lose nothing " << endl;
            if(_numPlayers > 1) cout << "\n";
            playerBank->addFunds(playerHand.getBet());
        }
        else // player wins (not with blackjack)
        {
            cout << "\n";
            cout << "You WIN $"<< playerHand.getBet() << "!!!! " << endl;
            if(_numPlayers>1) cout << "\n";
            playerBank->addFunds(playerHand.getBet()*2);
        }
    }
    int numAIHands = (int)otherPats.size();
    for(int i=1; i<=numAIHands; i++)
    {
        Hand AI = otherPats.back();
        otherPats.pop_back();
        cout << "Comp Player  has   :    " << AI.getHand() << "\n";
        if(AI.getValue() < dealerScore)  // player loses
        {
            cout << "Comp Player Loses \n\n";
        }
        else if(AI.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Comp Player Pushes \n\n";
        }
        else      // player wins (not with blackjack)
        {
            cout << "Comp Player Wins \n\n";
        }
    }
    cout << "-----------------------------  \n";
    cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
    cout << "----------------------------- \n";
    std::this_thread::sleep_for(duration);  // pause for 3 sec
    
    cout << "\n";
    cout << "\n'c' to Continue  | 'q' to Quit " << endl;
    char temp;
    cin >> temp;    //TODO: toupper this
    if(temp=='q' || temp=='Q') return -1;
    while(!cin || (temp != 'c' && temp != 'C') )
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
        cin >> temp;
        if(temp == 'q' || temp == 'Q') return -1;
    }
    return 0;
}
  
/**
 @brief
    calls Hand::hit() and adds some dealer talking text. Time delays added for player to count, suspense
 @returns   int
    -1      player busts
    X       player's hand value
 */
int Dealer::hitPlayer(Hand& player, Shoe* shoe)
{
    cout << "Player has         :    " << player.getHand() <<  "      Initially\n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);
    int newPlayerVal = player.hit(shoe);
    if(newPlayerVal < 0)
    {
        cout << "Player has         :    " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "\nOh NO!! You busted.. You LOST your bet of $"<< player.getBet() <<"    >:-(\n";
    }
    else
    {
        cout << "Player has         :    " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "for a value of  " << newPlayerVal <<" \n";
    }
    return newPlayerVal;
}

/**
 @brief
    Just like hitPlayer() but user can choose hit card. For testing only.
 @returns int
    X   -   player new hand value
    -1  -   player busts
 */
int Dealer::testHitPlayer(Hand& player)
{
    unordered_map<char, uint8_t> revCardMap =
    {
        {'A',0x11},{'2', 0x21}, {'3',0x31},{'4',0x41}, {'5',0x51},{'6',0x61}, {'7',0x71}, {'8',0x81}, {'9',0x91}, {'T',0xA1}, {'J',0xB1}, {'Q',0xC1},{'K',0xD1}
    };
    cout << "Test Hitting \n" << endl;
    cout << "Player has         :    " << player.getHand() <<  "      Initially\n";
    cout << "\n";
    cout << "Input desired card from hitting" << endl;
    char temp;
    cin >> temp;
    temp = toupper(temp);
    while(!cin || (temp != 'A' && temp != '2' && temp != '3' && temp != '4' && temp != '5' && temp != '6' && temp != '7' && temp != '8' && temp != '9' && temp != 'T' && temp != 'J' && temp != 'Q' && temp != 'K'))
    {
        cout << "wrong input. enter 2-9 or T,J,Q,K " << endl;
        cin >> temp;
        temp = toupper(temp);
    }
    int newPlayerVal = player.testHit(revCardMap[temp]);
    std::chrono::seconds duration(2);
    if(newPlayerVal < 0)
    {
        cout << "Player has         :    " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "\nOh NO!! You busted.. You LOST your bet of $"<< player.getBet() <<"    >:-(\n";
    }
    else
    {
        cout << "Player has         :    " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "for a value of  " << newPlayerVal <<" \n";
    }
    return newPlayerVal;
}

/**
 @brief
    get the basic strategy for any hand vs dealer's shown card.
 @discussion
    This method is used for user with print==true, so that it prints strategy hints
    to the console. When it is called for computer players' actions, print==false
    and only the action is returned, without anything being printed.
 @param count
    the count of the deck. Strategy will be adjusted accordings
 @param print (bool)
    true (default) -    print statements
    false                   no print statements. (player vs AI strategy)
 @return char for action
    'h' -   hit
    's' -   split
    'p' -   stand pat
    'd' -   double
    'x' -   surrender
 */
char Dealer::correctAction(Hand& player, Hand* dealer, int count, bool print)
{
    //
    //TODO: add print staements for adjusted count
    //
    char upCard = dealer->getFirstCard();
    char first = player.getFirstCard();
    char second = player.getSecondCard();
    int playerValue = player.getValue();
    
    if(player.getValue() == 21 )
    {
        if(print) cout << "\n** Of course STAND PAT on 21 !!! **\n";
        return 'p';
    }
    
    // Splits
    if(first == second && player.getNumCards()==2)
    {
        if(first == 'A')
        {
            if(print) cout << "** \nAlways split Aces **\n";
            return 's';
        }
        if(first == 'T' || first == 'J' || first == 'Q' || first == 'K')
        {
            if(print) cout << "\n** Always Stand Pat with a pair of 10's **\n";
            return 'p';
        }
        if(first == '9')
        {
            //A pair of 9’s splits against dealer 2 through 9, except for 7, otherwise stand.
            if(print) cout << "\n** Split a pair of 9's against dealer 2 through 9, except 7. Else stand. **\n";
            if(upCard >= '2' && upCard <= '6') return 's';
            else if(upCard == '7') return 'p';
            else if(upCard == '8' || upCard == '9') return 's';
            else return 'p';
        }
        if(first == '8')
        {
            if(print) cout << "\n** Always split 8's **\n";
            return 's';
        }
        if(first == '7')
        {
            if(print) cout << "\n** Split a pair of 7's against dealer 2 through 7, otherwise hit **\n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
        if(first == '6')
        {
            if(print) cout << "\n** Split a pair of 6's against dealer 5 through 6, otherwise hit **\n";
            if(upCard >= '2' && upCard <= '6') return 's';
            else return 'h';
        }
        if(first == '5')
        {
            if(print) cout << "\n** Double a pair of 5's against dealer 2 through 9, otherwise hit **\n";
            if(upCard >= '2' && upCard <= '9') return 'd';
            else return 'h';
        }
        if(first == '4')
        {
            if(print) cout << "\n** Split a pair of 4's against dealer 5 and 6, otherwise hit **\n";
            if(upCard == '5' || upCard == '6') return 's';
            else return 'h';
        }
        // A pair of 3’s splits against dealer 2 through 7, otherwise hit.
        if(first == '3')
        {
            if(print) cout << "\n** Split a pair of 3's against dealer 2 through 7, otherwise hit **\n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
        // pair of 2’s
        if(first == '2')
        {
            if(print) cout << "\n** Split a pair of 2's against dealer 2 through 7, otherwise hit **\n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
    }
    // Soft Hands
    else if(player.isSoft())
    {
        if(playerValue == 20)
        {
            if(print) cout << "\n** Always stand pat with soft 20 **\n\n";
            return 'p';
        }
        if(playerValue == 19 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 19 against dealer 6, otherwise STAND PAT **\n\n";
            if(upCard == '6') return 'd';
            else return 'p';
        }
        if(playerValue == 19)
        {
            if(print) cout << "\n** Always STAND with 19 when doubling is not an option **\n\n";
            return 'p';
        }
        if(playerValue == 18 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 18 against dealer 2 through 6, hit against 9 through A otherwise STAND PAT **\n\n";
            if(upCard >= '2' && upCard <= '6') return 'd';
            if(upCard == '9' || upCard == 'T' || upCard == 'J' || upCard =='Q' || upCard =='K' || upCard =='A') return 'h';
            else return 'p';
        }
        if(playerValue == 18)
        {
            if(print) cout << "\n** With No double option, HIT Soft 18 Hit vs 9 or 10, otherwise STAND PAT **\n\n";
            if(upCard == '9' || upCard == 'T' || upCard == 'J' || upCard =='Q' || upCard =='K' || upCard =='A') return 'h';
            else return 'p';
        }
        if(playerValue == 17 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 17 against dealer 3 through 6, otherwise HIT **\n\n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 17)  // more than 2 cards
        {
            if(print) cout << "\n** When Can't double, Always HIT Soft 17 **\n\n";
            return 'h';
        }
        if(playerValue == 16 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 16 against dealer 4 through 6, otherwise HIT **\n\n";
            if(upCard >= '4' && upCard <= '6')
            {
                return 'd';
            }
            else
            {
                return 'h';
            }
        }
        if(playerValue == 16)
        {
            if(print) cout << "\n** When Can't double, Always HIT soft 16 **\n\n";
            return 'h';
        }
        if(playerValue == 15 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 15 against dealer 4 through 6, otherwise HIT **\n\n";
            if(upCard >= '4' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 15)
        {
            if(print) cout << "\n** When no double option, always HIT soft 15 **\n\n";
            return 'h';
        }
        if(playerValue == 14 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 14 against dealer 5 or 6, otherwise HIT **\n\n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 14)
        {
            if(print) cout << "\n** When can't double, Always HIT soft 14 **\n\n";
            return 'h';
        }
        if(playerValue == 13 && player.getNumCards()==2)
        {
            if(print) cout << "\n** DOUBLE soft 13 against dealer 5 or 6, otherwise HIT **\n\n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 13)
        {
            if(print) cout << "\n** When no double option, Always HIT soft 13  **\n\n";
            return 'h';
        }
    }
    // Hard Hands
    else if(!player.isSoft())
    {
        if(playerValue >= 17)
        {
            if(print) cout << "\n** Always STAND PAT with hard 17 OR BETTER **\n\n";
            return 'p';
        }
        if(playerValue == 16)
        {
            if(print) cout << "\n** STAND PAT with hard 16 against dealer 2 through 6, otherwise HIT **\n\n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 15)
        {
            if(print) cout << "\n** STAND PAT with hard 15 against dealer 2 through 6, otherwise HIT **\n\n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 14)
        {
            if(print) cout << "\n** STAND PAT with hard 14 against dealer 2 through 6, otherwise HIT **\n\n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 13)
        {
            if(print) cout << "\n** STAND PAT with hard 13 against dealer 2 through 6, otherwise HIT **\n\n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 12)
        {
            if(print) cout << "\n** STAND PAT with hard 12 against dealer 4 through 6, otherwise HIT **\n\n";
            if(upCard >= '4' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 11 && player.getNumCards() == 2)
        {
            if(print) cout << "\n** Always DOUBLE 11 **\n\n";
            return 'd';
        }
        if(playerValue == 11)
        {
            if(print) cout << "\n** When no double option, Always HIT 11 **\n\n";
            return 'h';
        }
        if(playerValue == 10 && player.getNumCards() == 2)
        {
            if(print) cout << "\n** DOUBLE 10 against dealer 2 through 9, otherwise HIT **\n\n";
            if(upCard >= '2' && upCard <= '9') return 'd';
            else return 'h';
        }
        if(playerValue == 10)
        {
            if(print) cout << "\n** When no double option, always HIT 10 **\n\n";
            return 'h';
        }
        if(playerValue == 9 && player.getNumCards() == 2)
        {
            if(print) cout << "\n** DOUBLE 9 against dealer 3 through 6, otherwise HIT **\n\n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 9)
        {
            if(print) cout << "\n** When no double option, HIT 9  **\n\n";
            return 'h';
        }
        if(playerValue <= 8)        // TODO: double check this is the final condition I need
        {
            if(print) cout << "\n** HIT 8 or less **\n\n";
            return 'h';
        }
    }
    return 'h';
}
                                            /*
string printCard(char first, char sec)
{
    string ret = "";
    
    string s = "   _____\n";
            s+="   |A .  |\n";
            s+="   | /.\ |\n";
            s+="   |  |  |\n";
            s+="   |____V|\n";
    
    
    
}
                                             */

/**
 @brief
    Destructor for Dealer
 */
Dealer::~Dealer()
{
    //cout << "Dealer destructor being called \n";
    delete dealerHand;
}
                                                    /*
Dealer& Dealer::operator= (const Dealer& dealer)
{
    Dealer deal(dealer._numPlayers);
    deal
                                                        
}
                                                     */
/**
 @brief
    Overloaded ostream operator<< to easily print off a dealer's info
 */
    /*
std::ostream& operator<<(std::ostream& s, const Dealer& dealer)
{
    s << "_numPlayers == " << dealer._numPlayers << "\n";
    s << "User handArray: \n";
    for(int i=0; i<dealer.handArray.size(); i++)
    {
        s << "i == " << i << " " << dealer.handArray[i] <<"\n";
    }
    s << "User Pat handArray: \n";
    for(int i=0; i<dealer.handArray.size(); i++)
    {
        s << "i == " << i << " " << dealer.patHands[i] <<"\n";
    }
    s << "otherPlayers Hands: \n";
    for(int i=0; i<dealer.otherPlayers.size(); i++)
    {
        s << "i == " << i << " " << dealer.otherPlayers[i] <<"\n";
    }
    s << "otherPlayers Pat Hands: \n";
    for(int i=0; i<dealer.otherPlayers.size(); i++)
    {
        s << "i == " << i << " " << dealer.otherPats[i] <<"\n";
    }
    s << "dealerHand: " << dealer.dealerHand->getHand() << "\n";
    return s;
    
}
     */
