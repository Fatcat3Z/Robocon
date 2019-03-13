//
// Created by zyz on 19-3-8.
//

#include "finderInfo.h"

using namespace std;

void FinderInfo::set(unsigned char dataArea, double variable1, double variable2, double variable3)
{
    lock_guard<mutex> l(info_mutex);
    this->dataArea = dataArea;
    this->variable1 = variable1;
    this->variable2 = variable2;
    this->variable3 = variable3;
}
bool FinderInfo::get(double *res)
{
    lock_guard<mutex> l(info_mutex);
    res[0] = dataArea;
    res[1] = variable1;
    res[2] = variable2;
    res[3] = variable3;
    return true;
}