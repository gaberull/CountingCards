# Counting Cards - A blackjack game by Gabe Scott

Source code files are in ./CountingCards/

## Summary

Who doesn't love a good game of blackjack? ***CountingCards*** helps the player learn blackjack gameplay and basic strategy, provides a fun way to practice keeping the count of the deck, offers advice on adjusting your bet and adjusting strategy based on the count of the deck.  

CountingCards is a fun and educational way to help the you bring your best game to the casino or to a blackjack game among friends. It will help you ***win money at blackjack***, or at the very least, ***limit your losses***

## Installation Instructions

For Unix architecture (Mac or Linux):

- Download folder from this repository containing source files: ./CountingCards/
- In a terminal, navigate to that source code folder
- Run Makefile with command:  
    ```make```
    OR  
    ```make all```
- Run resultant executable file named "counting" with command: ```./counting```  
- *optional*: After compilation, execute command ```make clean``` to remove object files and the compiled executable file called "counting"

**Note:**

- If you have limited/no developer experince, and haven't installed command line tools before, open INSTALLATION.md and follow the more detailed instructions to compile the code

## Features of CountingCards

- Choose how many decks of cards you want in the dealer's shoe
- Choose how deep to set the cut card - This simluates a real world blackjack game and should affect one's counting and betting strategy  
  - Play 100% of shoe *(This isn't done in real life games)*
  - Play 90% of shoe
  - Play 75% of shoe
  - Play 50% of shoe
- Choose total number of players (e.g. choosing 4 would result in 3 computer players and him/herself)
- Add funds to bankroll as desired
  - Add up to MAX_RELOAD when out of funds
  - add on up to an amount that brings bankroll total up to MAX_RELOAD * 3/4 when not out of funds
- Check your own count of the shoe against the true count, kept by CountingCards
- Track net win/loss over entire session
- Computer players all play according to common basic strategy for their hand vs the dealer's shown card
- In-hand menu of play options include:
  - Hit hand
  - Stand pat
  - Split hand
    - Double down
      - split and double can be stacked multiple times as in a real game of blackjack
    - Strategy hint
      - get basic strategy for the player's hand vs the dealer's shown card
      - get adjusted strategy based on count of the deck vs dealer's shown card
    - Get current running count and true count (ratio of running count to decks remaining)
    - List rules of the game
    - Add on funds to bankroll
      - game tracks add on amounts, and doesn't let user add more than an amount that would bring player's bankroll to MAX_RELOAD * 3/4 during a single hand, even if option to add on is selected multiple times. 
    - Quit with option from menu prompt
- Net session results are displayed at end of game, when the quit option is requested  
- Testing Mode (hidden feature):
  - If user inputs the number of players to be 9, they can choose the hand that is dealt to them
  - User can then choose the card they receive from hitting or doubling.
  - Since these cards are not dealt out of the shoe, they do not affect the count
  - This is a hidden feature and there is no prompt on-screen that tells the player about this

## Blackjack Terminology

- ***Blackjack*** - A two card hand that contains exactly one ace, and one hand counting for 10 points (King, Queen, Jack, or Ten). This pays out at a ratio of 3:2
- ***Dealer*** - The person distributing hands to all the players and him/herself.
- ***Bust, busted*** - When a hand goes over 21 and is now dead
- ***Soft Hand*** - A hand that contains at least one ace still counting for 11 points. Since Aces can count for either 1 or 11 points, this hand has no chance of busting by taking a card
- ***Hard, hard hand*** - A hand that does not have any aces counting for 11 in it. It can either contain aces or not  
- ***Shoe*** - the box that holds the deck(s) that the cards are being distributed from
- ***Hit, hitting*** - Taking a card from the shoe
- ***Double, doubling down*** - The action of doubling the bet on a hand and taking just one card. This hand's action is now complete
- ***Split, splitting hand*** - The action of splitting a pair in a hand into two hands, with each hand then receiving one card. The original bet is doubled and then split between the two hands
- ***Standing, standing pat*** - when a player has not busted, and will not be taking any more cards. The action for that hand is complete  
- ***Count*** - The positive or negative value that is calculated by adding one for high cards, and subtracting one for low cards. A higher count is better for the player, because a deck full of high cards works in the player's favor, due to a higher payout when hitting blackjack

## Game of Blackjack Rules

- Players act before the dealer acts
- Players attempt to get the better score between him/herself and the dealer, up to 21, without going over (busting)  
- Aces can be worth 11 or 1 points, with the better of the two options applying automatically  
- A hand that wins is paid 1:1, unless it is a blackjack, which pays 3:2
- For this particular implementation of Blackjack, the dealer will stand pat on both soft and hard 17s, and any hand with better than 17 points
- For more information about blackjack rules see [Bicycle's Blackjack Rules](https://bicyclecards.com/how-to-play/blackjack/)

## Some info on card counting in the game of Blackjack

Generally speaking, the player does not have an edge long-term against the casino in the game of blackjack... However, this can change when the count of the deck becomes high enough, and a player adjusts his bet-sizing accordingly. This is for the simple reason that players get paid 3:2 when hitting blackjack. The more high cards in the deck, the more likely this is to happen. So, if a player can place smaller bets when there are few high cards in the shoe, and larger bets when there are more high cards in the shoe, the player can gain and edge against the casino. CountingCards is a game that aims to help the player learn to do just that.

## Shuffle algorithm implementation

The CountinCards shuffle algorithm is based on the modern Fisher-Yates shuffle algorithm, in which each draw of a card from the dealer's shoe is pseudo-random. The algorithm works as follows:

- Before each card draw, the random number generator is seeded with the system's current time:  

```C++  
srand((unsigned int)std::time(0));
```

- Next, a random number between 0 and _cardsRemaining-1 is selected:

```C++  
int i = rand() % _cardsRemaining; 
```

- Then, the card that was selected is swapped with the final card in the shoe that is still eligible to be drawn:

```C++
swap(fullShoe[i], fullShoe[_cardsRemaining-1]);
```

- After the card is dealt, _cardsRemaining is decremented. This makes the card that was just drawn ineligible to be drawn again:

```C++  
_cardsRemaining--
```

*This code can be viewed in the file [shoe.cpp](https://github.com/gaberull/CountingCards/blob/master/CountingCards/shoe.cpp). Specifically in the function Shoe::dealCard().*

## Demo

**One Player Gameplay:**

<br><br>![Start Screen](https://raw.githubusercontent.com/gaberull/CountingCards/master/screenshots/8_14_22/1p_gameplay.gif)
<br><br><br>

**Testing Mode:**

<br><br>![Start Screen](https://raw.githubusercontent.com/gaberull/CountingCards/master/screenshots/8_14_22/testing_mode.gif)
<br><br><br>

### Screenshots

<br>
**Start Screen:**
![Start Screen](/screenshots/8_12_22/start_game.png)
**Hitting blackjack:**
![blackjack](/screenshots/8_12_22/blackjack.png)
**Strategy hint:**
![double](/screenshots/8_12_22/strat_double9.png)
**Doubling hand:**
![double](/screenshots/8_12_22/double9_and_strat.png)
**Hitting hand:**
![hitting](/screenshots/8_12_22/hitting.png)
**Busting hand:**
![busting](/screenshots/8_12_22/busting.png)
**Getting count:**
![get count](/screenshots/8_12_22/count.png)
**Dealer busts:**
![dealer bust](/screenshots/8_12_22/dealer_bust.png)
**Testing mode:**
![test mode](/screenshots/8_12_22/test_mode.png)
**Exit game:**
![exit](/screenshots/8_12_22/exit.png)

## Future Features

- GUI
  - Currently working on a GUI and full mobile game for iOS and Android using **React Native**, connecting it to the **C++** backend. Ideally, the use of React should make it very portable, and able to run on multiple platforms without much modification from one to the next
- Add in ability for player to play multiple hands, with a second acting as another player and reducing the number of players allowed at the table by one
- Analysis of player's betting patterns, and advice on improvement
- Analysis and tabulation of times when player did not act correctly
  - option to print to file
- More implementation ideas and tasks to complete are in the file [TODO.md](https://github.com/gaberull/CountingCards/blob/master/TODO.md)

### References

#### Blackjack

- [blackjackapprenticeship.com](https://www.blackjackapprenticeship.com/blackjack-strategy-charts/) for Blackjack basic strategy charts
- [Wizards of Odds Counting Strategy](https://wizardofodds.com/games/blackjack/card-counting/high-low/)
- [Bicycle.com - How to play blackjack](https://bicyclecards.com/how-to-play/blackjack/)

#### Coding

- [Rounding Floating Point int C++ - G4G](https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/) for using implicit int conversion to round a float to 2 digits
- [Markdown Basic Syntax - markdownguide.org](https://www.markdownguide.org/basic-syntax/#link-best-practices)

<br><br><br>
Copyright (c) 2022 Gabe Scott
