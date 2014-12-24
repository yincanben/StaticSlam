/*************************************************************************
	> File Name: frame_diff.cpp
	> Author: yincanben
	> Mail: yincanben@163.com
	> Created Time: Mon 22 Dec 2014 04:31:58 PM CST
 ************************************************************************/

#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#define threshold_diff 40 //设置简单帧差法阈值

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    Mat img_src1, img_src2, img_dst, gray1, gray2, gray_diff, dst1, dst2 ;

    img_src1 = imread( "../images/image1.jpg" ) ;
    img_src2 = imread( "../images/image4.jpg" ) ;
    if( !img_src1.data )    ( cout << "image1 hasn't data! " ) ;
    if( !img_src2.data )    ( cout << "image2 hasn't data! " ) ;
    namedWindow("foreground", 1) ;
    cvtColor(img_src1, gray1, CV_BGR2GRAY) ;
    cvtColor( img_src2, gray2, CV_BGR2GRAY ) ;
    GaussianBlur( gray1, dst1, Size( 11, 11 ), 0, 0 );
    GaussianBlur( gray2, dst2, Size( 11, 11 ), 0, 0 );
    subtract( dst1, dst2, gray_diff ) ;
    for(int i=0;i<gray_diff.rows;i++)
        for(int j=0;j<gray_diff.cols;j++)
            if(abs(gray_diff.at<unsigned char>(i,j))>=threshold_diff)//这里模板参数一定要用unsigned char，否则就一直报错
                gray_diff.at<unsigned char>(i,j)=255;
            else 
                gray_diff.at<signed char>(i,j)=0;
    imshow("foreground",gray_diff);
    waitKey(0) ;
    return 0;
}

