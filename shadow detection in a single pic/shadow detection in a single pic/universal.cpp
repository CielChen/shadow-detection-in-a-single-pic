#include "universal.h"

string pic_path="F:\\Code\\Shadow detection in asingle pic\\data\\RGB\\";  //����ͼƬ��·��
//string picName="HoloLens2"; 
string picName="20170228111043_scene";

IplImage *input=cvLoadImage((pic_path+picName+".jpg").data());  //cvLoadImage��ȡͼ��