//
//  shoe.js
//  CountingCards
//
//  Copyright (c) 2023 Gabe Scott
//

class Shoe {

    constructor (numDecks, cutPoint) {
        if(typeof numDecks !== 'number' || typeof cutPoint !== 'number') {
            throw new Error('expected types number, got ' + typeof startBalance + ', ' + typeof cutPoint);
        }
        this.cardsRemaining = numDecks * 52;
        this.count = 0;
        this.aceCount = 0;  //TODO: check if I need this at all
        this.endOfShoe = false;
        this.fullShoe = Array(cardsRemaining).fill('0z');

        switch (cutPoint) {
            case 0:
                this.cutPoint = 0;
                break;
            case 1:
                this.cutPoint = cardsRemaining / 10;
                break;
            case 2:
                this.cutPoint = cardsRemaining / 4;
                break;
            case 3:
                this.cutPoint = cardsRemaining / 2;
                break;
            default:    // default case won't be hit
                break;
        }

        const chars = 'A23456789TJQK';
        const suits = 'sdch';
        for(var i=0; i<cardsRemaining; i++) {
            //this.fullShoe[i].replace('0', chars[(i%13)]);
            var character = chars[(i%13)];
            var suit = suis[(i%4)];
            var newStr = character + suit;
            this.fullShoe[i] = newStr;
        }
    }

    dealCard() {
        if(this.cardsRemaining <= 0) {
            var i = Math.floor(Math.random() * (this.numDecks * 52));
            this.count = 0;
            this.aceCount = 0;
        }
        else {
            var i = Math.floor(Math.random() * (this.cardsRemaining));
        }
        var card = this.fullShoe[i];
        var firstChar = card[0];
        
        switch (firstChar) {
            case 'A':
                this.aceCount++;
                this.count--;
                
                break;

            case '2':
                this.count++;
                
                break;

            case '3':
                this.count++;
                
                break;

            case '4':
                this.count++;
                        
                break;

            case '5':
                this.count++;
                
                break;

            case '6':
                this.count++;
                
                break;

                case 'T':
                this.count--;
                        
                break;     

            case 'J':
                this.count--;
                        
                break; 

            case 'Q':
                this.count--;
                        
                break;  

            case 'K':
                this.count--;
                        
                break;  
        
            default:
                break;
        }

        if (this.cardsRemaining > 0) {
            var temp = fullShoe[i];
            this.fullShoe[i] = fullShoe[this.cardsRemaining-1];
            this.fullShoe[this.cardsRemaining-1] = temp;
            //swap(fullShoe[i], fullShoe[_cardsRemaining-1]);
        }
        else {
            this.count = 0;
            this.aceCount = 0;
        }
        this.cardsRemaining--;
        if(this.cardsRemaining <= this.cutPoint) this.endOfShoe = true;

        return card;
    }

    endOfShoe() {
        return this.endOfShoe;
    }

    getCount() {
        if(this.cardsRemaining <= 0)
        {
            return 0;
        }
        return this.count;
    }

    getTrueCount() {    //TODO: implement
        if(this.cardsRemaining <= 0) return 0;
        
        var ret = 0.00;
        var decksLeft = this.cardsRemaining / 52.0;
        ret = ret * 100 + 0.5;
        ret /= 100;
        return ret;
    }

    getAceCount() {
        return this.aceCount;
    }

    getCardsLeft() {
        return this.cardsRemaining;
    }
};