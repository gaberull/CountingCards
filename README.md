# Counting Cards 

### A blackjack Game by Gabe Scott

## Installation Instructions

For Unix architecture (Mac or Linux):
- Download folder containing source files: ./CountingCards/
- In a terminal, naviage to source code folder
- Run Makefile with command: "make" or "make all"
- run executable file named "counting" with command: "./counting" 
- optional: After, run "make clean" to remove object files and executable file
- **If you have limited/no developer experince, and haven't installed command line tools before, open INSTRUCTIONS.txt and follow the more detailed instructions to compile the code**

## TODO:

- [ ] complete the features list below
- [ ] complete the blackjack rules list below
- [ ] complete info on counting cards in general
- [x] decide on README.md header sizes
- [ ] **Remove this list and section**
- [ ] add ability for player to play multiple hands
- [ ] figure out where to put/remove **Notes:**:

    - ***Source files are in ./CountingCards/***   (probably keep this one)
    - dimensions of screenshot images: 91 x 28

## Summary

Who doesn't love a good game of blackjack? ***CountingCards*** helps the player learn blackjack gameplay and basic strategy, provides a fun way to practice keeping the count of the deck, offers advice on adjusting your bet and adjusting strategy based on the count of the deck. This game will be a fun way to help the you bring your best game to the casino or a blackjack game with friends. 

***This game will help the user to win money at the casino, or at the very least, limit their losses***

## Features of CountingCards

- Player chooses how many decks of cards they want in the dealer's shoe
- Player chooses total number of players (choosing 4 would result in 3 computer players and him/herself)
- Add funds to player bank as desired
- Request optimal play at any point based on blackjack basic strategy
- Check your own count of the shoe against the true count, kept by CountingCards
- Track net win/loss over entire session
- Select option from menu prompt to get optimal strategy for any hand against the dealer's shown card
- Select option from menu prompt to get current count of deck
- Select option from menu prompt to get the rules of the game
- Computer players all play according to common basic strategy for their hand vs the dealer's shown card
- In hand play options include:
    - Hit hand
    - Stand pat
    - Split hand
    - Double down
        * split and double can be stacked like in a real game of blackjack
    - Strategy hint
        * get basic strategy for the player's hand vs the dealer's shown card
        * get adjusted strategy based on count of the deck vs dealer's shown card
    - Get current running count
    - List rules
    - Add funds
    - Quit  
- Net session results are displayed at end of game, when the quit option is requested    

## Blackjack Terminology

- Blackjack - A two card hand that contains exactly one ace, and one hand counting for 10 points (King, Queen, Jack, or Ten). This pays out at a ratio of 3:2.
- Dealer - The person distributing hands to all the players and him/herself.
- Bust, busted - When a hand goes over 21 and is now dead.
- Soft Hand - A hand that contains at least one ace still counting for 11 points. Since Aces can count for either 1 or 11 points, this hand has no chance of busting by taking a card.
- Hard, hard hand - A hand that does not have any aces counting for 11 in it. It can either contain aces or not.  
- Shoe - the box that holds the deck(s) that the cards are being distributed from.
- Hit, hitting - Taking a card from the shoe.
- Double, doubling down - The action of doubling the bet on a hand and taking just one card. This hand's action is now complete.
- Split, splitting - The action of splitting a pair in a hand into two hands, with each hand then receiving one card. The original bet is doubled and then split between the two hands.
- Standing, standing pat - when a player has not busted, and will not be taking any more cards. The action for that hand is complete. 
- Count - The positive or negative value that is calculated by adding one for high cards, and subtracting one for low cards. A higher count is better for the player, because a deck full of high cards works in the player's favor, due to a higher payout when hitting blackjack.

## Game of Blackjack Rules

- Players act before the dealer acts
- Players attempt to get the better score between him/herself and the dealer, up to 21, without going over . 
- Aces can be worth 11 or 1 points, with the better of the two options applying automatically. 
- For this particular implementation of Blackjack, dealer will stand on both soft and hard 17s and any hand with better than 17 points.

For more information about blackjack rules see [Bicycle's Blackjack Rules](https://bicyclecards.com/how-to-play/blackjack/).

### Some info on card counting in the game of Blackjack

- first item

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

### References:

#### Blackjack

- [blackjackapprenticeship.com](https://www.blackjackapprenticeship.com/blackjack-strategy-charts/) for Blackjack basic strategy charts
- [Wizards of Odds Counting Strategy](https://wizardofodds.com/games/blackjack/card-counting/high-low/)
- [Bicycle.com - How to play blackjack](https://bicyclecards.com/how-to-play/blackjack/)

#### Coding

- [Rounding Floating Point int C++ - G4G](https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/) for using implicit int conversion to round a float to 2 digits
- [Markdown Basic Syntax - markdownguide.org](https://www.markdownguide.org/basic-syntax/#link-best-practices)
