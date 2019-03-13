//
// Created by zyz on 19-3-8.
//
#pragma once
#ifndef FINDER_THREADINFO_H
#define FINDER_THREADINFO_H

#include <mutex>

class ThreadInfo {
protected:
    std::mutex info_mutex;
};

#endif //FINDER_THREADINFO_H
