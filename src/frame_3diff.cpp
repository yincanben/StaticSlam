/*************************************************************************
	> File Name: frame_3diff.cpp
	> Author:yincanben 
	> Mail: yincanben@163.com
	> Created Time: Mon 22 Dec 2014 04:45:51 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#define threshold_diff1 10 //设置简单帧差法阈值
#define threshold_diff2 10 //设置简单帧差法阈值

using namespace cv;
using namespace std;

int main(int argc,unsigned char* argv[])
{
    Mat img_src1,img_src2,img_src3;//3帧法需要3帧图片
    Mat img_dst,gray1,gray2,gray3;
    Mat gray_diff1,gray_diff2;//存储2次相减的图片
    Mat gray;//用来显示前景的
    bool pause=false;

    VideoCapture vido_file("IndoorGTTest1.avi");//在这里改相应的文件名
    namedWindow("foreground",0);
    for (;;)
    {
        if(!false)
        {
            vido_file >>img_src1;
            cvtColor(img_src1,gray1,CV_BGR2GRAY);
        
            waitKey(5);
            vido_file >>img_src2;
            cvtColor(img_src2,gray2,CV_BGR2GRAY);
            imshow("video_src",img_src2);//

            waitKey(5);
            vido_file >>img_src3;
            cvtColor(img_src3,gray3,CV_BGR2GRAY);

            subtract(gray2,gray1,gray_diff1);//第二帧减第一帧
            subtract(gray3,gray2,gray_diff2);//第三帧减第二帧

            for(int i=0;i<gray_diff1.rows;i++)
                for(int j=0;j<gray_diff1.cols;j++)
                {
                    if(abs(gray_diff1.at<unsigned char>(i,j))>=threshold_diff1)//这里模板参数一定要用unsigned char，否则就一直报错
                        gray_diff1.at<unsigned char>(i,j)=255;            //第一次相减阈值处理
                    else gray_diff1.at<unsigned char>(i,j)=0;

                    if(abs(gray_diff2.at<unsigned char>(i,j))>=threshold_diff2)//第二次相减阈值处理
                        gray_diff2.at<unsigned char>(i,j)=255;
                    else gray_diff2.at<unsigned char>(i,j)=0;
                }
            bitwise_and(gray_diff1,gray_diff2,gray);
            imshow("foreground",gray);
        }
        char c=(char)waitKey(10);
        if (c==27)
        {
            break;    
        }
        if(c==' ')
            pause=!pause;//为什么暂停不了？？
    }
    return 0;
}
