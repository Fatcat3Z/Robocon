//
// Created by zyz on 19-3-8.
//

#ifndef FINDER_INFO_H
#define FINDER_INFO_H

#include "vector"
#include <string.h>
#include <stdint.h>

static const int sumNum = 4;

struct InMeta {
    struct {//head
        unsigned char dataHead[2];
        unsigned char dataArea[2];
    };
    struct {
        unsigned char sum[sumNum];
    };
};
union In {
    InMeta meta{};
    unsigned char data[8];
};

struct mode1{//巡线模式
    unsigned char deflection[4];
    unsigned char distance[4];
    unsigned char meaningless[4];
};
struct mode2{//十字模式
    unsigned char deflection[4];
    unsigned char axisX[4];
    unsigned char axisY[4];
};
struct mode3{//抓取模式
    unsigned char deflection[4];
    unsigned char distance[4];
    unsigned char rotationAngle[4];
};

struct OutMeta {
    struct {//head
        unsigned char dataHead[2];
        unsigned char dataArea[2];
    };
    union {//body
        struct mode1 mode1;
        struct mode2 mode2;
        struct mode3 mode3;
    };
    struct {//end
        unsigned char sum[sumNum];
    };
};
union Out {
    OutMeta meta;
    unsigned char data[20];
};

void assignSum(union Out *res);

class Info {
private:
    const unsigned char Head[2] = { 0xaa, 0xbb };
    std::vector<unsigned char> data;
    int ableLength = 0;
    const int inLength = 16;
public:
    In result;

    int push(unsigned char od);
    int getSum();
    void getData();
};

#endif //FINDER_INFO_H
