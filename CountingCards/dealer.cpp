//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include <thread>   // for sleep_for
#include <chrono>   // for sleep_for
#include <iostream>
#include "dealer.hpp"
#include "shoe.hpp"
#include "hand.hpp"
#include "bank.hpp"
#include <iostream>

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
    removes bet from playerBank at start of hand. Bet is then added to hand in Hand() constructor
 @returns   int
    1   -   hand is ongoing
    0   -   hand is finished
    -1  -   quit game
 */
int Dealer::dealHands(Shoe* shoe, Bank* playerBank, int bet)
{
    delete dealerHand;
    handArray = std::vector<Hand>();    // TODO: these should reset arrays at each hand deal. Check if necessary
    otherPlayers = std::vector<Hand>();
    //this->dealerBlackjack = false;
    
    if(playerBank->getBalance() < bet)      // Check that bet amount is in playerBank
    {
        cout <<" You have bet more than you have. Input new Bet \n\n";
        char bet_str[10];
        cout << "\nENTER NEW BET     |     Enter 'q' to quit. \n";
        cin >> bet_str;
        if(bet_str[0] == 'q' || bet_str[0] == 'Q') return -1;
        while(!cin || bet_str[0] < '0' || bet_str[0] > '9')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter number to bet or 'q' to quit \n";
            cin >> bet_str;
            if(bet_str[0] == 'q' || bet_str[0] == 'Q') return -1;
        }
        int bet = (int) stol(bet_str);
        return dealHands(shoe, playerBank, bet);
    }
    
    playerBank->removeFunds(bet);
    bool blackjack = false;
    
    std::cout << "\n** Dealing Cards **\n";
    
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
            cout << "Player " <<_numPlayers - i+1<< " has BLACKJACK!! Gets paid 3:2 \n\n";
        }
        //FIXME: change ordering so that dealer checks hand first, then payout or don't payout
        //else if(AIHand.isBlackjack() && dealerHand->isBlackjack())
        //{
        //    cout << "Player " <<_numPlayers - i+1<< " has Blackjack, but so does dealer. Hand is a push\n\n";
        //}
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
    }
    if(shoe->endOfShoe())
    {
        std::cout << "This will be the last hand on this shoe \n";
    }
    //TODO: maybe display computer players here in order of back to front of otherPlayers vector
    if(!blackjack && dealerHand->isBlackjack())
    {
        handArray.pop_back();
        cout << "Dealer Has         :    " << dealerHand->getHand() << "\n";
        cout << "\nThat lucky buffoon has BLACKJACK.. Got us this time.. \n\n";   //FIXME: too much displayed when dealer wins here heads up
        cout << "_____________________________ \n \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        
        //////////////////////////////                 Continue sequence                /////////////////////////////////////////////////
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
        //////////////////////////////               End of  Continue sequence             ////////////////////////////////////////////
        return 0;
    }
    if(blackjack)
    {
        if(dealerHand->isBlackjack())
        {
            /// HAND IS A PUSH
            handArray.pop_back();
            playerBank->addFunds(bet);   // put funds back in that we took out at start for bet
            //playerBank.addFunds(bet);
            cout << "OH NO!!!! You have BLACKJACK, BUT so does the dealer! :(  This round is a push! \n";
            cout << "Dealer Has         :    " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            //////////////////////////////                 Continue sequence                /////////////////////////////////////////////////
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
            //////////////////////////////               End of  Continue sequence             ////////////////////////////////////////////
            
            return 0;
        }
        else    // Player beats dealer with a blackjack
        {
            playerBank->addFunds(bet);
            playerBank->payBlackjack(bet);
            int win = bet * 3/2;
            string win_str = std::to_string(win);
            cout << "****   You hit a BLACKJACK!!!  You Win $" << win_str << "  ****\n\n";
            // add bet back in first. then payout blackjack
            
            //playerBank->payBlackjack(bet);
            handArray.pop_back();
            //cout << "Dealer Has         :    " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            //////////////////////////////                 Continue sequence                /////////////////////////////////////////////////
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
            //////////////////////////////               End of  Continue sequence             ////////////////////////////////////////////
            return 0;
        }
        
    }
    return 1;   // 1 means hand is not over. funds have already been removed from playerBank
    
}

/**
 @brief
    Dealer offers menu of option and then performs it. The user
    can input an action character. if action parameter is the default 'a',
    will request action character, otherwise skips that
 
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
int Dealer::action(Shoe* shoe, Bank* playerBank, char action)
{
    if(handArray.size() == 0)
    {
        return 0;
    }
    Hand playerHand = handArray.back();
    handArray.pop_back();
    
    // TODO: bet=0 default, and can pull bet from hands.
    
    int player = playerHand.getValue();
    if(action =='a')
    {
        cout << "\n------------------------------------\n";
        cout << "Your Cards         :    "<< playerHand.getHand() << " \n";
        cout << "Dealer shows       :    " << dealerHand->displayOne() << "\n";
        cout << "------------------------------------\n";
        // List menu of options for the player
        cout << "\n***  Please choose the action you would like to take  ***\n";
        cout << "---------------------------------------------------------------------------\n";
        cout << "||  'h' - Hit            |  'p' - Stand Pat      |  's' - Split          ||\n";
        cout << "||  'd' - Double Down    |  'm' - Strategy Hint  |  'c' - Current Count  ||\n";
        cout << "||  'r' - List Rules     |  'x' - Surrender      |  'q' or 'Q' to Quit   ||\n";
        cout << "---------------------------------------------------------------------------\n\n";
        
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
            cout << "\nPlayer Requests to Quit. \n\n";
            return -1;
            break;
        }
        case 'q': {  // Double down
            cout << "\nPlayer Requests to Quit. \n\n";
            return -1;
            break;
        }
            /// Hit the Hand
        case 'h': {
            //cout << "\nYOU CHOSE TO HIT! \n";
            cout << "\n** Hitting player's hand **\n\n";
            player = hitPlayer(playerHand, shoe);
            if(player<0)   // player busts. money removed from bank and added to hand in dealHands()
            {
                //cout << "Player busts\n\n";
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
                cout << "----------------------------- \n \n";
            }
            else
            {
                handArray.push_back(playerHand);
            }
            return Dealer::action(shoe, playerBank);
            break;
        }
        
            /// Stand Pat
        case 'p': {
            cout << "\nPlayer Stands with :    "<< playerHand.getHand() << " \n";
            cout << "for a value of     "<< playerHand.getValue() << " \n\n";
            playerHand.setPat(true);
            patHands.push_back(playerHand);
            
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
            
            return Dealer::action(shoe, playerBank);
            
            break;
        }
            // Split the hand - must be a pair. won't return a 0 or 1. Only calls action again.
            //TODO: Double check split works correctly
        case 's': {  //Player splits a pair. Must double bet OR add remainder of bankroll to bet
            cout << "\nPlayer Splits \n";
            if(!playerHand.isSplittable())
            {
                cout << "\n** This hand cannot be split. Please choose another action **\n";
                                                                                                /*
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
                                                                                                 */
                std::chrono::seconds duration(2);
                std::this_thread::sleep_for(duration);
                
                handArray.push_back(playerHand);
                return Dealer::action(shoe, playerBank);  // ??
            }
            else    // split the hand
            {
                int bet = playerHand.getBet();
                if(playerBank->getBalance() < bet)
                {
                    cout << "Not enough funds to split. Choose another action \n";
                    
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
                    handArray.push_back(playerHand);
                    return Dealer::action(shoe, playerBank);
                }
                
                // subtract bet again from bank. Betting 2x original bank now
                playerBank->removeFunds(playerHand.getBet());
                Hand newHand = playerHand.split(shoe);    // this will change playerHand and create newHand

                
                
                cout << "\nHand 1             :    " << playerHand.getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                cout << "\nHand 2             :    " << newHand.getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                //cout << "\nNew Hand 1       : "<< handArray[handArray.size()-1].getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                //cout << "\nNew Hand 2       : "<< handArray[handArray.size()-2].getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                
                // play the new (split) hands
                cout << "\nWe will now play the two new hands \n";
                //////////////////////////////                 Continue sequence                /////////////////////////////////////////////////
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
                //////////////////////////////               End of  Continue sequence             ////////////////////////////////////////////
                
                handArray.push_back(playerHand);
                if(playerHand.isBlackjack())
                {
                    int bet = playerHand.getBet();
                    string bet_str = to_string(bet * 3/2);
                    cout << "\nHand 1 is blackjack!! You win $"<< bet_str << " \n\n";
                    playerBank->addFunds(playerHand.getBet());
                    playerBank->payBlackjack(bet);
                    handArray.pop_back();
                }
                handArray.push_back(newHand);
                if(newHand.isBlackjack())
                {
                    int bet = newHand.getBet();
                    string bet_str = to_string(bet * 3/2);
                    cout << "\nHand 2 is blackjack!! You win $"<< bet_str << " \n\n";
                    playerBank->addFunds(newHand.getBet());
                    playerBank->payBlackjack(bet);
                    handArray.pop_back();
                }
                
                return Dealer::action(shoe, playerBank);
            }
            break;
        }
        case 'd': {// Double down
            cout << "Player chooses to DOUBLE!! \n";
            
            if(playerHand.getNumCards() > 2)
            {
                cout << "\n** You can only double down on a hand with exactly two cards.**\n";
                cout << "** Please choose another action. **\n";
                handArray.push_back(playerHand);
                // pause for user to take in action
                std::chrono::seconds duration(3);
                std::this_thread::sleep_for(duration);
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
                return Dealer::action(shoe, playerBank);
            }
            else if(playerBank->getBalance() < newBet)
            {
                cout << "Player doesn't have enough to double bet. Player adds their roll to bet of $"<< playerBank->getBalance() <<" to hand. \n";
                newBet = playerBank->getBalance();
            }
            
            //doubleHand( newbet
            playerBank->removeFunds(newBet);
            
            int oldbet = playerHand.getBet();
            playerHand.setBet(oldbet + newBet);
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
            cout << "| CURRENT BET  : $"<< playerHand.getBet() << "  \n";
            cout << "----------------------------- \n \n";
            
            // add one and only one card to player's hand. Then it is pat. or bust.
            cout << "** Doubling player's hand **\n\n";
            player = hitPlayer(playerHand, shoe);
            playerHand.setPat(true);
            
            if(player<0)   // player busts
            {
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
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
                
                return Dealer::action(shoe, playerBank);
            }
            else    // still alive after player doubled
            {
                patHands.push_back(playerHand);
                //cout << "\nPlayer has a score of "<< player << " \n\n";
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
                
                return Dealer::action(shoe, playerBank);   // hand finished
            }
            break;
        }
        case 'm': {  // Get strategy Hint: 1)basic strategy 2)count adjusted strategy
            cout << "\nPlayer Requests a strategy hint \n\n";
            cout << "Your hand is         :    " << playerHand.getHand() << "\n";
            cout << "Against Dealer's     :    " << dealerHand->displayOne() << "\n";
            cout << "Your hand value     " << playerHand.getValue() << "\n";
            this->correctAction(playerHand, dealerHand);
            handArray.push_back(playerHand);
            cout << "\n***  Please choose the action you would like to take  ***\n";    // TODO: Maybe ask for action input
            char temp;
            cin >> temp;
            if(temp=='q' || temp=='Q') return -1;
            if(!cin || (temp != 'h' && temp != 'p' && temp != 's' && temp != 'd' && temp != 'm' && temp != 'c' && temp != 'r' && temp != 'x'))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nWrong Input. \n";
                return Dealer::action(shoe, playerBank);
            }
            return Dealer::action(shoe, playerBank, temp);
            break;
        }
        case 'c': {  // Current count of Deck
            //cout << "\n**Player Requests the count of the deck**\n";  //TODO: see about adding asterisks to display
            cout << "\nPlayer Requests the count of the deck\n\n";
            cout << "The Running Count is "<< shoe->getCount() << "\n";
            cout << "True count (ratio) is "<< shoe->getTrueCount() << "\n"; 
            std::chrono::seconds duration(2);
            std::this_thread::sleep_for(duration);
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
            return Dealer::action(shoe, playerBank);
            break;
        }
        case 'r': {  // List Rules      // TODO: implement listing of blackjack rules
            cout << "\nPlayer Requests a list of the rules  \n\n";
            cout << "* Dealer hits until he has 17 or better  \n";
            cout << "* Dealer stands on Soft 17s  \n";
            cout << "* The running count is calculated as follows:  \n";
            cout << "   - high cards add 1 to count  \n";
            cout << "   - low cards subtract 1 from count  \n";
            cout << "   - The true count is ratio running count to decks remaining  \n";
            cout << "   - The higher the count, the better  \n";
            cout << "   - The true count will be used later on  \n";
            cout << "*** The rest of the rule list has not yet been implemented as of 8/7/22. - Gabe \n\n";
            
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
            
            cout << "\nInput 'c' to continue or 'q' to quit \n";
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
            return Dealer::action(shoe, playerBank);
            break;
        }
        case 'x': {  // Surrender       //TODO: maybe just get rid of surrender altogether
            cout << "\nPlayer Surrenders  :-(  \n";
            cout << "Surrendering has not yet been implemented as of 8/7/22. \n";
            cout << "I likely will just end up removing it from this game altogether - Gabe \n";
            cout << "Please select another option (after 3 second delay)\n";
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
            handArray.push_back(playerHand);        // put hand back in vector of hands being played
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
    //int numAIHands = (int)otherPlayers.size();
    //int numPats = (int)otherPats.size();
    if(otherPlayers.empty())
    {
        return 0;
    }
    //cout << "AI Player has " << otherPlayers.back().getHand() << "\n";
    Hand curr = otherPlayers.back();
    otherPlayers.pop_back();
    char action = this->correctAction(curr, dealerHand, 0, false);
    std::chrono::seconds duration(2);
    switch (action) {
        case 'h':
        {
            
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player hits \n";
            std::this_thread::sleep_for(duration);
            curr.hit(shoe);
            //cout << "\nAfter hit, computer player has " << curr.getHand() <<"\n";
            std::this_thread::sleep_for(duration);
            if(curr.getValue() < 0)
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
        case 's':
        {
            
            cout << "\nComp Player has    :    " << curr.getHand() << "\n";
            cout << "Comp Player splits \n";
            std::this_thread::sleep_for(duration);
            Hand newHand = curr.split(shoe);    // this will change playerHand and create newHand
            otherPlayers.push_back(curr);
            otherPlayers.push_back(newHand);
            cout << "\nHand 1             :    "<< otherPlayers[otherPlayers.size()-1].getHand() <<"\n";
            cout << "\nHand 2             :    "<< otherPlayers[otherPlayers.size()-2].getHand() <<"\n";
            return computerAction(shoe);
            break;
        }
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
            //default will not execute
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
                                                                                                /*
    if(dealerHand->isBlackjack() )   // TODO: check the action, see if I need more than this
    {
        cout << "\n** Action is Complete **\n\n";
        cout << "\n** LINE 689 Dealer.cpp  **\n\n";
        return 0;
    }
                                                                                                 */
    // if there are none of this player's hands to play, or AI hands to play
    if((patHands.size()==0 && otherPats.size()==0) || dealerHand->isBlackjack())
    {
        //cout << "** Action is Complete. Dealer Had   " << dealerHand->getHand() << "**\n\n";  // TODO: see if i need this line
        cout << "\n** Action is Complete **\n\n";
        cout << "\nInput 'c' to continue or 'q' to quit \n";
        char temp;
        cin >> temp;
        if(temp=='q' || temp=='Q') return -1;
        while(!cin || (temp != 'c' && temp != 'C' && temp != 'q' && temp != 'Q'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue. 'Q' or 'q' to quit\n";
            cin >> temp;
        }
        return 0;
    }
    
    cout << "\n** Dealer Acts on Hand **\n";
    int dealerScore = dealerHand->getValue();
    cout << "\nDealer Has         :    " << dealerHand->getHand() << "      initially \n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);              // Pause so player can count cards
    int numHits = 0;
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealerHand->hit(shoe);
        numHits++;
        cout << "Dealer Has         :    " << dealerHand->getHand() << "     after hitting "<< numHits <<" time(s) \n";
        std::this_thread::sleep_for(duration);
    }
    
    if(dealerScore > 0)
    {
        cout << "Dealer action is finished \n\n";
    }
    else
    {
        cout << "Dealer BUSTS!!!!  \n";
    }
    
    cout << "\n** Hand Showdown **\n";
    (dealerHand->getValue() > 0) ?
    cout << "\nDEALER Has         :    " << dealerHand->getHand() << "\n\n":
    cout <<"\nDealer is BUST \n";   //TODO: maybe add \n
    
    int numYourHands = (int)patHands.size();
    for(int i=1; i<=numYourHands; i++)  // 1-indexed for purposes of printing
    {
        Hand playerHand = patHands.back();
        patHands.pop_back();
        (numYourHands<=1) ? cout << "\nYour hand has      :    " << playerHand.getValue() << "\n" : cout << "Your hand number " << i << " has " << playerHand.getValue() << "\n";
        (dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's   :    " << dealerScore << "\n";
        if(playerHand.getValue() < dealerScore)  // player loses
        {
            cout << "YOU LOST your bet of $" << playerHand.getBet() << "\n\n";
        }
        else if(playerHand.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Hand is a Push. You win and lose nothing \n\n";
            playerBank->addFunds(playerHand.getBet());
        }
        else // player wins (not with blackjack)
        {
            cout << "YOU WIN $" << playerHand.getBet() << "!!!\n\n";
            playerBank->addFunds(playerHand.getBet()*2);
        }
    }
    
    int numAIHands = (int)otherPats.size();
    for(int i=1; i<=numAIHands; i++)  // 1-indexed for purposes of printing
    {
        Hand AI = otherPats.back();  // TODO: check this operator= works fine
        otherPats.pop_back();
        //cout << "\nYour cards     :     " << playerHand.getHand() << "\n\n";
        cout << "Comp Player  has   :    " << AI.getHand() << "\n";
        //(dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's :  " << dealerHand->getHand() << "\n";
        if(AI.getValue() < dealerScore)  // player loses
        {
            cout << "Comp Player Loses \n\n";
        }
        else if(AI.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Comp Player Pushes \n\n";
        }
        else // player wins (not with blackjack)
        {
            cout << "Comp Player Wins \n\n";
        }
    }

    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
    cout << "----------------------------- \n";
    std::this_thread::sleep_for(duration);  // pause for 3 sec
    
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
  
/**
 @brief
    calls Hand::hit() and adds some dealer talking text. Time delays added for player to count, suspense
 @returns   int
    -1      player busts
    X       player's hand value
 */
int Dealer::hitPlayer(Hand& player, Shoe* shoe)  //TODO: Check and see if I even need this function - only outputs text.
{
    cout << "Player has         :    " << player.getHand() <<  "      Initially\n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);
    int newPlayerVal = player.hit(shoe);
    if(newPlayerVal < 0)
    {
        cout << "Player has         :    " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "\nOh NO!! You busted.. You have lost your bet of $"<< player.getBet() <<"\n";
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
    get the basic strategy for any hand vs dealer's hand with currently shown card.
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
 */                                 //TODO: add print staements for adjusted count
char Dealer::correctAction(Hand& player, Hand* dealer, int count, bool print)
{
    char upCard = dealer->getFirstCard();
    char first = player.getFirstCard();
    char second = player.getSecondCard();
    int playerValue = player.getValue();
    
    if(player.getValue() == 21 )
    {
        if(print) cout << "Of course STAND PAT on 21 !!! \n";
        return 'p';
    }
    
    // Splits
    if(first == second)
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
            if(print) cout << "\n** STAND PAT with hard 15 against dealer 2 or 6, otherwise HIT **\n\n";
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
    // TODO: remove these stmts and put if(print) before printing
    cout << "\n** HIT **\n\n";
    cout << "\n** ERROR - SHOULD NOT HIT THIS MESSAGE **\n";
    cout << "\n The hand that got here was "<<player.getHand() << "\n";
    cout << "\n The hand that got here has "<<player.getValue() << "\n";
    cout << "\n Soft ? "<<player.isSoft() << "\n";
    cout << "\nInput 'c' to continue or 'q' to quit \n";
    char temp;
    cin >> temp;
    while(!cin || (temp != 'c' && temp != 'C'))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Wrong Input. Enter 'C' or 'c' to continue. \n";
        cin >> temp;
    }
    return 'h';
}

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
                                                                    

/**     display bankroll
 
 cout << "_____________________________ \n \n";
 cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
 cout << "----------------------------- \n \n";
 
 */
