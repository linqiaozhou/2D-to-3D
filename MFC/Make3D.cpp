#include "stdafx.h"
#include "Make3D.h"
//#include "BilDialog.h"  //Ϊʲô����������ͻᱨ��������������Ͳ������أ����������������ƣ�����������
#include "Resource.h"
#include <math.h>
#include <malloc.h>
#include<iostream>
#include<windows.h>             //��MessageBox
#include "BilDialog.h"
#include "BlockDialog.h"


using namespace std;
Make3D::Make3D()
{
	
	m_pImgDataOut=NULL;//���ͼ��λͼ����ָ��Ϊ��
	m_pImgDataOutBinary=NULL;

	m_lpColorTableOut=NULL;//���ͼ����ɫ��ָ��Ϊ��
	
	m_nColorTableLengthOut=0;//���ͼ����ɫ����Ϊ0

    m_nBitCountOut=0;//���ͼ��ÿ����λ��Ϊ0	
    
	m_pImgDataOutleft=NULL;//����ͼ�ĳ�ʼ��
	m_pImgDataOutright=NULL;//����ͼ�ĳ�ʼ��
	m_pImgDataOutfinal=NULL;//red-cyan��ָ��


	m_pImgDataOut1=NULL;//���¼���ָ�������غ�������
    m_pImgDataOut2=NULL;

	//���ͼ��Ŀ�
	 m_imgWidthOut=0;

	//���ͼ��ĸ�
	 m_imgHeightOut=0;
//	rad=0;
//	gamma=0;

 
   // int linByteOut;
    




}

Make3D::Make3D(CSize size, int nBitCount, LPRGBQUAD lpColorTable, unsigned char *pImgData):
ImgCenterDib(size, nBitCount, lpColorTable, pImgData)
{
	
    m_nBitCountOut=0;
	m_pImgDataOut=NULL;
	m_pImgDataOutBinary=NULL;

	m_pImgDataOutleft=NULL;
	m_pImgDataOutright=NULL;
	m_pImgDataOutfinal=NULL;//red-cyan��ָ��

	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
	m_pImgDataOut1=NULL;//���¼���ָ�������غ�������
    m_pImgDataOut2=NULL;
		//���ͼ��Ŀ�
	 m_imgWidthOut=0;

	//���ͼ��ĸ�
	 m_imgHeightOut=0;
//	 rad=0;
//	gamma=0;

}

Make3D::~Make3D()
{
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_pImgDataOutBinary!=NULL)
	{
		delete []m_pImgDataOutBinary;
		m_pImgDataOutBinary=NULL;
	}
	if(m_pImgDataOutleft!=NULL)
	{
		delete []m_pImgDataOutleft;
		m_pImgDataOutleft=NULL;
	} 
	if(m_pImgDataOutright!=NULL)
	{
		delete []m_pImgDataOutright;
		m_pImgDataOutright=NULL;
	}
    if(m_pImgDataOutfinal!=NULL)//red-cyan��ָ��
    {
		delete []m_pImgDataOutfinal;
		m_pImgDataOutfinal=NULL;

    }
	if(m_lpColorTableOut==NULL)
	{
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	if(m_pImgDataOut1!=NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1=NULL;
	}
	if(m_pImgDataOut2!=NULL)
	{
		delete []m_pImgDataOut2;
		m_pImgDataOut2=NULL;
	}
}


/**********************************************************************
*
* �������ƣ�
* MedianFilter(int FilterH, int FilterW, int FilterCX, int FilterCY)
*
* ������
* int FilterH     ģ��ĸ߶�        
* int FilterW     ģ��Ŀ��
* int FilterCX    ģ�������Ԫ��X���� ( < FilterW - 1)
* int FilterCY    ģ�������Ԫ��Y���� ( < FilterH - 1)
*
* ����ֵ��
* void
*
* ˵����
* ˫���˲����㷨
*
**********************************************************************/

void Make3D::BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY)
{
	unsigned char*	pSrc;
	unsigned char*  pDst;
    unsigned char	cenv,otherv;
	float   cof;
    float   dep;
	float  sigmas=50;
	float  sigmar=40 ;
    int		i,j,k,l;

	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//����ͼ��ÿ�е��ֽ���
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	//�����ڴ棬�Ա�����ͼ��
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut + j);
	}

	for (i = FilterCY; i < m_imgHeight - FilterH +FilterCY+1; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW+FilterCX +1; j++)//+ FilterCX + 1
		{
			  dep=0;
			  cof=0;
		      cenv =*(m_pImgData + lineByte *  i  + j)  ;
	          pDst=m_pImgDataOut + lineByte * i + j;
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{
				
                  pSrc = m_pImgData + lineByte * ( i + FilterCY - k) + j - FilterCX + l;
				  otherv=*( m_pImgData + lineByte * ( i + FilterCY - k) + j - FilterCX + l);
		    float e=(float)(abs(k-i)+abs(l-j))*(abs(k-i)+abs(l-j))/(sigmas*sigmas)+(otherv-cenv)*(otherv-cenv)/(sigmar*sigmar);
                  cof=cof+(float)exp(-0.5*e);
                  dep=dep+otherv*(float)exp(-0.5*e);
				}
			}
			*pDst =(unsigned char)(dep/cof);//������float�͸���char�ͣ����ս��Ϊchar�ͣ��˴���̫��ȷ��ȴҲ��Ӱ��
		}
	}

}
/**************************************************************************
*������
*    ColorToGray()
*
*������
*    ��
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬽������ֵ�������������m_pImgDataOut�С�
***************************************************************************/
void Make3D::ColorToGray()//��ֵ��
{
	//���Ҷ�ͼ��,�򷵻�
	if(m_nBitCount==8) return;

	//�ͷžɵ����ͼ�����ݼ���ɫ������
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//��ֵ����,ÿ����λ��Ϊ8����
	m_nBitCountOut=8;

	//��ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//������ɫ������,���ɻҶ�ͼ�����ɫ��
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++)
		{
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//����ͼ��ÿ�����ֽ���,��ɫͼ��Ϊ3�ֽ�/����
	int pixelByteIn=3;
	
	//����ͼ��ÿ��������ռ�ֽ���,������4�ı���
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;



	//���ͼ��ÿ��������ռ�ֽ���,������4�ı���
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//�������ͼ��λͼ���ݻ�����
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	

	//ѭ������,ͼ�������
	int i,j;

	//���ݻ�ֵ����ʽΪ���ͼ��ֵ
	for(i=0;i<m_imgHeight;i++)
	{
		for(j=0;j<m_imgWidth;j++)
			*(m_pImgDataOut+i*lineByteOut+j)=0.11**(m_pImgData+i*lineByteIn+j*pixelByteIn+0)
		        	+0.59**(m_pImgData+i*lineByteIn+j*pixelByteIn+1)
		        	+0.30**(m_pImgData+i*lineByteIn+j*pixelByteIn+2)+0.5;
	}
   
}



/**************************************************************************
*������
*    Binary()
*
*������
*    int threshold   -��ֵ
*
*����ֵ
*    ��
*
*˵�����ú�����m_pImgData��Ϊ����ͼ�񣬸��ݸ�������ֵthreshold��ͼ����ж�ֵ����
*      �������m_pImgDataOut�С����ǲ�ɫͼ�����Ƚ�ͼ����л�ֵͼ����ٶ�ֵ����
***************************************************************************/
void Make3D::Binary(int threshold)
{
	//���ڻҶ�ͼ��
	if(m_nBitCount==8){
		//�ͷžɵ����ͼ�����ݼ���ɫ������
		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//���ͼ���ÿ����λ������ɫ����
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//�������ͼ����ɫ����������������ͼ����ɫ���������ͼ����ɫ����
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
	
		
		
		//ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ������
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++){
				if(*(m_pImgData+i*lineByte+j)<threshold)
    				*(m_pImgDataOut+i*lineByte+j)=0;
				else
					*(m_pImgDataOut+i*lineByte+j)=255;
			}
		}
	}
	else{

		//���ǲ�ɫͼ��,�ȱ�ɻҶȸ�ʽ���任��m_pImgDataOut�Ѿ��������ڴ棬
        //������˻Ҷ�ͼ������
		ColorToGray();

		//���ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ�������
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++)
				if(*(m_pImgDataOut+i*lineByteOut+j)<threshold)
					*(m_pImgDataOut+i*lineByteOut+j)=0;
				else
					*(m_pImgDataOut+i*lineByteOut+j)=255;
		}
	}
	
}
/**************************************************************************
*������
*    Binary()
*
*������
*    int threshold   -��ֵ
*
*����ֵ
*    ��
*
*˵�������صĶ�ֵ������
***************************************************************************/
void Make3D::Binary(unsigned char *buf,int threshold)
{
	//���ڻҶ�ͼ��
	if(m_nBitCount==8){
		//�ͷžɵ����ͼ�����ݼ���ɫ������
		if(m_pImgDataOutBinary!=NULL){
			delete []m_pImgDataOutBinary;
			m_pImgDataOutBinary=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//���ͼ���ÿ����λ������ɫ����
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//�������ͼ����ɫ����������������ͼ����ɫ���������ͼ����ɫ����
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
	
		
		
		//ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ������
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//�������ͼ��λͼ���ݻ�����
		m_pImgDataOutBinary=new unsigned char[lineByte*m_imgHeight];

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++)
		{
			for(j=0;j<m_imgWidth;j++)
			{
				if(*(buf+i*lineByte+j)<threshold)
    				*(m_pImgDataOutBinary+i*lineByte+j)=0;
				else
					*(m_pImgDataOutBinary+i*lineByte+j)=255;
			}
		}
	}
/*	else{

		//���ǲ�ɫͼ��,�ȱ�ɻҶȸ�ʽ���任��m_pImgDataOut�Ѿ��������ڴ棬
        //������˻Ҷ�ͼ������
		ColorToGray();

		//���ͼ��ÿ��������ռ�ֽ���������ͼ�������ͼ��ÿ��������ռ�ֽ�������
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//ѭ��������ͼ�������
		int i,j;

		//��ֵ��
		for(i=0;i<m_imgHeight;i++){
			for(j=0;j<m_imgWidth;j++)
				if(*(m_pImgDataOutBinary+i*lineByteOut+j)<threshold)
					*(m_pImgDataOutBinary+i*lineByteOut+j)=0;
				else
					*(m_pImgDataOutBinary+i*lineByteOut+j)=255;
		}
		
	}
	*/
	
}

/**********************************************************************
*
* �������ƣ�MSTsegment(int blockw,int blockh)
* 
*
* ������
*      ��MST�ָ�ʱ���õ������Сint blockw,int blockh
* ����ֵ��
* void
*
* ˵����
* �����������С������ͼ��ָ���㷨
*
**********************************************************************/
void Make3D::MSTsegment(int blockw,int blockh)
{

   	int	i,j,m;
	int SetValue=5;   //������ֵ
	int Grayvalue=0;
	int regeionnum=1;
	int THregion;  //���÷ָ������   ��һ������������blokcsize/20ʵ��
	int steplength=2;   //���÷ָ���ֵÿ�θı�Ĳ���

    float d;
	long int k;

	typedef struct
	{
       int fromvex,endvex; //�ߵ������յ�
       int length;         //�ߵ�Ȩֵ
	}edge;
	

	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//����ͼ��ÿ�е��ֽ���
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	//�����ڴ棬�Ա�����ͼ��
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut + j);
	}

 long int Sumblocks=(m_imgWidth/blockw)*( m_imgHeight/blockh); 
if(Sumblocks>30000)
 {
       AfxMessageBox("��ѡ�����������С");
		return ;
 } 
 unsigned char*grah=new unsigned char[Sumblocks*Sumblocks] ;
 unsigned char*flag;    //����һ��ָ��ı�־λ����ֹ��ָ����������
 flag=grah;
 /*if(grah==NULL)
   {
	   return NULL;
   }*/    //��Ϊ�����Ҷ�������޷����͵ģ��������Ҳ������

	//������������ͼ
  unsigned char  cize=blockw*blockh;
  int  Sum=0;		
 for(i =  0; i < m_imgHeight/blockh; i=i++)      
		for(j = 0; j < m_imgWidth/blockw ; j=j++)	
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut + (i*blockh+k) * lineByteOut +j*blockw +m);  //�����ܺ�
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		/*for(i =  0; i < m_imgHeight; i=i+blockh)      
		for(j = 0; j < m_imgWidth ; j=j+blockw)	  //���� ��m_imgWidth=9ʱ��j==8ʱӦ����ֹ�ˣ�������Ϊ8<9,�����ǽ���ѭ���ˣ����Ի����
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut1 + (i+k) * lineByteOut +j+m);  //�����ܺ�
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		*/
        //  grah=grah-Sumblocks;  //�ָ�ָ��
 
		   
		   edge*T=(edge*)malloc(sizeof(edge) *(Sumblocks-1)); //��С������
 /*  if(T==NULL)
   {
	   return NULL;
   }*/
	

//��̬���ٶ�ά���飬��������ߵ���Ϣ������Ȩ�ڽӾ���


unsigned char **dist = (unsigned char **) malloc( sizeof(unsigned char*) *Sumblocks );
for(i = 0; i < Sumblocks; i++)
{
   dist[i] = (unsigned char*)malloc( sizeof(unsigned char) *Sumblocks );
}
     
  //��ʼ���ڽӾ���
	
	for(i =  0; i <Sumblocks ; i++)
   {
		for(j = 0; j < Sumblocks ; j++)
		dist[i][j]	=255;             

	}

	//��ͼ���������뵽��Ȩ�ڽӾ���
	for(i =  0; i <( m_imgHeight/blockh-1); i++)
	{
		for(j = 0; j < (m_imgWidth/blockw-1); j++)
		
       
		{
		k=i*(m_imgWidth/blockw)+j;
        dist[k][k+1]=abs(*(grah +k) -*(grah +k+1));   
        dist[k+1][k]=dist[k][k+1];
		dist[k][k+m_imgWidth/blockw]=abs(*(grah +i*(m_imgWidth/blockw)+j) -*(grah +(i+1)*(m_imgWidth/blockw)+j));
        dist[k+m_imgWidth/blockw][k]=dist[k][k+m_imgWidth/blockw];
		}
	}
    
/*	flag--;                //��֪��Ϊɶ�������ᱨ��            
	if(flag!=NULL)         
{
	delete flag;	
}*/
   
if(grah!=NULL)     
{
	delete []grah;
	grah=NULL;
	
}

   //��prime�㷨������С������   
   int v=0;  //�������õ�һ�����붥��Ϊ��
   //�����һ������
   for(j=0;j<Sumblocks-1;j++)
   {
	   T[j].fromvex=v;
	   if(j>=v)
	   {
		   T[j].endvex=j+1;
		   T[j].length=dist[v][j+1];

	   }
	   else
	   {
		   T[i].endvex =j;
		   T[i].length=dist[v][j];
	   }
   }
 for(k=0;k<Sumblocks-1;k++)     //���K����
	{
		int min=500;
		for(j=k;j<Sumblocks-1;j++)
		{
			if(T[j].length<min)
			{
			 min=T[j].length;   
		    	m=j;
			}
		}
      
			edge e=T[m];
			T[m]=T[k];
			T[k]=e;
			v=T[k].endvex;   //v������ҵ�����̱���V-U�еĶ���	
		
      for(j=k+1;j<Sumblocks-1;j++)  //�޸����洢����С�߼�(���޸ĳ�ʼ��edge�е�ֵ)
		{
		 d=dist[v][T[j].endvex];
		 if(d<T[j].length)
		 {
				  T[j].length=d;
				  T[j].fromvex=v;
		 }
	}
 }//��С�������������
 
  if(dist!=NULL)             //�ͷŶ�ά����Ŀռ�
 {
	 for(i = 0; i < Sumblocks; i++)
	 {
        free(dist[i]);
        dist[i]=NULL;

	 }

      free(dist);    
       dist=NULL;
 }


//Ԥ����
 Make3D::Binary(m_pImgDataOut,128);                                 //�õ�������������ʹ�������ݶȼ���
 Make3D::longestLineDetectByHough(m_pImgDataOutBinary,1,1);

 if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }


 //����ģ��ѡ������ݶȵ�ģʽ��������������ѡ��
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>8&&abs(template1-template3)>8)         //���Ͻ����ģʽ,���1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>8&&abs(template2-template3)>8)         //���Ͻ����ģʽ�����1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=0.5;
     Wud=-0.5;
 }
  else 
  {
      Wrl=0;
    Wud=-1;

  }
    
//���´����������÷ָ���
    THregion=Sumblocks/12;
  //THregion=300;
    int fixR;
 	if(blockw==2)
		fixR=10;
	else if(blockw==4)
       fixR=2;
	else 
       fixR=0;
 
regeionnum=THregion+1;

while( regeionnum>THregion)
{   
	SetValue=SetValue+steplength;
    regeionnum=0;
	for(i=0;i<Sumblocks-1;i++)
  {
      if(T[i].length<SetValue)
	    continue;
	  else
	   regeionnum++;
	 
	}

}
//SetValue=20;
	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth; j++)
		{
			*(m_pImgDataOut+i*lineByteOut+j)=255;


		}
	}

  //���´�������С��������ɶ�ͼ����и�,���Բ�ͬ�������費ͬ�ĻҶ�ֵ(���³������ͼƬ���������4�ı���ʱ��Ч)
 //����ȼ����ʼ��
   // SetValue=5;
  //����ȼ����ʼ��
 int *flagM=new int[lineByteOut * m_imgHeight];
  	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth ; j++)
			*(flagM + i * lineByteOut +j) = -1;
	}
float pixel_num=1;
float  hypothesis=0;
for(i=0;i<Sumblocks-1;i++)
  {
	int y,x; 
       if((T[i].length<SetValue||pixel_num<fixR)&&i<Sumblocks-2)  //<Sumblocks-2�ж��Ƿ������һ�������� ||pixel_num<50
	   {		 
          y=T[i].fromvex/(m_imgWidth/blockw);
          x=T[i].fromvex%(m_imgWidth/blockw);
		 pixel_num=pixel_num+1;
	     hypothesis=hypothesis+Wrl*(x-(m_imgWidth/blockw)/2)/(m_imgWidth/blockw)+Wud*(y-(m_imgHeight/blockh)/2)/((m_imgHeight/blockh));
	     continue;
	 }
    float  depth=128+255*hypothesis/pixel_num ;//�õ�������� 
	for(j=i-pixel_num+1;j<(i+1);j++)   
      {
			 
		 int row=T[j].fromvex/(m_imgWidth/blockw);
         int line=T[j].fromvex%(m_imgWidth/blockw); 
		 int k,m;
		 for(k=0;k<blockh;k++)
		 for(m=0;m<blockw;m++)
		 {
			  if(*(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)==-1)
			 {
		       *(m_pImgDataOut+(blockw*row+k)*lineByteOut+blockh*line+m)=depth;
		       *(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)=0;
			 }
		  
		 }
         row=T[j].endvex/(m_imgWidth/blockw);
         line=T[j].endvex%(m_imgWidth/blockw);
		 for(k=0;k<blockh;k++)
		 for(m=0;m<blockw;m++)
		 {
			  if(*(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)==-1)
			 {
		       *(m_pImgDataOut+(blockw*row+k)*lineByteOut+blockh*line+m)=depth;
		       *(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)=0;
			 }
		  
		 }
	}

       
	   pixel_num=1;
       hypothesis=0;
	   depth=0;
}  

	if(T!=NULL)
{
	free(T);
	T=NULL;
	} 
	int FilterH=3;
	int FilterW=3;
	int FilterCY=1;
	int FilterCX=1;

	//�Եõ������ͼ���к����Ĵ����˵�һЩ���Բ�����ĵ�
	//��ʱ�����ڴ棬�Ա����˲�������
	unsigned char*value = new unsigned char[FilterH * FilterW];
	unsigned char*	pSrc;
    unsigned char*	pDst;
	for (i = FilterCY; i < m_imgHeight - FilterH ; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW ; j++)//+ FilterCX + 1
		{
			
			pDst = m_pImgDataOut + lineByte * (m_imgHeight - 1 - i) + j;
			for (k = 0; k < FilterH; k++)
			{
				for (int l = 0; l < FilterW; l++)
				{
					pSrc = m_pImgDataOut + lineByte * (m_imgHeight - l - i 
						+ FilterCY - k) + j - FilterCX + l;
					value[k * FilterW + l] = *pSrc;
				}
			}
			if(abs(*pDst-FindMedianValue(value,FilterW * FilterH))>SetValue)
			       *pDst = FindMedianValue(value,FilterW * FilterH);
		}
	}
	
	
	
  }

/**********************************************************************
*
* �������ƣ�
* BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY,unsigned char*	pImpuf	)
*
* ������
* int FilterH     ģ��ĸ߶�        
* int FilterW     ģ��Ŀ��
* int FilterCX    ģ�������Ԫ��X���� ( < FilterW - 1)
* int FilterCY    ģ�������Ԫ��Y���� ( < FilterH - 1)
*
* ����ֵ��
* void
*
* ˵����
* ���ص�˫���˲����㷨
*
**********************************************************************/

void Make3D::BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY,unsigned char*pImbuf)
{
	unsigned char*	pSrc;
	unsigned char*  pDst;
    unsigned char	cenv,otherv;
	float   cof;
    float   dep;
	float  sigmas=50;
	float  sigmar=40 ;
    int		i,j,k,l;

	if(m_pImgDataOut2!= NULL)
	{
		delete []m_pImgDataOut2;
		m_pImgDataOut2 = NULL;
	}
     //�����ڴ棬�Ա�����ͼ��
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut2)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut2 = new unsigned char[lineByteOut * m_imgHeight];
	}
	int pixelByte = m_nBitCountOut / 8;
	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
			*(m_pImgDataOut2 + i * lineByteOut +j) = *(pImbuf + i * lineByteOut + j);
	}

	for (i = FilterCY; i < m_imgHeight - FilterH +FilterCY+1; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW+FilterCX +1; j++)//+ FilterCX + 1
		{
			  dep=0;
			  cof=0;
		      cenv =*(pImbuf+ lineByteOut *  i  + j)  ;
	          pDst=m_pImgDataOut2 + lineByteOut * i + j;
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{
				
                  pSrc = pImbuf+ lineByteOut * ( i + FilterCY - k) + j - FilterCX + l;
				  otherv=*( pImbuf + lineByteOut * ( i + FilterCY - k) + j - FilterCX + l);
		    float e=(float)(abs(k-i)+abs(l-j))*(abs(k-i)+abs(l-j))/(sigmas*sigmas)+(otherv-cenv)*(otherv-cenv)/(sigmar*sigmar);
                  cof=cof+(float)exp(-0.5*e);
                  dep=dep+otherv*(float)exp(-0.5*e);
				}
			}
			*pDst =(unsigned char)(dep/cof);//������float�͸���char�ͣ����ս��Ϊchar�ͣ��˴���̫��ȷ��ȴҲ��Ӱ��
		}
	}
}

/**********************************************************************
*
* �������ƣ�MSTsegment(unsigned char*	pImpuf,int blockw,int blockh)
* 
*
* ������
*      ͼƬ������BUF����С�������ָ�ʱ���������С blockw,blockh
* ����ֵ��
* void
*
* ˵����
* ���صĻ����������С������ͼ��ָ���㷨
*
**********************************************************************/
void Make3D::MSTsegment(unsigned char*	pImpuf,int blockw,int blockh)
{
   	int	i,j,m;
	int SetValue=5;   //������ֵ
	int Grayvalue=0;
	int regeionnum=1;
	int THregion;  //���÷ָ�����򣬸���ͼ��ĳߴ��С���趨���ֵ
	int steplength=2;   //���÷ָ���ֵÿ�θı�Ĳ���
    float d;
	long int k;

	typedef struct
	{
       int fromvex,endvex; //�ߵ������յ�
       int length;         //�ߵ�Ȩֵ
	}edge;
	

	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1 = NULL;
	}

	
	//�����ڴ棬�Ա�����ͼ��

    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut1)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut1 = new unsigned char[lineByteOut * m_imgHeight];
	}

	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth ; j++)
			*(m_pImgDataOut1 + i * lineByteOut +j) = *(pImpuf + i * lineByteOut + j);
	}

 long int Sumblocks=(m_imgWidth/blockw)*( m_imgHeight/blockh); 
 if(Sumblocks>30000)
 {
       AfxMessageBox("��ѡ�����������С");
		return ;
 } 
 unsigned char*grah=new unsigned char[Sumblocks*Sumblocks] ;
 unsigned char*flag;    //����һ��ָ��ı�־λ����ֹ��ָ����������
 flag=grah;

 /*if(grah==NULL)
   {
	   return NULL;
   }*/    //��Ϊ�����Ҷ�������޷����͵ģ��������Ҳ������

	//������������ͼ
  unsigned char  cize=blockw*blockh;
   int  Sum=0;
   for(i =  0; i < m_imgHeight/blockh; i=i++)      
		for(j = 0; j < m_imgWidth/blockw ; j=j++)	
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut + (i*blockh+k) * lineByteOut +j*blockw +m);  //�����ܺ�
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
 /*for(i =  0; i < m_imgHeight; i=i+blockh)      
		for(j = 0; j < m_imgWidth ; j=j+blockw)	
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut1 + (i+k) * lineByteOut +j+m);  //�����ܺ�
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		*/
         //  grah=grah-Sumblocks;  //�ָ�ָ�루ע��һ��������ܳ���


   edge*T=(edge*)malloc(sizeof(edge) *(Sumblocks-1)); //��С������
 /*  if(T==NULL)
   {
	   return NULL;
   }*/
	

//��̬���ٶ�ά���飬��������ߵ���Ϣ������Ȩ�ڽӾ���


unsigned char **dist = (unsigned char **) malloc( sizeof(unsigned char*) *Sumblocks );
for(i = 0; i < Sumblocks; i++)
{
   dist[i] = (unsigned char*)malloc( sizeof(unsigned char) *Sumblocks );
}

      
  //��ʼ���ڽӾ���
	
	for(i =  0; i <Sumblocks ; i++)
   {
		for(j = 0; j < Sumblocks ; j++)
		dist[i][j]	=255;             

	}

//��ͼ���������뵽��Ȩ�ڽӾ���
    
 
	for(i =  0; i <( m_imgHeight/blockh-1); i++)     //���¿��ܻ����Խ��
	{
		for(j = 0; j < (m_imgWidth/blockw-1); j++)    
		{
		k=i*(m_imgWidth/blockw)+j;
        dist[k][k+1]=abs(*(grah +k) -*(grah +k+1));   
        dist[k+1][k]=dist[k][k+1];
		dist[k][k+m_imgWidth/blockw]=abs(*(grah +i*(m_imgWidth/blockw)+j) -*(grah +(i+1)*(m_imgWidth/blockw)+j));
        dist[k+m_imgWidth/blockw][k]=dist[k][k+m_imgWidth/blockw];
		}
	}
      
 /*	flag--;            //��֪��Ϊɶ�������ᱨ��
	if(flag!=NULL)         
{
	delete flag;	
}
   */
if(grah!=NULL)     
{
	delete []grah;
	grah=NULL;
	
}
   //��prime�㷨������С������
   int v=0;  //�������õ�һ�����붥��Ϊ��
   //�����һ������
   for(j=0;j<Sumblocks-1;j++)
   {
	   T[j].fromvex=v;
	   if(j>=v)
	   {
		   T[j].endvex=j+1;
		   T[j].length=dist[v][j+1];

	   }
	   else
	   {
		   T[i].endvex =j;
		   T[i].length=dist[v][j];
	   }
   }
 for(k=0;k<Sumblocks-1;k++)     //���K����
	{
		int min=500;
		for(j=k;j<Sumblocks-1;j++)
		{
			if(T[j].length<min)
			{
			 min=T[j].length;   
		    	m=j;
			}
		}
      
			edge e=T[m];
			T[m]=T[k];
			T[k]=e;
			v=T[k].endvex;   //v������ҵ�����̱���V-U�еĶ���	
		
      for(j=k+1;j<Sumblocks-1;j++)  //�޸����洢����С�߼�(���޸ĳ�ʼ��edge�е�ֵ)
		{
		 d=dist[v][T[j].endvex];
		 if(d<T[j].length)
		 {
				  T[j].length=d;
				  T[j].fromvex=v;
		 }
	}
 }//��С�������������
 if(dist!=NULL)             //�ͷŶ�ά����Ŀռ�
 {
	 for(i = 0; i < Sumblocks; i++)
	 {
        free(dist[i]);
        dist[i]=NULL;

	 }

      free(dist);    
       dist=NULL;
 }

 Make3D::Binary(m_pImgDataOut1,128);                                 //�õ�������������ʹ�������ݶȼ���
 Make3D::longestLineDetectByHough(m_pImgDataOutBinary,1,1);

 if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }


//����ģ��ѡ���������ݶ�
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>5&&abs(template1-template3)>5)         //���Ͻ����ģʽ,���1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>5&&abs(template2-template3)>5)         //���Ͻ����ģʽ�����1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=0.5;
     Wud=-0.5;
 }
  else 
  {
      Wrl=0;
    Wud=-1;

  }
 

//���´����������÷ָ���ֵ
THregion= Sumblocks/12;
regeionnum=THregion+1;
while( regeionnum>THregion)
{   
	SetValue=SetValue+steplength;
    regeionnum=0;
	for(i=0;i<Sumblocks-1;i++)
  {
      if(T[i].length<SetValue)
	     continue;
	 else
	  regeionnum++;
	}

}
// SetValue=20;    
 //���´�������С��������ɶ�ͼ����и�,���Բ�ͬ�������費ͬ�ĻҶ�ֵ(���³������ͼƬ���������4�ı���ʱ��Ч)
 //����ȼ����ʼ��
//SetValue=5;
  //����ȼ����ʼ��
 int *flagM=new int[lineByteOut * m_imgHeight];
  	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth ; j++)
			*(flagM + i * lineByteOut +j) = -1;
	}
float pixel_num=1;
float  hypothesis=0;
int  fixR; 
for(i=0;i<Sumblocks-1;i++)
  {
	int y,x;
	if(blockw==2)
		fixR=10;
	else if(blockw==4)
       fixR=2;
	else
       fixR=0;

       if((T[i].length<SetValue||pixel_num< fixR)&&i<Sumblocks-2)  //<Sumblocks-2�ж��Ƿ������һ�������� ||pixel_num<50
	   {		 
          y=T[i].fromvex/(m_imgWidth/blockw);
          x=T[i].fromvex%(m_imgWidth/blockw);
		 pixel_num=pixel_num+1;
	     hypothesis=hypothesis+Wrl*(x-(m_imgWidth/blockw)/2)/(m_imgWidth/blockw)+Wud*(y-(m_imgHeight/blockh)/2)/((m_imgHeight/blockh));
	     continue;
	 }
    float  depth=128+255*hypothesis/pixel_num ;//�õ�������� 
	for(j=i-pixel_num+1;j<(i+1);j++)   
      {
			 
		 int row=T[j].fromvex/(m_imgWidth/blockw);
         int line=T[j].fromvex%(m_imgWidth/blockw); 
		 int k,m;
		 for(k=0;k<blockh;k++)
		 for(m=0;m<blockw;m++)
		 {
			  if(*(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)==-1)
			 {
		       *(m_pImgDataOut1+(blockw*row+k)*lineByteOut+blockh*line+m)=depth;
		       *(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)=0;
			 }
		  
		 }
         row=T[j].endvex/(m_imgWidth/blockw);
         line=T[j].endvex%(m_imgWidth/blockw);
		 for(k=0;k<blockh;k++)
		 for(m=0;m<blockw;m++)
		 {
			  if(*(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)==-1)
			 {
		       *(m_pImgDataOut1+(blockw*row+k)*lineByteOut+blockh*line+m)=depth;
		       *(flagM+(blockw*row+k)*lineByteOut+blockh*line+m)=0;
			 }
		  
		 }
	}

       
	   pixel_num=1;
       hypothesis=0;
	   depth=0;
}  

	if(T!=NULL)
{
	free(T);
	T=NULL;
} 
	int FilterH=3;
	int FilterW=3;
	int FilterCY=1;
	int FilterCX=1;

	//�Եõ������ͼ���к����Ĵ����˵�һЩ���Բ�����ĵ�
	//��ʱ�����ڴ棬�Ա����˲�������
	unsigned char*value = new unsigned char[FilterH * FilterW];
	unsigned char*	pSrc;
    unsigned char*	pDst;
	for (i = FilterCY; i < m_imgHeight - FilterH ; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW ; j++)//+ FilterCX + 1
		{
			
			pDst = m_pImgDataOut1 + lineByteOut * (m_imgHeight - 1 - i) + j;
			for (k = 0; k < FilterH; k++)
			{
				for (int l = 0; l < FilterW; l++)
				{
					pSrc = m_pImgDataOut1 + lineByteOut * (m_imgHeight - l - i 
						+ FilterCY - k) + j - FilterCX + l;
					value[k * FilterW + l] = *pSrc;
				}
			}
			if(abs(*pDst-FindMedianValue(value,FilterW * FilterH))>SetValue)
			       *pDst = FindMedianValue(value,FilterW * FilterH);
		}
	}		
	
 }                                                                                 

/**********************************************************************
*
* �������ƣ�DIBR()
* 
*
* ������
*      ��
* ����ֵ��
* void
*
* ˵����
* �������ͼ��Ⱦɫ
*
**********************************************************************/
void Make3D::DIBR()
{
	int		d=50; //˫�ۼ�ľ���
	int     f=20; //���ࣨ��λ��mm)
	int i,j;

	Make3D::ColorToGray();
    CBlockDialog *dlg=new CBlockDialog();
	dlg->DoModal();
	int blockw;
	int blockh;
	switch(dlg->m_nIndex1)
	{
	case 0:
		 blockw=2;
	     blockh=2;
	      break;
	case 1:
		blockw=4;
	     blockh=4;
		break;
	case 2:
		blockw=8;
	    blockh=8;
		break;
	default:
		break;
	}
	Make3D::MSTsegment(m_pImgDataOut,blockw,blockh); 
	if(m_pImgDataOut!= NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}                         //�ͷſռ� 
		
	CBilDialog *childdlg=new CBilDialog();
	childdlg->DoModal();
	int modal;
	switch(childdlg->m_nIndex)
	{
	case 0:
		modal=3;break;
	case 1:
		modal=5;break;
	case 2:
		modal=7;break;
	default:
		break;
	}
	int iFilterH=modal;
	int iFilterW=modal;
	int iFilterCX=(modal-1)/2;
	int iFilterCY=(modal-1)/2;
	Make3D::BilateralFilter(iFilterH, iFilterW, iFilterCX, iFilterCY,m_pImgDataOut1); //�õ��Ľ���洢��m_pImgDataOut2
   
	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1= NULL;
	}        //�ͷ�1�ռ�           

   		//���²����ǽ�����������ͼ�洢����
   
     	if(m_pImgDataOutleft!= NULL)
	{
		delete []m_pImgDataOutleft;
		m_pImgDataOutleft = NULL;
	}
	  	if(m_pImgDataOutright!= NULL)
	{
		delete []m_pImgDataOutright;
		m_pImgDataOutright = NULL;
	}
////////////////////////////////
    m_nBitCountOut=24;             //�����Ϊ��ɫͼ
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOutleft)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
		m_pImgDataOutleft = new unsigned char[lineByteOut * m_imgHeight];
	}
	if (!m_pImgDataOutright)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
		m_pImgDataOutright = new unsigned char[lineByteOut * m_imgHeight];
	}

   int	 pixelByte = m_nBitCountOut / 8;

   for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
		{
			*(m_pImgDataOutleft + i * lineByteOut +j) = 0 ;
         
            *(m_pImgDataOutright+ i * lineByteOut +j) =	0;
		}		
   }	 //ͼƬ�ĳ�ʼ�����
    
	int  xl,xr;  //��������ͼ�ֱ����
	for(i =  0; i < m_imgHeight-1; i++)    //-1��Ϊ�˷�ֹ���渳ֵ��ʱ��Խ��
	{
		for(j = 0; j < m_imgWidth-1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//���ﲻ��ͨ��
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
		 // if(z!=0)                                   
           // xl=j+(d*f)/(2*z);	  
		  //else 
			//  continue;
			

			//�����ܱ���ܶ�����
			z=z+1;             
			xl=j+(d*f)/(2*z);
		  if((xl>=0)&&(xl<=m_imgWidth))
		  {
		    for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutleft + i * lineByteOut +xl*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //������		  
              
		  }
		  else 
			  continue;
		}
	}

	
	//������ͼ����

	
		for(i =  0; i < m_imgHeight-1; i++)    //-1��Ϊ�˷�ֹ���渳ֵ��ʱ��Խ��
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//���ﲻ��ͨ��
		
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
  	    // if(z!=0)	 
          //  xr=j-(d*f)/(2*z);
		  //else 
		//	  continue;
			z=z+1;
			xr=j-(d*f)/(2*z);
		  if((xr>=0)&&(xr<=m_imgWidth))
		  {
		  for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutright + i * lineByteOut +xr*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //������
		    
                
		  }
		  else 
			  continue;
		}
	}
}

/**********************************************************************
*
* �������ƣ�Hole filling()
* 
*
* ������
*      ��
* ����ֵ��
* void
*
* ˵����
* �ն��������DIBR����΢����һ����ģ�ͼ��ĳ�ʼ����ΪԭͼƬ�����þ����˲����ģ�
*
**********************************************************************/
void Make3D::Holefilling()
{
	int		d=50; //˫�ۼ�ľ���
	int     f=20; //���ࣨ��λ��mm)
	int i,j,k,l;
	Make3D::ColorToGray();
    CBlockDialog *dlg=new CBlockDialog();
	dlg->DoModal();
	int blockw;
	int blockh;
	switch(dlg->m_nIndex1)
	{
	case 0:
		 blockw=2;
	     blockh=2;
	      break;
	case 1:
		blockw=4;
	     blockh=4;
		break;
	case 2:
		blockw=8;
	    blockh=8;
		break;
	default:
		break;
	}
	
	  Make3D::MSTsegment(m_pImgDataOut,blockw,blockh); //�õ������ͼ�洢��m_pImgDataOut1
   
    if(m_pImgDataOut!= NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}   
              
	CBilDialog *childdlg=new CBilDialog();
	childdlg->DoModal();
	int modal;
	switch(childdlg->m_nIndex)
	{
	case 0:
		modal=3;break;
	case 1:
		modal=5;break;
	case 2:
		modal=7;break;
	default:
		break;
	}
	int iFilterH=modal;
	int iFilterW=modal;
	int iFilterCX=(modal-1)/2;
	int iFilterCY=(modal-1)/2;
	Make3D::BilateralFilter(iFilterH, iFilterW, iFilterCX, iFilterCY,m_pImgDataOut1); //�õ��Ľ���洢��m_pImgDataOut2
   
	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1= NULL;
	}        //�ͷ�1�ռ�           

   		//���²����ǽ�����������ͼ�洢����
   
     	if(m_pImgDataOutleft!= NULL)
	{
		delete []m_pImgDataOutleft;
		m_pImgDataOutleft = NULL;
	}
	  	if(m_pImgDataOutright!= NULL)
	{
		delete []m_pImgDataOutright;
		m_pImgDataOutright = NULL;
	}
////////////////////////////////
    m_nBitCountOut=24;             //�����Ϊ��ɫͼ
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOutleft)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
		m_pImgDataOutleft = new unsigned char[lineByteOut * m_imgHeight];
	}
	if (!m_pImgDataOutright)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
		m_pImgDataOutright = new unsigned char[lineByteOut * m_imgHeight];
	}

   int	 pixelByte = m_nBitCountOut / 8;

   for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
		{
			*(m_pImgDataOutleft + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut +j);//ͼƬ�ĳ�ʼ��
		   //  *(m_pImgDataOutleft + i * lineByteOut +j) =0;       
            *(m_pImgDataOutright+ i * lineByteOut +j) = *(m_pImgData + i * lineByteOut +j);
		}		    		
	}

    unsigned char   *flag=NULL;                //������־λ�����ڼ�¼�ն���
	int  lineByteOutflag=(m_imgWidth  + 3) / 4 * 4;
	if (!flag)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
    	flag = new unsigned char[lineByteOutflag* m_imgHeight];
	}
    for(i =  0; i < m_imgHeight; i++)           //��־λ�ĳ�ʼ��
	{
		for(j = 0; j < m_imgWidth ; j++)
		{
			*(flag + i * lineByteOutflag +j) = 0;
         
		}
	}
    
	int  xl,xr;  //��������ͼ�ֱ����
	for(i =  0; i < m_imgHeight-1; i++)    //-1��Ϊ�˷�ֹ���渳ֵ��ʱ��Խ��
	{
		for(j = 0; j < m_imgWidth-1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//���ﲻ��ͨ��
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
		 // if(z!=0)                                   
           // xl=j+(d*f)/(2*z);	  
		  //else 
			//  continue;
			
			//�����ܱ���ܶ�����
			z=z+1;             
			xl=j+(d*f)/(2*z);
		  if((xl>=0)&&(xl<=m_imgWidth))
		  {
		    for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutleft + i * lineByteOut +xl*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //������		  
               *(flag+ i * lineByteOutflag +xl)=1;
		  }
		  else 
			  continue;
		}
	}
	//������ͼ��ƽ���˲����пն��

	int FilterCY=5,FilterCX=5,FilterH=11,FilterW=11;
	for (i = FilterCY; i < m_imgHeight - FilterH +FilterCY+1; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW+FilterCX +1; j++)//+ FilterCX + 1
		{
		
		int non_hole=0;
		int sum1=0,sum2=0,sum3=0;
		if( *(flag + i * lineByteOutflag +j)==0 )
			
		{     
			 
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{ 
				if(*( flag + lineByteOutflag * ( i + FilterCY - k) + j - FilterCX + l )!=0)
					
				{
				   non_hole++;
                   sum1=sum1+*( m_pImgDataOutleft + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte);
				   sum2=sum2+*( m_pImgDataOutleft + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte+1);
				   sum3=sum3+*( m_pImgDataOutleft + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte+2);
				}
				}
			}
		}
		if(non_hole!=0)
		{
          *(m_pImgDataOutleft + i * lineByteOut +j*pixelByte)=(sum1/non_hole);
		  *(m_pImgDataOutleft + i * lineByteOut +j*pixelByte+1)=(sum2/non_hole);
		  *(m_pImgDataOutleft + i * lineByteOut +j*pixelByte+2)=(sum3/non_hole);

		}
		else
			continue;
				
		}
	}
		
	//������ͼ����
	  for(i =  0; i < m_imgHeight; i++)           //��־λ�ĳ�ʼ��
	{
		for(j = 0; j < m_imgWidth ; j++)
		{
			*(flag + i * lineByteOutflag +j) = 0;
         
		}
	}
		for(i =  0; i < m_imgHeight-1; i++)    //-1��Ϊ�˷�ֹ���渳ֵ��ʱ��Խ��
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//���ﲻ��ͨ��
		
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
  	     //if(z!=0)	 
         //   xr=j-(d*f)/(2*z);
		 // else 
		//	  continue;
			z=z+1;
			xr=j-(d*f)/(2*z);
		  if((xr>=0)&&(xr<=m_imgWidth))
		  {
		  for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutright + i * lineByteOut +xr*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //������
		       *(flag+ i * lineByteOutflag +xr)=1;
                
		  }
		  else 
			  continue;
		}
	}


//����ͼ�ն��
	for (i = FilterCY; i < m_imgHeight - FilterH +FilterCY+1; i++)//+ FilterCY + 1
	{
		for (j = FilterCX; j < m_imgWidth - FilterW+FilterCX +1; j++)//+ FilterCX + 1
		{
		
		int non_hole=0;
		int sum1=0,sum2=0,sum3=0;
		if( *(flag + i * lineByteOutflag +j)==0 )
		{     
			 
			for (k = 0; k < FilterH; k++)
			{
				for (l = 0; l < FilterW; l++)
				{ 
				if(*( flag + lineByteOutflag * ( i + FilterCY - k) + j - FilterCX + l )!=0)
				{
				   non_hole++;
                   sum1=sum1+*( m_pImgDataOutright + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte);
				   sum2=sum2+*( m_pImgDataOutright + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte+1);
				   sum3=sum3+*( m_pImgDataOutright + lineByteOut  * ( i + FilterCY - k) + (j - FilterCX + l)*pixelByte+2);
				  

				}
				}
			}
		}
		if(non_hole!=0)
		{
          *(m_pImgDataOutright + i * lineByteOut +j*pixelByte)=(sum1/non_hole);
		  *(m_pImgDataOutright + i * lineByteOut +j*pixelByte+1)=(sum2/non_hole);
		  *(m_pImgDataOutright + i * lineByteOut +j*pixelByte+2)=(sum3/non_hole);

		}
		else
			continue;		
		}
	}
 }
/**********************************************************************
*
* �������ƣ�Red-Cyan()
* 
*
* ������
*      ��
* ����ֵ��
* void
*
* ˵����
* ����������ͼ�õ�����ͼ
*
**********************************************************************/
void Make3D::RedCyan()
{   int i,j;
	Make3D::Holefilling();
  //  Make3D::DIBR();

  	if(m_pImgDataOutfinal!= NULL)
	{
		delete []m_pImgDataOutfinal;
		m_pImgDataOutfinal = NULL;
	}
     //�����Ϊ��ɫͼ
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;

	if (!m_pImgDataOutfinal)
	{
		//Ϊ����������ͼ�����ڴ�ռ�
		m_pImgDataOutfinal = new unsigned char[lineByteOut * m_imgHeight];
	}
 
      int pixelByte=3;
	for(i =  0; i < m_imgHeight-1; i++)    
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		  
               *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte  ) = *(m_pImgDataOutright + i * lineByteOut + j*pixelByte);  //	Bͨ�� 
               *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte+1) = *(m_pImgDataOutright + i * lineByteOut + j*pixelByte+1); //     Gͨ��
			   *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte+2) = *(m_pImgDataOutleft  + i * lineByteOut + j*pixelByte+2) ; //     Rͨ��
		
		
		
		}
	}
       
}
/***********************************************************************
* �������ƣ�
* HoughTransform()
*
*����������
*  unsigned char *imgBinaryIn  -��ֵͼ����������ָ��
*  int width  -ͼ��� ��
*  int height -ͼ��ĸ�
*  int *houghBuf  -Hough�任����Ҫ�Ļ�����ָ��
*  int  houghWidth  -Hough�任�������Ŀ�
*  int houghHeight  -Hough�任�������ĸ�
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵��������ͼ�����ݣ�ͨ���߼��hough�任���ֱ��
***********************************************************************/
void Make3D::HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    int *max, int *maxR,int *maxA)
{
	//����ѭ������
	int i, j;
//	int threshold=100;   //�ж�Ϊ��Ҫֱ�ߵ��ż�

	//����任�ռ��ۼ�����
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//ѭ������
	int r, a;

	//�м����
	float tempR, tempA; 

	//����ͼ������
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			//����һ��Ŀ���
			if(*(imgBinaryIn+i*width+j)==255){

				//a����Ƕȵ�ѭ���������ڱ任�ռ��ۼ�����Ĵ�ֱ������
				for(a=0;a<houghHeight;a++){

					//���ո����任�Ƕȵķֱ��ʣ���ȡ�Ƕ�
					tempA=(a-houghHeight/2)*angleResolution;

					//���ݵ�ǰ�����ĽǶȼ�x,yֵ��ȡ��Ӧ���뾶
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//�ۼ������ۼ�
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}

	/*

	//���ۼ�����ļ���ֵ������¼��ʱ����������
	int max, maxR, maxA;
	max=*(houghBuf+0*houghWidth+0);
	maxR=0;
	maxA=0;
	for(a=0;a<houghHeight;a++){
		for(r=0;r<houghWidth;r++){
			if(max<=*(houghBuf+a*houghWidth+r)){
				max=*(houghBuf+a*houghWidth+r);
				maxR=r;
				maxA=a;
			}
		}
	}

	//������ֵλ��ת���ɼ�����뾶�ͽǶ�,��ͨ����������
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
	gamma=*angle;         //���Ƕȴ洢����������ѡ���ݶ�ģ��
	rad=*radius;
	*/

	//�Ľ��Ĵ���  ���ڼ�����ֱ��

	for(r=0;r<30;r++)     //��ʼ��
	{
			   
				max[r]=*(houghBuf+r);
				maxR[r]=r;
				maxA[r]=0;
			
	}
	int temp;
	//ð�ݷ�����
	for(i=0;i<30;i++)
	{
		int swap=0;
		for(j=29;j>i;j--)
		{
			if(max[j-1]>max[j])
			{
				temp=max[j-1];
				max[j]=temp;
				max[j-1]=max[j];
				temp=maxR[j-1];
				maxR[j]=temp;
				maxR[j-1]=maxR[j];
				temp=maxA[j-1];
				maxA[j]=temp;
				maxA[j-1]=maxA[j];
				swap=1;


			}
			if(!swap)
				break;
		}
			
	}//�������

	for(a=0;a<houghHeight;a++)
	{
		for(r=0;r<houghWidth;r++)
		{
			if(max[0]<*(houghBuf+a*houghWidth+r))
			{
		    	int flag=-1;
				for(i=0;i<30;i++)
				{
					if(*(houghBuf+a*houghWidth+r)<max[i])
					{
						flag=i;
					}
				}
		    	if(flag==1)
				{
					max[0]=*(houghBuf+a*houghWidth+r);
                    maxA[0]=a;
					maxR[0]=r;

				}
				else if(flag==-1)
				{
					for(j=1;j<30;j++)
					{
						max[j-1]=max[j];
						maxR[j-1]=maxR[j];
						maxA[j-1]=maxA[j];
			                
					 }
						max[29]=*(houghBuf+a*houghWidth+r);
                        maxA[29]=a;
						maxR[29]=r;
				}
				else
				{
                    for(j=1;j<flag;j++)
					{
						max[j-1]=max[j];
						maxR[j-1]=maxR[j];
						maxA[j-1]=maxA[j];
			                
					 }
				     max[flag-1]=*(houghBuf+a*houghWidth+r);
                     maxA[flag-1]=a;
				   	 maxR[flag-1]=r;
				}
				
		
			}
		}
	}
}

/***********************************************************************
* �������ƣ�
* longestLineDetectByHough(float radiusResolution, float angleResolution)
*
*����������
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵��������Hough�任�ļ��뾶�ֱ��ʺͽǶȷֱ��ʣ�ͨ������HoughTransform()
*      ������ͼ��m_pImgData���߼�⣬������ߵĽǶȺͼ�����뾶,����
*     ���������m_pImgDataOut��
***********************************************************************/
void Make3D::longestLineDetectByHough(float radiusResolution, float angleResolution)
{


	int i, j;
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;

	//�ͷŻ�����
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL)
	{
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//ͼ�������Ϊ����ԭ��,����ԭ��ľ������
	//Ϊsqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,����Hough�任
	//�ĸ߶�Ϊ��
	int houghWidth=sqrt((float)(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight));
	 houghWidth /= radiusResolution;

	//�ߵĽǶ���[-90,90]֮��,����������ۼ�����߶�Ϊ181/angleResolution
	int houghHeight=181/angleResolution;

	//�����ۼ����黺����
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough�任���������houghBuf��
	
	int max[30];
	int maxR[30];
	int maxA[30];

	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, max, maxR,maxA);

	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//������õĽǶȺͼ�����뾶,�ڰ�ɫ�����»����ֱ�߲���Ϊ���ͼ����ʾ
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	float radius[30];
	float angle[30];
    for(i=0;i<30;i++)
	{
	   radius[i]=(maxR[i]-houghWidth/2)*radiusResolution;
	   angle[i]=(maxA[i]-houghHeight/2)*angleResolution;
	}
 


       template1=0;     //����Ƕȣ�
	   template2=0;
	   template3=0;


	for(i=0;i<30;i++)
	{
		if(angle[i]>22.5&&angle[i]<67.5)   
			 template2++;                             //������ʵӦ��Ϊ template1++�ģ�����֪Ϊɶ��angle�ĽǶȲ���
		else if(angle[i]<-22.5&&angle[i]>-67.5)
			 template1++;                              //������ʵӦ��Ϊ template2++�ģ�����֪Ϊɶ��angle�ĽǶȲ���
		else
     		template3++;


	}


//���²����ǻ�ͼ

		//angle�ĵ�λ�Ƕ�,�˴�ת��Ϊ���Ƚ��м���
	float alfa[30];
	for(i=0;i<30;i++)
	     alfa[i]=angle[i]*2*3.1415926/360;
     float slope[30];     //���ڼ���б��
	 float yaxis[30];     //������y��Ľ���
	int x, y,k;
	for(k=0;k<30;k++)
	{
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++)
	{
		//ͼ�������λ��Ϊ����ԭ��,(x,y)Ϊֱ������ϵ�еĵ�,
		//����ת��������ԭ����ͼ�����½ǵ�����ϵ(i,j)
		y=radius[k]/sin(alfa[k])-x/tan(alfa[k]);
		slope[k]=-1/tan(alfa[k]);
        yaxis[k]=radius[k]/sin(alfa[k]);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}
	}


	//�ռ��ͷ�
	delete []houghBuf;





}


/***********************************************************************
* �������ƣ�
* longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution)
*
*����������
*  float radiusResolution  -Hough�任�ļ�����뾶�ļ��ֱ���
*  float angleResolution  -Hough�任�ĽǶȼ��ֱ��� 
*  float *radius  -��������Hough�任���������ֱ�ߵļ��뾶
*  float *angle   -��������Hough�任���������ֱ�ߵĽǶ�

*����ֵ��
*   ��
*
*˵�������صĵõ������ߵĺ���
***********************************************************************/

void Make3D::longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution)
{
	int i;
	//ֻ����Ҷ�ͼ��
	if(m_nBitCount!=8)
		return;
/*
	//�ͷŻ�����
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	*/

	//ͼ�������Ϊ����ԭ��,����ԭ��ľ������
	//Ϊsqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,����Hough�任
	//�ĸ߶�Ϊ��
	int houghWidth=sqrt((float)(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight));
	 houghWidth /= radiusResolution;

	//�ߵĽǶ���[-90,90]֮��,����������ۼ�����߶�Ϊ181/angleResolution
	int houghHeight=181/angleResolution;

	//�����ۼ����黺����
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough�任���������houghBuf��
	





   int max[30];
   int maxR[30];
   int maxA[30];
   HoughTransform(imagein, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, max, maxR,maxA);

    float radius[30];
	float angle[30];
    for(i=0;i<30;i++)
	{
	   radius[i]=(maxR[i]-houghWidth/2)*radiusResolution;
	   angle[i]=(maxA[i]-houghHeight/2)*angleResolution;
	}	
       template1=0;     //����Ƕȣ�
	   template2=0;
	   template3=0;

	for(i=0;i<30;i++)
	{
		if(angle[i]>22.5&&angle[i]<67.5)   
			 template2++;                             //������ʵӦ��Ϊ template1++�ģ�����֪Ϊɶ��angle�ĽǶȲ���
		else if(angle[i]<-22.5&&angle[i]>-67.5)
			 template1++;                              //������ʵӦ��Ϊ template2++�ģ�����֪Ϊɶ��angle�ĽǶȲ���
		else
     		template3++;


	}


/*
	//���ͼ��Ĵ�С������ͼ����ͬ
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//������õĽǶȺͼ�����뾶,�ڰ�ɫ�����»����ֱ�߲���Ϊ���ͼ����ʾ
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	//angle�ĵ�λ�Ƕ�,�˴�ת��Ϊ���Ƚ��м���
	float alfa=angle*2*3.1415926/360;

	
	int x, y, i, j;
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++){
		//ͼ�������λ��Ϊ����ԭ��,(x,y)Ϊֱ������ϵ�еĵ�,
		//����ת��������ԭ����ͼ�����½ǵ�����ϵ(i,j)
		y=radius/sin(alfa)-x/tan(alfa);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}
*/
	//�ռ��ͷ�
	delete []houghBuf;
      houghBuf=NULL;
}

/***********************************************************************
* �������ƣ�
* Sobel()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Sobel��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void Make3D::Sobel()
{
	//�ͷ�m_pImgDataOutָ���ͼ�����ݿռ�
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//�ͷ���ɫ��ռ�
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ��������ͼ��Ϊͬһ����
	m_nBitCountOut=m_nBitCount;

	//���ͼ����ɫ����
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//���ͼ��Ŀ��,������ͼ�����
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//ÿ��������ռ�ֽ��������ͼ��������ͼ����ͬ
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//�������ͼ�񻺳���
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//ѭ��������ͼ�������
	int i,j;

	//ÿ����ռ�ֽ��������ͼ��������ͼ����ͬ
	int pixelByte=m_nBitCount/8;

	//ѭ������,�������ص�ÿ��ͨ��,�����ɫͼ����������
	int k;

	//�м����
	int x, y, t;

	//Sobel����
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x�����ݶ�
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + 2 * *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y�����ݶ�
				y= *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 + 2 * *(m_pImgData+(i-1)*lineByte+j*pixelByte+k)
				 + *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+(i+1)*lineByte+j*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k);

				t=sqrt((float)(x*x+y*y))+0.5;
				if(t>255)
					t=255;
				*(m_pImgDataOut+i*lineByte+j*pixelByte+k)=t;
			}
		}
	}

}

/***********************************************************************
* �������ƣ�
* Template()
*
*����������
*  ��
*
*����ֵ��
*   ��
*
*˵����Sobel��Ե��⣬������ͼ��������ͨ�����ݵĺϳɣ��ڲ�ͬͨ����
*      ����˱�Ե��⣬��˿�ͬʱ�����ڻҶȺͲ�ɫͼ��
***********************************************************************/
void Make3D::Template()
{


	int i,j;		
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;	
	int lineByteOut=lineByte;
	if(m_lpColorTableOut!=NULL)
	{
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//���ͼ���ÿ����λ������ɫ����
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//���ͼ����ɫ��������ͼ����ͬ
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

    
	//Ԥ����
 //Make3D::Binary(m_pImgData,128);  
 
 Make3D::longestLineDetectByHough(m_pImgData,1,1);
 if (!m_pImgDataOut)
	{
		//Ϊ������ͼ������ڴ�ռ�
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}

/* if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }
 */


   //�õ�������������ʹ�������ݶȼ���

 //����ģ��ѡ������ݶȵ�ģʽ��������������ѡ��
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>8&&abs(template1-template3)>8)         //���Ͻ����ģʽ,���1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>8&&abs(template2-template3)>8)         //���Ͻ����ģʽ�����1,2����ģ��ͳ�ƵĲ���Ǻܴ�Ļ�����ѡ������ģ��
 {

	 Wrl=0.5;
     Wud=-0.5;
 }
  else 
  {
      Wrl=0;
    Wud=-1;

  }


	for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth ; j++)
		{
			float hypothesis=Wrl*(j-m_imgWidth/2)/m_imgWidth+Wud*(i-m_imgHeight/2)/m_imgHeight;
            float depth=128+255*hypothesis;
			*(m_pImgDataOut + i * lineByteOut +j) = (unsigned char)depth;
		}
	}



}

/**********************************************************************
*
* �������ƣ�
* FindMedianValue(unsigned char* pbArray,int ArrayLen)
*
* ������
* unsigned char* pbArray	ָ��ģ�������׵�ַ��ָ��
* int ArrayLen             ģ������ĳ���
*
* ����ֵ��
* unsigned char
*
* ˵����
* /��ֵ�˲��в���ģ����������ֵ���㷨_����ð������
*
**********************************************************************/

unsigned char Make3D::FindMedianValue(unsigned char* pbArray,int ArrayLen)
{
	int i,j;
	unsigned char Temp;
	
	for(j =0; j<ArrayLen; j++)
	{
		for(i =0; i <ArrayLen-j-1; i++)
		{
			if(pbArray[i]>pbArray[i+1])
			{
				Temp=pbArray[i];
				pbArray[i]=pbArray[i+1];
				pbArray[i+1]=Temp;
			}
			
		}
	}
	if ((ArrayLen&1)>0)
	{
		Temp=pbArray[(ArrayLen+1)/2];
	}
	else
	{
		Temp=(pbArray[ArrayLen/2]+pbArray[ArrayLen/2+1])/2;
	}
	
	return Temp;
}
