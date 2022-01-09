#include <iostream>
#include "fdogserialize.h"
#include <typeinfo>
#include <iostream>
using namespace fsj;

vector<bool> a;

int main()
{
    int a = 10;
    string json_;
    FdogSerialize::Instance()->Serialize(json_, a);
    cout << json_ << endl;
    
    return 0;
}