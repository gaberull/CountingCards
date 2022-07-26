//
//  bank.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

#include "bank.hpp"
class Bank
{
private:
    unsigned int _balance;
    
public:
    int getBalance() {return _balance; }
    void setBalance(int value=10000) {_balance = value; }
    void addFunds( int value ) {_balance += value; }
};



