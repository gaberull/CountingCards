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
 */
int Dealer::dealHands(Shoe* shoe, Bank& playerBank, int bet)
{
    handArray = std::vector<Hand>();
    otherPlayers = std::vector<Hand>();
    
    if(playerBank.getBalance() < bet)
    {
        cout <<" You have bet more than you have. Try again \n\n";
        return 0;
    }
    playerBank.removeFunds(bet);
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
            Hand currHand = Hand(shoe->dealCard(), shoe->dealCard(), bet);
            handArray.push_back(currHand);
            std::cout << "Your cards     :     " << currHand.getHand() << std::endl;
            blackjack = currHand.isBlackjack();
            
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
        else
        {
            // putting bet=0 on AI hands
            Hand AIHand = Hand(shoe->dealCard(), shoe->dealCard());
            otherPlayers.push_back(AIHand);
            
        }
    }
    if(dealerBlackjack && !blackjack)
    {
        cout << "Dealer Has:       " << dealerHand->getHand() << "\n";
        cout << "That lucky buffoon has BLACKJACK.. Got us this time.. \n\n";
        cout << "_____________________________ \n \n";
        cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
        cout << "----------------------------- \n \n";
        
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
        return 0;
    }
    if(blackjack)
    {
        if(dealerBlackjack)
        {
            playerBank.addFunds(bet);
            cout << "OH NO!!!! You have BLACKJACK, BUT so does the dealer! :(  This round is a push! \n";
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
            cout << "----------------------------- \n \n";
            
            
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
            return 0;
        }
        else
        {
            cout << "****   CONGRATS!!!! You have a BLACKJACK!! It pays 3:2!!   ****\n\n\n";
            playerBank.addFunds(bet);   // add bet back in first. then payout
            playerBank.payBlackjack(bet);
            cout << "Dealer Has:       " << dealerHand->getHand() << "\n\n";
            cout << "_____________________________ \n \n";
            cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
            cout << "----------------------------- \n \n";
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
            return 0;
            
        }
    }
    return 1;
    
}
/**
 @brief     Dealer offers menu of option and then performs it. The user
            can input an action character. if action parameter is the default 'a',
            will request action character, otherwise skips that
 @param     shoe - the shoe containing all the decks that the game is being played with
 @param     playerBank - the user's current balance of funds, and functions to update that balance
 @param     bet - the player's bet is input when this function is called
 @param     action - this char is set to default value='a'   in the function declaration, and may not end up being used
        TODO: check this line of documentation above for action char
 @returns   0 if hand is done, 1 if hand continues, -1 to quit program
 @note      this function does the paying out to player and removing of lost bets
 @discussion    action() adds the main functionality of the gameplay. It covers all actions that the player can take.
                actions include: hit, stand, split, double, get strategy hint, get current running count, list rules, surrender
 */
int Dealer::action(Shoe* shoe, Bank& playerBank, int bet, char action) // TODO: maybe remove this action default value
{
    cout << "\n     Good Luck!!  \n\n";
    cout << "_____________________________ \n \n";
    cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
    cout << "| CURRENT BET  : $"<< bet << "  \n";
    cout << "----------------------------- \n \n";
 
    // taking bet out at start in case it gets called again. will add back 2*bet
    playerBank.removeFunds(bet);    // TODO: change this. bet=0 default, and can pull bet from hands.
    
    //TODO: somehow need to call action() for each hand in handArray to handle split hands
    // ideas: pop current hand off back of array, then iterate through array from back to start
    
    // get last hand
    Hand playerHand = handArray.back();
    playerBank.removeFunds(playerHand.getBet());
    
    handArray.pop_back();
    //  Pop off back one at end
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
        case 'h': {  //hitPlayer(int bet, Shoe shoe, Bank playerBank)
            cout << "\nPLAYER WILL HIT! \n";
            player = hitPlayer(shoe);
            if(player<0)   // player busts
            {
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
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
                if(temp=='c' || temp=='C') return 0;
                if(temp=='q' || temp=='Q') return -1;
            }
            else    // still alive after player hit
            {
                playerBank.addFunds(bet);
                return Dealer::action(shoe, playerBank, bet);  // return 1; ??
            }
            break;
        }
        
            /// Stand Pat - 'p'
        case 'p': {
            cout << "\nPlayer Chooses to Stand Pat with score of "<< player << " \n\n";
            dealer = dealerAction(shoe);
            if(dealer < 0)  // dealer busts
            {
                playerBank.addFunds(bet*2);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
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
                
            }
            else    // dealer did not bust
            {
                cout << "\n Action is Done. \n";
                cout << "\nPlayer has a score of "<< player << " \n\n";
                cout << "\nDealer has a score of "<< dealer << " \n\n";
                if(player < dealer)
                {
                    cout << "\nDealer wins :-( \n";
                    cout << "\nYou lost your bet of  $"<< bet << "\n\n";
                }
                else if(player > dealer)
                {
                    cout << "\nYOU WIN!!! :)  \n";
                    cout << "\nYou won  $"<< bet << "\n\n";
                    playerBank.addFunds(bet*2);
                }
                else // draw
                {
                    cout << "\nHAND IS A DRAW \n";
                    cout << "\nGood luck on the next one\n\n";
                    playerBank.addFunds(bet);
                }
                
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
                cout << "----------------------------- \n \n";
                
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
                if(handArray.size()>0)
                {
                    return Dealer::action(shoe, playerBank, bet);
                }
                return 0;
            }
               // this hand is finished
            break;
        }
            /// Split the hand - must be a pair
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
                
                // this hand will not be played. put funds for bet back
                playerBank.addFunds(bet);
                return Dealer::action(shoe, playerBank, bet);  // ??
            }
            else    //TODO: Write code to split a hand
            {
                // subtract bet again from bank. Betting 2x original bank now
                playerBank.addFunds(bet);
                //TODO: fix this. All new hands need to play against same dealer action
                // I think maybe call action twice. With new hands
                                                                        /*
                int a = Dealer::action(shoe, playerBank, bet);
                int b = Dealer::action(shoe, playerBank, bet);
                                                                        */
    
                ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LEFT OFF HERE     START
                std::vector<Hand> newHands;
                newHands = playerHand.split(shoe);
                handArray.push_back(newHands[0]);
                handArray.push_back(newHands[1]);
                int a = Dealer::action(shoe, playerBank, bet);
                // if a == 0
                while(a==1)
                {
                    a = Dealer::action(shoe, playerBank, bet);
                }
                // figure out how to get the dealer in here: dealer = hitDealer(shoe);
                int b = Dealer::action(shoe, playerBank, bet);
                while(b==1)
                {
                    b = Dealer::action(shoe, playerBank, bet);
                }
                
                
                
                
                dealer = dealerAction(shoe);
                if(dealer < 0)  // dealer busts
                {
                    playerBank.addFunds(bet*4);
                    cout << "_____________________________ \n \n";
                    cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
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
                    
                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// LEFT OFF HERE      END
            }
            
            break;
        }
        case 'd': {// Double down
            cout << "Player chooses to DOUBLE!! \n";
            cout << "Player gets one additional card and doubles bet \n";
            int remaining_funds = playerBank.getBalance()-bet;
            if(remaining_funds < bet)
            {
                cout << "Player only has $" << remaining_funds << " to bet\n";
                bet += remaining_funds;
            }
            else
            {
                bet *= 2;
            }
            player = hitPlayer(shoe);
            if(player<0)   // player busts
            {
                playerBank.removeFunds(bet);
                cout << "_____________________________ \n \n";
                cout << "| BANKROLL     : $"<< playerBank.getBalance() <<" \n";
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
                if(temp=='c' || temp=='C') return 0;
                if(temp=='q' || temp=='Q') return -1;
            }
            else    // still alive after player doubled
            {
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
                
                return 1;   // dealer has yet to act
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
  
// TODO: LEFT OFF HERE on 7/29 at 8:18 PM. FINISH this and fix this up
// returns -1 if player busts. player's hands value otherwise
//
// hit() is for player only
//
int Dealer::hitPlayer(Shoe* shoe)
{
    Hand player = handArray[0];
    Hand dealer = *(dealerHand);
    cout << "Dealer::hit() - Hitting player's hand \n";
    int newPlayerVal = player.hit(shoe);
    if(newPlayerVal < 0)
    {
        cout << "\nOh NO!! You busted.. You have lost your bet \n";
        //playerBank.removeFunds(bet);  // wrong spot
        // Dealer shows hand but it is over
        // TODO: change this when I add in more computer players. Hand won't be displayed yet
        cout << "Dealer Had:       " << dealer.getHand() << "\n\n"; // TODO: check that Hand dealer works (persists)
        
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
        
        return newPlayerVal;
    }
    else
    {
        cout << "Player has "<< player.getHand() << " for a value of " << newPlayerVal <<" \n";
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
        return newPlayerVal;
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
 This function will hit the Dealer until they bust or have a good enough score to stand pat.
 @returns -1 if dealer busts   ||     else score of the dealer's hand
 */
int Dealer::dealerAction(Shoe* shoe)
{
    Hand dealer = (*dealerHand);
    int dealerScore = dealer.getValue();
    cout << "Dealer Has   " << dealer.getHand() << "   initially \n\n";
    cout << "Dealer's hand has a value of " << dealerScore <<" \n\n";
    int numHits = 0;
    
    while(dealerScore < 17 && dealerScore>0)
    {
        dealerScore = dealer.hit(shoe);
        numHits++;
        cout << "Dealer Has   " << dealer.getHand() << "    after hitting "<< numHits <<" times \n\n";
        if(dealerScore > 0)
        {
            cout << "Dealer's hand has a score of " << dealer.getValue() << "  \n\n";
        }
        else
        {
            cout << "Dealer BUSTS!!!! WOOOOOOT! Enjoy that money \n";
        }
        
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
    }
    return dealerScore;
}
    
/**
 TODO: take a look at this. May need editing
 
 Pops back of handArray off, splits it in 2, puts both new news back in handArray
 @returns Hand for no reason really
 */
Hand Dealer::splitHand(Hand& hand, Shoe* shoe)
{
    vector<Hand> arr = hand.split(shoe);
    handArray.pop_back();
    handArray.push_back(arr[0]);
    handArray.push_back(arr[1]);
    
    return arr[1];
    
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
