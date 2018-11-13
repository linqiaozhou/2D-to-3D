#include "stdafx.h"
#include "Make3D.h"
//#include "BilDialog.h"  //为什么在这里包含就会报错，而在下面包涵就不报错呢？？？？？？？纳闷！！！！！！
#include "Resource.h"
#include <math.h>
#include <malloc.h>
#include<iostream>
#include<windows.h>             //用MessageBox
#include "BilDialog.h"
#include "BlockDialog.h"


using namespace std;
Make3D::Make3D()
{
	
	m_pImgDataOut=NULL;//输出图像位图数据指针为空
	m_pImgDataOutBinary=NULL;

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	
    
	m_pImgDataOutleft=NULL;//左视图的初始化
	m_pImgDataOutright=NULL;//左视图的初始化
	m_pImgDataOutfinal=NULL;//red-cyan的指针


	m_pImgDataOut1=NULL;//以下几个指针在重载函数中用
    m_pImgDataOut2=NULL;

	//输出图像的宽
	 m_imgWidthOut=0;

	//输出图像的高
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
	m_pImgDataOutfinal=NULL;//red-cyan的指针

	m_lpColorTableOut=NULL;
	m_nColorTableLengthOut=0;
	m_pImgDataOut1=NULL;//以下几个指针在重载函数中用
    m_pImgDataOut2=NULL;
		//输出图像的宽
	 m_imgWidthOut=0;

	//输出图像的高
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
    if(m_pImgDataOutfinal!=NULL)//red-cyan的指针
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
* 函数名称：
* MedianFilter(int FilterH, int FilterW, int FilterCX, int FilterCY)
*
* 参数：
* int FilterH     模板的高度        
* int FilterW     模板的宽度
* int FilterCX    模板的中心元素X坐标 ( < FilterW - 1)
* int FilterCY    模板的中心元素Y坐标 ( < FilterH - 1)
*
* 返回值：
* void
*
* 说明：
* 双边滤波的算法
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
	//计算图像每行的字节数
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	//分配内存，以保存新图像
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//为处理后的图像分配内存空间
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
			*pDst =(unsigned char)(dep/cof);//把两个float型赋给char型，最终结果为char型，此处不太精确，却也不影响
		}
	}

}
/**************************************************************************
*函数名
*    ColorToGray()
*
*参数名
*    无
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，将其求灰值操作，结果存入m_pImgDataOut中。
***************************************************************************/
void Make3D::ColorToGray()//灰值化
{
	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//申请颜色表缓冲区,生成灰度图像的颜色表
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

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;



	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];
	

	//循环变量,图像的坐标
	int i,j;

	//根据灰值化公式为输出图像赋值
	for(i=0;i<m_imgHeight;i++)
	{
		for(j=0;j<m_imgWidth;j++)
			*(m_pImgDataOut+i*lineByteOut+j)=0.11**(m_pImgData+i*lineByteIn+j*pixelByteIn+0)
		        	+0.59**(m_pImgData+i*lineByteIn+j*pixelByteIn+1)
		        	+0.30**(m_pImgData+i*lineByteIn+j*pixelByteIn+2)+0.5;
	}
   
}



/**************************************************************************
*函数名
*    Binary()
*
*参数名
*    int threshold   -阈值
*
*返回值
*    无
*
*说明：该函数将m_pImgData作为输入图像，根据给定的阈值threshold对图像进行二值化，
*      结果存入m_pImgDataOut中。若是彩色图像，则先将图像进行灰值图像后再二值化。
***************************************************************************/
void Make3D::Binary(int threshold)
{
	//对于灰度图像
	if(m_nBitCount==8){
		//释放旧的输出图像数据及颜色表缓冲区
		if(m_pImgDataOut!=NULL){
			delete []m_pImgDataOut;
			m_pImgDataOut=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//输出图像的每像素位数、颜色表长度
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//申请输出图像颜色表缓冲区，并将输入图像颜色表拷贝至输出图像颜色表中
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
	
		
		
		//图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数相等
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//申请输出图像位图数据缓冲区
		m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];

		//循环变量，图像的坐标
		int i,j;

		//二值化
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

		//若是彩色图像,先变成灰度格式，变换后m_pImgDataOut已经申请了内存，
        //并存放了灰度图像数据
		ColorToGray();

		//输出图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数不等
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//循环变量，图像的坐标
		int i,j;

		//二值化
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
*函数名
*    Binary()
*
*参数名
*    int threshold   -阈值
*
*返回值
*    无
*
*说明：重载的二值化函数
***************************************************************************/
void Make3D::Binary(unsigned char *buf,int threshold)
{
	//对于灰度图像
	if(m_nBitCount==8){
		//释放旧的输出图像数据及颜色表缓冲区
		if(m_pImgDataOutBinary!=NULL){
			delete []m_pImgDataOutBinary;
			m_pImgDataOutBinary=NULL;
		}
		if(m_lpColorTableOut!=NULL){
			delete []m_lpColorTableOut;
			m_lpColorTableOut=NULL;
		}
		//输出图像的每像素位数、颜色表长度
		m_nBitCountOut=m_nBitCount;
		m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
		
		//申请输出图像颜色表缓冲区，并将输入图像颜色表拷贝至输出图像颜色表中
		m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
		
	
		
		
		//图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数相等
		int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

		//申请输出图像位图数据缓冲区
		m_pImgDataOutBinary=new unsigned char[lineByte*m_imgHeight];

		//循环变量，图像的坐标
		int i,j;

		//二值化
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

		//若是彩色图像,先变成灰度格式，变换后m_pImgDataOut已经申请了内存，
        //并存放了灰度图像数据
		ColorToGray();

		//输出图像每行像素所占字节数，输入图像与输出图像每行像素所占字节数不等
		int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

		//循环变量，图像的坐标
		int i,j;

		//二值化
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
* 函数名称：MSTsegment(int blockw,int blockh)
* 
*
* 参数：
*      用MST分割时所用的区域大小int blockw,int blockh
* 返回值：
* void
*
* 说明：
* 基于区域的最小生成树图像分割的算法
*
**********************************************************************/
void Make3D::MSTsegment(int blockw,int blockh)
{

   	int	i,j,m;
	int SetValue=5;   //设置阈值
	int Grayvalue=0;
	int regeionnum=1;
	int THregion;  //设置分割的区域   这一步下面做，用blokcsize/20实现
	int steplength=2;   //设置分割阈值每次改变的步长

    float d;
	long int k;

	typedef struct
	{
       int fromvex,endvex; //边的起点和终点
       int length;         //边的权值
	}edge;
	

	if(m_pImgDataOut != NULL)
	{
		delete []m_pImgDataOut;
		m_pImgDataOut = NULL;
	}
	//计算图像每行的字节数
   	int lineByte = (m_imgWidth * m_nBitCount / 8 + 3) / 4 * 4;
	
	//分配内存，以保存新图像
	m_nBitCountOut = m_nBitCount;
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut)
	{
		//为处理后的图像分配内存空间
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
       AfxMessageBox("请选择更大的区域大小");
		return ;
 } 
 unsigned char*grah=new unsigned char[Sumblocks*Sumblocks] ;
 unsigned char*flag;    //设置一个指针的标志位，防止对指针的运算出错
 flag=grah;
 /*if(grah==NULL)
   {
	   return NULL;
   }*/    //因为函数我定义的是无返回型的，所以暂且不用这句

	//构造基于区域的图
  unsigned char  cize=blockw*blockh;
  int  Sum=0;		
 for(i =  0; i < m_imgHeight/blockh; i=i++)      
		for(j = 0; j < m_imgWidth/blockw ; j=j++)	
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut + (i*blockh+k) * lineByteOut +j*blockw +m);  //计算总和
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		/*for(i =  0; i < m_imgHeight; i=i+blockh)      
		for(j = 0; j < m_imgWidth ; j=j+blockw)	  //例如 当m_imgWidth=9时，j==8时应该终止了，但是因为8<9,他还是进入循环了，所以会出错。
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut1 + (i+k) * lineByteOut +j+m);  //计算总和
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		*/
        //  grah=grah-Sumblocks;  //恢复指针
 
		   
		   edge*T=(edge*)malloc(sizeof(edge) *(Sumblocks-1)); //最小生成树
 /*  if(T==NULL)
   {
	   return NULL;
   }*/
	

//动态开辟二维数组，用来储存边的信息（即带权邻接矩阵）


unsigned char **dist = (unsigned char **) malloc( sizeof(unsigned char*) *Sumblocks );
for(i = 0; i < Sumblocks; i++)
{
   dist[i] = (unsigned char*)malloc( sizeof(unsigned char) *Sumblocks );
}
     
  //初始化邻接矩阵
	
	for(i =  0; i <Sumblocks ; i++)
   {
		for(j = 0; j < Sumblocks ; j++)
		dist[i][j]	=255;             

	}

	//将图像数据输入到带权邻接矩阵
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
    
/*	flag--;                //不知道为啥，用这句会报错            
	if(flag!=NULL)         
{
	delete flag;	
}*/
   
if(grah!=NULL)     
{
	delete []grah;
	grah=NULL;
	
}

   //用prime算法构造最小生成树   
   int v=0;  //这里设置第一个输入顶点为零
   //构造第一个顶点
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
 for(k=0;k<Sumblocks-1;k++)     //求第K条边
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
			v=T[k].endvex;   //v存放新找到的最短边在V-U中的顶点	
		
      for(j=k+1;j<Sumblocks-1;j++)  //修改所存储的最小边集(即修改初始化edge中的值)
		{
		 d=dist[v][T[j].endvex];
		 if(d<T[j].length)
		 {
				  T[j].length=d;
				  T[j].fromvex=v;
		 }
	}
 }//最小生成树构造完毕
 
  if(dist!=NULL)             //释放二维数组的空间
 {
	 for(i = 0; i < Sumblocks; i++)
	 {
        free(dist[i]);
        dist[i]=NULL;

	 }

      free(dist);    
       dist=NULL;
 }


//预处理
 Make3D::Binary(m_pImgDataOut,128);                                 //得到主导线来决定使用哪种梯度假设
 Make3D::longestLineDetectByHough(m_pImgDataOutBinary,1,1);

 if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }


 //运用模板选择假设梯度的模式（根据主导线来选择）
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>8&&abs(template1-template3)>8)         //右上角最黑模式,如果1,2两种模板统计的差别不是很大的话，则选第三种模板
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>8&&abs(template2-template3)>8)         //左上角最黑模式，如果1,2两种模板统计的差别不是很大的话，则选第三种模板
 {

	 Wrl=0.5;
     Wud=-0.5;
 }
  else 
  {
      Wrl=0;
    Wud=-1;

  }
    
//以下代码用于设置分割阈
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

  //以下代码用最小生成树完成对图像的切割,并对不同的区域赋予不同的灰度值(以下程序仅对图片长款均满足4的倍数时有效)
 //即深度假设初始化
   // SetValue=5;
  //即深度假设初始化
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
       if((T[i].length<SetValue||pixel_num<fixR)&&i<Sumblocks-2)  //<Sumblocks-2判断是否到了最后一个区域了 ||pixel_num<50
	   {		 
          y=T[i].fromvex/(m_imgWidth/blockw);
          x=T[i].fromvex%(m_imgWidth/blockw);
		 pixel_num=pixel_num+1;
	     hypothesis=hypothesis+Wrl*(x-(m_imgWidth/blockw)/2)/(m_imgWidth/blockw)+Wud*(y-(m_imgHeight/blockh)/2)/((m_imgHeight/blockh));
	     continue;
	 }
    float  depth=128+255*hypothesis/pixel_num ;//得到假设深度 
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

	//对得到的深度图进行后续的处理，滤掉一些明显不合理的点
	//暂时分配内存，以保存滤波器数组
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
* 函数名称：
* BilateralFilter(int FilterH, int FilterW, int FilterCX, int FilterCY,unsigned char*	pImpuf	)
*
* 参数：
* int FilterH     模板的高度        
* int FilterW     模板的宽度
* int FilterCX    模板的中心元素X坐标 ( < FilterW - 1)
* int FilterCY    模板的中心元素Y坐标 ( < FilterH - 1)
*
* 返回值：
* void
*
* 说明：
* 重载的双边滤波的算法
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
     //分配内存，以保存新图像
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut2)
	{
		//为处理后的图像分配内存空间
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
			*pDst =(unsigned char)(dep/cof);//把两个float型赋给char型，最终结果为char型，此处不太精确，却也不影响
		}
	}
}

/**********************************************************************
*
* 函数名称：MSTsegment(unsigned char*	pImpuf,int blockw,int blockh)
* 
*
* 参数：
*      图片缓存区BUF，最小生成树分割时所有区域大小 blockw,blockh
* 返回值：
* void
*
* 说明：
* 重载的基于区域的最小生成树图像分割的算法
*
**********************************************************************/
void Make3D::MSTsegment(unsigned char*	pImpuf,int blockw,int blockh)
{
   	int	i,j,m;
	int SetValue=5;   //设置阈值
	int Grayvalue=0;
	int regeionnum=1;
	int THregion;  //设置分割的区域，根据图像的尺寸大小来设定这个值
	int steplength=2;   //设置分割阈值每次改变的步长
    float d;
	long int k;

	typedef struct
	{
       int fromvex,endvex; //边的起点和终点
       int length;         //边的权值
	}edge;
	

	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1 = NULL;
	}

	
	//分配内存，以保存新图像

    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOut1)
	{
		//为处理后的图像分配内存空间
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
       AfxMessageBox("请选择更大的区域大小");
		return ;
 } 
 unsigned char*grah=new unsigned char[Sumblocks*Sumblocks] ;
 unsigned char*flag;    //设置一个指针的标志位，防止对指针的运算出错
 flag=grah;

 /*if(grah==NULL)
   {
	   return NULL;
   }*/    //因为函数我定义的是无返回型的，所以暂且不用这句

	//构造基于区域的图
  unsigned char  cize=blockw*blockh;
   int  Sum=0;
   for(i =  0; i < m_imgHeight/blockh; i=i++)      
		for(j = 0; j < m_imgWidth/blockw ; j=j++)	
		{ 
			for(k=0;k<blockh;k++)
				for(m=0;m<blockw;m++)
				{
                     Sum=Sum+(int)*(m_pImgDataOut + (i*blockh+k) * lineByteOut +j*blockw +m);  //计算总和
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
                     Sum=Sum+(int)*(m_pImgDataOut1 + (i+k) * lineByteOut +j+m);  //计算总和
				}
			
		    unsigned char  mean=Sum/cize;
             *flag=mean;
			 flag++;
			 Sum=0;
		}
		*/
         //  grah=grah-Sumblocks;  //恢复指针（注意一下这里可能出错）


   edge*T=(edge*)malloc(sizeof(edge) *(Sumblocks-1)); //最小生成树
 /*  if(T==NULL)
   {
	   return NULL;
   }*/
	

//动态开辟二维数组，用来储存边的信息（即带权邻接矩阵）


unsigned char **dist = (unsigned char **) malloc( sizeof(unsigned char*) *Sumblocks );
for(i = 0; i < Sumblocks; i++)
{
   dist[i] = (unsigned char*)malloc( sizeof(unsigned char) *Sumblocks );
}

      
  //初始化邻接矩阵
	
	for(i =  0; i <Sumblocks ; i++)
   {
		for(j = 0; j < Sumblocks ; j++)
		dist[i][j]	=255;             

	}

//将图像数据输入到带权邻接矩阵
    
 
	for(i =  0; i <( m_imgHeight/blockh-1); i++)     //以下可能会出现越界
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
      
 /*	flag--;            //不知道为啥，用这句会报错
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
   //用prime算法构造最小生成树
   int v=0;  //这里设置第一个输入顶点为零
   //构造第一个顶点
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
 for(k=0;k<Sumblocks-1;k++)     //求第K条边
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
			v=T[k].endvex;   //v存放新找到的最短边在V-U中的顶点	
		
      for(j=k+1;j<Sumblocks-1;j++)  //修改所存储的最小边集(即修改初始化edge中的值)
		{
		 d=dist[v][T[j].endvex];
		 if(d<T[j].length)
		 {
				  T[j].length=d;
				  T[j].fromvex=v;
		 }
	}
 }//最小生成树构造完毕
 if(dist!=NULL)             //释放二维数组的空间
 {
	 for(i = 0; i < Sumblocks; i++)
	 {
        free(dist[i]);
        dist[i]=NULL;

	 }

      free(dist);    
       dist=NULL;
 }

 Make3D::Binary(m_pImgDataOut1,128);                                 //得到主导线来决定使用哪种梯度假设
 Make3D::longestLineDetectByHough(m_pImgDataOutBinary,1,1);

 if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }


//运用模板选择假设深度梯度
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>5&&abs(template1-template3)>5)         //右上角最黑模式,如果1,2两种模板统计的差别不是很大的话，则选第三种模板
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>5&&abs(template2-template3)>5)         //左上角最黑模式，如果1,2两种模板统计的差别不是很大的话，则选第三种模板
 {

	 Wrl=0.5;
     Wud=-0.5;
 }
  else 
  {
      Wrl=0;
    Wud=-1;

  }
 

//以下代码用于设置分割阈值
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
 //以下代码用最小生成树完成对图像的切割,并对不同的区域赋予不同的灰度值(以下程序仅对图片长款均满足4的倍数时有效)
 //即深度假设初始化
//SetValue=5;
  //即深度假设初始化
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

       if((T[i].length<SetValue||pixel_num< fixR)&&i<Sumblocks-2)  //<Sumblocks-2判断是否到了最后一个区域了 ||pixel_num<50
	   {		 
          y=T[i].fromvex/(m_imgWidth/blockw);
          x=T[i].fromvex%(m_imgWidth/blockw);
		 pixel_num=pixel_num+1;
	     hypothesis=hypothesis+Wrl*(x-(m_imgWidth/blockw)/2)/(m_imgWidth/blockw)+Wud*(y-(m_imgHeight/blockh)/2)/((m_imgHeight/blockh));
	     continue;
	 }
    float  depth=128+255*hypothesis/pixel_num ;//得到假设深度 
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

	//对得到的深度图进行后续的处理，滤掉一些明显不合理的点
	//暂时分配内存，以保存滤波器数组
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
* 函数名称：DIBR()
* 
*
* 参数：
*      无
* 返回值：
* void
*
* 说明：
* 基于深度图的染色
*
**********************************************************************/
void Make3D::DIBR()
{
	int		d=50; //双眼间的距离
	int     f=20; //焦距（单位是mm)
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
	}                         //释放空间 
		
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
	Make3D::BilateralFilter(iFilterH, iFilterW, iFilterCX, iFilterCY,m_pImgDataOut1); //得到的结果存储在m_pImgDataOut2
   
	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1= NULL;
	}        //释放1空间           

   		//以下步奏是将处理完的深度图存储起来
   
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
    m_nBitCountOut=24;             //输出的为彩色图
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOutleft)
	{
		//为处理后的左视图分配内存空间
		m_pImgDataOutleft = new unsigned char[lineByteOut * m_imgHeight];
	}
	if (!m_pImgDataOutright)
	{
		//为处理后的右视图分配内存空间
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
   }	 //图片的初始化完毕
    
	int  xl,xr;  //对左右视图分别操作
	for(i =  0; i < m_imgHeight-1; i++)    //-1是为了防止下面赋值的时候越界
	{
		for(j = 0; j < m_imgWidth-1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//这里不能通用
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
		 // if(z!=0)                                   
           // xl=j+(d*f)/(2*z);	  
		  //else 
			//  continue;
			

			//这样能避免很多问题
			z=z+1;             
			xl=j+(d*f)/(2*z);
		  if((xl>=0)&&(xl<=m_imgWidth))
		  {
		    for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutleft + i * lineByteOut +xl*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //这句错了		  
              
		  }
		  else 
			  continue;
		}
	}

	
	//对右视图操作

	
		for(i =  0; i < m_imgHeight-1; i++)    //-1是为了防止下面赋值的时候越界
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//这里不能通用
		
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
               *(m_pImgDataOutright + i * lineByteOut +xr*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //这句错了
		    
                
		  }
		  else 
			  continue;
		}
	}
}

/**********************************************************************
*
* 函数名称：Hole filling()
* 
*
* 参数：
*      无
* 返回值：
* void
*
* 说明：
* 空洞的填补（在DIBR上稍微做了一点更改，图像的初始化变为原图片，再用均匀滤波更改）
*
**********************************************************************/
void Make3D::Holefilling()
{
	int		d=50; //双眼间的距离
	int     f=20; //焦距（单位是mm)
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
	
	  Make3D::MSTsegment(m_pImgDataOut,blockw,blockh); //得到的深度图存储在m_pImgDataOut1
   
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
	Make3D::BilateralFilter(iFilterH, iFilterW, iFilterCX, iFilterCY,m_pImgDataOut1); //得到的结果存储在m_pImgDataOut2
   
	if(m_pImgDataOut1!= NULL)
	{
		delete []m_pImgDataOut1;
		m_pImgDataOut1= NULL;
	}        //释放1空间           

   		//以下步奏是将处理完的深度图存储起来
   
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
    m_nBitCountOut=24;             //输出的为彩色图
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;
	if (!m_pImgDataOutleft)
	{
		//为处理后的左视图分配内存空间
		m_pImgDataOutleft = new unsigned char[lineByteOut * m_imgHeight];
	}
	if (!m_pImgDataOutright)
	{
		//为处理后的右视图分配内存空间
		m_pImgDataOutright = new unsigned char[lineByteOut * m_imgHeight];
	}

   int	 pixelByte = m_nBitCountOut / 8;

   for(i =  0; i < m_imgHeight; i++)
	{
		for(j = 0; j < m_imgWidth * pixelByte; j++)
		{
			*(m_pImgDataOutleft + i * lineByteOut +j) = *(m_pImgData + i * lineByteOut +j);//图片的初始化
		   //  *(m_pImgDataOutleft + i * lineByteOut +j) =0;       
            *(m_pImgDataOutright+ i * lineByteOut +j) = *(m_pImgData + i * lineByteOut +j);
		}		    		
	}

    unsigned char   *flag=NULL;                //设立标志位，用于记录空洞点
	int  lineByteOutflag=(m_imgWidth  + 3) / 4 * 4;
	if (!flag)
	{
		//为处理后的右视图分配内存空间
    	flag = new unsigned char[lineByteOutflag* m_imgHeight];
	}
    for(i =  0; i < m_imgHeight; i++)           //标志位的初始化
	{
		for(j = 0; j < m_imgWidth ; j++)
		{
			*(flag + i * lineByteOutflag +j) = 0;
         
		}
	}
    
	int  xl,xr;  //对左右视图分别操作
	for(i =  0; i < m_imgHeight-1; i++)    //-1是为了防止下面赋值的时候越界
	{
		for(j = 0; j < m_imgWidth-1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//这里不能通用
			//unsigned char  z=*(m_pImgDataOut2+ i *m_imgWidth  + j);
		 // if(z!=0)                                   
           // xl=j+(d*f)/(2*z);	  
		  //else 
			//  continue;
			
			//这样能避免很多问题
			z=z+1;             
			xl=j+(d*f)/(2*z);
		  if((xl>=0)&&(xl<=m_imgWidth))
		  {
		    for(int k=0;k<pixelByte;k++)
               *(m_pImgDataOutleft + i * lineByteOut +xl*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //这句错了		  
               *(flag+ i * lineByteOutflag +xl)=1;
		  }
		  else 
			  continue;
		}
	}
	//对左视图用平均滤波进行空洞填补

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
		
	//对右视图操作
	  for(i =  0; i < m_imgHeight; i++)           //标志位的初始化
	{
		for(j = 0; j < m_imgWidth ; j++)
		{
			*(flag + i * lineByteOutflag +j) = 0;
         
		}
	}
		for(i =  0; i < m_imgHeight-1; i++)    //-1是为了防止下面赋值的时候越界
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		    unsigned char  z=*(m_pImgDataOut2+ i * (lineByteOut/3) + j);//这里不能通用
		
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
               *(m_pImgDataOutright + i * lineByteOut +xr*pixelByte+k) = *(m_pImgData + i * lineByteOut + j*pixelByte+k);   //这句错了
		       *(flag+ i * lineByteOutflag +xr)=1;
                
		  }
		  else 
			  continue;
		}
	}


//右视图空洞填补
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
* 函数名称：Red-Cyan()
* 
*
* 参数：
*      无
* 返回值：
* void
*
* 说明：
* 根据左右视图得到红蓝图
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
     //输出的为彩色图
    int lineByteOut = (m_imgWidth * m_nBitCountOut / 8 + 3) / 4 * 4;

	if (!m_pImgDataOutfinal)
	{
		//为处理后的右视图分配内存空间
		m_pImgDataOutfinal = new unsigned char[lineByteOut * m_imgHeight];
	}
 
      int pixelByte=3;
	for(i =  0; i < m_imgHeight-1; i++)    
	{
		for(j = 0; j < m_imgWidth -1; j++)
		{
         
		  
               *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte  ) = *(m_pImgDataOutright + i * lineByteOut + j*pixelByte);  //	B通道 
               *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte+1) = *(m_pImgDataOutright + i * lineByteOut + j*pixelByte+1); //     G通道
			   *(m_pImgDataOutfinal + i * lineByteOut +j*pixelByte+2) = *(m_pImgDataOutleft  + i * lineByteOut + j*pixelByte+2) ; //     R通道
		
		
		
		}
	}
       
}
/***********************************************************************
* 函数名称：
* HoughTransform()
*
*函数参数：
*  unsigned char *imgBinaryIn  -二值图像输入数据指针
*  int width  -图像的 宽
*  int height -图像的高
*  int *houghBuf  -Hough变换所需要的缓冲区指针
*  int  houghWidth  -Hough变换缓冲区的宽
*  int houghHeight  -Hough变换缓冲区的高
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：给定图像数据，通过线检测hough变换检测直线
***********************************************************************/
void Make3D::HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    int *max, int *maxR,int *maxA)
{
	//申请循环变量
	int i, j;
//	int threshold=100;   //判断为主要直线的门槛

	//清理变换空间累加数组
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//循环变量
	int r, a;

	//中间变量
	float tempR, tempA; 

	//遍历图像数据
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			//出现一个目标点
			if(*(imgBinaryIn+i*width+j)==255){

				//a代表角度的循环变量，在变换空间累加数组的垂直方向上
				for(a=0;a<houghHeight;a++){

					//按照给定变换角度的分辨率，求取角度
					tempA=(a-houghHeight/2)*angleResolution;

					//根据当前遍历的角度及x,y值求取对应极半径
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//累加数组累加
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}

	/*

	//求累加数组的极大值，并记录此时的数组坐标
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

	//将极大值位置转换成极坐标半径和角度,并通过参数返回
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
	gamma=*angle;         //将角度存储起来，用于选择梯度模型
	rad=*radius;
	*/

	//改进的代码  用于检测多条直线

	for(r=0;r<30;r++)     //初始化
	{
			   
				max[r]=*(houghBuf+r);
				maxR[r]=r;
				maxA[r]=0;
			
	}
	int temp;
	//冒泡法排序
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
			
	}//排序完毕

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
* 函数名称：
* longestLineDetectByHough(float radiusResolution, float angleResolution)
*
*函数参数：
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：给定Hough变换的极半径分辨率和角度分辨率，通过调用HoughTransform()
*      对输入图像m_pImgData做线检测，根据最长线的角度和极坐标半径,将最
*     长线输出到m_pImgDataOut中
***********************************************************************/
void Make3D::longestLineDetectByHough(float radiusResolution, float angleResolution)
{


	int i, j;
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;

	//释放缓冲区
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

	//输出图像的每像素位数及颜色表长度
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

	//图像的中心为坐标原点,线与原点的距离最大
	//为sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,所以Hough变换
	//的高度为：
	int houghWidth=sqrt((float)(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight));
	 houghWidth /= radiusResolution;

	//线的角度在[-90,90]之间,所以申请的累加数组高度为181/angleResolution
	int houghHeight=181/angleResolution;

	//申请累加数组缓冲区
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough变换，结果存入houghBuf中
	
	int max[30];
	int maxR[30];
	int maxA[30];

	HoughTransform(m_pImgData, m_imgWidth, m_imgHeight, houghBuf,  houghWidth,
	        	houghHeight,radiusResolution, angleResolution, max, maxR,maxA);

	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//根据求得的角度和极坐标半径,在白色背景下画出最长直线并作为输出图像显示
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	float radius[30];
	float angle[30];
    for(i=0;i<30;i++)
	{
	   radius[i]=(maxR[i]-houghWidth/2)*radiusResolution;
	   angle[i]=(maxA[i]-houghHeight/2)*angleResolution;
	}
 


       template1=0;     //保存角度，
	   template2=0;
	   template3=0;


	for(i=0;i<30;i++)
	{
		if(angle[i]>22.5&&angle[i]<67.5)   
			 template2++;                             //这里其实应该为 template1++的，但不知为啥，angle的角度不对
		else if(angle[i]<-22.5&&angle[i]>-67.5)
			 template1++;                              //这里其实应该为 template2++的，但不知为啥，angle的角度不对
		else
     		template3++;


	}


//以下步骤是画图

		//angle的单位是度,此处转换为弧度进行计算
	float alfa[30];
	for(i=0;i<30;i++)
	     alfa[i]=angle[i]*2*3.1415926/360;
     float slope[30];     //用于记载斜率
	 float yaxis[30];     //记载与y轴的交点
	int x, y,k;
	for(k=0;k<30;k++)
	{
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++)
	{
		//图像的中心位置为坐标原点,(x,y)为直角坐标系中的点,
		//将其转换至坐标原点在图像左下角的坐标系(i,j)
		y=radius[k]/sin(alfa[k])-x/tan(alfa[k]);
		slope[k]=-1/tan(alfa[k]);
        yaxis[k]=radius[k]/sin(alfa[k]);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}
	}


	//空间释放
	delete []houghBuf;





}


/***********************************************************************
* 函数名称：
* longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution)
*
*函数参数：
*  float radiusResolution  -Hough变换的极坐标半径的检测分辨率
*  float angleResolution  -Hough变换的角度检测分辨率 
*  float *radius  -用来返回Hough变换检测出来的最长直线的极半径
*  float *angle   -用来返回Hough变换检测出来的最长直线的角度

*返回值：
*   无
*
*说明：重载的得到主导线的函数
***********************************************************************/

void Make3D::longestLineDetectByHough(unsigned char *imagein,float radiusResolution, float angleResolution)
{
	int i;
	//只处理灰度图像
	if(m_nBitCount!=8)
		return;
/*
	//释放缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
	

	//输出图像的每像素位数及颜色表长度
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	*/

	//图像的中心为坐标原点,线与原点的距离最大
	//为sqrt(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight)/2,所以Hough变换
	//的高度为：
	int houghWidth=sqrt((float)(m_imgWidth*m_imgWidth+m_imgHeight*m_imgHeight));
	 houghWidth /= radiusResolution;

	//线的角度在[-90,90]之间,所以申请的累加数组高度为181/angleResolution
	int houghHeight=181/angleResolution;

	//申请累加数组缓冲区
	int *houghBuf=new int[houghWidth*houghHeight];

	//Hough变换，结果存入houghBuf中
	





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
       template1=0;     //保存角度，
	   template2=0;
	   template3=0;

	for(i=0;i<30;i++)
	{
		if(angle[i]>22.5&&angle[i]<67.5)   
			 template2++;                             //这里其实应该为 template1++的，但不知为啥，angle的角度不对
		else if(angle[i]<-22.5&&angle[i]>-67.5)
			 template1++;                              //这里其实应该为 template2++的，但不知为啥，angle的角度不对
		else
     		template3++;


	}


/*
	//输出图像的大小与输入图像相同
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;
	int lineByteOut=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeightOut];

	//根据求得的角度和极坐标半径,在白色背景下画出最长直线并作为输出图像显示
	memset(m_pImgDataOut,255,lineByteOut*m_imgHeightOut);

	//angle的单位是度,此处转换为弧度进行计算
	float alfa=angle*2*3.1415926/360;

	
	int x, y, i, j;
	for( x=-m_imgWidthOut/2;x<m_imgWidthOut/2;x++){
		//图像的中心位置为坐标原点,(x,y)为直角坐标系中的点,
		//将其转换至坐标原点在图像左下角的坐标系(i,j)
		y=radius/sin(alfa)-x/tan(alfa);
		j=x-m_imgWidthOut/2;
		i=y+m_imgHeightOut/2;
		if(i>0&&i<m_imgHeightOut)
	    	*(m_pImgDataOut+i*m_imgWidthOut+j)=0;
	}
*/
	//空间释放
	delete []houghBuf;
      houghBuf=NULL;
}

/***********************************************************************
* 函数名称：
* Sobel()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Sobel边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
***********************************************************************/
void Make3D::Sobel()
{
	//释放m_pImgDataOut指向的图像数据空间
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}
	//释放颜色表空间
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//输出图像与输入图像为同一类型
	m_nBitCountOut=m_nBitCount;

	//输出图像颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表，与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//每行像素所占字节数，输出图像与输入图像相同
	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;

	//申请输出图像缓冲区
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeight];


	//循环变量，图像的坐标
	int i,j;

	//每像素占字节数，输出图像与输入图像相同
	int pixelByte=m_nBitCount/8;

	//循环变量,遍历像素的每个通道,比如彩色图像三个分量
	int k;

	//中间变量
	int x, y, t;

	//Sobel算子
	for(i=1;i<m_imgHeight-1;i++){
		for(j=1;j<m_imgWidth-1;j++){
			for(k=0;k<pixelByte;k++){
				//x方向梯度
				x= *(m_pImgData+(i-1)*lineByte+(j+1)*pixelByte+k)
				 + 2 * *(m_pImgData+i*lineByte+(j+1)*pixelByte+k)
				 + *(m_pImgData+(i+1)*lineByte+(j+1)*pixelByte+k)
				 - *(m_pImgData+(i-1)*lineByte+(j-1)*pixelByte+k)
				 - 2 * *(m_pImgData+i*lineByte+(j-1)*pixelByte+k)
				 - *(m_pImgData+(i+1)*lineByte+(j-1)*pixelByte+k);

				//y方向梯度
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
* 函数名称：
* Template()
*
*函数参数：
*  无
*
*返回值：
*   无
*
*说明：Sobel边缘检测，函数将图像看作若干通道数据的合成，在不同通道上
*      完成了边缘检测，因此可同时适用于灰度和彩色图像
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

	//输出图像的每像素位数及颜色表长度
	m_nBitCountOut=m_nBitCount;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//输出图像颜色表与输入图像相同
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		memcpy(m_lpColorTableOut,m_lpColorTable,
			sizeof(RGBQUAD)*m_nColorTableLengthOut);
	}
	

    
	//预处理
 //Make3D::Binary(m_pImgData,128);  
 
 Make3D::longestLineDetectByHough(m_pImgData,1,1);
 if (!m_pImgDataOut)
	{
		//为处理后的图像分配内存空间
		m_pImgDataOut = new unsigned char[lineByteOut * m_imgHeight];
	}

/* if(m_pImgDataOutBinary!=NULL)
 {
	 delete[]m_pImgDataOutBinary;
	 m_pImgDataOutBinary=NULL;
 }
 */


   //得到主导线来决定使用哪种梯度假设

 //运用模板选择假设梯度的模式（根据主导线来选择）
 float Wrl,Wud;
 if( template1 >template2&& template1>template3&&abs(template1-template2)>8&&abs(template1-template3)>8)         //右上角最黑模式,如果1,2两种模板统计的差别不是很大的话，则选第三种模板
 {

	 Wrl=-0.5;
     Wud=-0.5;
 }
  else if(template2>template1&& template2>template3&&abs(template1-template2)>8&&abs(template2-template3)>8)         //左上角最黑模式，如果1,2两种模板统计的差别不是很大的话，则选第三种模板
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
* 函数名称：
* FindMedianValue(unsigned char* pbArray,int ArrayLen)
*
* 参数：
* unsigned char* pbArray	指向模板数组首地址的指针
* int ArrayLen             模板数组的长度
*
* 返回值：
* unsigned char
*
* 说明：
* /中值滤波中查找模版中像素中值的算法_利用冒泡排序
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
