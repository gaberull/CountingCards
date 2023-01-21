//
//  bank.js
//  CountingCards
//
//  Copyright (c) 2023 Gabe Scott
//

class Bank {
    constructor(startBalance) {
        if(typeof(startBalance) != "number") {
            throw new Error('expected type number, got ' + typeof startBalance);
        }
        this.balance = startBalance;
        this.addNextHand = 0;
    }

    getBalance() {
        return this.balance;
    }

    addFunds(value) {
        this.balance += value;
    }

    removeFunds(value) {
        this.balance -= value;
    }

    payBlackjack(bet) {
        this.balance += (bet * 3 / 2); 
        return this.balance;
    }

    payWin(bet) {
        this.balance += bet;
    }

    getAddNextHand() {
        return this.addNextHand;
    }

    setAddNextHand(amount) {
        this.addNextHand = amount;
    }
}