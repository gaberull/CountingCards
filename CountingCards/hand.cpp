//
//  hand.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/28/22.
//

#include <cstdlib>
#include <utility>
#include <string>
#include <vector>

class Hand
{
    std::vector<char> cardArray;
    std::vector<char> suitArray;
    int numCards = 0;
    bool blackjack = false;
    bool soft = false;
    int _value = 0;
    
    
public:
    Hand();
    Hand(uint8_t card1, uint8_t card2);
    std::string getHand();
    std::pair<int,int> hit();
    int getValue();
    bool isBlackjack();
};

Hand::Hand()
{
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
}
Hand::Hand(uint8_t card1, uint8_t card2)
{
    cardArray = std::vector<char>('0', 2);
    suitArray = std::vector<char>('0', 2);
    
    uint8_t rank1 = card1 >> 4;
    uint8_t suit1 = card1 & 0x0F;
    switch (rank1) {
        case 0x01:
            cardArray[0] = 'A';
            _value += 11;
            soft = true;
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
            if(soft)
            {
                _value +=1;
            }
            else
            {
                _value += 11;
                soft = true;
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
    std::string ret = "";
    for(int i=0; i<cardArray.size(); i++)
    {
        ret.push_back(cardArray[i]);
        ret.push_back(suitArray[i]);
        if(i <cardArray.size()-1) ret.push_back(' ');
        
    }
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


int Hand::getValue()
{
    if(isBlackjack())
    {
        return 21;
    }
    
    //TODO: finish this return value
    return 0;
}
