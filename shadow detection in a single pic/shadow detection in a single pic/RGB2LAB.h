struct pixelInfo{
	bool shadow; 
};

int rgb2lab();

//����L,A,Bͨ����ƽ��ֵ
int labMean();
double computeL();
double computeA();
double computeB();

int shadowDetction();