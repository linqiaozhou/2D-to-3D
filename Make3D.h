#ifndef _INSIDE_VISUAL_CPP_Make3D
#define _INSIDE_VISUAL_CPP_Make3D
#include "ImageCenterDib.h"

//处理图像增强类
class Make3D:public ImgCenterDib  
{
public:
	int m_nBitbuf;
    int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	unsigned char * m_pImgDataOutBinary;//左视图的指针
	unsigned char * m_pImgDataOutleft;//左视图的指针
	unsigned char * m_pImgDataOutright;//右视图的指针
	unsigned char * m_pImgDataOutfinal;//red-cyan的指针


    int linByteOut;
    int 	m_nColorTableLengthOut;
	LPRGBQUAD m_lpColorTableOut;
    unsigned char * m_pImgDataOut1;//以下几个指针在重载函数中用
    unsigned char * m_pImgDataOut2;
 private:
	//输出图像的宽
	int m_imgWidthOut;

	//输出图像的高
	int m_imgHeightOut;


	float template1;     //保存角度，
	float template2;
	float template3;


	
public:
	//构造函数
	Make3D();
	Make3D(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
    ~Make3D();
	
    void ColorToGray();
	void BilateralFilter(int iFilterH, int iFilterW, int iFilterCX, int iFilterCY);
	void MSTsegment(int blockw,int blockh);
	void MSTsegment(unsigned char*	pImpuf,int blockw,int blockh);//重载最小生成树分割
	void DIBR();
	void Holefilling();      //空洞的填补
	void RedCyan();         //得到最终的红蓝图 
    void BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY,unsigned char*pImbuf);//重载滤波函数

	void Binary(int threshold);//二值化
	void Binary(unsigned char *buf,int threshold);//重载的二值化函数
	
	
     void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    int *max, int *maxR,int *maxA);

	void longestLineDetectByHough(float radiusResolution, float angleResolution);//hough变换检测最长直线
	void longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution);//重载的二值化函数
	void Sobel();   //Soble算子边缘检测
	void Template();//用于显示所选用的模板	
	unsigned char FindMedianValue(unsigned char* pbArray,int ArrayLen);//用于中值滤波
};

#endif // _INSIDE_VISUAL_CPP_Make3D