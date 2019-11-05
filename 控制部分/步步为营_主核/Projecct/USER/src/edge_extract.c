#include "headfile.h"


double sin_value[180];
double cos_value[180];
uint8 temp[ROW][COL]={0};
uint8 SL_img[ROW][COL]={0};
int16 tp=0;
uint8 hough_acc[AG_max][diagonal_line]={0};
int16 line_AG[20]={0};
int16 line_DS[20]={0};
int16 left_edge[ROW]={0};
int16 right_edge[ROW]={0};
int16 heng_edge[COL]={0};
int16 midline[ROW]={0};
int center_flag=64;
uint8 midline_flag[ROW]={0};
uint8 left_flag[ROW]={0},right_flag[ROW]={0},heng_flag[COL]={0};
uint8 break_row;
uint8 W_of_road[ROW]={0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
		      0,0,0,0,0,0,0,0,0,8, 10,12,14,15,17,19,21,2,2,2,
		      2,3,3,3,3,3,4,4,4,4, 4,4,5,5,5,5,5,6,6,6,
		      6,7,7,7
		      };

/********************************************
计算三角函数，存于数组，减少计算量
********************************************/
void trigonometric_function_int(void)
{
  for(int i=0; i<180; i++)
  {
    sin_value[i] = sin(i*3.1415926/180);
    cos_value[i] = cos(i*3.1415926/180);
  }
}

void huaxian(int x1,int x2) //x1<x2，把两点之间连接起来
{
  int i,hy;
  int y1=midline[x1],y2=midline[x2];
  for(i=x1;i<x2;i++)
  {
    if((x2-x1)!=0)
    {
      hy=y1+(i-x1)*(y2-y1)/(x2-x1);
      midline[i]=hy;
      midline_flag[i]=1;
    }
  }
}
uint16 row=0;
uint16  line=80;
void edge_extract(void)
{
  img_binary();
  memset(temp,(byte)sWHITE,sizeof(temp));//原图中坐标符合kmax和pmax的值
  
  
             if(img[63][64]==sWHITE)
           {
             uint8 i=0;
             uint8 zflag=0;
             uint8 yflag=128;
             for(i=64;i>=0;i--)
             {
               if((img[63][i]==sWHITE)&&(img[63][i-1]==sBLACK))
               {
                 zflag=i;
                 break;
               }
             }
             for(i=64;i<=127;i++)
             {
               if((img[63][i]==sWHITE)&&(img[63][i+1]==sBLACK))
               {
                 yflag=i;
                 break;
               }
             }
             if((64-zflag)<(yflag-64))
               center_flag=50;
             else 
               center_flag=80;
           }
           else
             center_flag=64;
             
  for(row=ROW-1;row>0;row--)
  {
               /*边线flag清零*/
           left_flag[row]=0;
           right_flag[row]=0;
           midline_flag[row]=0;
           uint8 edge_lost=0;
              for(line=center_flag;line>2;line--)
          {
              if((img[row][line]==sBLACK)&&(img[row][line-2]==sWHITE))          //由line_flag向左寻找跳变
              {
                uint8 i=0;
                uint8 cnt=0;
                for(i=line;i<128;i++)
                {
                  if(img[row][i]==sBLACK)
                  {
                    cnt++;
                  }
                  if((cnt>=5)||(img[row][i]==sWHITE))
                    break;
                }
                if(cnt>=5)
                {
                left_edge[row]=line;
                left_flag[row]=1;
                break;
                }
              }
          }
          for(line=center_flag;line<COL-2;line++)
          {
              if((img[row][line]==sBLACK)&&(img[row][line+2]==sWHITE))          //由line_flag向右寻找跳变
              {
                uint8 i=0;
                uint8 cnt=0;
                for(i=line;i>0;i--)
                {
                  if(img[row][i]==sBLACK)
                  {
                    cnt++;
                  }
                  if((cnt>=5)||(img[row][i]==sWHITE))
                    break;
                }
                if(cnt>=5)
                {
                right_edge[row]=line;
                right_flag[row]=1;
                break;
                }
              }

          }
          
          if(row<=62)
          {
            if((left_flag[row]==1)&&ABS(left_edge[row]-left_edge[row+1])>=5)
           {
             left_flag[row]=0;
           }
            if((right_flag[row]==1)&&ABS(right_edge[row]-right_edge[row+1])>=5)
           {
             right_flag[row]=0;
           }
          }
          
          
          if(left_flag[row]==1&&right_flag[row]==1)                             //如果找到两个边缘
	  {

	      midline[row]= (left_edge[row]+right_edge[row])/2;
              midline_flag[row]=1;
              center_flag=midline[row];
	  }
          else
          {
            if(left_flag[row]+right_flag[row]==0)
            {
              uint8 i=0;
              for(i=row+1;i<ROW;i++)
              {
                if(left_flag[i]+right_flag[i]==0)
                  edge_lost++;
                if(left_flag[i]+right_flag[i]!=0)
                {
                  edge_lost=0;
                  break;
                }
                if(edge_lost>=6)
                  break;
              }
              center_flag=midline[row+1];
            }
            else if(left_flag[row]+right_flag[row]==1)//如果找到一个边缘
            {
              if(left_flag[row]==1)//如果找到一个左边缘
              {
                  midline[row]=left_edge[row]+W_of_road[row]/2;
                  midline_flag[row]=1;
                  center_flag=midline[row];          
		  if(center_flag<0)	center_flag=0;				//防止center_flag超出左右边界
		  else if(center_flag>127)	center_flag=127;

              }
              else if(right_flag[row]==1)//如果找到一个右边缘
              {
                  midline[row]=right_edge[row]-W_of_road[row]/2; 
                  midline_flag[row]=1;
                  center_flag=midline[row];          
		  if(center_flag<0)	center_flag=0;				//防止center_flag超出左右边界
		  else if(center_flag>127)	center_flag=127;

              }
            }
          }
          
          if(center_flag<0)
          center_flag=0;
        else if(center_flag>127)
          center_flag=127;
        
        /*跳出*/
	if(edge_lost>=6)//&&(bai_flag<1800||Start_crossroad>0))//如果这行line_flag太靠近边缘，说明到达图像左右边缘，跳出
	{
	     break_row=row;
	     break;
	}
  }
  for(row=ROW-1;row>break_row;row--)
  {
             if((left_flag[row]+right_flag[row]==0)&&midline_flag[row]==0)                               //如果没找到边缘
	    {
	      uint8 t;
              uint8 cnt=0;
              uint8 xia=0;
              uint8 shang=0;
              for(t=row;t<ROW;t++)
              {
                if(midline_flag[t]==1)
                  cnt++;
                if(cnt>=5)
                {
                  xia=t;
                  break;
                }
              }
              cnt=0;
              for(t=row;t>0;t--)
              {
                if(midline_flag[t]==1)
                  cnt++;
                if(cnt>=5)
                {
                  shang=t;
                  break;
                }
              }
              if(xia!=0&&shang!=0)
                huaxian(shang,xia);
//              else if(xia==0&&shang!=0)
//              {
//                for(t=6;t<=127-shang;t++)
//                {
//                  midline[shang+t]=midline[shang+5]+(t-5)*(midline[shang+5]-midline[shang])/5;
//                }
//              }
//              else if(xia!=0&&shang==0)
//              {
//                for(t=6;t<=127-shang;t++)
//                {
//                  midline[xia-t]=midline[xia-5]+(t-5)*(midline[xia-5]-midline[xia])/5;
//                }
//              }
	    }
            
            if(midline_flag[row]==1)
              temp[row][midline[row]]=sBLACK;
             if(left_flag[row]==1)
               temp[row][left_edge[row]]=sBLACK;
             if(right_flag[row]==1)
               temp[row][right_edge[row]]=sBLACK;
  }
  
  /**提取横线**/
  uint8 line_start=COL;
  for(line=0;line<COL;line++)
  {
    if(img[63][line]==sBLACK)
    {
      line_start=line;
      break;
    }
  }
  for(line=line_start;line<COL;line++)
  {
    for(row=45;row>1;row--)
    {
      if(img[row][line]==sBLACK&&img[row-1][line]==sWHITE)  //向上寻找跳变
      {
        uint8 i=0;
        uint8 wcnt=0;
        uint8 bcnt=0;
        for(i=row;i>=0;i--)
        {
          if(img[i][line-1]==sWHITE)
            wcnt++;
          if(img[i][line-1]==sBLACK)
            break;
        }
        for(i=row;i<ROW;i++)
        {
          if(img[i][line]==sBLACK)
            bcnt++;
          if(img[i][line]==sWHITE)
            break;
        }
        if(wcnt>=3&&bcnt>=5)
        {
          heng_edge[line]=row;
          heng_flag[line]=1;
        }
      }
    }
    
    if((heng_flag[line]==1)&&ABS(heng_edge[line]-heng_edge[line-1])>=5)
          {
            heng_flag[row]=0;
          }
  }
  for(line=line_start;line<COL;line++)
  {
    if(heng_flag[line]=0)
    {
              uint8 t;
              uint8 cnt=0;
              uint8 zuo=0;
              uint8 you=0;
              for(t=line;t>=line_start;t--)
              {
                if(heng_flag[t]==1)
                  cnt++;
                if(cnt>=5)
                {
                  zuo=t;
                  break;
                }
              }
              cnt=0;
              for(t=line;t<COL;t++)
              {
                if(heng_flag[t]==1)
                  cnt++;
                if(cnt>=5)
                {
                  you=t;
                  break;
                }
              }
              if(zuo!=0&&you!=0)
              {
                int i,hy;
                int y1=heng_edge[zuo],y2=heng_edge[you];
                for(i=zuo;i<you;i++)
                {
                  if((you-zuo)!=0)
                  {
                    hy=y1+(i-zuo)*(y2-y1)/(you-zuo);
                    heng_edge[i]=hy;
                    heng_flag[i]=1;
                   }
                }
              }
    }
    if(heng_flag[line]=1)
      temp[heng_edge[line]][line]=sBLACK;
  }
}


/********************************************
霍夫变换提取直线
参考：
https://blog.csdn.net/liujia2100/article/details/6989693
https://blog.csdn.net/autocyz/article/details/42649187
********************************************/
void Hough_Transfom(int Hough_Threshold)
{
  edge_extract();                              //提取边沿
  for(row=ROW-1;row>=0;row--)
  {
    for(line=0;line<COL;line++)
    {
      uint8 k=0;
      if(temp[row][line]==sBLACK)              //对边缘检测后的数据（存在temp中）进行hough变化
      {
        //对θ所有值取值
        for(k=0;k<=AG_max;k++)
        {
          tp = (int)(line*sin_value[k] + row*cos_value[k]);   //hough变换中距离参数
          tp=(int)(tp/2+diagonal_line/2);    //对tp值优化防止为负
          hough_acc[k][tp] += 1;        //对变换域中对应重复出现的点累加
        }
      }
    }
  }
  memset(line_AG,0,sizeof(line_AG));
  memset(line_DS,0,sizeof(line_DS));
  uint8 num=0;
  int16 i=0;
  int16 j=0;
  for(i=0;i<=AG_max;i++)              //提取直线
  {
    for(j=0;j<=diagonal_line;j++)
    {
      if(hough_acc[i][j]>=Hough_Threshold)    //Hough_Threshold阈值
      {
        line_AG[num]=i;
        line_DS[num]=j;
        num++;
      }
    }
  }
  
  uint8 DS=0;
  memset(SL_img,(byte)sWHITE,sizeof(SL_img));    
  for(i=ROW-1;i>=0;i--)    //划线
  {
    for(j=0;j<COL;j++)
    {
      if((temp[i][j]==sBLACK)&&num>0)
      {
        DS=(int)(j*sin_value[line_AG[num-1]] + i*cos_value[num-1]);
        DS=(int)(DS/2+diagonal_line/2);
        if(DS==line_DS[num-1])
        {
          SL_img[i][j]=sBLACK;
          num--;
        }
      }
    }
  }
}