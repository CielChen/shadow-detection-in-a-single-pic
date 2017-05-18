struct pixelInfo{
	bool shadow; 
};

int rgb2lab();

//计算L,A,B通道的平均值
int labMean();
double computeL();
double computeA();
double computeB();

int shadowDetction();