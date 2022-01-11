#include "utils.h"

void removeFirstComma(string & return_){
    return_ = return_.substr(1);
}

void removeLastComma(string & return_){
    return_.pop_back();
}

void removeNumbers(string & return_){
    string::iterator it = return_.begin();
    while (it != return_.end()) {
        if ((*it >= '0') && (*it <= '9')) {
            it = return_.erase(it);
        } else {
            it++;
        }
    }
}