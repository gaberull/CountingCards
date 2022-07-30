//
//  bank.cpp
//  CountingCards
//
//  Created by Gabe Scott on 7/25/22.
//

//#include "bank.hpp"
                                            
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
    void payBlackjack(int bet) { _balance += (bet * 3 / 2); }
    void payWin(int bet) { _balance += bet; }
};


                                             

