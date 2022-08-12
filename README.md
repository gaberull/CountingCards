# Counting Cards - A blackjack game by Gabe Scott

Source files are in ./CountingCards/

## Summary

Who doesn't love a good game of blackjack? ***CountingCards*** helps the player learn blackjack gameplay and basic strategy, provides a fun way to practice keeping the count of the deck, offers advice on adjusting your bet and adjusting strategy based on the count of the deck. 

CountingCards is a fun and educational way to help the you bring your best game to the casino or to a blackjack game among friends. It will help you ***win money at blackjack***, or at the very least, ***limit your losses***

## Installation Instructions

For Unix architecture (Mac or Linux):
- Download folder containing source files: ./CountingCards/
- In a terminal, navigate to source code folder
- Run Makefile with command: 
    ```make```
    OR 
    ```make all```
- run resultant executable file named "counting" with command: "./counting" 
- *optional*: After, run ```make clean``` to remove object files and the executable file-

**Note:**
 - If you have limited/no developer experince, and haven't installed command line tools before, open INSTALLATION.md and follow the more detailed instructions to compile the code

## Features of CountingCards

- Choose how many decks of cards you want in the dealer's shoe
- Choose how deep to set the cut card - This simluates a real world blackjack game and should affect one's counting and betting strategy. 
    - Play 100% of shoe (This isn't done in real life games)
    - Play 90% of shoe
    - Play 75% of shoe
    - Play 50% of shoe
- Choose total number of players (e.g. choosing 4 would result in 3 computer players and him/herself)
- Add funds to bankroll as desired
- Check your own count of the shoe against the true count, kept by CountingCards
- Track net win/loss over entire session
- Computer players all play according to common basic strategy for their hand vs the dealer's shown card
- In-hand menu of play options include:
    - Hit hand
    - Stand pat
    - Split hand
    - Double down
        * split and double can be stacked multiple times as in a real game of blackjack
    - Strategy hint
        * get basic strategy for the player's hand vs the dealer's shown card
        * get adjusted strategy based on count of the deck vs dealer's shown card
    - Get current running count and true count (ratio of running count to decks remaining)
    - List rules of the game
    - Add funds after running out
    - Quit with option from menu prompt
- Net session results are displayed at end of game, when the quit option is requested    
- Testing Mode (hidden feature):
    * If user inputs the number of players to be 9, they can choose the hand that is dealt to them
    * User can then choose the card they receive from hitting or doubling.
    * Since these cards are not dealt out of the shoe, they do not affect the count
    * This is a hidden feature and there is no prompt on-screen that tells the player about this


## Blackjack Terminology

- ***Blackjack*** - A two card hand that contains exactly one ace, and one hand counting for 10 points (King, Queen, Jack, or Ten). This pays out at a ratio of 3:2.
- ***Dealer*** - The person distributing hands to all the players and him/herself.
- ***Bust, busted*** - When a hand goes over 21 and is now dead.
- ***Soft Hand*** - A hand that contains at least one ace still counting for 11 points. Since Aces can count for either 1 or 11 points, this hand has no chance of busting by taking a card.
- ***Hard, hard hand*** - A hand that does not have any aces counting for 11 in it. It can either contain aces or not.  
- ***Shoe*** - the box that holds the deck(s) that the cards are being distributed from.
- ***Hit, hitting*** - Taking a card from the shoe.
- ***Double, doubling down*** - The action of doubling the bet on a hand and taking just one card. This hand's action is now complete.
- ***Split, splitting hand*** - The action of splitting a pair in a hand into two hands, with each hand then receiving one card. The original bet is doubled and then split between the two hands.
- ***Standing, standing pat*** - when a player has not busted, and will not be taking any more cards. The action for that hand is complete. 
- ***Count*** - The positive or negative value that is calculated by adding one for high cards, and subtracting one for low cards. A higher count is better for the player, because a deck full of high cards works in the player's favor, due to a higher payout when hitting blackjack.

## Game of Blackjack Rules

- Players act before the dealer acts
- Players attempt to get the better score between him/herself and the dealer, up to 21, without going over . 
- Aces can be worth 11 or 1 points, with the better of the two options applying automatically. 
- A hand that wins is paid 1:1, unless it is a blackjack, which pays 3:2
- For this particular implementation of Blackjack, dealer will stand pat on both soft and hard 17s, and any hand with better than 17 points.
- For more information about blackjack rules see [Bicycle's Blackjack Rules](https://bicyclecards.com/how-to-play/blackjack/).

### Some info on card counting in the game of Blackjack

- [ ] **fill out this section**

## Screenshots

**Menu of options, New hand starting with two players (1 computer player):**
![New Hand - 2 Players](/screenshots/8_7_22/newhand_2players.png "New Hand with 2 Players")
**Menu Display, New hand starting with three players, Deck Count:**
![New Hand - 3 Players, count](/screenshots/8_7_22/count_menu_multihands.png "New Hand with 3 Players")
**Hand showdown and payout of bet against dealer:**
![Hand showdown](/screenshots/8_7_22/showdown.png "Showdown Against Dealer")
**Splitting a Hand, Display of Deck Count:**
![Hand Splitting and Deck Count](/screenshots/8_7_22/split_count.png "Splitting a Hand and Deck Count")
**Requesting a strategy hint from menu:**
![Strategy Request](/screenshots/8_7_22/strats_menu.png "Requesting Strategy Hint")
**Doubling a Hand (2 images):**
![Double Hand](/screenshots/8_7_22/double2.png "Doubling Hand")
![Double Hand 2](/screenshots/8_7_22/double.png "Doubling Hand 2")
**Computer Players Acting on their Hands:**
![Computer Hands Acting](/screenshots/8_7_22/multi_player_action.png "Computer Action")
**Dealer Acting on its Hand:**
![Dealer Acting](/screenshots/8_7_22/pat_dealeraction.png "Dealer Action")
**End of Game and Results Disaply - After user has input 'Q' or 'q' to quit:**
![Quit Screen](/screenshots/8_7_22/quitscreen.png "End of Game")

## Future Features:

**(8/7/22):** 

- GUI : I'm current working on a GUI and full mobile game using **React Native**, connecting it to the C++ backend. Ideally, the use of React should make it very portable, and able to run on multiple platforms without much modification from one to the next.
- Add in ability for player to play multiple hands, reduce total players to 5
- Add in analysis of betting patterns
- Add in analysis of times when player did not act correctly (and count)
    - option to print to file

### References:

#### Blackjack

- [blackjackapprenticeship.com](https://www.blackjackapprenticeship.com/blackjack-strategy-charts/) for Blackjack basic strategy charts
- [Wizards of Odds Counting Strategy](https://wizardofodds.com/games/blackjack/card-counting/high-low/)
- [Bicycle.com - How to play blackjack](https://bicyclecards.com/how-to-play/blackjack/)

#### Coding

- [Rounding Floating Point int C++ - G4G](https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/) for using implicit int conversion to round a float to 2 digits
- [Markdown Basic Syntax - markdownguide.org](https://www.markdownguide.org/basic-syntax/#link-best-practices)



Copyright (c) 2022 Gabe Scott