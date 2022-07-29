//
//  hand.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//

#include <cstdlib>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>

class Hand
{
    std::vector<char> cardArray;
    std::vector<char> suitArray;
    std::unordered_map<char, int> valueMap; // valueMap doesn't include Ace
    int numCards = 0;
    bool blackjack = false;
    int _value = 0;
    
    
public:
    Hand();
    Hand(uint8_t card1, uint8_t card2);
    std::string getHand();
    std::pair<int,int> hit();
    int getValue();
    bool isBlackjack();
};
/*
 Default Constructor
 */
Hand::Hand()
{
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
    // valuemap doesn't include Ace
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    int numCards = 0;
    bool blackjack = false;
    int _value = 0;
};
/**
 Main constructor that will be used
 */
Hand::Hand(uint8_t card1, uint8_t card2)
{
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
    // valuemap doesn't include Ace
    valueMap = {{'2',2},{'3',3},{'4',4},{'5',5},{'6',6},{'7',7},{'8',8},{'9',9},{'T',10},{'J',10},{'Q',10},{'K',10}};
    
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
};

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
 @brief hit the handreturns 1 if busted, false otherwise
 @return possible scores counting aces high and low. if same, only possible one.
        first in pair is score     |    second in pair is busted or not (1 - true, 2 - false)
 */
std::pair<int, int> Hand::hit()
{
    return std::make_pair(0, 0);
};

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
    if(isBlackjack())
    {
        return 21;
    }
    
    return _value;
    
}
