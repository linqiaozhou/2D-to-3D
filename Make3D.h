#ifndef _INSIDE_VISUAL_CPP_Make3D
#define _INSIDE_VISUAL_CPP_Make3D
#include "ImageCenterDib.h"

//����ͼ����ǿ��
class Make3D:public ImgCenterDib  
{
public:
	int m_nBitbuf;
    int m_nBitCountOut;
	unsigned char * m_pImgDataOut;
	unsigned char * m_pImgDataOutBinary;//����ͼ��ָ��
	unsigned char * m_pImgDataOutleft;//����ͼ��ָ��
	unsigned char * m_pImgDataOutright;//����ͼ��ָ��
	unsigned char * m_pImgDataOutfinal;//red-cyan��ָ��


    int linByteOut;
    int 	m_nColorTableLengthOut;
	LPRGBQUAD m_lpColorTableOut;
    unsigned char * m_pImgDataOut1;//���¼���ָ�������غ�������
    unsigned char * m_pImgDataOut2;
 private:
	//���ͼ��Ŀ�
	int m_imgWidthOut;

	//���ͼ��ĸ�
	int m_imgHeightOut;


	float template1;     //����Ƕȣ�
	float template2;
	float template3;


	
public:
	//���캯��
	Make3D();
	Make3D(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData);
    ~Make3D();
	
    void ColorToGray();
	void BilateralFilter(int iFilterH, int iFilterW, int iFilterCX, int iFilterCY);
	void MSTsegment(int blockw,int blockh);
	void MSTsegment(unsigned char*	pImpuf,int blockw,int blockh);//������С�������ָ�
	void DIBR();
	void Holefilling();      //�ն����
	void RedCyan();         //�õ����յĺ���ͼ 
    void BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY,unsigned char*pImbuf);//�����˲�����

	void Binary(int threshold);//��ֵ��
	void Binary(unsigned char *buf,int threshold);//���صĶ�ֵ������
	
	
     void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    int *max, int *maxR,int *maxA);

	void longestLineDetectByHough(float radiusResolution, float angleResolution);//hough�任����ֱ��
	void longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution);//���صĶ�ֵ������
	void Sobel();   //Soble���ӱ�Ե���
	void Template();//������ʾ��ѡ�õ�ģ��	
	unsigned char FindMedianValue(unsigned char* pbArray,int ArrayLen);//������ֵ�˲�
};

#endif // _INSIDE_VISUAL_CPP_Make3D