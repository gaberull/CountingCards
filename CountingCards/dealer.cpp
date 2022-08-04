//
//  dealer.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include <iostream>
#include "dealer.hpp"
#include "shoe.hpp"
#include "hand.hpp"
#include "bank.hpp"
#include <iostream>

using namespace std;
                                                                        /*
class Dealer
{
private:
    int _numPlayers;
    std::vector<Hand> handArray;
    std::vector<Hand> otherPlayers;
    Hand* dealerHand;
    Dealer();
public:
    // to deny implicit conversion (cahr to int) using "explicit"
    explicit Dealer(int numPlayers);
    int dealHands(Shoe* shoe, Bank& playerBank, int bet);
    int action(Shoe* shoe, Bank& playerBank, int bet, char action ='a');
    int hitPlayer(Shoe* shoe);
    int hitDealer(Shoe* shoe);
    Hand splitHand(Hand& hand, Shoe* shoe);
    ~Dealer();
    
    friend std::ostream& operator<<(std::ostream& s, const Dealer& dealer);
};
                                                                         */
                                                     
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
 
    @returns 1 if hand is ongoing
             0 if hand is finished
            - 1 to quit game
 
@discussion removes bet from playerBank at start of hand. Adds it back for push.
 */
int Dealer::dealHands(Shoe* shoe, Bank* playerBank, int bet)
{
    handArray = std::vector<Hand>();
    otherPlayers = std::vector<Hand>();
    
    if(playerBank->getBalance() < bet)
    {
        cout <<" You have bet more than you have. Try again \n\n";
        return 0;
    }
    playerBank->removeFunds(bet);
    bool lastRound = false;
    bool blackjack = false; // TODO: add this to hand, remove from here
    bool dealerBlackjack = false;
    //dealerHand = new Hand();
    // vector of player hands. User will be zero, dealer will be _numPlayers
    
    for(int i=0; i<_numPlayers+1; i++)
    {
        // deal two cards to every player and dealer
        //Hand currHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
        // we are on the user
        if(i == 0)
        {
            Hand playerHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
            handArray.push_back(playerHand);
            std::cout << "Your cards     :     " << playerHand.getHand() << "\n\n\n";
            blackjack = playerHand.isBlackjack();
            
        }
        // if we are on dealer
        else if(i == _numPlayers)
        {
            dealerHand = new Hand(shoe->dealCard(), shoe->dealCard());
            std::cout << "Dealer shows   :     " << dealerHand->displayOne() << "\n \n";
            // TODO: overloaded operator= not working
            
            dealerBlackjack = dealerHand->isBlackjack();
            lastRound = shoe->shoeFinished();
            if(lastRound)
            {
                std::cout << "This will be the last hand on this shoe \n";
            }
        }
        // we are on other AIPlayers
        else
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
            playerBank->payBlackjack(bet);   // TODO: check that these two statements add correctly for BJ
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
 
 @returns   0 if hand is done, 1 if hand continues, -1 to quit program
 
 @discussion    adds the main functionality of the gameplay. It covers all actions that the player can take.
                actions include: hit, stand, split, double, get strategy hint, get current running count, list rules, surrender
                Hands are popped off the back of handArray at end of action. Specifically before a 0 is returned
                bet is removed from bank in dealHands(), and then again in 'd' - double and 's' - split (double can add less than bet, split can't)
 */
int Dealer::action(Shoe* shoe, Bank* playerBank, char action) // TODO: maybe remove this action default value
{
    Hand playerHand = handArray.back();
    
    cout << "\n     Good Luck!!  \n\n";
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank->getBalance() <<" \n";
    cout << "| CURRENT BET  : $"<< playerHand.getBet() << "  \n";
    cout << "----------------------------- \n \n";
    
    //handArray.pop_back(); // do this later. when hand done
 
    // TODO: bet=0 default, and can pull bet from hands.
    //TODO: somehow need to call action() for each hand in handArray to handle split hands
    
    int player = playerHand.getValue();
    int dealer = dealerHand->getValue();
    if(action =='a')
    {
        // List menu of options for the player
        cout << "***  Please choose the action you would like to take  ***\n";
        cout << "---------------------------------------------------------\n\n";
        cout << "||  'h' - hit           |  'p' - stand pat                  |  's' - split       |  'd' - double down  ||\n";
        cout << "||  'm' - Strategy Hint |  'c' - get current running count  |  'r' - list rules  |  'x' - surrender    ||\n";
        cout << "    'q' or 'Q' to quit\n\n";
        cout << "---------------------------------------------------------\n\n";
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
            cout << "\nYOU CHOSE TO HIT YOUR HAND! \n";
            player = hitPlayer(playerHand, shoe);
            if(player<0)   // player busts. money removed from bank and added to hand in dealHands()
            {
                handArray.pop_back();
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
            cout << "\nPlayer Chooses to Stand Pat with score of "<< player << " \n\n";
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
                
                // play the new (split) hands
                cout << "\nWe will now play the two new split hands \n";
                
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
                ///
                return Dealer::action(shoe, playerBank);
                
                // ALL THIS BELOW MAYBE GOES IN DEALERACTION() or hitDEALER() or playAIHANDS()
                                                                                            /*
                                
                dealer = dealerAction(shoe);
                if(dealer < 0)  // dealer busts
                {
                    
                    //////////////////////////////                 Bankroll sequence                //////////////////////////////////////////////////////////
                    cout << "_____________________________ \n \n";
                    cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                    cout << "----------------------------- \n \n";
                    //////////////////////////////                 End of Bankroll sequence                /////////////////////////////////////////////////
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
                    
                                                                                             */
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
            
            // add one and only one card to player's hand. Then it is pat. or bust.
            playerHand.setPat(true);
            player = hitPlayer(playerHand, shoe);
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
                
                handArray.pop_back();
                if(handArray.size() > 0)
                {
                    return Dealer::action(shoe, playerBank);
                }
                return 0;
            }
            else    // still alive after player doubled
            {
                
                patHands.push_back(playerHand);
                cout << "\nPlayer has a score of "<< player << " \n\n";
                
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
        case 'm': {  // Get basic strategy Hint
            cout << "\nPlayer Requests a strategy hint \n";
            cout << "\nYou have a score of "<< player << " \n\n";
            cout << "\nAgainst the dealer's score of "<< dealer << " \n\n";
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
        case 'x': {  // Surrender
            cout << "\nPlayer Surrenders  :-(  \n";
            
            break;
        }
            
        default: {
            break;
        }
    }
    
    
    return 1;   //TODO: 1== not done
}
  
/**
 @returns -1 if player busts, player's hand value otherwise
 @brief this function calls Hand::hit() and adds some dealer talking text
 
 TODO: Check and see if I even need this function. Currently only outputs text.
 */
int Dealer::hitPlayer(Hand& player, Shoe* shoe)
{
    //Hand dealer = *(dealerHand);
    cout << "Dealer says \"Hitting player's hand\" \n";
    int newPlayerVal = player.hit(shoe);    // TODO: handArray not getting updated hand yet
    handArray.pop_back();           //TODO: double check this works
    handArray.push_back(player);
    if(newPlayerVal < 0)
    {
        cout << "Player has " << player.getHand() << " after hitting \n";
        cout << "\nOh NO!! You busted.. You have lost your bet \n";
        //playerBank.removeFunds(bet);  // wrong spot
        // Dealer shows hand but it is over
        // TODO: change this when I add in more computer players. Hand won't be displayed yet
        //cout << "Dealer Had:       " << dealerHand->getHand() << "\n\n"; // TODO: check that Hand dealer works (persists)
        
        // TODO: REmove this continue sequence. Already have it where it's being called I think
                                                                    /*
        cout << "Input 'c' to continue \n";
        char temp;
        cin >> temp;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
            cin >> temp;
        }
                                                                     */
    }
    else
    {
        cout << "Player has " << player.getHand() << " after hitting \n";
        cout << "Player hand has a value of " << newPlayerVal <<" \n";
                                                                    /*
        cout << "Input 'c' to continue \n";
        char temp;
        cin >> temp;
        while(!cin || (temp != 'c' && temp != 'C'))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Wrong Input. Enter 'C' or 'c' to continue \n";
            cin >> temp;
        }
                                                                     */
    }
    return newPlayerVal;
}
   
//TODO: write this function
                                    /*
int Dealer::playAIHands(Shoe shoe, int numHands)
{
    if(numHands == 0)
    {
        numHands = numPlayers-1;
    }
}
                                     */

/**
 @brief This function will hit the Dealer until they bust or have a good enough score to stand pat.
 
 @returns 0 if hand done, -1 to quit
 
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
        cout << "Everybody is Complete. Dealer Had   " << dealerHand->getHand() << "\n\n";
        
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
    
    //TODO: play AI hands
    
    int dealerScore = dealerHand->getValue();
    cout << "Dealer Has   " << dealerHand->getHand() << "   initially \n\n";
    cout << "Dealer's hand has a value of " << dealerScore <<" \n\n";
    int numHits = 0;
    
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealerHand->hit(shoe);
        numHits++;
        cout << "Dealer Has   " << dealerHand->getHand() << "    after hitting "<< numHits <<" time(s) \n\n";
    }
    
    if(dealerScore > 0)
    {
        cout << "Delaer action is finished \n";
        cout << "Dealer's hand has a score of " << dealerHand->getValue() << "  \n\n";
    }
    else
    {
        cout << "Dealer BUSTS!!!! WOOOOOOT! Enjoy that money \n";
    }
    
    int numYourHands = (int)patHands.size();
    // TODO: maybe move from indexing to back() and pop_back() from patHands array
    for(int i=1; i<=numYourHands; i++)  // 1-indexed for purposes of printing
    {
        Hand playerHand = patHands.back();  // TODO: check this operator= works fine
        patHands.pop_back();
        (numYourHands>1) ? cout << "Your hand has " << playerHand.getValue() << "\n" : cout << "Your hand number " << i << " has " << playerHand.getValue() << "\n";
        (dealerScore<0) ? cout << "Against Dealer's Busted Hand \n" : cout << "Against Dealer's " << dealerScore << "\n";
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
    
    //TODO: do AI pat hands against dealer
    
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
    
Dealer::~Dealer()
{
    cout << "Dealer destructor being called \n";
    delete dealerHand;
}

/**     display bankroll
 
 cout << "_____________________________ \n \n";
 cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
 cout << "----------------------------- \n \n";
 
 */

                                                                    
/**
 Overloaded ostream operator<< to easily print off a dealer's info
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
