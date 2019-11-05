#include "headfile.h"

void        menu();
void        menu_deal(menu_t *menu);
void        menu_display(menu_t *menu,uint8 new_tab);
void        Send_Img(void);
void        Run(void);
void	    Speed_Select(void);

uint8 MENU_flag=1;
uint8 menu__flag=1;
KEY_MSG_t keymsg;
uint8 speed_select;

/*****************主菜单函数**************/
void menu()
{
    LCD_CLS();
    menu_t menu[3];
    menu[0].strbuff="Run";
    menu[1].strbuff="Send Img";
    menu[2].strbuff="Speed_Select";


    menu[0].pfunc=Run;
    menu[1].pfunc=Send_Img;
    menu[2].pfunc=Speed_Select;
    menu_display(menu,0);
    menu_deal(menu);
}

/*****************菜单选择****************/
void menu_deal(menu_t *menu)
{
    uint8 new_tab=0;
    while(menu__flag==1&&MENU_flag==1)
   {
     while(get_key_msg(&keymsg) == 1)
    {
        if(keymsg.status == KEY_DOWN)
        {

            switch(keymsg.key)
            {
            case KEY_D:
                {
                    if(new_tab < max_tab-1)
                    {
                        new_tab++;

                    }
                    else
                    {
                        new_tab=0;
                    }
                    menu_display(menu,new_tab);
                    break;
                }
//            case KEY_U:
//                {
//                    if(new_tab == 0)
//                    {
//                        new_tab = max_tab - 1;
//                    }
//                    else
//                    {
//                        new_tab--;
//                    }
//                    menu_display(menu,new_tab);
//                    break;
//                }
            case KEY_START:
                {
                  if(menu[new_tab].strbuff=="Run")
                  {
                    menu__flag=0;//不再回到主菜单，直接发车
                    Run();
                    break;  //退出按键查询
                  }
		  else if(menu[new_tab].strbuff=="Speed_Select")
                  {
                    menu__flag=0;//不再回到主菜单，因为按键数目有限
                    Speed_Select();
                    break;
                  }
		  else if(menu[new_tab].strbuff=="Send Img")
                  {
		    menu__flag=0;//不再回到主菜单，因为按键数目有限
                    Send_Img();
                    break;
                  }
                }
            default:
                break;

            }

        }
    }
  }


}

/****************移动光标并显示菜单*****************/
void menu_display(menu_t *menu,uint8 new_tab) //
{
    uint8 tab=0;
    LCD_CLS();
    do
    {
        LCD_P6x8Str(0,tab,menu[tab].strbuff);
        if(tab == new_tab)
            LCD_P6x8Str(80,tab,"<---");
        else
            LCD_P6x8Str(80,tab,"    ");
        tab++;

    }
    while(tab!=max_tab);
}

/****************发车启动****************/
void Run(void)
{
	  MENU_flag=0;		 //关闭菜单(关闭了主菜单的menu函数和中断里的key_IRQHandler函数)
	  LCD_CLS();		 //清屏
//          delay_go=200;		 //延时2s发车
//          startjudge_flag=800;	//延时8s检测起跑线
}


/****************速度挡位选择****************/
void Speed_Select(void)
{
   char temp[1][100];							//修改13个数值，为13
   uint8 new_tab=0,Data_num=1;						//参数数量为Data_num=13
   while(menu__flag==0)
   {
     while(get_key_msg(&keymsg) == 1)
      {
          LCD_CLS();
          if(keymsg.status == KEY_DOWN)
          {
              switch(keymsg.key)
              {
//	      case KEY_U:
//		   {
//		      if(new_tab == 0)
//		      {
//			  new_tab = Data_num-1;
//		      }
//		      else
//		      {
//			  (new_tab)--;
//		      }
//		      break;
//		   }
	      case KEY_D:
		   {
		      if(new_tab < Data_num-1)
		      {
			  (new_tab)++;
		      }
		      else
		      {
			  new_tab=0;
		      }
		      break;
		   }
              case KEY_R:
                  {
		    if(new_tab==0)
		      speed_select+=1;
                      break;
                  }
              case KEY_L:
                  {
		    if(new_tab==0)
		      speed_select-=1;
		    break;
                  }
              case KEY_START:
                  {
                    menu__flag=1;
                    break;
                  }
              default:
                  break;
              }
          }
	  if(new_tab/3==0)				//第1页
	  {
	      sprintf(temp[0],"speed_select=%9d",speed_select);		        	LCD_Print(0,0,temp[0]);

	      LCD_P6x8Str(100,new_tab,"<---");
	  }
//	  else if(new_tab/8==1)			//第2页
//	  {
//	      sprintf(temp[8],"R100_S=%5d",R100_Speed);		        LCD_Print(0,0,temp[8]);
//	      sprintf(temp[9],"Out_R=%6d",Out_Ring_Speed);		LCD_Print(0,1,temp[9]);
//	      sprintf(temp[10],"Ramp_S=%5d",Ramp_Speed);		LCD_Print(0,2,temp[10]);
//              sprintf(temp[11],"Obst_S=%5d",Obstacle_Speed);		LCD_Print(0,3,temp[11]);
//              sprintf(temp[12],"Long_S=%5d",Long_straightroad_Speed);	LCD_Print(0,4,temp[12]);
//	      LCD_P6x8Str(100,new_tab-8,"<---");
//	  }
      }
   }
}

/******************显示图像*****************/
void Send_Img()
{
   char temp[5][100];
   LCD_CLS();
   int num=100,num1=0,KEY_START_num=0;
   while(menu__flag==0)
   {
     while(get_key_msg(&keymsg) == 1)
      {
          if(keymsg.status == KEY_DOWN)
          {
              switch(keymsg.key)
              {
//              case KEY_U:
//                  {
//                      num+=1;
//                      break;
//                  }
              case KEY_D:
                  {
                      num-=1;
                      break;
                  }
              case KEY_START:
                  {
		    if(KEY_START_num==0)
		    {
		        num1=num;	//按1下中键，记下此时num的数值，根据num的数值显示图像或边线
		        LCD_CLS();	//顺便清屏，清掉光标
		    }
		    else
                    {
		    	menu__flag=1;	//按2下中键，退出图像显示
                    }
		    KEY_START_num++;
                    break;
                  }
              default:
                  break;
              }
          }
      }
     if(num1==0)
     {
	 LCD_Print(0,0,"Img Show");
	 LCD_Print(0,2,"Line Show");
	 if(num%2==0)
	 {
	    LCD_P6x8Str(100,0,"<---");
	    LCD_P6x8Str(100,2,"    ");
	 }
	 else if(num%2==1)
	 {
	    LCD_P6x8Str(100,0,"    ");
	    LCD_P6x8Str(100,2,"<---");
	 }
     }
//     camera_get_img(); 					//菜单是死循环函数，进入菜单后不执行main里其他函数，要显示图像的时候在这里调用camera_get_img（）；
     if(num1%2==0&&num1>0)				//选择子函数Img_Show();
     {
//	 sprintf(temp[0],"er=%3d",err_p);                 LCD_Print(80,0,temp[0]);
//         sprintf(temp[1],"bk=%3d",break_row);             LCD_Print(80,1,temp[1]);
//         sprintf(temp[2],"CN=%3d",Threshold_Camera);      LCD_Print(80,2,temp[2]);
//         sprintf(temp[3],"s=%4d",set_value);              LCD_Print(80,3,temp[3]);
//	 Img_Show();
     }
     else if(num1%2==1&&num1>0)			//选择子函数Line_Show();
     {
//	 sprintf(temp[0],"er=%3d",err_p);                 LCD_Print(80,0,temp[0]);
//         sprintf(temp[1],"bk=%3d",break_row);             LCD_Print(80,1,temp[1]);
//         sprintf(temp[2],"CN=%3d",Threshold_Camera);      LCD_Print(80,2,temp[2]);
//         sprintf(temp[3],"s=%4d",set_value);              LCD_Print(80,3,temp[3]);
//	 Line_Show();
     }
   }
}

