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
 constructor to be used
 */
Dealer::Dealer(int numPlayers)
{
    _numPlayers = numPlayers;
    otherPlayers = std::vector<Hand>(); // TODO: was vector<Hand>(_numplayers-1) check this
    handArray = std::vector<Hand>();
    dealerHand = new Hand();
    dealerBlackjack = false;
}

/**
 @returns bool - this dealer has blackjack
 */
bool Dealer::hasBlackjack()
{
    return dealerBlackjack;
}

/**
 @brief      removes bet from playerBank at start of hand. Adds it back for push.
 @returns    1 - hand is ongoing
             0 - hand is finished
             -1 - quit game
 */
int Dealer::dealHands(Shoe* shoe, Bank* playerBank, int bet)
{
    delete dealerHand;
    handArray = std::vector<Hand>();    // TODO: these should reset arrays at each hand deal. Check if necessary
    otherPlayers = std::vector<Hand>();
    this->dealerBlackjack = false;
    
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
    bool lastRound = false;
    bool blackjack = false;
    // vector of player hands. User will be zero, dealer will be _numPlayers
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        // we are on the user
        if(i == 0)
        {
            Hand playerHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
            handArray.push_back(playerHand);
            std::cout << "\nYour cards     :     " << playerHand.getHand() << "\n\n";
            blackjack = playerHand.isBlackjack();
        }
        else if(i == _numPlayers) // we are on dealer
        {
            dealerHand = new Hand(shoe->dealCard(), shoe->dealCard());
            std::cout << "\nDealer shows   :     " << dealerHand->displayOne() << "\n \n";
            this->dealerBlackjack = dealerHand->isBlackjack();
            lastRound = shoe->endOfShoe();
            if(lastRound)
            {
                std::cout << "This will be the last hand on this shoe \n";
            }
        }
        else    // Computer Players
        {
            // putting bet=0 on computer hands
            Hand AIHand = Hand(shoe->dealCard(), shoe->dealCard());
            std::cout << "Player "<<_numPlayers - i+1<< " has   :     " << AIHand.getHand() << std::endl; // FIXME: print ordering doesn't match computerAction()
            if(AIHand.isBlackjack())    //FIXME: doesn't handle if dealer also has bj
            {
                cout << "Player " <<_numPlayers - i+1<< " has BLACKJACK!! Gets paid 3:2 \n";
            }
            else
            {
                otherPlayers.push_back(AIHand);
            }
        }
        if(_numPlayers > 1)     // pause if we have dealt out more than just the one player
        {
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
        }
    }
    //TODO: maybe display computer players here in order of back to front of otherPlayers vector
    if(dealerBlackjack && !blackjack)
    {
        handArray.pop_back();
        cout << "Dealer Has:       " << dealerHand->getHand() << "\n";
        cout << "That lucky buffoon has BLACKJACK.. Got us this time.. \n\n";
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
        if(dealerBlackjack)
        {
            /// HAND IS A PUSH
            handArray.pop_back();
            playerBank->addFunds(bet);   // put funds back in that we took out at start for bet
            //playerBank.addFunds(bet);
            cout << "OH NO!!!! You have BLACKJACK, BUT so does the dealer! :(  This round is a push! \n";
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
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
            cout << "****   CONGRATS!!!! You have a BLACKJACK!! It pays 3:2!!   ****\n\n\n";
            // add bet back in first. then payout blackjack
            playerBank->addFunds(bet);
            playerBank->payBlackjack(bet);
            handArray.pop_back();
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
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
 @brief     Dealer offers menu of option and then performs it. The user
            can input an action character. if action parameter is the default 'a',
            will request action character, otherwise skips that
 @param     shoe - the shoe containing all the decks that the game is being played with
 @param     playerBank - the user's current balance of funds, and functions to update that balance
 @param     action - this char is set to default value='a'   in the function declaration, and may not end up being used
        TODO: check this line of documentation above for 'action' parameter
 
 @discussion    adds the main functionality of the gameplay. It covers all actions that the player can take.
                actions include: hit, stand, split, double, get strategy hint, get current running count, list rules, surrender
                Hands are popped off the back of handArray at end of action. Specifically before a 0 is returned
                bet is removed from bank in dealHands(), and then again in 'd' - double and 's' - split (double can add less than bet, split can't)
 
 @returns   0 if hand is done, 1 if hand continues, -1 to quit program
 */
//TODO: check some action
int Dealer::action(Shoe* shoe, Bank* playerBank, char action)
{
    //FIXME: multiple splits don't work
    if(handArray.size() == 0)
    {
        return 0;
    }
    Hand playerHand = handArray.back();
    handArray.pop_back();
    
    // TODO: bet=0 default, and can pull bet from hands.
    //TODO: somehow need to call action() for each hand in handArray to handle split hands
    
    int player = playerHand.getValue();
    if(action =='a')
    {
        cout << "\n-------------------------------\n";
        cout << "Your Cards     :     "<< playerHand.getHand() << " \n";
        cout << "Dealer shows   :     " << dealerHand->displayOne() << "\n";
        cout << "-------------------------------\n";
        // List menu of options for the player
        cout << "\n***  Please choose the action you would like to take  ***\n";
        cout << "------------------------------------------------------------------------------\n\n";
        cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
        cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n";
        cout << "||  'q' or 'Q' to quit                                                                                 ||\n";
        cout << "------------------------------------------------------------------------------\n\n";
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
            /// Hit the Hand
        case 'h': {
            cout << "\nYOU CHOSE TO HIT! \n";
            player = hitPlayer(playerHand, shoe);
            if(player<0)   // player busts. money removed from bank and added to hand in dealHands()
            {
                cout << "Player busts\n\n";
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
            cout << "\nPlayer Chooses to Stand with         "<< playerHand.getHand() << " \n\n";
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
            /// Split the hand - must be a pair. We don't end on split, so it won't return a 0 or 1. Only calls action again.
        case 's': {  //Player splits a pair. Must double bet OR add remainder of bankroll to bet
            cout << "\nPlayer Splits \n";
            if(!playerHand.isSplittable())
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
                //TODO: All new hands need to play against same dealer action
                
                // subtract bet again from bank. Betting 2x original bank now
                playerBank->removeFunds(playerHand.getBet());
                Hand newHand = playerHand.split(shoe);    // this will change playerHand and create newHand

                handArray.push_back(playerHand);
                handArray.push_back(newHand);
                cout << "\nNew Hand 1       : "<< handArray[handArray.size()-1].getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                cout << "\nNew Hand 2       : "<< handArray[handArray.size()-2].getHand() <<"  bet: $"<<playerHand.getBet()<< "\n";
                
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
                
                return Dealer::action(shoe, playerBank);
            }
            break;
        }
        case 'd': {// Double down
            cout << "Player chooses to DOUBLE!! \n";
            cout << "Player gets one additional card and doubles bet \n";
            
            int newBet = playerHand.getBet();// = playerHand.getBet();
            if(playerBank->getBalance() < newBet)
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
            cout << "Your hand is       :   " << playerHand.getHand() << "\n";
            cout << "Against Dealer's   :   " << dealerHand->displayOne() << "\n";
            cout << "Your hand value   " << playerHand.getValue() << "\n";
            this->correctAction(playerHand, dealerHand);    // print=false by default and it prints to console
            handArray.push_back(playerHand);
            // pause for user to take in action
            std::chrono::seconds duration(3);
            std::this_thread::sleep_for(duration);
                                                                                            /*
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
                                                                                             */
            return Dealer::action(shoe, playerBank);
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
        case 'x': {  // Surrender TODO: maybe just get rid of surrender altogether
            cout << "\nPlayer Surrenders  :-(  \n";
            break;
        }
            // default will not execute
        default: {
            break;
        }
    }
    return 1;   // should never be hit
}

//TODO: Finish this function
/**
 @brief Plays the AI hands (hitting, standing, splitting)
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
            cout << "\nComp Player has " << curr.getHand() << "\n";
            cout << "Computer Player will hit \n";
            std::this_thread::sleep_for(duration);
            curr.hit(shoe);
            //cout << "\nAfter hit, computer player has " << curr.getHand() <<"\n";
            std::this_thread::sleep_for(duration);
            if(curr.getValue() < 0)
            {
                cout << "\nComp Player has " << curr.getHand() << "\n";
                cout << "\nComp Player has busted \n";
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
            cout << "\nComp Player has " << curr.getHand() << "\n";
            cout << "Computer Player will Stand Pat \n";
            otherPats.push_back(curr);
            std::this_thread::sleep_for(duration);
            // computerPat()
            return computerAction(shoe);
            break;
        }
        case 's':
        {
            cout << "\nComp Player has " << curr.getHand() << "\n";
            cout << "Computer Player will split \n";
            std::this_thread::sleep_for(duration);
            Hand newHand = curr.split(shoe);    // this will change playerHand and create newHand
            otherPlayers.push_back(curr);
            otherPlayers.push_back(newHand);
            cout << "\nHand 2    :       "<< otherPlayers[otherPlayers.size()-1].getHand() <<"\n";
            cout << "\nHand 1    :       "<< otherPlayers[otherPlayers.size()-2].getHand() <<"\n";
            return computerAction(shoe);
            break;
        }
        case 'd':
        {
            cout << "\nComp Player has " << curr.getHand() << "\n";
            cout << "Computer Player will double \n";   // actually just hitting once
            std::this_thread::sleep_for(duration);
            curr.hit(shoe);
            //cout << "\nAfter Double, computer player has " << curr.getHand() <<"\n";
            std::this_thread::sleep_for(duration);
            cout << "\nComp Player has " << curr.getHand() << " after doubling\n";
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
 @brief     This function will hit the Dealer until they bust or have a good enough score to stand pat.
            Called after user hands and computer hands have completed hand actions
 @returns   0 - hand is done
            -1 - to quit
 */
int Dealer::dealerAction(Shoe* shoe, Bank* playerBank)  //TODO: don't need to return anything.
{
/*
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
    cout << "----------------------------- \n \n";
*/
    // if there are none of this player's hands to play, or AI hands to play
    if(patHands.size()==0 && otherPats.size()==0 && !dealerHand->isBlackjack())     //TODO: may have handled dealer bj already
    {
        cout << "Action is Complete. Dealer Had   " << dealerHand->getHand() << "\n\n";
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
    
    int dealerScore = dealerHand->getValue();
    cout << "\nDealer Has      " << dealerHand->getHand() << "   initially \n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);              // Pause so player can count cards
    int numHits = 0;
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealerHand->hit(shoe);
        numHits++;
        cout << "Dealer Has      " << dealerHand->getHand() << "    after hitting "<< numHits <<" time(s) \n\n";
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
    
    int numYourHands = (int)patHands.size();
    for(int i=1; i<=numYourHands; i++)  // 1-indexed for purposes of printing
    {
        Hand playerHand = patHands.back();  // TODO: check this operator= works fine
        patHands.pop_back();
        (numYourHands<=1) ? cout << "Your hand has    :  " << playerHand.getValue() << "\n" : cout << "Your hand number " << i << " has " << playerHand.getValue() << "\n";
        (dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's :  " << dealerScore << "\n";
        if(playerHand.getValue() < dealerScore)  // player loses
        {
            cout << "You have lost your bet of $" << playerHand.getBet() << "\n";
        }
        else if(playerHand.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Hand is a Push. You win and lose nothing \n";
            playerBank->addFunds(playerHand.getBet());
        }
        else // player wins (not with blackjack)
        {
            cout << "You win $" << playerHand.getBet() << "!!!\n";
            playerBank->addFunds(playerHand.getBet()*2);
        }
        cout << "_____________________________ \n \n";
        cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        std::this_thread::sleep_for(duration);  // pause for 3 sec
    }
    
    int numAIHands = (int)otherPats.size();
    for(int i=1; i<=numAIHands; i++)  // 1-indexed for purposes of printing
    {
        Hand AI = otherPats.back();  // TODO: check this operator= works fine
        otherPats.pop_back();
        cout << "Comp Player " << i << " has " << AI.getHand() << "\n";
        (dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's :  " << dealerHand->getHand() << "\n";
        if(AI.getValue() < dealerScore)  // player loses
        {
            cout << "Comp Player " << i << " Loses \n\n";
        }
        else if(AI.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "Comp Player " << i << " Pushes \n\n";
        }
        else // player wins (not with blackjack)
        {
            cout << "Comp Player " << i << " Wins \n\n";
        }
    }
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
 @brief calls Hand::hit() and adds some dealer talking text. Time delays added for player to count, suspense
 
 @returns   -1 - player busts
            player's hand value otherwise
 */
int Dealer::hitPlayer(Hand& player, Shoe* shoe)  //TODO: Check and see if I even need this function - only outputs text.
{
    //Hand dealer = *(dealerHand);
    cout << "Dealer says \"Hitting player's hand\" \n";
    cout << "Player has      :   " << player.getHand() <<  "      Initially\n";
    std::chrono::seconds duration(2);
    std::this_thread::sleep_for(duration);
    int newPlayerVal = player.hit(shoe);    // TODO: handArray not getting updated hand yet
    //handArray.pop_back();           //TODO: double check this works
    //handArray.push_back(player);
    if(newPlayerVal < 0)
    {
        cout << "Player has      :   " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "\nOh NO!! You busted.. You have lost your bet \n";
    }
    else
    {
        cout << "Player has      :   " << player.getHand() << "      after hitting \n";
        std::this_thread::sleep_for(duration);
        cout << "for a value of  " << newPlayerVal <<" \n";
    }
    return newPlayerVal;
}

/**
 @brief     get the basic strategy for any hand vs dealer's hand with currently shown card
 @param     count - the count of the deck. Strategy will be adjusted accordings
 @param     print - if true (default), print statements, if false, no print statements. (player vs AI strategy)
 @returns   char for action
            'h'  -  hit
            's'  -  split
            'p'  -  stand pat
            'd'  -  double
            'x'  -  surrender
 */
char Dealer::correctAction(Hand& player, Hand* dealer, int count, bool print)   //TODO: add print staements for strats
{
    char upCard = dealer->getFirstCard();
    char first = player.getFirstCard();
    char second = player.getSecondCard();
    int playerValue = player.getValue();
    
    //TODO: implement surrender later
    /// Surrenders
    
    /// Splits
    if(first == second)
    {
        if(first == 'A')
        {
            if(print) cout << "Always split Aces \n";
            return 's';
        }
        if(first == 'T' || first == 'J' || first == 'Q' || first == 'K')
        {
            if(print) cout << "Always Stand Pat with a pair of 10's \n";
            return 'p';
        }
        if(first == '9')
        {
            //A pair of 9’s splits against dealer 2 through 9, except for 7, otherwise stand.
            if(print) cout << "Split a pair of 9's against dealer 2 through 9, except 7. Else stand. \n";
            if(upCard >= '2' && upCard <= '6') return 's';
            else if(upCard == '7') return 'p';
            else if(upCard == '8' || upCard == '9') return 's';
            else return 'p';
        }
        if(first == '8')
        {
            if(print) cout << "Always split 8's \n";
            return 's';
        }
        if(first == '7')
        {
            if(print) cout << "Split a pair of 7's against dealer 2 through 7, otherwise hit \n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
        if(first == '6')
        {
            if(print) cout << "Split a pair of 6's against dealer 5 through 6, otherwise hit \n";
            if(upCard >= '2' && upCard <= '6') return 's';
            else return 'h';
        }
        if(first == '5')
        {
            if(print) cout << "Double a pair of 5's against dealer 2 through 9, otherwise hit \n";
            if(upCard >= '2' && upCard <= '9') return 'd';
            else return 'h';
        }
        if(first == '4')
        {
            if(print) cout << "Split a pair of 4's against dealer 5 and 6, otherwise hit \n";
            if(upCard == '5' || upCard == '6') return 's';
            else return 'h';
        }
        // A pair of 3’s splits against dealer 2 through 7, otherwise hit.
        if(first == '3')
        {
            if(print) cout << "Split a pair of 3's against dealer 2 through 7, otherwise hit \n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
        // pair of 2’s
        if(first == '2')
        {
            if(print) cout << "Split a pair of 2's against dealer 2 through 7, otherwise hit\n";
            if(upCard >= '2' && upCard <= '7') return 's';
            else return 'h';
        }
    }
    /// Soft Hands
    else if(player.isSoft())
    {
        if(playerValue == 20)
        {
            if(print) cout << "Always stand pat with soft 20 \n";
            return 'p';
        }
        if(playerValue == 19 && player.getNumCards()==2)
        {
            if(print) cout << "Double soft 19 against dealer 6, otherwise stand pat \n";
            if(upCard == '6') return 'd';
            else return 'p';
        }
        if(playerValue == 19)
        {
            if(print) cout << "stand with 19 with more than 2 cards \n";
            return 'p';
        }
        if(playerValue == 18 && player.getNumCards()==2)
        {
            if(print) cout << "Double soft 18 against dealer 2 through 6, hit against 9 through A otherwise stand pat \n";
            if(upCard >= '2' && upCard <= '6') return 'd';
            if(upCard == '9' || upCard == 'T' || upCard == 'J' || upCard =='Q' || upCard =='K' || upCard =='A') return 'h';
            else return 'p';
        }
        if(playerValue == 18) // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Can't double. Soft 18 Hit vs 9,10 otherwise stand pat \n";
            if(upCard == '9' || upCard == 'T' || upCard == 'J' || upCard =='Q' || upCard =='K' || upCard =='A') return 'h';
            else return 'p';
        }
        if(playerValue == 17 && player.getNumCards()==2)
        {
            if(print) cout << "Double soft 17 against dealer 3 through 6, otherwise hit \n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 17)  // more than 2 cards
        {
            if(print) cout << "Can't double. Always hit Soft 17 \n";
            return 'h';
        }
        if(playerValue == 16 && player.getNumCards()==2)  // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Double soft 16 against dealer 4 through 6, otherwise hit \n";
            if(upCard >= '4' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 16)
        {
            if(print) cout << "Can't double. Always Hit soft 16 \n";
            return 'h';
        }
        if(playerValue == 15 && player.getNumCards()==2)  // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Double soft 15 against dealer 4 through 6, otherwise hit \n";
            if(upCard >= '4' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 15)  // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Hit soft 15 when can't double \n";
            return 'h';
        }
        if(playerValue == 14 && player.getNumCards()==2)
        {
            if(print) cout << "Double soft 14 against dealer 5 or 6, otherwise hit \n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 14)  // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Always hit soft 14 when can't double \n";
            return 'h';
        }
        if(playerValue == 13 && player.getNumCards()==2)  // TODO: Add how to play when more than 2 cards
        {
            if(print) cout << "Double soft 13 against dealer 5 or 6, otherwise hit \n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 13)
        {
            if(print) cout << "Always hit soft 13 when can't double \n";
            return 'h';
        }
    }
    /// Hard Hands
    else if(!player.isSoft())
    {
        if(playerValue >= 17)
        {
            if(print) cout << "Always stand pat with hard 17 \n";
            return 'p';
        }
        if(playerValue == 16)
        {
            if(print) cout << "stand pat with hard 16 against dealer 2 through 6, otherwise hit \n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 15)
        {
            if(print) cout << "stand pat with hard 15 against dealer 2 or 6, otherwise hit \n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 14)
        {
            if(print) cout << "stand pat with hard 14 against dealer 2 through 6, otherwise hit \n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 13)
        {
            if(print) cout << "stand pat with hard 13 against dealer 2 through 6, otherwise hit \n";
            if(upCard >= '2' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 12)
        {
            if(print) cout << "stand pat with hard 12 against dealer 4 through 6, otherwise hit \n";
            if(upCard >= '4' && upCard <= '6') return 'p';
            else return 'h';
        }
        if(playerValue == 11 && player.getNumCards() == 2)
        {
            if(print) cout << "Always double 11 \n";
            return 'd';
        }
        if(playerValue == 11)
        {
            if(print) cout << "Hit 11\n";
            return 'h';
        }
        if(playerValue == 10 && player.getNumCards() == 2)
        {
            if(print) cout << "Double 10 against dealer 2 through 9, otherwise hit \n";
            if(upCard >= '2' && upCard <= '9') return 'd';
            else return 'h';
        }
        if(playerValue == 10)
        {
            if(print) cout << "Hit 10 with more than 2 cards \n";
            return 'h';
        }
        if(playerValue == 9 && player.getNumCards() == 2)
        {
            if(print) cout << "Double 9 against dealer 3 through 6, otherwise hit \n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 9)
        {
            if(print) cout << "hit 9 with more than 2 cards \n";
            return 'h';
        }
        if(playerValue <= 8)        // TODO: double check this is the final condition I need
        {
            if(print) cout << "Hit 8 or less \n";
            return 'h';
        }
    }
    cout << "Hit \n";
    return 'h';
}
/**
    Destructor for Dealer
 */
Dealer::~Dealer()
{
    //cout << "Dealer destructor being called \n";
    delete dealerHand;
}
                                                                    
/**
 @brief Overloaded ostream operator<< to easily print off a dealer's info
 */
/*
friend ostream& operator<<(ostream& s, const Dealer& dealer)
{
    s << "_numPlayers == " << _numPlayers << "\n";
    s << "handArray: \n";
    for(int i=0; i<handArray.size(); i++)
    {
        s << "i == " << i << " " << handArray[i] <<"\n";
    }
    s << "otherPlayers: \n";
    for(int i=0; i<otherPlayers.size(); i++)
    {
        s << "i == " << i << " " << otherPlayers[i] <<"\n";
    }
    s << "dealerHand: " << *dealerHand << "\n";
    
}
                                                                    */

/**     display bankroll
 
 cout << "_____________________________ \n \n";
 cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
 cout << "----------------------------- \n \n";
 
 */
