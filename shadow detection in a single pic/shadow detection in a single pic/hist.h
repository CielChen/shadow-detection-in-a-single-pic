#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;
using namespace std;

//Lab空间的直方图
int hist();
IplImage* DrawHistogram(CvHistogram* hist, float scaleX, float scaleY);