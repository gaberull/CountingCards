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
    otherPlayers = std::vector<Hand>(numPlayers-1);
    handArray = std::vector<Hand>();
    dealerHand = new Hand();
}

/**
 @brief      removes bet from playerBank at start of hand. Adds it back for push.
 
 @returns    1 if hand is ongoing
             0 if hand is finished
             -1 to quit game
 */
int Dealer::dealHands(Shoe* shoe, Bank* playerBank, int bet)
{
    handArray = std::vector<Hand>();    // TODO: these should reset arrays at each hand deal. Check if necessary
    otherPlayers = std::vector<Hand>();
    
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
    bool blackjack = false; // TODO: add this to hand, remove from here
    bool dealerBlackjack = false;
    // vector of player hands. User will be zero, dealer will be _numPlayers
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        // we are on the user
        if(i == 0)
        {
            Hand playerHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
            handArray.push_back(playerHand);
            std::cout << "\nYour cards     :     " << playerHand.getHand() << "\n\n\n";
            blackjack = playerHand.isBlackjack();
            
        }
        else if(i == _numPlayers) // we are on dealer
        {
            dealerHand = new Hand(shoe->dealCard(), shoe->dealCard());
            std::cout << "Dealer shows   :     " << dealerHand->displayOne() << "\n \n";
            
            dealerBlackjack = dealerHand->isBlackjack();
            lastRound = shoe->shoeFinished();
            if(lastRound)
            {
                std::cout << "This will be the last hand on this shoe \n";
            }
        }
        else    // other AIPlayers
        {
            // putting bet=0 on AI hands
            Hand AIHand = Hand(shoe->dealCard(), shoe->dealCard());
            std::cout << "Player "<<i+1<< " has   :     " << AIHand.getHand() << std::endl;
            otherPlayers.push_back(AIHand);
            
        }
        if(_numPlayers > 1)     // pause if we have dealt out more than just the one player
        {
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
    }
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
int Dealer::action(Shoe* shoe, Bank* playerBank, char action) // TODO: maybe remove this action default value
{
    Hand playerHand = handArray.back();
    
    //cout << "\n     Good Luck!!  \n\n";
                                                                                /*
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
    cout << "| CURRENT BET  : $"<< playerHand.getBet() << "  \n";
    cout << "----------------------------- \n \n";
                                                                                 */
    
    //handArray.pop_back(); // do this later. when hand done
 
    // TODO: bet=0 default, and can pull bet from hands.
    //TODO: somehow need to call action() for each hand in handArray to handle split hands
    
    int player = playerHand.getValue();
    if(action =='a')
    {
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
                handArray.pop_back();
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
                if(handArray.size()>0)
                {
                    cout << "Playing your next hand (from splitting)!! \n\n";
                    return Dealer::action(shoe, playerBank);
                }
                return 0;
            }
            else    // still alive after player hit
            {
                //TODO: set hand to persist somehow
                return Dealer::action(shoe, playerBank);  // return 1; ??
            }
            break;
        }
        
            /// Stand Pat - 'p'
        case 'p': {
            cout << "\nPlayer Chooses to Stand with         "<< playerHand.getHand() << " \n\n";
            playerHand.setPat(true);
            handArray.pop_back();
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
            
            // if we still have some hands from splitting:
            if(handArray.size() > 0)
            {
                return Dealer::action(shoe, playerBank);
            }
            return 0; // this hand is finished
            break;
        }
            /// Split the hand - must be a pair. We don't end on split, so it won't return a 0 or 1. Only calls action again.
        case 's': {  //Player splits a pair. Must double bet or add remainder of stack
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
                
                
                return Dealer::action(shoe, playerBank);  // ??
            }
            else    // split the  hand
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
                    
                    return Dealer::action(shoe, playerBank);
                }
                //TODO: All new hands need to play against same dealer action
                
                // subtract bet again from bank. Betting 2x original bank now
                playerBank->removeFunds(bet);
                Hand newHand = playerHand.split(shoe);    // this will change playerHand and create newHand
                handArray.pop_back();
                handArray.push_back(playerHand);
                handArray.push_back(newHand);
                cout << "\nNew Hand 1        : "<< handArray[handArray.size()-1].getHand() <<"\n";
                cout << "\nNew Hand 2        : "<< handArray[handArray.size()-2].getHand() <<"\n";
                
                
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
            handArray.pop_back();   //TODO: check if necessary
            
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
                
                if(handArray.size() > 0)
                {
                    return Dealer::action(shoe, playerBank);
                }
                return 0;
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
                
                return 0;   // hand finished
            }
            break;
        }
        case 'm': {  // Get strategy Hint: 1)basic strategy 2)count adjusted strategy
            cout << "\nPlayer Requests a strategy hint \n\n";
            cout << "Your hand is       :   " << playerHand.getHand() << "\n";
            cout << "Against Dealer's   :   " << dealerHand->displayOne() << "\n";
            cout << "Your hand value   " << playerHand.getValue() << "\n";
            this->correctAction(playerHand, dealerHand);    // print=false by default and it prints to console
            
            // pause for user to take in action
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
            
        default: {
            break;
        }
    }
    return 1;   //TODO: 1== not done
}

//TODO: Finish this function
/**
 @brief Plays the AI hands (hitting, standing, splitting)
 */
void Dealer::computerAction(Shoe* shoe)    //TODO: finish this - Like action() for my hand
{
    int numAIHands = (int)otherPlayers.size();
    //int numPats = (int)otherPats.size();
    if(numAIHands == 0)
    {
        return;
    }
    cout << "Performing AI hand actions \n";
    for(int i=1; i<=numAIHands; i++)                        //TODO: finish this. Gotta figure out new functions maybe
    {
        cout << "AI Player has " << otherPlayers.back().getHand() << "\n";
        Hand curr = otherPlayers.back();
        char action = this->correctAction(curr, dealerHand, false);
        
        switch (action) {
            case 'h':
                cout << "Computer Player will hit \n";
                curr.hit(shoe);
                break;
            case 'p':
                cout << "Computer Player will Stand Pat \n";
                break;
            case 's':
                cout << "Computer Player will split \n";
                break;
            case 'd':
                cout << "Computer Player will double \n";   // actually just hitting once
                break;
            default:
                break;
        }
    }
}

/**
 @brief     This function will hit the Dealer until they bust or have a good enough score to stand pat.
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
    if(patHands.size()==0 && otherPats.size()==0 && !dealerHand->isBlackjack())
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
    
    //TODO: play AI hands
    
    int dealerScore = dealerHand->getValue();
    cout << "\nDealer Has      " << dealerHand->getHand() << "   initially \n";
    std::chrono::seconds duration(3);
    std::this_thread::sleep_for(duration);                      // Pause so player can count cards
    //cout << "for a value of  " << dealerScore <<" \n\n";
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
        //cout << "Dealer Has      " << dealerHand->getValue() << "  \n\n";
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
    }
    
    //TODO: check this -  AI pat hands against dealer
    
    int numAIHands = (int)otherPats.size();
    for(int i=1; i<=numAIHands; i++)  // 1-indexed for purposes of printing
    {
        Hand AI = otherPats.back();  // TODO: check this operator= works fine
        otherPats.pop_back();
        cout << "AI Player " << i << " has " << AI.getHand() << "\n";
        (dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's :  " << dealerHand->getHand() << "\n";
        if(AI.getValue() < dealerScore)  // player loses
        {
            cout << "AI Player " << i << " Loses \n";
        }
        else if(AI.getValue() == dealerHand->getValue())    // player ties
        {
            cout << "AI Player " << i << " Pushes \n";
        }
        else // player wins (not with blackjack)
        {
            cout << "AI Player " << i << " Wins \n";
            
        }
    }
    //cout << "\nDealer will now complete AI players' hands\n\n";
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
    std::chrono::seconds duration(3);
    std::this_thread::sleep_for(duration);
    int newPlayerVal = player.hit(shoe);    // TODO: handArray not getting updated hand yet
    handArray.pop_back();           //TODO: double check this works
    handArray.push_back(player);
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
    
    //TODO: implement surrenders later
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
        if(playerValue == 19)
        {
            if(print) cout << "Double soft 19 against dealer 6, otherwise stand pat \n";
            if(upCard == '6') return 'd';
            else return 'p';
        }
        if(playerValue == 18)
        {
            if(print) cout << "Double soft 18 against dealer 2 through 6, hit against 9 through A otherwise stand pat \n";
            if(upCard >= '2' && upCard <= '6') return 'd';
            if(upCard == '9' || upCard == 'T' || upCard == 'J' || upCard =='Q' || upCard =='K' || upCard =='A') return 'h';
            else return 'p';
        }
        if(playerValue == 17)
        {
            if(print) cout << "Double soft 17 against dealer 3 through 6, otherwise hit \n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 16)
        {
            if(print) cout << "Double soft 16 against dealer 4 through 6, otherwise hit \n";
            if(upCard >= '4' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 15)
        {
            if(print) cout << "Double soft 15 against dealer 4 through 6, otherwise hit \n";
            if(upCard >= '4' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 14)
        {
            if(print) cout << "Double soft 14 against dealer 5 or 6, otherwise hit \n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue == 13)
        {
            if(print) cout << "Double soft 13 against dealer 5 or 6, otherwise hit \n";
            if(upCard >= '5' && upCard <= '6') return 'd';
            else return 'h';
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
        if(playerValue == 11)
        {
            cout << "Always double 11 \n";
            return 'd';
        }
        if(playerValue == 10)
        {
            cout << "Double 10 against dealer 2 through 9, otherwise hit \n";
            if(upCard >= '2' && upCard <= '9') return 'd';
            else return 'h';
        }
        if(playerValue == 9)
        {
            cout << "Double 9 against dealer 3 through 6, otherwise hit \n";
            if(upCard >= '3' && upCard <= '6') return 'd';
            else return 'h';
        }
        if(playerValue <= 8)        // TODO: double check this is the final condition I need
        {
            cout << "Hit 8 or less \n";
            return 'h';
        }
    }
    return 'h';
}
/**
 @brief  Destructor for Dealer
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
