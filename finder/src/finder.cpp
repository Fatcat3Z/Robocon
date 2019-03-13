//
// Created by zyz on 19-3-8.
//

#include "finder.h"

using namespace std;
using namespace cv;

void Finder::get_threshold(Mat pic)
{
    img_original = pic;
    //resize(pic, img_original, Size(400, 400));
    Mat binary;
    cvtColor(img_original, binary, CV_BGR2GRAY);
    GaussianBlur(binary, binary, Size(5, 5), 2, 2);
    Scalar tempVal = cv::mean(binary);//平均值
    double matMean = tempVal.val[0];
    threshold(binary, img_binary, matMean + 20, 255, CV_THRESH_BINARY);
    //const int elesize = 15;
    //Mat element = getStructuringElement(MORPH_RECT, Size(elesize, elesize));
    //erode(img_binary, img_binary, element);
    //morphologyEx(img_binary, img_binary, MORPH_CLOSE, element);//闭
    //morphologyEx(img_binary, img_binary, MORPH_OPEN, element);//开
}
bool Finder::find_first_midpoint_front()
{
    int left = 0, right = 0;
    for (int i = 1; i < img_binary.cols; i++)//寻找left点
    {
        if (img_binary.at<uchar>(20, i) == img_binary.at<uchar>(20, i - 1))
            continue;
        else {
            left = i;
            break;
        }
    }
    for (int i = left + 1; i < img_binary.cols; i++)
    {
        if (img_binary.at<uchar>(20, i) == img_binary.at<uchar>(20, i - 1))
            continue;
        else {
            right = i;
            break;
        }
    }
    if ((left != 0) && (right != 0) && (abs(right - left) < 100) && (abs(right - left) > 30)) {
        //如果right点能找到且与left点相差合理则左右点取平均输出。
        first_midpoint_front.y = 20;
        first_midpoint_front.x = (left + right) / 2;
        midpoint_group_front.push_back(first_midpoint_front);
        return true;
    }
    else if ((left != 0) || (img_binary.at<uchar>(0, 0) && (left == 0)))
    {//如果右点找不到，则向下一定距离，继续寻找左右点
        left = 0;
        for (int i = 1; i < img_binary.cols; i++)
        {
            if (img_binary.at<uchar>(interval_first, i)
                == img_binary.at<uchar>(interval_first, i - 1))
                continue;
            else {
                left = i;
                break;
            }
        }
        if (left == 0)//如果左点仍然找不到，返回false
            return false;
        else {//如果左点能找到，寻找右点
            right = 0;
            for (int i = left + 1; i < img_binary.cols; i++)
            {
                if (img_binary.at<uchar>(interval_first, i)
                    == img_binary.at<uchar>(interval_first, i - 1))
                    continue;
                else {
                    right = i;
                    break;
                }
            }
            if ((right != 0) && (abs(left - right) < 200))//如果右点能找到
            {
                first_midpoint_front.y = interval_first;
                first_midpoint_front.x = (left + right) / 2;
                midpoint_group_front.push_back(first_midpoint_front);
                return true;
            }
            else {//这次再找不到右点则彻底放弃
                return false;
            }
        }
    }
    else
        return false;
}
bool Finder::find_first_midpoint_front_cross()
{
    int left = 0, right = 0;
    for (int i = 1; i < img_binary.cols; i++)//寻找left点
    {
        if (img_binary.at<uchar>(interval_first_cross, i) ==
            img_binary.at<uchar>(interval_first_cross, i - 1))
            continue;
        else {
            left = i;
            break;
        }
    }
    for (int i = left + 1; i < img_binary.cols; i++)
    {
        if (img_binary.at<uchar>(interval_first_cross, i) ==
            img_binary.at<uchar>(interval_first_cross, i - 1))
            continue;
        else {
            right = i;
            break;
        }
    }
    if ((left != 0) && (right != 0) && (abs(right - left) < 100) && (abs(right - left) > 30)) {
        //如果right点能找到且与left点相差合理则左右点取平均输出。
        first_midpoint_front.y = interval_first_cross;
        first_midpoint_front.x = (left + right) / 2;
        midpoint_group_front.push_back(first_midpoint_front);
        return true;
    }
}
bool Finder::find_first_midpoint_left()
{
    int front = 0, back = 0;
    for (int i = 1; i < img_binary.rows; i++)//寻找front点
    {
        if (img_binary.at<uchar>(i, 0) == img_binary.at<uchar>(i - 1, 0))
            continue;
        else {
            front = i;
            break;
        }
    }
    for (int i = front + 1; i < img_binary.cols; i++)
    {
        if (img_binary.at<uchar>(i, 0) == img_binary.at<uchar>(i - 1, 0))
            continue;
        else {
            back = i;
            break;
        }
    }
    if ((front != 0) && (back != 0) && (abs(back - front) < 100) && (abs(back - front) > 30)) {
        //如果back点能找到且与front点相差合理则左右点取平均输出。
        first_midpoint_left.x = 0;
        first_midpoint_left.y = (front + back) / 2;
        midpoint_group_left.push_back(first_midpoint_left);
        return true;
    }
    else if (front != 0)
    {//如果back找不到，则向下一定距离，继续寻找左右点
        front = 0;
        for (int i = 1; i < img_binary.rows; i++)
        {
            if (img_binary.at<uchar>(i, interval_first)
                == img_binary.at<uchar>(i - 1, interval_first))
                continue;
            else {
                front = i;
                break;
            }
        }
        if (front == 0)//如果左点仍然找不到，返回false
            return false;
        else {//如果左点能找到，寻找右点
            back = 0;
            for (int i = front + 1; i < img_binary.rows; i++)
            {
                if (img_binary.at<uchar>(i, interval_first)
                    == img_binary.at<uchar>(i - 1, interval_first))
                    continue;
                else {
                    back = i;
                    break;
                }
            }
            if ((back != 0) && (abs(front - back) < 200))//如果右点能找到
            {
                first_midpoint_left.x = interval_first;
                first_midpoint_left.y = (front + back) / 2;
                midpoint_group_left.push_back(first_midpoint_left);
                return true;
            }
            else {//这次再找不到右点则彻底放弃
                return false;
            }
        }
    }
    else
        return false;
}
bool Finder::find_first_midpoint_right()
{
    int front = 0, back = 0;
    for (int i = 1; i < img_binary.rows; i++)//寻找front点
    {
        if (img_binary.at<uchar>(i, img_binary.rows - 1) ==
            img_binary.at<uchar>(i - 1, img_binary.rows - 1))
            continue;
        else {
            front = i;
            break;
        }
    }
    for (int i = front + 1; i < img_binary.cols; i++)
    {
        if (img_binary.at<uchar>(i, img_binary.rows - 1) ==
            img_binary.at<uchar>(i - 1, img_binary.rows - 1))
            continue;
        else {
            back = i;
            break;
        }
    }
    if ((front != 0) && (back != 0) && (abs(back - front) < 100) && (abs(front - back) > 30)) {
        //如果back点能找到且与front点相差合理则左右点取平均输出。
        first_midpoint_right.x = img_binary.rows - 1;
        first_midpoint_right.y = (front + back) / 2;
        midpoint_group_right.push_back(first_midpoint_right);
        return true;
    }
    else if (front != 0)
    {//如果back点找不到，则向左一定距离，继续寻找front,back点
        front = 0;
        for (int i = 1; i < img_binary.rows; i++)
        {
            if (img_binary.at<uchar>(i, img_binary.rows - 1 - interval_first) ==
                img_binary.at<uchar>(i - 1, img_binary.rows - 1 - interval_first))
                continue;
            else {
                front = i;
                break;
            }
        }
        if (front == 0)//如果左点仍然找不到，返回false
            return false;
        else {//如果左点能找到，寻找右点
            back = 0;
            for (int i = front + 1; i < img_binary.rows; i++)
            {
                if (img_binary.at<uchar>(i, img_binary.rows - 1 - interval_first) ==
                    img_binary.at<uchar>(i - 1, img_binary.rows - 1 - interval_first))
                    continue;
                else {
                    back = i;
                    break;
                }
            }
            if ((back != 0) && (abs(front - back) < 200))//如果右点能找到
            {
                first_midpoint_right.x = img_binary.rows - 1 - interval_first;
                first_midpoint_right.y = (front + back) / 2;
                midpoint_group_right.push_back(first_midpoint_right);
                return true;
            }
            else {//这次再找不到右点则彻底放弃
                return false;
            }
        }
    }
    else
        return false;
}
bool Finder::find_next_midpoint_front(Point2f *midpoint_next,Point2f midpoint_now)
{
    int left = 0, right = 0;
    (*midpoint_next).y = midpoint_now.y + interval_next;
    if (midpoint_next->y > 399)//越界
        return false;
    for (int i = 1; (i < 100) && (i < midpoint_now.x - 1); i++)//采左点
    {
        (*midpoint_next).x = midpoint_now.x - i;
        if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
            img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x - 1))
            continue;
        else {
            left = midpoint_now.x - i;
            break;
        }
    }
    if (left == 0)//左点采不到，失败
        return false;
    else {
        for (int i = 1; (i < 100) && (i < 399 - midpoint_now.x); i++)//采右点
        {
            (*midpoint_next).x = midpoint_now.x + i;
            if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
                img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x + 1))
                continue;
            else {
                right = midpoint_now.x + i;
                break;
            }
        }
        if (right == 0)
            return false;
        else {
            (*midpoint_next).x = (left + right) / 2;
            midpoint_group_front.push_back((*midpoint_next));
            return true;
        }
    }
}
bool Finder::find_next_midpoint_left(Point2f *midpoint_next, Point2f midpoint_now)
{
    int front = 0, back = 0;
    (*midpoint_next).x = midpoint_now.x + interval_next;
    if (midpoint_next->x > 399)//越界
        return false;
    for (int i = 1; (i < 100) && (i < midpoint_now.y - 1); i++)//采左点
    {
        (*midpoint_next).y = midpoint_now.y - i;
        if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
            img_binary.at<uchar>((*midpoint_next).y - 1, (*midpoint_next).x))
            continue;
        else {
            front = midpoint_now.y - i;
            break;
        }
    }
    if (front == 0)//front点采不到，失败
        return false;
    else {
        for (int i = 1; (i < 100) && (i < 399 - midpoint_now.y); i++)//采右点
        {
            (*midpoint_next).y = midpoint_now.y + i;
            if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
                img_binary.at<uchar>((*midpoint_next).y + 1, (*midpoint_next).x))
                continue;
            else {
                back = midpoint_now.y + i;
                break;
            }
        }
        if (back == 0)
            return false;
        else {
            (*midpoint_next).y = (front + back) / 2;
            midpoint_group_left.push_back((*midpoint_next));
            return true;
        }
    }
}
bool Finder::find_next_midpoint_right(Point2f *midpoint_next, Point2f midpoint_now)
{
    int front = 0, back = 0;
    (*midpoint_next).x = midpoint_now.x - interval_next;
    if (midpoint_next->x < 1)//越界
        return false;
    for (int i = 1; (i < 100) && (i < midpoint_now.y - 1); i++)//采左点
    {
        (*midpoint_next).y = midpoint_now.y - i;
        if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
            img_binary.at<uchar>((*midpoint_next).y - 1, (*midpoint_next).x))
            continue;
        else {
            front = midpoint_now.y - i;
            break;
        }
    }
    if (front == 0)//front点采不到，失败
        return false;
    else {
        for (int i = 1; (i < 100) && (i < 399 - midpoint_now.y); i++)//采右点
        {
            (*midpoint_next).y = midpoint_now.y + i;
            if (img_binary.at<uchar>((*midpoint_next).y, (*midpoint_next).x) ==
                img_binary.at<uchar>((*midpoint_next).y + 1, (*midpoint_next).x))
                continue;
            else {
                back = midpoint_now.y + i;
                break;
            }
        }
        if (back == 0)
            return false;
        else {
            (*midpoint_next).y = (front + back) / 2;
            midpoint_group_right.push_back((*midpoint_next));
            return true;
        }
    }
}
bool Finder::find_all_midpoint_front()
{
    Point2f midpoint_last;
    Point2f midpoint_now;
    Point2f midpoint_next;
    double first_point_x;
    double first_point_x_second;
    int flag = find_first_midpoint_front();
    if (midpoint_group_front.size() == 0) //如果中点组点数为0，从前方找点失败
        return false;
    if (midpoint_group_front.size() == 1) {//如果中点组点数为1，采用非预测采点模式
        first_point_x = midpoint_group_front[0].x;
        midpoint_last = midpoint_group_front[0];
        midpoint_now = midpoint_group_front[0];
        bool flag = 0;
        flag = find_next_midpoint_front(&midpoint_next, midpoint_now);
    }
    if (midpoint_group_front.size() >= 2) {//中点组点数大于等于2，采用预测采点模式
        int i = midpoint_group_front.size();
        midpoint_last = midpoint_group_front[i - 2];
        midpoint_now = midpoint_group_front[i - 1];
        double diff1, diff2;
        bool flag = 1;
        while (flag) {
            flag = find_next_midpoint_front(&midpoint_next, midpoint_now);
            if (flag == false)
                break;
            diff1 = midpoint_now.x - midpoint_last.x;
            diff2 = midpoint_next.x - midpoint_now.x;
            if ((abs(diff1) <= 5) && (abs(diff2) <= 5))
                flag = 1;
            else {
                if (((abs(diff2) > 0.5*abs(diff1)) && (abs(diff2) < 2 * abs(diff1))
                     && (diff1*diff2 > 0)) == false) {//误差在预测范围内
                    midpoint_group_front.pop_back();
                    flag = 0;
                }
            }
            midpoint_last = midpoint_now;
            midpoint_now = midpoint_next;
        }
    }
    if (midpoint_group_front.size() >= point_min)//点数大于point_min该有效点组才有效
    {
        if (midpoint_group_front.size() >= 2 * point_min)
        {
            int num = midpoint_group_front.size();
            double diff1 = midpoint_group_front[num / 2].x - midpoint_group_front[0].x;
            double diff2 = midpoint_group_front[num - 1].x - midpoint_group_front[num / 2].x;
            if ((abs(diff1) > 20) || (abs(diff2) > 20)) {
                if (diff1*diff2 > 0)
                    return true;
                else {
                    for (int i = 0; i < num / 2; i++)
                        midpoint_group_front.pop_back();
                    return true;
                }
            }
            else
                return true;
        }
        else
            return true;
    }
    else if (flag == true) {
        midpoint_group_front.clear();
        int flag2 = find_first_midpoint_front_cross();
        first_point_x_second = midpoint_group_front[0].x;
        if (abs(first_point_x_second - first_point_x) < 60) {
            if (midpoint_group_front.size() == 0) //如果中点组点数为0，从前方找点失败
                return false;
            if (midpoint_group_front.size() == 1) {//如果中点组点数为1，采用非预测采点模式
                midpoint_last = midpoint_group_front[0];
                midpoint_now = midpoint_group_front[0];
                bool flag = 0;
                flag = find_next_midpoint_front(&midpoint_next, midpoint_now);
                if (flag == true) {}
                else
                    return false;
            }
            if (midpoint_group_front.size() >= 2) {//中点组点数大于等于2，采用预测采点模式
                int i = midpoint_group_front.size();
                midpoint_last = midpoint_group_front[i - 2];
                midpoint_now = midpoint_group_front[i - 1];
                double diff1, diff2;
                bool flag = 1;
                while (flag) {
                    flag = find_next_midpoint_front(&midpoint_next, midpoint_now);
                    if (flag == 0)
                        break;
                    diff1 = midpoint_now.x - midpoint_last.x;
                    diff2 = midpoint_next.x - midpoint_now.x;
                    if ((abs(diff1) <= 5) && (abs(diff2) <= 5))
                        flag = 1;
                    else {
                        if (((abs(diff2) >= 0.5*abs(diff1)) && (abs(diff2) <= 2 * abs(diff1))
                             && (diff1*diff2 > 0)) == false) {//误差在预测范围内
                            midpoint_group_front.pop_back();
                            flag = 0;
                        }
                    }
                    midpoint_last = midpoint_now;
                    midpoint_now = midpoint_next;
                }
            }
            if (midpoint_group_front.size() >= point_min)//点数大于point_min该有效点组才有效
                return true;
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}
bool Finder::find_all_midpoint_left()
{
    Point2f midpoint_last;
    Point2f midpoint_now;
    Point2f midpoint_next;
    int flag_left = find_first_midpoint_left();
    if (midpoint_group_left.size() == 0) //如果中点组点数为0，从左边采点失败
        return false;
    if (midpoint_group_left.size() == 1) {//如果中点组点数为1，采用非预测采点模式
        midpoint_last = midpoint_group_left[0];
        midpoint_now = midpoint_group_left[0];
        bool flag = 0;
        flag = find_next_midpoint_left(&midpoint_next, midpoint_now);
        if (flag == true) {}
        else
            return false;
    }
    if (midpoint_group_left.size() >= 2) {//中点组点数大于等于2，采用预测采点模式
        int i = midpoint_group_left.size();
        midpoint_last = midpoint_group_left[i - 2];
        midpoint_now = midpoint_group_left[i - 1];
        double diff1, diff2;
        bool flag = 1;
        while (flag) {
            flag = find_next_midpoint_left(&midpoint_next, midpoint_now);
            if (flag == 0)
                break;
            diff1 = midpoint_now.y - midpoint_last.y;
            diff2 = midpoint_next.y - midpoint_now.y;
            if ((abs(diff1) <= 5) && (abs(diff2) <= 5))
                flag = 1;
            else {
                if (((abs(diff2) > 0.5*abs(diff1)) && (abs(diff2) < 2 * abs(diff1))
                     && (diff1*diff2 > 0)) == false) {//误差在预测范围内
                    midpoint_group_left.pop_back();
                    flag = 0;
                }
            }
            midpoint_last = midpoint_now;
            midpoint_now = midpoint_next;
        }
    }
    if (midpoint_group_left.size() >= point_min)//点数大于point_min该有效点组才有效
        return true;
    else
        return false;
}
bool Finder::find_all_midpoint_right()
{
    Point2f midpoint_last;
    Point2f midpoint_now;
    Point2f midpoint_next;
    int flag_right = find_first_midpoint_right();
    if (midpoint_group_right.size() == 0) //如果中点组点数为0，从右方找点失败
        return false;
    if (midpoint_group_right.size() == 1) {//如果中点组点数为1，采用非预测采点模式
        midpoint_last = midpoint_group_right[0];
        midpoint_now = midpoint_group_right[0];
        bool flag = 0;
        flag = find_next_midpoint_right(&midpoint_next, midpoint_now);
        if (flag == true) {}
        else
            return false;
    }
    if (midpoint_group_right.size() >= 2) {//中点组点数大于等于2，采用预测采点模式
        int i = midpoint_group_right.size();
        midpoint_last = midpoint_group_right[i - 2];
        midpoint_now = midpoint_group_right[i - 1];
        double diff1, diff2;
        bool flag = 1;
        while (flag) {
            flag = find_next_midpoint_right(&midpoint_next, midpoint_now);
            if (flag == 0)
                break;
            diff1 = midpoint_now.y - midpoint_last.y;
            diff2 = midpoint_next.y - midpoint_now.y;
            if ((abs(diff1) <= 5) && (abs(diff2) <= 5))
                flag = 1;
            else {
                if (((abs(diff2) > 0.5*abs(diff1)) && (abs(diff2) < 2 * abs(diff1))
                     && (diff1*diff2 > 0)) == false) {//误差在预测范围内
                    midpoint_group_right.pop_back();
                    flag = 0;
                }
            }
            midpoint_last = midpoint_now;
            midpoint_now = midpoint_next;
        }
    }
    if (midpoint_group_right.size() >= point_min)//点数大于point_min该有效点组才有效
        return true;
    else
        return false;
}
bool Finder::polynomial_curve_fit(vector<Point2f> midpoint_group, int n, Mat &A) {
    //最小二乘法拟合直线
    int N = midpoint_group.size();
    Mat X = Mat::zeros(n + 1, n + 1, CV_64FC1);
    for (int ii = 0; ii < n + 1; ii++) {
        for (int jj = 0; jj < n + 1; jj++) {
            for (int kkk = 0; kkk < N; kkk++) {
                X.at<double>(ii, jj) = X.at<double>(ii, jj) +
                                       std::pow(midpoint_group[kkk].x, ii + jj);
            }
        }
    }
    cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    for (int ii = 0; ii < n + 1; ii++) {
        for (int kkk = 0; kkk < N; kkk++) {
            Y.at<double>(ii, 0) = Y.at<double>(ii, 0) +
                                  std::pow(midpoint_group[kkk].x, ii) * midpoint_group[kkk].y;
        }
    }
    A = cv::Mat::zeros(n + 1, 1, CV_64FC1);
    cv::solve(X, Y, A, cv::DECOMP_LU);
    return true;
}
void Finder::get_theta(double k_out,double b_out)
{
    double theta_original = atan(k_out);//theta为0到180度
    double PI = 3.1415926;
    theta_original = theta_original * 180 / PI;
    if ((k_out == 0) && (b_out == 0))//此时斜率实际为无穷大
        theta = 90;
    else {
        if (flag_left == true) {
            if (theta_original <= 0)
                theta = 0;
            else
                theta = theta_original;
        }
        if (flag_right == true) {
            if (theta_original >= 0)
                theta = 180;
            else
                theta = theta_original + 180;
        }
        if (flag_front == true) {
            if (theta_original >= 0)
                theta = theta_original;
            else
                theta = theta_original + 180;
        }
    }
}
void Finder::get_distance(double k_out, double b_out)
{
    double PI = 3.1415926;
    double theta_rad = theta * PI / 180;;
    if ((k_out == 0) && (b_out == 0)) {//此时斜率实际为无穷大
        double x_real, x_sum = 0;
        for (int i = 0; i < midpoint_group.size(); i++)
            x_sum = x_sum + midpoint_group[i].x;
        x_real = x_sum / midpoint_group.size();
        distance = x_real - width / 2;
    }
    else {
        if ((theta == 0) || (theta == 180))
            distance = 200 - b_out;
        else
            distance = ((length/2 - b_out) / k_out - 200)*sin(theta_rad);
    }
}
int Finder::operator()(FinderInfo &finderInfo)
{
    VideoCapture capture("/home/fatcat/Downloads/finder/视频-全程.mp4");
    //VideoCapture capture("/dev/video1");

    if (!capture.isOpened()) //检查打开是否成功 
    {
        cout<<"camera is not opened"<<endl;
        return -1;
    }
    cout<<"camera is opened"<<endl;
    float fps = capture.get(CAP_PROP_FPS);
    Mat frame;

    while(capture.isOpened())
    {
        capture >> frame;
        frame = frame(Rect(50, 0, 400, 400));
        get_threshold(frame);//二值化
        imshow("img_binary", img_binary);

        flag_front = find_all_midpoint_front();
        flag_left = find_all_midpoint_left();
        flag_right = find_all_midpoint_right();
        cout << "flag_front:" << flag_front << endl;
        cout << "flag_right:" << flag_right << endl;
        cout << "flag_left:" << flag_left << endl;

        if ((flag_front == false) && (flag_left == false) && (flag_right == false))
            cout << "没有寻到任何线" << endl;
        else if ((flag_front == true) && (flag_left == true) && (flag_right == true))
        {//从上方，左方，右方都能找到点，则为十字模式
            Mat A1, A2;
            double k1, k2, b1, b2;
            polynomial_curve_fit(midpoint_group_front, 1, A1);
            polynomial_curve_fit(midpoint_group_right, 1, A2);
            k1 = A1.at<double>(1, 0);
            k2 = A2.at<double>(1, 0);
            b1 = A1.at<double>(0, 0);
            b2 = A2.at<double>(0, 0);

            if ((k1 == 0) && (b1 == 0)) {
                double x_sum = 0;
                for (int i = 0; i < midpoint_group_front.size(); i++)
                    x_sum = x_sum + midpoint_group_front[i].x;
                center.x = x_sum / midpoint_group_front.size();
                center.y = k2 * center.x + b2;
                center_k1 = 1000000;//正常情况为无穷，用一个很大的数表示
                center_k2 = k2;

                Point2d a1, a2, c1, c2;
                a1.x = center.x;
                a1.y = 0;
                a2.x = center.x;
                a2.y = length - 1;
                c1.x = 0;
                c1.y = k2 * c1.x + b2;
                c2.x = width - 1;
                c2.y = k2 * c2.x + b2;
                line(img_original, a1, a2, Scalar(0, 255, 0));
                line(img_original, c1, c2, Scalar(0, 255, 0));
            }
            else {
                center.x = (b2 - b1) / (k1 - k2);
                center.y = k1 * center.x + b1;
                center_k1 = k1;
                center_k2 = k2;

                Point2d a1, a2, c1, c2;
                a1.x = 0;
                a1.y = k1 * a1.x + b1;
                a2.x = width - 1;
                a2.y = k1 * a2.x + b1;
                c1.x = 0;
                c1.y = k2 * c1.x + b2;
                c2.x = width - 1;
                c2.y = k2 * c2.x + b2;
                line(img_original, a1, a2, Scalar(0, 255, 0));
                line(img_original, c1, c2, Scalar(0, 255, 0));
            }

            cout << "十字模式" << endl;
            cout << "center:" << center << endl;
            cout << "center_k1:" << center_k1 << endl;
            cout << "center_k2:" << center_k2 << endl;

            finderInfo.set(1, center_k1, center.x, center.y);

            circle(img_original,
                   center,
                   2,
                   cv::Scalar(0, 0, 255),
                   2,
                   8,
                   0);
            for (int i = 0; i < midpoint_group_front.size() - 1; i++)
            {
                circle(img_original,
                       midpoint_group_front[i],
                       2,
                       cv::Scalar(255, 0, 0),
                       2,
                       8,
                       0);
            }
            for (int i = 0; i < midpoint_group_left.size() - 1; i++)
            {
                circle(img_original,
                       midpoint_group_left[i],
                       2,
                       cv::Scalar(255, 0, 0),
                       2,
                       8,
                       0);
            }
            for (int i = 0; i < midpoint_group_right.size() - 1; i++)
            {
                circle(img_original,
                       midpoint_group_right[i],
                       2,
                       cv::Scalar(255, 0, 0),
                       2,
                       8,
                       0);
            }
            Point end = cv::Point(50, 50);
            string str = "find cross";
            putText(img_original, str, end, FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255), 2);
        }
        else {//巡线模式
            if (flag_front == true)
                midpoint_group = midpoint_group_front;
            else if (flag_left == true)
                midpoint_group = midpoint_group_left;
            else
                midpoint_group = midpoint_group_right;

            Mat A;
            double k, b;
            polynomial_curve_fit(midpoint_group, 1, A);
            k = A.at<double>(1, 0);
            b = A.at<double>(0, 0);
            get_theta(k, b);
            get_distance(k, b);
            cout << "巡线模式" << endl;
            cout << "k:" << k << endl;
            cout << "b:" << b << endl;
            cout << "theta" << theta << endl;
            cout << "distance" << distance << endl;

            finderInfo.set(0, theta, distance, 0);

            for (int i = 0; i < midpoint_group.size() - 1; i++)
            {
                circle(img_original,
                       midpoint_group[i],
                       2,
                       cv::Scalar(255, 0, 0),
                       2,
                       8,
                       0);
            }
            Point end = cv::Point(50, 50);
            string str = "find line";
            putText(img_original, str, end, FONT_HERSHEY_DUPLEX, 1, Scalar(255, 0, 0), 2);
        }
        imshow("img_original", img_original);
        midpoint_group.clear();
        midpoint_group_front.clear();
        midpoint_group_left.clear();
        midpoint_group_right.clear();
        waitKey(10);
    }


}