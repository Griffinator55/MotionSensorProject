#include "application.h"
#include <string>
#include <vector>
#include <iostream>
#include <ChangeTime.h>
std::vector<ChangeTime*> list;

void newChange(int millis, int state) {
    int length = list.size();
    if(state == 0 && length > 2) {
        if((millis - (list.at(length-1)->getMilli())) < 60000) {
            delete list.at(length-1);
            list.pop_back();
        } else {
            list.push_back(new ChangeTime(millis, state));
        }
    } else {
        list.push_back(new ChangeTime(millis, state));
    }
}
