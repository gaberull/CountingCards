//
//  hand.js
//  CountingCards
//
//  Copyright (c) 2023 Gabe Scott
//

class Hand {
    static VALUE_MAP;
    #cardArray;
    #card1
    #card2
    #numCards;
    #blackjack;
    #splittable;
    #isPat;
    #handBet;
    #soft;
    #value;

    constructor (card1, card2, bet=0) {
        if(card1 === undefined || card2 === undefined || bet === undefined)
        {
            this.VALUE_MAP = new Map(
                ['2', 2],
                ['3', 3],
                ['4', 4],
                ['5', 5],
                ['6', 6],
                ['7', 7],
                ['8', 8],
                ['9', 9],
                ['T', 10],
                ['J', 10],
                ['Q', 10],
                ['K', 10]
            );
            this.#card1 = '0X';
            this.#card2 = '0X';
            this.#cardArray = new Array();
            this.#numCards = 2;
            this.#blackjack = false;
            this.#value = 0;
            this.#splittable = false;
            this.#isPat = false;
            this.#handBet = 0;
            this.#soft = false;
            return;
        }
        this.VALUE_MAP = new Map(
            ['2', 2],
            ['3', 3],
            ['4', 4],
            ['5', 5],
            ['6', 6],
            ['7', 7],
            ['8', 8],
            ['9', 9],
            ['T', 10],
            ['J', 10],
            ['Q', 10],
            ['K', 10]
        );
        this.#handBet = bet;  
        this.#isPat = false;
        this.#card1 = card1;
        this.#card2 = card2;
        this.#numCards = 2;    
        
        switch (card1[0]) {
            case 'A':
                soft = true;
                this.#value += 11;
                break;
            case '2':
                this.#value += 2;
                break;
            case '3':
                this.#value += 3;
                break;
            case '4':
                this.#value += 4;
                break;
            case '5':
                this.#value += 5;
                break;
            case '6':
                this.#value += 6;
                break;
            case '7':
                this.#value += 7;
                break;
            case '8':
                this.#value += 8;
                break;
            case '9':
                this.#value += 9;
                break;
            case 'T':
                this.#value += 10;
                break;
            case 'J':
                this.#value += 10;
                break;
            case 'Q':
                this.#value += 10;
                break;
            case 'K':
                this.#value += 10;
                break;
            default:
                break;
        }
        this.#cardArray[0] = card1;
        this.#cardArray[1] = card2;
        switch (card2[0]) {
            case 'A':
                if(this.#value <=10)
                {
                    this.#value +=11;
                    this.#soft = true;   
                }
                else
                {
                    this.#value += 1;
                }
                break;
            case '2':
                this.#value += 2;
                break;
            case '3':
                this.#value += 3;
                break;
            case '4':
                this.#value += 4;
                break;
            case '5':
                this.#value += 5;
                break;
            case '6':
                this.#value += 6;
                break;
            case '7':
                this.#value += 7;
                break;
            case '8':
                this.#value += 8;
                break;
            case '9':
                this.#value += 9;
                break;
            case 'T':
                this.#value += 10;
                break;
            case 'J':
                this.#value += 10;
                break;
            case 'Q':
                this.#value += 10;
                break;
            case 'K':
                this.#value += 10;
                break;
            default:
                break;
        }
        if(cardArray[0][0]==cardArray[1][0]) {
            this.#splittable = true;
        }
        if(cardArray[0][0] == 'A' && (cardArray[1][0] == 'T' || cardArray[1][0] == 'J' || cardArray[1][0] == 'Q' || cardArray[1][0] == 'K')) {
            this.#blackjack = true;
        }
        else if(cardArray[1][0] == 'A' && (cardArray[0][0] == 'T' || cardArray[0][0] == 'J' || cardArray[0][0] == 'Q' || cardArray[0][0] == 'K')) {
            this.#blackjack = true;
        }
        else if(cardArray[0][0] == 'A' || cardArray[1][0] =='A') {
            this.#soft = true;
        }
    }

    getHand() {
        var ret = "";
        for(let i=0; i<this.#numCards; i++) {
            ret += this.#cardArray[i];
            if(i != numCards-1) ret += (' ');
        }
        ret.push_back('|');
        return ret;
    }   
    getFirstCard() {
        return this.#cardArray[0];
    }
    getSecondCard() {
        return this.#cardArray[1];
    }
    hit(shoe) {
        if (! shoe instanceof Shoe) {
            throw new Error('expected type Shoe, got ' + typeof shoe);
        }
        this.#blackjack = false; 
        this.#splittable = false;
        this.#numCards++;
        
        const card = shoe.dealCard();
        var cardValue = 0;  // value of card received while hitting
        
        if(this.#VALUE_MAP.has(card[0])) {
            cardValue = this.#VALUE_MAP[card[0]];
        }
        else {
            if(this.#value <= 10) {
                cardValue = 11;
                this.#soft = true;
            }
            else {
                cardValue = 1;
            }
        }
        this.#value += cardValue;
        if(this.#value > 21) {
            if(this.#soft) {
                this.#value -= 10;
                this.#soft = false;
            }
            else {
                this.#value = -1;
            }
        }
        return this.#value;
    }
    testHit(card) {
        this.#blackjack = false;  // hand is no longer a blackjack or splittable if we are hitting
        this.#splittable = false;
        this.#numCards++;
        
        // var cardSymbol = card[0];
        this.#cardArray.push(card);
        var cardValue = 0;  // value of card received while hitting
        
        if(this.#VALUE_MAP.has(card[0])) {
            cardValue = valueMap[card[0]];
        }
        else {
            if(this.#value <= 10) {
                cardValue = 11;
                this.#soft = true;
            }
            else {
                cardValue = 1;
            }
        }
        this.#value += cardValue;
        if(this.#value > 21)
        {
            if(soft)    // handle if hand is soft
            {
                this.#value -= 10;
                this.#soft = false;
            }
            else 
            {
                this.#value = -1;
            }
        }
        return this.#value;
    }
    setBet(bet) {
        this.#handBet = bet;
    }         
    split(shoe) {
        var bet = this.getBet();
        var newHand1 = new Hand(this.#card1, shoe.dealCard(), bet);
        var newHand2 = new Hand(this.#card2, shoe.dealCard(), bet);
        this.#card1 = newHand1.#card1;
        this.#card2 = newHand2.#card2;
        this.#soft = newHand1.#soft;
        this.#splittable = newHand1.#splittable;
        this.#value = newHand1.#value;
        this.#blackjack = newHand1.#blackjack;
        this.#handBet = newHand1.#handBet;
        this.#isPat = newHand1.#isPat;
        this.#numCards = newHand1.#numCards;
        
        for(let i=0; i<newHand1.#numCards; i++)
        {
            this.#cardArray[i] = newHand1.#cardArray[i];
        }
        return newHand2;
    }
             
    isBlackjack() {
        if(this.#numCards == 2) {
            if(cardArray[0][0] == 'A' && (cardArray[1][0] == 'T' || cardArray[1][0] == 'J' || cardArray[1][0] == 'Q' || cardArray[1][0] == 'K')) {
                return true;
            }
            if(cardArray[1][0] == 'A' && (cardArray[0][0] == 'T' || cardArray[0][0] == 'J' || cardArray[0][0] == 'Q' || cardArray[0][0] == 'K')) {
                return true;
            }
        }
        return false;
    }
    isSplittable() {
        return this.#splittable;
    }
    getNumCards() {
        return this.#numCards;
    }
    getBet() {
        return this.#handBet
    }
    getValue() {
        return this.#value;
    }
    getPat() {
        return this.#isPat;
    }
    setPat() {
        this.#isPat = true;
    }
    isSoft() {
        return this.#soft;
    }
}





// Hand::Hand(const Hand& diffHand)
// {

//     this->_card1 = diffHand._card1;
//     this->_card2 = diffHand._card2;
//     this->numCards = diffHand.numCards;
//     this->cardArray = vector<char>(numCards);
    
//     for(int i=0; i<numCards; i++)
//     {
//         this->cardArray[i] = diffHand.cardArray[i];
//     }
//     this->suitArray = std::vector<char>(numCards);
//     for(int i=0; i<numCards; i++)
//     {
//         this->suitArray[i] = diffHand.suitArray[i];
//     }
    
//     this->blackjack = diffHand.blackjack;
//     this->splittable = diffHand.splittable;
//     this->_value = diffHand._value;
//     this->isPat = diffHand.isPat;
//     this->handBet = diffHand.handBet;
//     this->soft = diffHand.soft;
// }           

