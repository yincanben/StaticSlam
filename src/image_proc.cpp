/*************************************************************************
	> File Name: image_pro.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 19 Dec 2014 09:28:24 AM CST
 ************************************************************************/

#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

/// 自定义函数声明
Mat dst3, dst ;
void Threshold_Demo( int, void* );

/**
 * @主函数
 */

int main(int argc , char** argv){
    Mat src1 , src2 , gray1 , gray2 , dst1 ,dst2 ;
    double alpha = 0.5;
    double beta , input ;
    src1 = imread("../images/image1.jpg") ;
    src2 = imread("../images/image4.jpg") ;
    if(!src1.data)(cout << "image1 hasn't data.") ;
    if(!src2.data)(cout << "image1 hasn't data.") ;
    cvtColor( src1, gray1, CV_BGR2GRAY ) ;
    cvtColor( src2, gray2, CV_BGR2GRAY ) ;
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
        { 
            GaussianBlur( gray1, dst1, Size( i, i ), 0, 0 );
            GaussianBlur( gray2, dst2, Size( i, i ), 0, 0 );
        }
    namedWindow("Linear Blend", 1);
    dst3 = dst1 - dst2 ;
    
    imshow( "Linear Blend", dst3 );
    //waitKey(0);
    
    /// 创建一个窗口显示图片
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );

    /// 创建滑动条来控制阈值
    createTrackbar( trackbar_type, window_name, &threshold_type, max_type, Threshold_Demo );

    createTrackbar( trackbar_value, window_name, &threshold_value, max_value, Threshold_Demo );

    /// 初始化自定义的阈值函数
    Threshold_Demo( 0, 0 );

    /// 等待用户按键。如果是ESC健则退出等待过程。
    while(true){
        int c;
        c = waitKey( 20 );
        if( (char)c == 27 ){
            break; 
        }
    }
    return 0 ;
}
/**
 * @自定义的阈值函数
 */
void Threshold_Demo( int, void* )
{
  /* 0: 二进制阈值
     1: 反二进制阈值
     2: 截断阈值
     3: 0阈值
     4: 反0阈值
   */

  threshold( dst3, dst, threshold_value, max_BINARY_value,threshold_type );

  imshow( window_name, dst );
}
