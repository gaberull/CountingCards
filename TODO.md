# TODO

- [x] Handle error where putting too many funds twice
- [x] Implement rules - 'r' in action()
- [x] Handle input of capital letters everywhere where a char is input from stdin
- [x] Make hashmaps static for Card class
- [x] decide on README.md header sizes
- [x] Remove TODO section from README.md
- [x] figure out where to put/remove **Notes:** - Putting them in ideas in TODO.md
- [x] Create testing mode capabilities
  - [x] Choosing player hand
  - [x] Choosing hit card
  - [x] Choosing dealer hand
- [x] Put new screenshots in their own dated folder on github  
- [x] complete the features list on README.md
- [x] complete the blackjack rules list on README.md
- [x] Get new screenshots with the new (much better looking) lowercase  
- [x] Add ability for user to add more funds without having busted first
- [x] Replace menu option "surrender" with add funds after this hand
- [x] Sad face on loss, maybe in a box like the winnings, or some ascii art
- [ ] Look at IMGui and openGL for graphics rendering. View github repo
- [ ] High score component to game? 
- [x] check getting bj against dealer's bj in multiplayer and single player
- [x] Explain about the shuffling algorithm I'm using in README.md
- [ ] Add testing mode capabilities to splitting and dealer received cards
- [ ] report of when (and count) the player did not do the right action.  
  - [ ] Option to print to file
- [ ] Add count notices like "Count is high, maybe you should consider raising your bet"
- [ ] Handle computer player getting blackjack on a split hand - currently standing pat
- [ ] Implement change in basic strategy and info for adjusting betting patterns based on true count (the ratio)
- [ ] Add table limits (betting) and maybe allow player to select them
- [ ] complete info on counting cards in general on README.md
- [ ] add ability for player to play multiple hands
- [ ] in 1 player: user splits hand, the "You win $X" statement needs a newline after it if there is more than 1 hand left. Maybe add new line in either case

- [ ] Add commas if necessary to cash balances printed (bankroll, hand winning, etc)
- [ ] Add table limits (size of MAX_RELOAD)

## Notes and Ideas to self

- dimensions of terminal for screenshot images: 91 x 28
- Maybe add in words with a few microseconds delay for a hand won that spread across the console
- Require bets to be in $5 increments ??
- Have the game analyzea player's betting patterns and offer advice
