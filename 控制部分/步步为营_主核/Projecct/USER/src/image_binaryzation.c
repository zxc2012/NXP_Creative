#include "headfile.h"

uint8 img_threshold;

void img_binary();                                                           //图像二值化处理函数


/*******************全局动态阈值法：大津法*************************************************/
#define GrayScale 256   //frame灰度级

typedef unsigned char uchar;
int pixel[256]={0};  //这个标志位不能定义在函数内部，因为是一个大数组，百度 大数组，局部变量，会出bug。 你注意下，我当时这个问题困扰了三天。
 
/*********大津阈值分割法（优化）********************/
//参数解释：宽 高 图像指针 起始行  起始列 处理行大小  处理列大小
int MyOSTU(int width,int height,uint8 *Image)
{
	int threshold1=0;
	int32_t sum_gray=0;
	int32_t sum_pix_num=0;
	int32_t pl_pix_num=0;
	int32_t p2_pix_mum=0;
	int32_t p1_sum_gray=0;
	float m1=0;
	float m2=0;
	float V=0;
	float variance=0;
	int i,j,k=0;
	
	for(i = 0;i<256;i++)
		pixel[i] = 0;
	
	//统计每个灰度级中像素的个数   
    for(i = 0; i < height; i++)  
    {  
        for(j = 0;j < width;j++)  
        {  
            pixel[(int)Image[i*height+j]]++;  
        }  
    } 

		for(k=0;k<GrayScale;k++)
		{
			sum_gray+=k*pixel[k];//灰度直方图质量矩
			sum_pix_num+=pixel[k];//总像素个数
		}	
		
		for(k=0;k<GrayScale-1;k++)
		{
			pl_pix_num+=pixel[k];//第一部分像素个数
			p2_pix_mum=sum_pix_num-pl_pix_num;//第二部分像素个数
			p1_sum_gray+=k*pixel[k];   //第一部分质量矩
			m1=(float)p1_sum_gray/pl_pix_num;//第一部分灰度均值
			m2=(float)(sum_gray-p1_sum_gray)/p2_pix_mum;//第二部分灰度均值
			
			V=pl_pix_num*p2_pix_mum*(m1-m2)*(m1-m2);
			
			if(V>variance)//将类间方差较大时的灰度值作为阈值
			{
				variance=V;
				threshold1=k;
			}
		}
		return threshold1;
}


/*******************图像二值化处理函数*************************************************/
void img_binary()        //
{
      img_threshold=MyOSTU(COL,ROW,*image);

      for( uint8 ii=HEIGHT_STRAT ; ii<HEIGHT_END ; ii++ )
      {
            for( uint8 jj=0 ; jj< COL ; jj++ )
            {
                  if( image[ii][jj]>img_threshold)
                  {
                        img[ii][jj]=255;
                  }
                  else
                  {
                        img[ii][jj]=0;
                  }
            }
      }
}




