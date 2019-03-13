#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>
#include "finder.h"
#include "info.h"
#include "serial.h"

using namespace std;
using namespace cv;

union Out wdata {};

MySerial ms = MySerial();
int fd;

void printMes(int signo) {
    //printf("Get a SIGALRM, signal NO:%d\n", signo);
    //sum flag
    assignSum(&wdata);
    ms.nwrite(fd, wdata.data, sizeof(wdata.data));
    //cout << "wdata.meta.mode2.deflection" << wdata.meta.mode2.deflection << endl;
    //restore
    wdata = {};
}

int main()
{
//    fd = ms.open_port(1);
//    ms.set_opt(fd, BAUDRATE, 8, 'N', 1);
//    bool serialOpen = true;

    struct itimerval tick;
    signal(SIGALRM, printMes);
    memset(&tick, 0, sizeof(tick));
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = 50000;
    tick.it_interval.tv_sec = 0;
    tick.it_interval.tv_usec = 50000;
    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set time fail!");

    Finder finder;
    FinderInfo finderInfo;

    cout<<"over"<<endl;
    bool videoused = true;

    while (true) {
        //realtime find line
        if(videoused){
            videoused = false;
            thread thread1(finder, ref(finderInfo));
            thread1.detach();
        }
        double res[4];
        int flag = finderInfo.get(res);
        if (flag > 0) {
            float line[] = {
                    static_cast<float>(res[1]),
                    static_cast<float>(res[2]),
                    static_cast<float>(res[3])
            };
            if (res[0] == 0) {//巡线
                wdata.meta.dataArea[0] |= 0x01;
                memcpy(wdata.meta.mode1.deflection, &line[0], sizeof(line[0]));
                memcpy(wdata.meta.mode1.distance, &line[1], sizeof(line[1]));
                memcpy(wdata.meta.mode1.meaningless, &line[2], sizeof(line[2]));
            }
            if (res[0] == 1) {//十字
                wdata.meta.dataArea[0] |= 0x02;
                memcpy(wdata.meta.mode2.deflection, &line[0], sizeof(line[0]));
                memcpy(wdata.meta.mode2.axisX, &line[1], sizeof(line[1]));
                memcpy(wdata.meta.mode2.axisY, &line[2], sizeof(line[2]));
                //cout << "line[0]" << line[0] << endl;
                cout << "wdata.meta.mode2.deflection" << wdata.meta.mode2.deflection << endl;
            }
        }
        pause();
    };

    return 0;
}

//int main()
//{
//
//
//}

//int main()
//{
//	Mat pic = imread("C:\\Users\\LENOVO\\Desktop\\视觉寒假\\finder\\finder\\右侧寻线\\55.bmp");
//	resize(pic, pic, Size(400, 400));
//	finder finder1;
//	finder1.finder_operator(pic);
//	waitKey();
//	return 0;
//}

//int main()
//{
//	char c = 0;
//	VideoCapture capture(0);
//	//设置摄像头参数 不要随意修改
//	capture.set(CV_CAP_PROP_FRAME_WIDTH, 1080);//宽度
//	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 960);//高度
//	capture.set(CV_CAP_PROP_FPS, 30);//帧数
//	capture.set(CV_CAP_PROP_BRIGHTNESS, 1);//亮度 1
//	capture.set(CV_CAP_PROP_CONTRAST,40);//对比度 40
//	capture.set(CV_CAP_PROP_SATURATION, 50);//饱和度 50
//	capture.set(CV_CAP_PROP_HUE, 50);//调 50
//	capture.set(CV_CAP_PROP_EXPOSURE, 50);//曝光 50
//
//	//打印摄像头参数
//	printf("width = %.2f\n", capture.get(CV_CAP_PROP_FRAME_WIDTH));
//	printf("height = %.2f\n", capture.get(CV_CAP_PROP_FRAME_HEIGHT));
//	printf("fbs = %.2f\n", capture.get(CV_CAP_PROP_FPS));
//	printf("brightness = %.2f\n", capture.get(CV_CAP_PROP_BRIGHTNESS));
//	printf("contrast = %.2f\n", capture.get(CV_CAP_PROP_CONTRAST));
//	printf("saturation = %.2f\n", capture.get(CV_CAP_PROP_SATURATION));
//	printf("hue = %.2f\n", capture.get(CV_CAP_PROP_HUE));
//	printf("exposure = %.2f\n", capture.get(CV_CAP_PROP_EXPOSURE));
//	while (1)
//	{
//		Mat frame;
//		capture >> frame;
//		namedWindow("video");
//		imshow("video", frame);
//		c = cvWaitKey(30);
//		if (c == 27)//Esc键退出
//		{
//			break;
//		}
//	}
//	return 0;
//}