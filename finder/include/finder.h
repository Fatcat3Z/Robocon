//
// Created by zyz on 19-3-8.
//

#ifndef FINDER_FINDER_H
#define FINDER_FINDER_H

#include "stdio.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "finderInfo.h"

class Finder {
public:
    bool flag;//巡线模式flag为0，十字模式flag为1
    double theta;//巡线模式，输出偏角和距离
    double distance;
    cv::Point2f center;//十字模式，输出十字中心和两条线的斜率
    double center_k1, center_k2;
public:
    int operator()(FinderInfo &finderInfo);
private:
    cv::Mat img_original;
    cv::Mat img_binary;
    double length = 400;//从摄像头中截取视野的长宽
    double width = 400;
    int interval_first = 100; //第一次没有找到点，向下/左/右移动的距离
    int interval_first_cross = 200;//巡线到十字的过渡间隔
    int interval_next = 10;
    int point_min = 7;//有效中点组的最小点数
    bool flag_front = false;//从前方找点有效中点组
    bool flag_left = false;
    bool flag_right = false;
    cv::Point2f first_midpoint_front;
    cv::Point2f first_midpoint_left;
    cv::Point2f first_midpoint_right;
    std::vector<cv::Point2f> midpoint_group;
    std::vector<cv::Point2f> midpoint_group_front;
    std::vector<cv::Point2f> midpoint_group_left;
    std::vector<cv::Point2f> midpoint_group_right;
private:
    void get_threshold(cv::Mat pic);

    bool find_first_midpoint_front();//找第一个点
    bool find_first_midpoint_left();
    bool find_first_midpoint_right();
    bool find_first_midpoint_front_cross();//应对巡线到十字的过渡问题

    bool find_all_midpoint_front();//找所有点
    bool find_all_midpoint_left();
    bool find_all_midpoint_right();

    bool find_next_midpoint_front(cv::Point2f *midpoint_next, cv::Point2f midpoint_now);
    bool find_next_midpoint_left(cv::Point2f *midpoint_next, cv::Point2f midpoint_now);
    bool find_next_midpoint_right(cv::Point2f *midpoint_next, cv::Point2f midpoint_now);

    bool polynomial_curve_fit(std::vector<cv::Point2f> midpoint_group, int n, cv::Mat &A);
    void get_theta(double k_out, double b_out);
    void get_distance(double k_out, double b_out);
};

#endif //FINDER_FINDER_H
