#include "Find_path.h"
#include "All_solution.h"
#include "common.h"


int optimal_n = 0;
xy_t shortestXY[8];
xy_t now_position;
int  shortestLen = 65533;
int index;
Solution_E   Solution[92] = {0};
Positiont_t  Walk_Ctrl = {0};
xy_t Queen_location[8];
int S_Dis = 1000;
int S_Num = 0;
float K_XY =1;//XY比例

uint8 queen_near_flag[8] = {0,1,2,3,4,5,6,7};
//uint8 queen_near_flag[8] = {0,0,0,0,0,0,0,0};
int input_q[8]={1,17,21,25,29,40,42,56};
extern xy_t S_s[23];


void JudgeNearQueen(Solution_E* s_site)
{
    uint8 i,j,k;
  //  uint8 temp_id=0;
    int temp_num;
    for(i=0;i<92;i++)
    {
        if(s_site[i].same_n != optimal_n)
            continue;
        for(j=0;j<(8-s_site[i].same_n);j++)
        {
            for(k=0;k<8;k++)
            {
               TransferToNum(s_site[i].move_from[j],&temp_num);
               if(temp_num == input_q[k])
               {
                    s_site[i].move_from[j].near_flag = queen_near_flag[k];
               }
            }
        }
    }
}


void TransferToNum(xy_t source,int* num)
{
   *num = (source.x+source.y*8+1);
}

void TransferToXY_One(int source,xy_t *xy)
{
        xy->y = (source-1)/8;
        xy->x = (source-1)%8; 
}

void Find_path(int* input,xy_t start_site)
{
    
    ConversionToXY(input);  //输入的数字转化为坐标
    Calculate_All();        //计算出最好的解法
    
  //  JudgeNearQueen(Solution);
    
 //  Option_All_StorageQueen(Solution,start_site);
    Option_All(Solution,start_site);
}

void CalculatePath(uint8 num)
{
    TransferToNum(S_s[num],&Walk_Ctrl.start_point);
    TransferToNum(S_s[num+1],&Walk_Ctrl.stop_point);
//    printf("start %d,stop %d",start,Walk_Ctrl.stop_point);
    Walk_Ctrl.turn_point = CalculateWay(Walk_Ctrl.start_point,Walk_Ctrl.stop_point,&Walk_Ctrl.order_n,Walk_Ctrl.order,&Walk_Ctrl.dir); 
}


int CalculateWay(int StartNum,int DestNum,int *mount,char *pos,Dir_XY *dir)
{
    int d_x,d_y,delt_x,delt_y,i,point = 0;
    char *pp = pos;

//    if(StartNum > 64 || DestNum >64 || StartNum < 1 || DestNum < 1)
//       return 0;
    d_x = (StartNum-1)%8 - (DestNum-1)%8;

    if(d_x < 0)        delt_x = 1;
    else if(d_x > 0)   delt_x = -1;
    else               delt_x = 0;
    d_x = ABS(d_x);
    
    dir->x = delt_x;
    
    d_y = (StartNum - 1)/8 - (DestNum - 1)/8;

    if(d_y < 0)       delt_y = 1;
    else if(d_y > 0)  delt_y = -1;
    else              delt_y = 0;
    d_y = ABS(d_y);

    dir->y = delt_y;
    
    *pp++ = StartNum;
    for(i = 0;i < d_x;i++)
    {
      *pp = *(pp-1)+delt_x;pp++;
    }
    point = *(pp-1);
    for(i = 0;i < d_y;i++)
    {
      *pp = *(pp-1)+delt_y*8;pp++;
    }

    *mount = d_y + d_x + 1;



    return point;
}


xy_t Option_One(xy_t c_site,xy_t* o_site,int o_num,int* shortest)
{
    int i,short_l = 1000,l_buff;
    int s_n;
    xy_t site_buff;

    for(i=0;i<o_num;i++)
    {
        
  //      l_buff = ABS((c_site.x-o_site[i].x))*K_XY +ABS((c_site.y-o_site[i].y));
        l_buff = ABS((c_site.x-o_site[i].x)*(c_site.x-o_site[i].x))*K_XY +ABS((c_site.y-o_site[i].y)*(c_site.y-o_site[i].y));

        if(l_buff<=short_l)  //记录最短距离
        {
            short_l = l_buff;
            s_n = i;
        }
    }
    *shortest += short_l;  //距离累加

    site_buff = o_site[s_n];  //距离最短的点放在最后
    o_site[s_n] = o_site[o_num-1];
    o_site[o_num-1] = site_buff;

    return site_buff;  //返回距离最短的点
}


void Option_All(Solution_E* s_site,xy_t start_site)
{
    int i = 0,j = 0,k=0,ii = 0;
    int s_ds = 0,s_ds_min =1000;
    xy_t S_s_temp[19] = {0,0};
    xy_t S_s_temp_min[19] = {0,0};
    xy_t move_from_temp[9] = {0};
    
    for(i=0;i<92;i++)
    {
        if(s_site[i].same_n != optimal_n)  //只处理有最多相同位置棋子的那种解法
            continue;
        s_ds_min = 1000;
        S_s_temp[0] = start_site;
        
        for(ii=0;ii<8-s_site[i].same_n;ii++)
        {
            move_from_temp[ii] = s_site[i].move_from[ii]; //保存move_from
        }
        
        for(k=0;k<8-s_site[i].same_n;k++)
        {
            for(ii=0;ii<8-s_site[i].same_n;ii++)  //每次循环都恢复一次move_from
            {
                s_site[i].move_from[ii] = move_from_temp[ii];  //恢复move_from
            }
            s_ds = 0;
            S_s_temp[1] = s_site[i].move_from[k];
            
            move_from_temp[8] = s_site[i].move_from[k];                              //move_from最后一个值和第k个值交换
            s_site[i].move_from[k] = s_site[i].move_from[8-s_site[i].same_n-1];      //
            s_site[i].move_from[8-s_site[i].same_n-1] = move_from_temp[8];           //

            
            s_ds = ABS((start_site.x-S_s_temp[1].x)*((start_site.x-S_s_temp[1].x)))   //求起始点与move_from点之间的距离
                   +ABS(((start_site.y-S_s_temp[1].y))*((start_site.y-S_s_temp[1].y)));
          //  S_s_temp[1] = Option_One(start_site,s_site[i].move_from,8-s_site[i].same_n,&s_ds);
        
            for(j=0;j<(8-s_site[i].same_n-1);j++)  //求最短路线及其距离
            {
                S_s_temp[j*2+2] = Option_One(S_s_temp[j*2+1],s_site[i].move_to,8-s_site[i].same_n-j,&s_ds);

                
                S_s_temp[j*2+3] = Option_One(S_s_temp[j*2+2],s_site[i].move_from,8-s_site[i].same_n-1-j,&s_ds);

            }
            j--;
            S_s_temp[j*2+4] = Option_One(S_s_temp[j*2+3],s_site[i].move_to,1,&s_ds);         

            if(s_ds<=s_ds_min)                   //s_ds： 距离  S_s_temp：路线
            {
                s_ds_min = s_ds;

                for(j=0;j<19;j++)
                {
                    S_s_temp_min[j] = S_s_temp[j];
                }

            }
        }
        if(s_ds_min<=S_Dis)           //S_Dis：最短距离  S_Num：对应的解法  S_s：对应的路线
        {
            S_Dis = s_ds_min;
            S_Num = i;
            for(j=0;j<19;j++)
            {
                S_s[j] = S_s_temp_min[j];
            }

        }
//        printf("S_Dis %d,%d\n",S_Dis,S_Num);
    }
}



void Option_All_StorageQueen(Solution_E* s_site,xy_t start_site)
{
    int i = 0,j = 0,k=0;
    int s_ds = 0;
    xy_t S_s_temp[19] = {0,0};
    uint8 temp_flag = 0,temp_flag_num = 0;
    for(i=0;i<92;i++)
    {
        if(s_site[i].same_n != optimal_n)
            continue;
        s_ds = 0;
        S_s_temp[0] = start_site;
        S_s_temp[1] = Option_One(start_site,s_site[i].move_from,8-s_site[i].same_n,&s_ds);

        for(j=0;j<(8-s_site[i].same_n-1);j++)
        {
            for(k=0;k<8-s_site[i].same_n-j-1;k++)
            {
                if(S_s_temp[j*2+1].near_flag == s_site[i].move_from[k].near_flag)
                {
                    temp_flag++;
                }
                temp_flag_num = temp_flag;
            }
            if(temp_flag_num != 0)  //有相近的棋子
            {
                for(temp_flag=temp_flag_num;temp_flag>0;temp_flag--)
                {
                    S_s_temp[j*2+2+(temp_flag_num-temp_flag)] = Option_One(S_s_temp[j*2+2+(temp_flag_num-temp_flag)-1],s_site[i].move_from,(8-s_site[i].same_n-j-1)-(temp_flag_num-temp_flag),&s_ds);
                    S_s_temp[j*2+2+(temp_flag_num-temp_flag)].near_flag = 1;
                }
                for(temp_flag=temp_flag_num;temp_flag>0;temp_flag--)
                {
                    S_s_temp[j*2+2+(2*temp_flag_num-temp_flag)] = Option_One(S_s_temp[j*2+2+(2*temp_flag_num-temp_flag)-1],s_site[i].move_to,(8-s_site[i].same_n-j)-(temp_flag_num-temp_flag),&s_ds);
                }
                j+=(temp_flag_num-1);
            }
            else         //如果无相近的的棋子
            {
                S_s_temp[j*2+2] = Option_One(S_s_temp[j*2+1],s_site[i].move_to,8-s_site[i].same_n-j,&s_ds);
                
                S_s_temp[j*2+3] = Option_One(S_s_temp[j*2+2],s_site[i].move_from,8-s_site[i].same_n-1-j,&s_ds);
            }

        }
        j--;
        S_s_temp[j*2+4] = Option_One(S_s_temp[j*2+3],s_site[i].move_to,1,&s_ds);
//        printf("%d：",j*2+3);
//        printf("%d,%d\n",S_s[j*2+3].x,S_s[j*2+3].y);
//        printf("s_ds %d\n",s_ds);
        if(s_ds<=S_Dis)
        {
            S_Dis = s_ds;
            S_Num = i;
            for(j=0;j<19;j++)
            {
                S_s[j] = S_s_temp[j];
            }
            
        }
//        printf("S_Dis %d,%d\n",S_Dis,S_Num);
    }
}



/************************************************************************/
//* 功能：实现两个整形参数值交换
//* 参数：
//*       lhs--int类型的指针，指向待交换数1的地址
//*       rhs--int类型的指针，指向待交换数2的地址
/************************************************************************/
void Swap(xy_t *lhs, xy_t *rhs)
{
    xy_t t = *lhs;

    *lhs = *rhs;
    *rhs = t;
}


void CalculateDis(xy_t* buf,int cnt)
{
    int i,lenbuf = 0;

    for(i = 0;i < cnt;i++)
      lenbuf += (buf[i].x - Solution[index].move_to[i].x) * (buf[i].x - Solution[index].move_to[i].x) + (buf[i].y - Solution[index].move_to[i].y) * (buf[i].y - Solution[index].move_to[i].y);
    if(lenbuf <= shortestLen)
    {
        shortestLen = lenbuf;
        for(i = 0;i<cnt;i++)
        {
            shortestXY[i].x = buf[i].x;
            shortestXY[i].y = buf[i].y;
        }
    }
//    if(lenbuf <= 140)
//        printf("lenbuf:%d \n",lenbuf);
}
/************************************************************************/
//* 功能：实现全排列功能
//* 参数：
//*       source--整数数组，存放需要全排列的元素
//*       begin --查找一个排列的开始位置
//*       end   --查找一个排列的结束位置，当begin=end时，表明完成一个排列
/************************************************************************/
void FullPermutation(xy_t *source, int begin, int end)
{
    int i;

    if (begin >= end) // 找到一个排列
    {
        for (i = 0; i < end; i++)
        {
			CalculateDis(source,end);
        }
//        printf("\n");
    }
    else// 没有找完一个排列，则继续往下找下一个元素
    {
        for (i = begin; i < end; i++)
        {
            if (begin != i)
            {
                Swap(&source[begin], &source[i]); // 交换
            }

            // 递归排列剩余的从begin+1到end的元素
            FullPermutation(source, begin + 1, end);

            if (begin != i)
            {
                Swap(&source[begin], &source[i]); // 回溯时还原
            }
        }
    }
}



void ConversionToXY(int* input_n)
{
    int i;
    for(i=0;i<8;i++)
    {
        Queen_location[i].y = (input_n[i]-1)/8;
        Queen_location[i].x = (input_n[i]-1)%8;
//        printf("Queen_locationx:%d,Queen_locationy:%d \n",Queen_location[i].x,Queen_location[i].y);
    }


}
void ConversionToXYone(int input_n,xy_t *s)
{
        s->y = (input_n-1)/8;
        s->x = (input_n-1)%8;
}

void Calculate_One(xy_t* queen_location,Solution_E* solution,int answer_n)
{
    int i=0,dif_n=0;
    for(i=0;i<8;i++)  //遍历相同个数，记录不同的棋子位置与正解位置
    {
        if(queen_location[i].x==(All_solution[answer_n][i]-1) && queen_location[i].y==i)
            solution[answer_n].same_n++;
        else
        {
            solution[answer_n].move_to[dif_n].x = All_solution[answer_n][i]-1;
            solution[answer_n].move_to[dif_n].y = i;
            solution[answer_n].move_from[dif_n].x = Queen_location[i].x;
            solution[answer_n].move_from[dif_n].y = Queen_location[i].y;
            dif_n++;
        }
    }
}

void TransferToXY(const int *source,xy_t *xy)
{
    int i;
    for(i = 0;i<8;i++)
    {
        xy[i].x = source[i] - 1;
        xy[i].y = i;
    }
}

void CopysStructXY(xy_t* source,xy_t* dest,int num)
{
   int i;
   for(i = 0;i < num;i++){
      dest[i].x = source[i].x;
      dest[i].y = source[i].y;
   }
}

void SortDif_XY(xy_t* source,int* pos,int num)
{
   xy_t tmpXY[8];
   int i,cnt = 0,j = 0;
   for(i=0;i<8;i++)
   {
       if((pos[j] == i) && (j < num)){
         //printf("%d\n",i);
         j++;
         continue;
       }
       tmpXY[cnt].x = source[i].x;
       tmpXY[cnt++].y = source[i].y;
       //printf("%d\n",source[i].x);
   }
   CopysStructXY(tmpXY,source,8-num);
   //while(1);
}



void Calculate_All(void)
{
    int Qsame_pos[8],Ssame_pos[8];
    int i=0,j,k,same_n = 0;//,cnt1 = 0
    for(i = 0;i<92;i++)
    {
        same_n = 0;
        CopysStructXY(Queen_location,&(Solution[i].move_from[0]),8);  //八皇后的位置赋给移动出发点
        TransferToXY(&All_solution[i][0],&(Solution[i].move_to[0]));  //92种正解赋给移动目的点
        for(j=0;j<8;j++)
        {
            for(k=0;k<8;k++)
            {
                if((Solution[i].move_to[j].x == Solution[i].move_from[k].x) && (Solution[i].move_to[j].y == Solution[i].move_from[k].y))  //如果出发点与目的点重合
                {
                    Qsame_pos[same_n]   = k;  //记录相同的棋子位置并累加计数
                    Ssame_pos[same_n++] = j;
                    break;
                }
            }
        }
        Solution[i].same_n = same_n;
        SortDif_XY(Solution[i].move_from,Qsame_pos,same_n);  //过滤正解与棋局相同位置的棋子的位置，保留位置不同的位置

        SortDif_XY(Solution[i].move_to,Ssame_pos,same_n);    //过滤正解与棋局相同位置的棋子的位置，保留位置不同的位置
        if(optimal_n < same_n)   //记录92种解法与棋局中位置相同的最多的个数
            optimal_n = same_n;
//        printf("same_n:%d-----------\n",same_n);
//        for(j = 0;j<8-same_n;j++)
//           printf("x:%d,y:%d\n",Solution[i].move_from[j].x,Solution[i].move_from[j].y);
    }
//    printf("optimal_n:%d\n",optimal_n);



};/* 
2、冒泡排序 
基本思想： 不断比较相邻的两个数，让较大的数不断地往后移。经过一番比较，就选出了最大的数。经过第二轮比较，就选出了次大的数。以此类推。 
那么对于大小为N的数组，需要N-1轮比较。 
平均时间复杂度O（N^2） 
最好情况O（N） 
最坏情况O（N^2） 
空间复杂度O（1） 
*/  
void swap_int(int* a, int* b)  
{  
    int temp;  
    temp = *a;  
    *a = *b;  
    *b = temp;  
}  

void bubble_sort(int* a,int n)  
{  
    //要进行N-1轮比较  
    uint8 is_sorted = 1;  
    for(int i = 0; i < n-1; i++ )//[0,n-2]恰好n-1轮比较  
    {  
        for(int j = 0; j < n-i-1 ; j++)//已经排好序的最后i个不用比较，要比较的数的个数为n-i个，那么需要比较的次数为n-i-1  
        {  
            if(a[j] > a[j+1]){  
                is_sorted = 0;  
                swap_int(&a[j],&a[j+1]);  
            }  
        }  
        if(is_sorted)//如果没有发生交换，说明已经排好序了，提前退出循环,所以最好情况下时间复杂度为O（N）  
            break;  
    }  
}  




