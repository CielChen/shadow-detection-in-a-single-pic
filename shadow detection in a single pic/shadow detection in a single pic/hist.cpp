#include <core/affine.hpp>
#include <highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <cv.h>
#include <stdio.h>
#include <math.h>
#include <cxcore.h>
#include <cvaux.h>
#include <opencv2/opencv.hpp>
#include "hist.h"

using namespace cv;
using namespace std;

IplImage* DrawHistogram(CvHistogram* hist, float scaleX = 1, float scaleY = 1)
{
	float histMax = 0;  

	//cvGetMinMaxHistValue():发现最大和最小直方块以及它们的位置
    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0); //获取直方图的最大值  

	IplImage* imageHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8, 1);  
    cvZero(imageHist); 

	for(int i = 0; i < 255; ++i)
	{
		//查询直方图每个单位的x值对应的y值  
        float histValue = cvQueryHistValue_1D(hist, i);  
        float nextValue = cvQueryHistValue_1D(hist, i+1);

		CvPoint pt1 = cvPoint(i* scaleX, 64*scaleY);  
        CvPoint pt2 = cvPoint((i+1)*scaleX, 64*scaleY);  
        CvPoint pt3 = cvPoint((i+1)*scaleX, 64*scaleY - (nextValue/histMax)*64*scaleY);  
        CvPoint pt4 = cvPoint(i*scaleX, 64*scaleY - (histValue/histMax)*64*scaleY); 

		int numPts = 5;  
        CvPoint pts[5];  
        pts[0] = pt1;  
        pts[1] = pt2;  
        pts[2] = pt3;  
        pts[3] = pt4;  
        pts[4] = pt1; 

		//通道直方图为连续的，所以填充两两相邻直方柱之间区域  
        cvFillConvexPoly(imageHist, pts, numPts, cvScalar(255)); 
	}
	return imageHist;  
}

int hist()
{
	int dims=1;  //直方图维数  
	int size=256 ;  //直方图维数尺寸
	float range[]={0,256};
	float *ranges[] = {range};  
    CvHistogram * hist;  
    hist = cvCreateHist(dims, &size, CV_HIST_ARRAY, ranges, 1); //创建直方图  
    cvClearHist(hist);  //清除直方图  

	extern IplImage *LABimg;
	IplImage* imageL= cvCreateImage(cvGetSize(LABimg), 8, 1);  
    IplImage* imageA= cvCreateImage(cvGetSize(LABimg), 8, 1);  
    IplImage* imageB= cvCreateImage(cvGetSize(LABimg), 8, 1); 
	//将源图像分割出LAB三通道  
    cvSplit(LABimg, imageL, imageA, imageB, NULL);  

	cvCalcHist(&imageL, hist, 0, 0); //根据L通道计算直方图  
    IplImage* histL = DrawHistogram(hist, 1, 1);  
    cvClearHist(hist); 
	cvCalcHist(&imageA, hist, 0, 0); //根据A通道计算直方图  
    IplImage* histA = DrawHistogram(hist, 1, 1);  
    cvClearHist(hist);
	cvCalcHist(&imageB, hist, 0, 0); //根据B通道计算直方图  
    IplImage* histB = DrawHistogram(hist, 1, 1);  
    cvClearHist(hist);

	cvNamedWindow("L");  
    cvNamedWindow("A");  
    cvNamedWindow("B");  
  
    cvShowImage("L", histL);  
    cvShowImage("A", histA);  
    cvShowImage("B", histB);  
  
    cvWaitKey(0);
	return 0;
}