## TODO:

- [ ] Add count notices like "Count is high, maybe you should consider raising your bet
- [ ] Handle computer player getting blackjack on a split hand - currently standing pat
- [ ] check getting bj against dealer's bj in multiplayer and single player
- [x] Handle error where putting too many funds twice
- [x] Implement rules - 'r' in action()
- [ ] Implement change in basic strategy and info for adjusting betting patterns based on true count (the ratio)
- [x] Handle input of capital letters everywhere where a char is input from stdin
- [x] Make hashmaps static for Card class
- [ ] Add table limits (betting) and maybe allow player to select them
- [ ] complete the features list on README.md
- [ ] complete the blackjack rules list on README.md
- [ ] complete info on counting cards in general on README.md
- [x] decide on README.md header sizes
- [x] Remove TODO section from README.md
- [ ] add ability for player to play multiple hands
- [ ] Add ability for user to add more funds without having busted first
- [x] figure out where to put/remove **Notes:** - Putting them in ideas in TODO.md
- [ ] Get new screenshots after style change 8/9/22
- [x] Create testing mode capabilities
    - [x] Choosing player hand
    - [x] Choosing hit card
    - [x] Choosing dealer hand
- [ ] Add testing mode capabilities to splitting and dealer received cards
- [ ] Put new screenshots in their own dated folder on github 
    
## Notes and Ideas to self

- dimensions of terminal for screenshot images: 91 x 28
- Maybe add in words with a few microseconds delay for a hand won that spread across the console
- Sad face on loss, maybe in a box like the winnings
- try to make winnings boxes similar in size, using num of digits of win size and offsetting w/ spaces or lack thereof on opposite end of print statement
- Require bets to be in $5 increments
- Maybe have game analyze betting patters and do something with that