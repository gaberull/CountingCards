//
//  card.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include <string>
#include <vector>

class Card
{
public:
    static std::string getHand(uint8_t hand);
};

std::string Card::getHand(uint8_t hand)
{
    switch (hand) {
        case 0x11:             // 0x00
            return "hi\n";
            break;
            
    }
   
    return "";
};
