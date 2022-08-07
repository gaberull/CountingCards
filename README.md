# Counting Cards 

## A blackjack Game by Gabe Scott

## Documentation TODO:
- [ ] dimensions of images - 91 x 28
- [ ] complete features list below
- [ ] complete rules
- [ ] complete info on counting cards in general
- [ ] figure out where to put note *Source files are in ./CountingCards/*
- [ ] decide on readme header sizes
- [ ] **Remove this list and section**

### Summary
Who doesn't love a good game of blackjack? ***CountingCards*** helps the player learn blackjack gameplay and basic strategy, provides a fun way to practice keeping the count of the deck, offers advice on adjusting your bet and adjusting strategy based on the count of the deck. This game will be a fun way to help the you bring your best game to the casino. 

***This game will help the user to win money at the casino, or at the very least, limit their losses***

### Features of CountingCards
- Player chooses how many decks of cards they want in the dealer's shoe
- Player chooses total number of players (choosing 4 would result in 3 computer players and him/herself)
- Add funds to player bank as desired
- Request optimal play at any point based on blackjack basic strategy
- Check your own count of the shoe against the true count, kept by CountingCards
- Track net win/loss over entire session
- Select option from menu prompt to get optimal strategy for any hand against the dealer's shown card
- Select option from menu prompt to get current count of deck
- Select option from menu prompt to get the rules of the game
- Computer players all play according to basic strategy for their hand vs the dealer's shown card
- In hand play options include:
    - Hit hand
    - Stand pat
    - Split hand
    - Double down
    - Strategy hint
    - Get current running count
    - List rules
    - Surrender  **TODO:** Not implemented as of 8/5/22. Might not
    - Add funds
    - Quit      

### Some info on card counting in the game of blackjack
- first item

### Game of Blackjack Rules

- Player attempts to get the better score between him/herself and the dealer, up to 21, without going over. 
- Player acts before the dealer
- Aces can be worth 11 or 1 at the player's discretion. (In CountingCards, the optimal of the two is automatically awarded)
- Pairs can be split by matching the size of the original bet. They then become two hands.
- For this particular implementation of Blackjack, dealer will stand on both soft and hard 17s

For more information about blackjack rules see [Bicycle's Blackjack Rules](https://bicyclecards.com/how-to-play/blackjack/).

### Screenshots

// TODO: remove below note
**Note:** dimensions 91 x 28 in terminal for taking screenshots

**Menu of options, New hand starting with two players (one computerplayer):**
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

#### References:
- [blackjackapprenticeship.com](https://www.blackjackapprenticeship.com/blackjack-strategy-charts/) for Blackjack basic strategy charts
- [Wizards of Odds Counting Strategy](https://wizardofodds.com/games/blackjack/card-counting/high-low/)
- [Bicycle.com - How to play blackjack](https://bicyclecards.com/how-to-play/blackjack/)