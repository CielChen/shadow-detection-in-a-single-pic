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
#include "universal.h"
#include "RGB2LAB.h"

using namespace cv;
using namespace std;

//#define WIDTH 1408  //HoloLens图片宽度
//#define HEIGHT 792  //HoloLens图片高度

#define WIDTH 1216  //HoloLens视频截图宽度
#define HEIGHT 684  //HoloLens视频截图高度

#define selectThresh 256
#define lbThresh 100

Mat inputMat;
IplImage *LABimg;
IplImage *lab_l,*lab_a,*lab_b;
Mat labMat,lMat,aMat,bMat;
Mat shadowMat;
int pixelNum;
int step, step_l, channels, cd, cdlab, l, a, b;
uchar *data_lab, *data_l, *data_a, *data_b;
double lmean,amean,bmean;
double lStanDeviation;
struct pixelInfo pixel[HEIGHT][WIDTH];

int rgb2lab()
{
	inputMat=cvarrToMat(input,true);

	//高斯滤波，以平滑图像
	cvSmooth(input, input, CV_GAUSSIAN, 3, 0, 0, 0);

	LABimg=cvCreateImage(cvGetSize(input), 8, input->nChannels); //开辟一个LAB颜色模式的空间来存储转化后的图像，深度为8位，通道数与原图相同

	cvCvtColor(input, LABimg, CV_BGR2Lab);  //将图像img从RGB空间转到LAB空间

	//int step, step_l, channels, cd, cdlab, l, a, b;
	//uchar *data_lab, *data_l, *data_a, *data_b;

	lab_l= cvCreateImage(cvGetSize(LABimg), LABimg->depth, 1);  //创建L图像
	lab_a= cvCreateImage(cvGetSize(LABimg), LABimg->depth, 1);  //创建a图像
	lab_b= cvCreateImage(cvGetSize(LABimg), LABimg->depth, 1);  //创建b图像

	step=LABimg->widthStep;  //step存储同列相邻行之间的比特数
	channels=LABimg->nChannels;  //通道数
	data_lab=(uchar*)LABimg->imageData;  //data_lab存储指向LAB图像数据的指针
	step_l=lab_l->widthStep;   //step_l为子图像的相邻行之间的比特
	data_l=(uchar*)lab_l->imageData;  //存储指向子图像的数据指针
	data_a=(uchar*)lab_a->imageData; 
	data_b=(uchar*)lab_b->imageData; 


	pixelNum=LABimg->height * LABimg->width; 
	lmean=0;
	amean=0;
	bmean=0;
	lStanDeviation=0;
	for(int i=0; i<LABimg->height; i++){
		for(int j=0; j<LABimg->width; j++){
			pixel[i][j].shadow=false;

			cd= i*step + j*channels;  //计算LAB图像数据的位置
			cdlab= i*step_l + j;  //计算L/a/b子图像数据存储的位置

			l=data_lab[cd];
			a=data_lab[cd+1];
			b=data_lab[cd+2];

			lmean+=l;
			amean+=a;
			bmean+=b;

			//L分量
			data_l[cdlab]=l;

			//a分量
			data_a[cdlab]=a;

			//b分量
			data_b[cdlab]=b;
		}
	}
	lmean/=pixelNum;
	amean/=pixelNum;
	bmean/=pixelNum;

	for(int i=0; i<LABimg->height; i++){
		for(int j=0; j<LABimg->width; j++){
			cd= i*step + j*channels;  //计算LAB图像数据的位置
			cdlab= i*step_l + j;  //计算L/a/b子图像数据存储的位置

			l=data_lab[cd];
			
			lStanDeviation+=pow((l-lmean),2);
		}
	}
	lStanDeviation=sqrt(lStanDeviation/pixelNum);

	
	cvNamedWindow("Image_LAB", CV_WINDOW_AUTOSIZE);  //cvNamedWindow图像窗口
	cvNamedWindow("Image_L", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_A", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_B", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image_LAB", LABimg);  //cvShowImage显示图像
	cvShowImage("Image_L", lab_l);
	cvShowImage("Image_A", lab_a);
	cvShowImage("Image_B", lab_b);

	string labPath="F:\\Code\\Shadow detection in asingle pic\\data\\LAB\\lab\\";
	string lPath="F:\\Code\\Shadow detection in asingle pic\\data\\LAB\\L\\";
	string aPath="F:\\Code\\Shadow detection in asingle pic\\data\\LAB\\A\\";
	string bPath="F:\\Code\\Shadow detection in asingle pic\\data\\LAB\\B\\";
	string lab=labPath+picName+".bmp";
	string L=lPath+picName+".bmp";
	string A=aPath+picName+".bmp";
	string B=bPath+picName+".bmp";
	const char *labOutput=lab.c_str();
	const char *lOutput=L.c_str();
	const char *aOutput=A.c_str();
	const char *bOutput=B.c_str();
	cvSaveImage(labOutput,LABimg);
	cvSaveImage(lOutput,lab_l);
	cvSaveImage(aOutput,lab_a);
	cvSaveImage(bOutput,lab_b);
	

	//格式转换
	labMat=cvarrToMat(LABimg,true);
	lMat=cvarrToMat(lab_l,true);
	aMat=cvarrToMat(lab_a,true);
	bMat=cvarrToMat(lab_b,true);
	
	
	cvWaitKey(0);  //cvWaitKey程序暂停，等待用户触发一个按键操作
	cvDestroyAllWindows();

	return 0;
}

double computeL()
{
	cout<<"lmean="<<lmean<<endl;
	return lmean;
}

double computeA()
{
	cout<<"amean="<<amean<<endl;
	return amean;
}

double computeB()
{
	cout<<"bmean="<<bmean<<endl;
	return bmean;
}

int shadowDetction()
{

	for(int i=0; i<LABimg->height; i++){
		for(int j=0; j<LABimg->width; j++){
			pixel[i][j].shadow=false;
		}
	}

	if(amean+bmean <= selectThresh)
	{
		for(int i=0; i<LABimg->height; i++){
			for(int j=0; j<LABimg->width; j++){
				cd= i*step + j*channels;  //计算LAB图像数据的位置
				cdlab= i*step_l + j;  //计算L/a/b子图像数据存储的位置

				l=data_lab[cd];
				if(l <= (lmean-lStanDeviation/3))
				{
					pixel[i][j].shadow=true;
				}
			}
		}
	}
	else
	{
		for(int i=0; i<LABimg->height; i++){
			for(int j=0; j<LABimg->width; j++){
				cd= i*step + j*channels;  //计算LAB图像数据的位置
				cdlab= i*step_l + j;  //计算L/a/b子图像数据存储的位置

				l=data_lab[cd];
				b=data_lab[cd+2];
				if(l<lbThresh && b<lbThresh)
				{
					pixel[i][j].shadow=true;
				}
			}
		}
	}
	

	shadowMat=inputMat.clone();
	for(int i=0;i<shadowMat.rows;i++)
	{
		for(int j=0;j<shadowMat.cols;j++)
		{
			if(pixel[i][j].shadow==true)
			{
				shadowMat.at<Vec3b>(i,j)[0]=255;
				shadowMat.at<Vec3b>(i,j)[1]=255;
				shadowMat.at<Vec3b>(i,j)[2]=255;
			}
			else
			{
				shadowMat.at<Vec3b>(i,j)[0]=0;
				shadowMat.at<Vec3b>(i,j)[1]=0;
				shadowMat.at<Vec3b>(i,j)[2]=0;
			}
		}
	}
	imshow("shadow",shadowMat);
	waitKey(0);
	return 0;
}