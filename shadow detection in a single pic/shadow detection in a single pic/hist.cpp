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

	//cvGetMinMaxHistValue():����������Сֱ�����Լ����ǵ�λ��
    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0); //��ȡֱ��ͼ�����ֵ  

	IplImage* imageHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8, 1);  
    cvZero(imageHist); 

	for(int i = 0; i < 255; ++i)
	{
		//��ѯֱ��ͼÿ����λ��xֵ��Ӧ��yֵ  
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

		//ͨ��ֱ��ͼΪ�����ģ����������������ֱ����֮������  
        cvFillConvexPoly(imageHist, pts, numPts, cvScalar(255)); 
	}
	return imageHist;  
}

int hist()
{
	int dims=1;  //ֱ��ͼά��  
	int size=256 ;  //ֱ��ͼά���ߴ�
	float range[]={0,256};
	float *ranges[] = {range};  
    CvHistogram * hist;  
    hist = cvCreateHist(dims, &size, CV_HIST_ARRAY, ranges, 1); //����ֱ��ͼ  
    cvClearHist(hist);  //���ֱ��ͼ  

	extern IplImage *LABimg;
	IplImage* imageL= cvCreateImage(cvGetSize(LABimg), 8, 1);  
    IplImage* imageA= cvCreateImage(cvGetSize(LABimg), 8, 1);  
    IplImage* imageB= cvCreateImage(cvGetSize(LABimg), 8, 1); 
	//��Դͼ��ָ��LAB��ͨ��  
    cvSplit(LABimg, imageL, imageA, imageB, NULL);  

	cvCalcHist(&imageL, hist, 0, 0); //����Lͨ������ֱ��ͼ  
    IplImage* histL = DrawHistogram(hist, 1, 1);  
    cvClearHist(hist); 
	cvCalcHist(&imageA, hist, 0, 0); //����Aͨ������ֱ��ͼ  
    IplImage* histA = DrawHistogram(hist, 1, 1);  
    cvClearHist(hist);
	cvCalcHist(&imageB, hist, 0, 0); //����Bͨ������ֱ��ͼ  
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