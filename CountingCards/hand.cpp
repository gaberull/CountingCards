//
//  hand.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//  Copyright (c) 2022 Gabe Scott
//

#include "shoe.hpp"
#include "hand.hpp"
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

using namespace std;

/**
 @brief
    Default Constructor
 */
Hand::Hand()    //TODO: make maps static and const
{
    cardMap = { {0x01, 'A'}, {0x02, '2'},{0x03, '3'},{0x04, '4'},{0x05, '5'},{0x06, '6'},{0x07, '7'},{0x08, '8'},{0x09, '9'},{0x0A, 'T'},{0x0B, 'J'},{0x0C, 'Q'},{0x0D, 'K'} };
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    // Spades, Clubs, Hearts, Diamonds
    suitMap = { {0x01,'S'},{0x02,'C'},{0x03,'H'},{0x04,'D'} };
    _card1 = 0x00;
    _card2 = 0x01;
    cardArray = std::vector<char>(2, '0');
    suitArray = std::vector<char>(2, '0');
    numCards = 2;
    blackjack = false;
    _value = 0;
    splittable = false;
    isPat = false;
    handBet = 0;
    soft = false;
}

/**
@brief
 Copy constructor
 */
Hand::Hand(const Hand& diffHand)
{
    cardMap = { {0x01, 'A'}, {0x02, '2'},{0x03, '3'},{0x04, '4'},{0x05, '5'},{0x06, '6'},{0x07, '7'},{0x08, '8'},{0x09, '9'},{0x0A, 'T'},{0x0B, 'J'},{0x0C, 'Q'},{0x0D, 'K'} };
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    // Spades, Clubs, Hearts, Diamonds
    suitMap = { {0x01,'S'},{0x02,'C'},{0x03,'H'},{0x04,'D'} };
    
    this->_card1 = diffHand._card1;
    this->_card2 = diffHand._card2;
    this->numCards = diffHand.numCards;
    this->cardArray = vector<char>(numCards);
    
    for(int i=0; i<numCards; i++)
    {
        this->cardArray[i] = diffHand.cardArray[i];
    }
    this->suitArray = std::vector<char>(numCards);
    for(int i=0; i<numCards; i++)
    {
        this->suitArray[i] = diffHand.suitArray[i];
    }
    
    this->blackjack = diffHand.blackjack;
    this->splittable = diffHand.splittable;
    this->_value = diffHand._value;
    this->isPat = diffHand.isPat;
    this->handBet = diffHand.handBet;
    this->soft = diffHand.soft;
}

/**
 @brief
    Main constructor that will be used for Hand()
 @param card1
    first card in new hand
 @param card2
    second card in new hand
 @param bet = 0 (default value)
    bet for new hand. Computer hands will be called without inputting bet
    and thus will not be considered since their winnings and losses won't
    be totalled.
 */
Hand::Hand(uint8_t card1, uint8_t card2, int bet)
{
    handBet = bet;  // bet has default value of 0
    isPat = false;
    _card1 = card1;
    _card2 = card2;
    numCards=2;     // will have two cards by end of constructor
    
    cardArray = std::vector<char>(2, '0');
    suitArray = std::vector<char>(2,'0');
    
    cardMap = { {0x01, 'A'}, {0x02, '2'},{0x03, '3'},{0x04, '4'},{0x05, '5'},{0x06, '6'},{0x07, '7'},{0x08, '8'},{0x09, '9'},{0x0A, 'T'},{0x0B, 'J'},{0x0C, 'Q'},{0x0D, 'K'} };
    // valuemap doesn't include Ace
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    // Spades, Clubs, Hearts, Diamonds
    suitMap = { {0x01,'S'},{0x02,'C'},{0x03,'H'},{0x04,'D'} };
                                                                         
    uint8_t rank1 = card1 >> 4;
    uint8_t suit1 = card1 & 0x0F;
    
    switch (rank1) {
        case 0x01:
            cardArray[0] = 'A';
            soft = true;
            _value += 11;
            break;
        case 0x02:
            cardArray[0] = '2';
            _value += 2;
            break;
        case 0x03:
            cardArray[0] = '3';
            _value += 3;
            break;
        case 0x04:
            cardArray[0] = '4';
            _value += 4;
            break;
        case 0x05:
            cardArray[0] = '5';
            _value += 5;
            break;
        case 0x06:
            cardArray[0] = '6';
            _value += 6;
            break;
        case 0x07:
            cardArray[0] = '7';
            _value += 7;
            break;
        case 0x08:
            cardArray[0] = '8';
            _value += 8;
            break;
        case 0x09:
            cardArray[0] = '9';
            _value += 9;
            break;
        case 0x0A:
            cardArray[0] = 'T';
            _value += 10;
            break;
        case 0x0B:
            cardArray[0] = 'J';
            _value += 10;
            break;
        case 0x0C:
            cardArray[0] = 'Q';
            _value += 10;
            break;
        case 0x0D:
            cardArray[0] = 'K';
            _value += 10;
            break;
        default:
            break;
    }
    suitArray[0] = suitMap[suit1];
    
    // get 2nd card
    uint8_t rank2 = card2 >> 4;
    uint8_t suit2 = card2 & 0x0F;
    switch (rank2) {
        case 0x01:
            cardArray[1] = 'A';
            if(_value <=10)
            {
                _value +=11;
                soft = true;    //TODO: double check this
            }
            else
            {
                _value += 1;
            }
            break;
        case 0x02:
            cardArray[1] = '2';
            _value += 2;
            break;
        case 0x03:
            cardArray[1] = '3';
            _value += 3;
            break;
        case 0x04:
            cardArray[1] = '4';
            _value += 4;
            break;
        case 0x05:
            cardArray[1]= '5';
            _value += 5;
            break;
        case 0x06:
            cardArray[1] = '6';
            _value += 6;
            break;
        case 0x07:
            cardArray[1] = '7';
            _value += 7;
            break;
        case 0x08:
            cardArray[1] = '8';
            _value += 8;
            break;
        case 0x09:
            cardArray[1] = '9';
            _value += 9;
            break;
        case 0x0A:
            cardArray[1] = 'T';
            _value += 10;
            break;
        case 0x0B:
            cardArray[1] = 'J';
            _value += 10;
            break;
        case 0x0C:
            cardArray[1] = 'Q';
            _value += 10;
            break;
        case 0x0D:
            cardArray[1] = 'K';
            _value += 10;
            break;
        default:
            break;
    }
    
    suitArray[1] = suitMap[suit2];
    if(cardArray[0]==cardArray[1])
    {
        splittable = true;
    }
    if(cardArray[0] == 'A' && (cardArray[1] == 'T' || cardArray[1] == 'J' || cardArray[1] == 'Q' || cardArray[1] == 'K'))
    {
        blackjack = true;
    }
    else if(cardArray[1] == 'A' && (cardArray[0] == 'T' || cardArray[0] == 'J' || cardArray[0] == 'Q' || cardArray[0] == 'K'))
    {
        blackjack = true;
    }
    else if(cardArray[0] == 'A' || cardArray[1]=='A')
    {
        soft = true;
    }
}

/**
 @brief
    get the current makeup of all cards in the hand
 @return string
    string of the hand with verital lines and spaces
    on outsides of string
 */
string Hand::getHand() const
{
    string ret = "| ";
    
    for(int i=0; i<numCards; i++)
    {
        ret.push_back(cardArray[i]);
        ret.push_back(suitArray[i]);
        ret.push_back(' ');
    }
    ret.push_back('|');
    return ret;
}

/**
 @brief
    Display one card. Used for dealer's upward facing card.
 @return string
    String containing hand symbol, suit, and vertical lines on outside
    and spaces in between cards and outside lines
 */
string Hand::displayOne()
{
    std::string ret = "| ";
    ret.push_back(cardArray[0]);
    ret.push_back(suitArray[0]);
    ret.append("    |");
    return ret;
}

/**
 @brief
    get first card in cardArray
 @return char
    first card
 */
char Hand::getFirstCard()
{
    return cardArray[0];
}

/**
 @brief
    get second card in cardArray
 @returns char
    char for second card
 */
char Hand::getSecondCard()
{
    return cardArray[1];
}
/**
 @brief
    hit a hand. No print statements, just shoe action, hand actions
 @returns int
    -1  -   hand busts
    X   -   value of hand otherwise, between 1 and 21 inclusive
 */
int Hand::hit(Shoe* shoe)
{
    blackjack = false;  // hand is no longer a blackjack or splittable if we are hitting
    splittable = false;
    numCards++;
    
    uint8_t card = shoe->dealCard();
    uint8_t character = card>>4;
    uint8_t cardsuit = card&0x0F;
    
    char cardSymbol = cardMap[character];
    cardArray.push_back(cardSymbol);
    char suit = suitMap[cardsuit];
    suitArray.push_back(suit);
    int value = 0;  // value of card received while hitting
    
    if(valueMap.find(cardSymbol)!=valueMap.end())   // if our card is not an Ace (not in map)
    {
        value = valueMap[cardSymbol];
    }
    else    // our card is an Ace. Check current value of hand
    {
        if(_value <= 10)
        {
            value = 11;
            soft = true;
        }
        else
        {
            value = 1;
        }
    }
    
    _value += value;
    if(_value > 21)
    {
        if(soft)    // handle if hand is soft
        {
            _value -= 10;
            soft = false;
        }
    }
    if(_value > 21)
    {
        _value = -1;
    }
    return _value;
}

/**
 @brief
    doubles hand's bet and then calls hit(). player can enter a bet to double for less.
    if bet==0 (default val), the bet will be doubled
 @returns int
    -1  -   hand is busted
    x   -   value of hand after hit
 */
                                                        /*
int Hand::doubleHand(Shoe* shoe, int bet)   // TODO: see if I need this. check about removing bet from bank and calling this from 'd' in action
{
    if(bet != 0 )
    {
        handBet += bet;
    }
    else
    {
        handBet *= 2;
    }
    
    int ret = (*this).hit(shoe);
    isPat = true;
    return ret;
}
                                                         */
/**
 @brief
    get bet currently placed on this hand
 */
int Hand::getBet()
{
    return handBet;
}

/**
 @brief
    set bet for this hand
 */
void Hand::setBet(int bet)
{
    handBet = bet;
}
/**
 @brief
    Splits a hand containing 2 cards, both of same symbol. Suit doesn't matter.
 @discussion
    Hand is checked that it is splittable before split() is called - in action()
 @returns Hand
    a new hand. **Also sets the original hand to be a new hand!!** (Important)
 */
Hand Hand::split(Shoe* shoe)        //TODO: should I return pointer? reference? (prolly not reference)
{
    /* checks that it is splittable before it is called in action() */
    
    int bet = this->getBet();
    // Create two whole new hands using constructor w/ 2 card arguments
    Hand newHand1(this->_card1, shoe->dealCard(), bet);
    Hand newHand2(this->_card2, shoe->dealCard(), bet);
    this->_card1 = newHand1._card1;
    this->_card2 = newHand2._card2;
    this->soft = newHand1.soft;
    this->splittable = newHand1.splittable;
    this->_value = newHand1._value;
    this->blackjack = newHand1.blackjack;
    this->handBet = newHand1.handBet;
    this->isPat = newHand1.isPat;
    this->numCards = newHand1.numCards;
    
    for(int i=0; i<newHand1.numCards; i++)
    {
        this->cardArray[i] = newHand1.cardArray[i];
    }
    return newHand2;
}
                                                
/**
 @brief
    Overloaded = operator. Calls Hand copy constructor
 @returns Hand reference
    For purposes of chaining
 */
Hand& Hand::operator= (const Hand& diffHand) //TODO: check that this works. Changed it 8/7/22
{
    if(this != &diffHand)
    {
        (*this) = Hand(diffHand);
        return (*this);
    }
    return *this;
}
                                                    
/**
 @brief
    get whether or not hand is a blacjack
 @return bool
    true    -   blackjack
    false   -   not blackjack
 */
bool Hand::isBlackjack()
{
    if(numCards == 2)
    {
        if(cardArray[0] == 'A' && (cardArray[1] == 'T' || cardArray[1] == 'J' || cardArray[1] == 'Q' || cardArray[1] == 'K'))
        {
            return true;
        }
        if(cardArray[1] == 'A' && (cardArray[0] == 'T' || cardArray[0] == 'J' || cardArray[0] == 'Q' || cardArray[0] == 'K'))
        {
            return true;
        }
    }
    return false;
}

/**
 @brief
    get if hand can be split or not
 @return bool
    true    -   Hand can be split
    false   -   Hand cannot be split
 @discussion
    In order for a hand to be splittable, it must only have 2 cards, and each of the cards
    must have the same symbol. i.e. 'A', 'T', '4', etc
 */
bool Hand::isSplittable()
{
    return splittable;
}

/**
 @brief
    get the number of cards in the hand
 @returns int
    X   -   the number of cards in the hand (minimum of 2)
 */
int Hand::getNumCards()
{
    return numCards;
}

/**
 @brief
    get the current sum of the value of all cards in hand
 @returns int
    current value of hand (max of 21)
 */
int Hand::getValue()
{
    return _value;
}

/**
 @brief
    set whether the hand can receive more cards or is "standing pat"
 */
void Hand::setPat(const bool pat)
{
    isPat = pat;
}

/**
 @brief
    get whether or not the hand has been stood pat on
 @returns bool
    true    -   hand is standing pat
    false   -   hand is not standing pat
 */
bool Hand::getPat()
{
    return isPat;
}

/**
 @brief
    Get whether or not current hand is soft
 @discussion
    A soft hand is any hand that still has an ace in it that can be counted as either 11 or 1.
    In other words, a soft hand is a hand that has no risk of busting from hitting the hand.
 @returns bool
    true    -   hand is soft
    false   -   hand is NOT soft. AKA hard
 */
bool Hand::isSoft()
{
    return soft;
}

/**
 @brief
    Hand destructor. Doesn't do anything.
 */
Hand:: ~Hand()
{
    //cout << "Hand destructor called \n";
}

/**
@brief
  overloaded ostream operator= function. Prints hand.
@param hand
  Hand object
@param s
  ostream object reference
*/
ostream& operator<<(std::ostream& s, const Hand& hand) 
{
    s << hand.getHand();
    return s;
}
                                                         
