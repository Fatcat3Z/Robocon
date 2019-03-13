//
// Created by zyz on 19-3-8.
//

#ifndef FINDER_FINDERINFO_H
#define FINDER_FINDERINFO_H

#include <mutex>
#include "ThreadInfo.h"

class FinderInfo :public ThreadInfo {
private:
    unsigned char dataArea;
    double variable1;
    double variable2;
    double variable3;
    //union {
    //	struct Mode1{//巡线模式
    //		double deflection[4];
    //		double distance[4];
    //		double meaningless[4];
    //	};
    //	struct Mode2{//十字模式
    //		double deflection[4];
    //		double axisX[4];//double distance[4];
    //		double axisY[4];//double rotationAngle[4];
    //	};
    //};
public:
    void set(unsigned char flag, double variable1, double variable2, double variable3);
    bool get(double *res);
};

#endif //FINDER_FINDERINFO_H
