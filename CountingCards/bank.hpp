//
//  bank.hpp
//  CountingCards
//
//  Copyright (c) 2022 Gabe Scott
//

#pragma once
#include <iostream>

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
    void removeFunds(int value) {(_balance-value<=0) ? _balance=0 : _balance-=value; }
    int payBlackjack(int bet) { _balance += (bet * 3 / 2); return _balance; }
    void payWin(int bet) { _balance += bet; }
    ~Bank() { //std::cout << "Bank destructor called\n";
    }
};
