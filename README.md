# Counting Cards 

## A blackjack Game by Gabe Scott

### Summary
Who doesn't love a good game of blackjack? ***CountingCards*** helps the player learn blackjack gameplay and basic strategy, provides a fun way to practice keeping the count of the deck, offers advice on adjusting your bet and adjusting strategy based on the count of the deck. This game will be a fun way to help the you bring your best game to the casino. *This game helps the user win money at the casino, or at the very least, limit their losses*

> **TODO:** Finish these features (7/29/22)

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
    - list rules
    - surrender  **TODO:** Not implemented as of 8/5/22. Might not
    - quit      

> **TODO:** Finish these rules (7/29/22)


### Some info on card counting in the game of blackjack
- first item


### Game of Blackjack Rules

- Player attempts to get the better score between him/herself and the dealer, up to 21, without going over. 
- Player acts before the dealer
- Aces can be worth 11 or 1 at the player's discretion. (In CountingCards, the optimal of the two is automatically awarded)
- Pairs can be split by matching the size of the original bet. They then become two hands.
- For this particular implementation of Blackjack, dealer will stand on both soft and hard 17s

For more information about blackjack rules see [Bicycle's Blackjack Rules](https://bicyclecards.com/how-to-play/blackjack/).

*Source files are in ./CountingCards/*

#### References:
- [blackjackapprenticeship.com](https://www.blackjackapprenticeship.com/blackjack-strategy-charts/) for Blackjack basic strategy charts
- [Wizards of Odds Counting Strategy](https://wizardofodds.com/games/blackjack/card-counting/high-low/)
