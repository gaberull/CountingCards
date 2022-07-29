//
//  hand.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//

#include "shoe.hpp"
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>
using namespace std;

class Hand
{
    std::vector<char> cardArray;
    std::vector<char> suitArray;
    std::unordered_map<char, int> valueMap; // valueMap doesn't include Ace
    std::unordered_map<uint8_t, char> cardMap;
    std::unordered_map<uint8_t, char> suitMap;
    uint8_t _card1;
    uint8_t _card2;
    int numCards = 0;
    bool blackjack = false;
    bool splittable = false;
    int _value = 0;
    
    
public:
    Hand();
    Hand(Hand& diffHand); // Copy constructor
    Hand(uint8_t& card1, uint8_t& card2);
    std::string getHand();
    int hit(Shoe& shoe);
    std::vector<Hand> split(Shoe& shoe);
    int getValue();
    bool isBlackjack();
    // ~Hand() Destructor
    Hand operator= (Hand& diffHand);    // overload equals operator
    
};
/*
 Default Constructor
 */
Hand::Hand()
{
    _card1 = 0x00;
    _card2 = 0x01;
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
    // valuemap doesn't include Ace
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    cardMap = { {0x01,'A'}, {0x02,'2'},{0x03,'3'},{0x04,'4'},{0x05,'5'},{0x06,'6'},{0x07,'7'},{0x08,'8'},{0x09,'9'},{0x0A,'T'},{0x0B,'J'},{0x0C,'Q'},{0x0D,'K'} };
    // Spades, Clubs, Hearts, Diamonds
    suitMap = { {0x01,'S'},{0x02,'C'},{0x03,'H'},{0x04,'D'} };
    numCards = 0;
    blackjack = false;
    _value = 0;
};
/**
 Main constructor that will be used
 */
Hand::Hand(uint8_t& card1, uint8_t& card2)
{
    _card1 = card1;
    _card2 = card2;
    
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
    // valuemap doesn't include Ace
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    cardMap = { {0x01, 'A'}, {0x02, '2'},{0x03, '3'},{0x04, '4'},{0x05, '5'},{0x06, '6'},{0x07, '7'},{0x08, '8'},{0x09, '9'},{0x0A, 'T'},{0x0B, 'J'},{0x0C, 'Q'},{0x0D, 'K'} };
    // Spades, Clubs, Hearts, Diamonds
    suitMap = { {0x01,'S'},{0x02,'C'},{0x03,'H'},{0x04,'D'} };
    
    uint8_t rank1 = card1 >> 4;
    uint8_t suit1 = card1 & 0x0F;
    switch (rank1) {
        case 0x01:
            cardArray[0] = 'A';
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
    switch (suit1) {
        case 0x01:
            suitArray[0] = 'S';
            break;
        case 0x02:
            suitArray[0] = 'C';
            break;
        case 0x03:
            suitArray[0] = 'H';
            break;
        case 0x04:
            suitArray[0] = 'D';
            break;
            
        default:
            break;
    }
    
    // get 2nd card
    uint8_t rank2 = card2 >> 4;
    uint8_t suit2 = card2 & 0x0F;
    switch (rank2) {
        case 0x01:
            cardArray[1] = 'A';
            if(_value <=10)
            {
                _value +=11;
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
    switch (suit2) {
        case 0x01:
            suitArray[1] = 'S';
            break;
        case 0x02:
            suitArray[1] = 'C';
            break;
        case 0x03:
            suitArray[1] = 'H';
            break;
        case 0x04:
            suitArray[1] = 'D';
            break;
            
        default:
            break;
    }
    numCards = 2;
    if(cardArray[0]==cardArray[1])
    {
        splittable = true;
    }
    else if(cardArray[0] == 'A' && (cardArray[1] == 'T' || cardArray[1] == 'J' || cardArray[1] == 'Q' || cardArray[1] == 'K'))
    {
        blackjack = true;
    }
    else if(cardArray[1] == 'A' && (cardArray[0] == 'T' || cardArray[0] == 'J' || cardArray[0] == 'Q' || cardArray[0] == 'K'))
    {
        blackjack = true;
    }
};

/**
 Copy constructor
 */
Hand::Hand(Hand& diffHand)
{
    
    this->cardArray = diffHand.cardArray;
    this->suitArray = diffHand.suitArray;
    this->valueMap = diffHand.valueMap;
    this->suitMap = diffHand.suitMap;
    this->numCards = diffHand.numCards;
    this->blackjack = diffHand.blackjack;
    this->splittable = diffHand.splittable;
    this->_value = diffHand._value;
}


/**
 @brief get the current makeup of all cards in the hand
 */
std::string Hand::getHand()
{
    std::string ret = "| ";
    
    for(int i=0; i<numCards; i++)
    {
        ret.push_back(cardArray[i]);
        ret.push_back(suitArray[i]);
        ret.push_back(' ');
    }
    ret.push_back('|');
    return ret;
};
/**
 @brief hit() returns -1 if busted.  The value of the hand otherwise
 */
int Hand::hit(Shoe& shoe)
{
    blackjack = false;  // hand is no longer a blackjack or splittable if we are hitting
    splittable = false;
    numCards++;
    
    uint8_t card = shoe.dealCard();
    char cardSymbol = cardMap[card>>4];
    cardArray.push_back(cardSymbol);
    char suit = suitMap[card&0x0F];
    suitArray.push_back(suit);
    int value = 0;  // value of card received while hitting
    
    // if our card is not an Ace
    if(valueMap.find(cardSymbol)!=valueMap.end())
    {
        value = valueMap[cardSymbol];
    }
    else    // our card is an Ace. Check current value of hand
    {
        (_value <= 10) ? value = 11 : value = 1;
    }
    
    _value += value;
    if(_value > 21)
    {
        return -1;
    }
    return _value;
};

/**
 @brief Splits a hand containing 2 cards, both of same symbol. Suit doesn't matter.
 */
std::vector<Hand> Hand::split(Shoe& shoe)
{
    //create new vector of hands containing this hand
    std::vector<Hand> ret(1, *this);
    // check size on return. If hand can't be split, the return vector will be only size 1
    if(!splittable)
    {
        cout << "Hand can only have 2 of same card to split \n";
        return ret;
    }
    if(numCards != 2)
    {
        cout << "Hand can only have 2 of same card to split \n";
        return ret;
    }
    
    // Create two whole new hands using constructor w/ 2 card arguments
    uint8_t newCard1 = shoe.dealCard();
    Hand newHand1(this->_card1, newCard1);
    
    uint8_t newCard2 = shoe.dealCard();
    Hand newHand2(this->_card2, newCard2);
    
    (*this) = newHand1;
    
    ret[0] = newHand1;
    ret.push_back(newHand2);
    
    return ret;
    
}
                                                

Hand Hand::operator= (Hand& diffHand)
{
    Hand temp(diffHand);
    return temp;
                                            /*
    this->cardArray = diffHand.cardArray;
    this->suitArray = diffHand.cardArray;
    this->valueMap = diffHand.valueMap;
    this->suitMap = diffHand.suitMap;
    this->numCards = diffHand.numCards;
    this->blackjack = diffHand.blackjack;
    this->splittable = diffHand.splittable;
    this->_value = diffHand._value;
    return (*this);
                                            */
}
                                                    
/**
 @return bool | True ==blackjack, false==not blackjack
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
 @return current value of hand (max of 21)
 */
int Hand::getValue()
{
    return _value;
}
