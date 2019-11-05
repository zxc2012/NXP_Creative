#ifndef _FIND_PATH_H_
#define _FIND_PATH_H_

#include "common.h"

typedef struct{
    int x;
    int y;
    uint8 near_flag;
}xy_t;

typedef struct 
{
 //   char    TargetNumber;
    char    FoundNumber;
    uint8 IsFound;  
    xy_t    pos;
}BackCamDate_t;

typedef struct{
    int8 x;
    int8 y;
}Dir_XY;

typedef struct{
    int same_n;
    xy_t move_from[8];
    xy_t move_to[8];
    int move_distance;
    char order[20];
    int order_n;
}Solution_E;

typedef struct{
    Dir_XY  dir;
    int start_point;
    int stop_point;
    int turn_point;
    
    char    H_B_Flag;
    char    CameraId;
    char    TargetNumber;
    
    char    FoundNumber;
    uint8 IsFound;
    uint8 cnt;
    xy_t    num_pos;
    xy_t    pos;
    char     order[30];
    int     order_n;
    float     head_angle;
    
}Positiont_t;

typedef struct 
{
    uint16 st;
    char head;
    char number;
    short x;
    short y;
    float angle;
    short reserve;
    uint16 sp;	
}_usartInfo;

typedef union
{
    _usartInfo strutInfo;
    char buf[sizeof(_usartInfo)];
}_usartSendPackage;




void FullPermutation(xy_t *source, int begin, int end);
void ConversionToXY(int* input_n);
void ConversionToXYone(int input_n,xy_t *s);
void Calculate_All(void);
void Find_path(int* input,xy_t start_site);
void TransferToXY_One(int source,xy_t *xy);


extern BackCamDate_t  BackCamDate;
extern Dir_XY       walk_dir;
extern Positiont_t  Walk_Ctrl;
extern int optimal_n;
extern int S_Dis ;
extern int S_Num ;
extern xy_t now_position;
extern Solution_E Solution[92];
extern uint8 turn_point ;
extern int stop_point,start_point;
extern int input_q[8];
void bubble_sort(int* a,int n);
void Option_All(Solution_E* s_site,xy_t start_site);
int CalculateWay(int StartNum,int DestNum,int *mount,char *pos,Dir_XY *dir);
void CalculatePath(uint8 num);
extern void TransferToNum(xy_t source,int* num);

void Option_All_StorageQueen(Solution_E* s_site,xy_t start_site);

#endif