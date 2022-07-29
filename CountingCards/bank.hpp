//
//  bank.hpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#pragma once

#include <stdio.h>
class Bank
{
private:
    unsigned int _balance;
    Bank();     // private default constructor - can't be called
public:
    Bank(int startBalance): _balance(startBalance) { }
    int getBalance() {return _balance; }
    void setBalance(int value) {_balance = value; }
    void addFunds( int value ) {_balance += value; }
    void removeFunds(int value) {_balance -= value; }
};
