#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <core/affine.hpp>
#include <highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "RGB2LAB.h"
#include "hist.h"

using namespace cv;
using namespace std; 

int main()
{
	//step1. convert RGB to LAB
	rgb2lab();
	
	//计算LAB三个通道的像素均值
	//computeL();
	//computeA();
	//computeB();

	//阴影检测
	//shadowDetction();

	hist();

	system("pause");
	return 0;
}