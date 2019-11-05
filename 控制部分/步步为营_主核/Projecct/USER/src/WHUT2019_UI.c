#include "headfile.h"

char page,check,hold,sign,cursor=1;
uint8 go_direct_flag=0;

/*-----------------------------------------------------------------------------------------
* key_event
* 按键处理函数
---------------------------------------------------------------------------------------------*/
void key_event(void)
{
   KEY_MSG_t keymsg_two;
   sign=0;
   hold=0;
   while(get_key_msg(&keymsg_two)==1)
   {
     if(keymsg_two.status==KEY_DOWN)
     {
       switch(keymsg_two.key)
       {
       case KEY_U:
         if(check)
           sign=1;
         else
           cursor--;
         break;
       case KEY_D:
         if(check)
           sign=-1;
         else
           cursor++;
         break;
       case KEY_L:     //这里默认参数页为第一页
         if(check&&(page==1||page==2||page==3||page==4||page==5||page==7))
         {
           lsb/=10;
           if(lsb<1)lsb=1000000;
           OLED_int16(76,0,lsb);

         }
         else
         {
           page--;
           page = (page<0)?(PAGE_MAX):( (page>PAGE_MAX)?(0):(page) );
           cursor=1;
           page_show();
         }
         break;
       case KEY_R:
         if(check&&(page==1||page==2||page==3||page==4||page==5||page==7))
         {
           lsb*=10;
           if(lsb>1000000)lsb=1;
           OLED_int16(76,0,lsb);

         }
         else
         {
           page++;
           page = (page<0)?(PAGE_MAX):( (page>PAGE_MAX)?(0):(page) );
           cursor=1;
           page_show();
         }

         break;
       case KEY_A:
         check=!check;



         break;

       }

     }
   else if(keymsg_two.status==KEY_HOLD)
   {
     switch(keymsg_two.key)
     {
        case KEY_U:
           if(check)
           sign=1;
         else
           cursor--;
         break;
       case KEY_D:
         if(check)
           sign=-1;
         else
           cursor++;
         break;
       case KEY_A:
         hold=1;
         check=0;
         break;

     }

   }

   }



}

/*-----------------------------------------------------------------------------------------
* GUI_show
* UI
---------------------------------------------------------------------------------------------*/
uint8 GUI_show(void)
{  uint8 i;
   key_event();

   switch(page)
   {
   case 0:    //发车页
     if(hold)
     {
      if(go_direct_flag)
      {
       NVIC_DisableIRQ(PIT1_IRQn);
       OLED_image2;
        bi(10);
        systick_delay_ms(1000);
        engine=1;
        timer=0;
        mode=ready;
        crazy_flag=0;
        speed_cal_ready=0;
      }
      else
      {
        NVIC_DisableIRQ(PIT1_IRQn);
       OLED_image2;
        bi(10);
        systick_delay_ms(1000);
        engine=1;
        timer=0;
        mode=go;
        distance_cal=0;
        crazy_flag=0;
        speed_cal_ready=0;
      }
        return 0;
     }

     else{

       OLED_P6x8Str(0,0,"Powervalue");
       OLED_double(82,0,powervalue,1);
       OLED_P6x8Str(0,1,"ad_l");
       OLED_int16(30,1,ad[0]);
       OLED_P6x8Str(60,1,"ad_r");
       OLED_int16(90,1,ad[1]);
       OLED_P6x8Str(0,2,"Hori_sum");
       OLED_int16(84,2,ad[0]+ad[1]);

       OLED_P6x8Str(0,3,"ad_vl");
       OLED_int16(36,3,ad[2]);

       OLED_P6x8Str(66,3,"ad_vr");
       OLED_int16(102,3,ad[3]);

       OLED_P6x8Str(0,4,"disance_cal");

      OLED_int16(80,4,timer);

       OLED_P6x8Str(0,5,"car_error");
       OLED_int16(76,5,car_error);
       OLED_P6x8Str(0,6,"car_angle");
       OLED_double(74,6,car_angle,1);

       OLED_int16(0,7,ad_coeff[0]);
       OLED_int16(28,7,ad_coeff[1]);
       OLED_int16(56,7,ad_coeff[2]);
       OLED_int16(84,7,ad_coeff[3]);



     }
     break;
   case 1:
     if(check)
     {
       switch(cursor)
       {
         case 7:
          params_reset(sector_active);
          check=0;
          page_show();
          bi(10);
          break;
       case 6:
         error_mode_flag+=1;
         error_mode_flag=(error_mode_flag>2)?0:(error_mode_flag);
          if(error_mode_flag==1)OLED_P6x8Str(76,6,"/+");
          else if(error_mode_flag==2)OLED_P6x8Str(76,6,"FD");
        else OLED_P6x8Str(76,6,"/*");
        check=0;
        break;
       default:
         i=cursor;
         if(sign!=0)
         {

            *((float *)temp1+cursor-1) += (sign*lsb/(FACTOR*100));
            OLED_int16(76,i,*((float *)temp1+cursor-1) * FACTOR*100);

            if(i==5)
            {
            stop_distance += (sign*lsb/FACTOR);
            OLED_int16(76,i,stop_distance * FACTOR);

            }
         }
        break;

       }

     }
    cursor_show();
    break;
   case 2:
       if(check)
     {
       switch(cursor)
       {
         case 7:
          params_reset(sector_active);
          check=0;
          page_show();
          bi(10);
          break;
          case 4:
            big_circle_flag=1-big_circle_flag;
            if(big_circle_flag==1)OLED_P6x8Str(76,4,"YES");
        else OLED_P6x8Str(76,4,"NOT");
        check=0;

            break;
           case 5:
            circle_by_threshold=1-circle_by_threshold;
            if(circle_by_threshold==1)OLED_P6x8Str(76,5,"YES");
        else OLED_P6x8Str(76,5,"NOT");
        check=0;

            break;
             case 6:
            no_circle_flag=1-no_circle_flag;
            if(no_circle_flag==1)OLED_P6x8Str(76,6,"YES");
        else OLED_P6x8Str(76,6,"NOT");
        check=0;

            break;
       default:
         i=cursor;
         if(sign!=0)
         {
           if(cursor==1)
           {
               *((float *)temp2) += (sign*lsb/(FACTOR*10));
               OLED_int16(76,i,*((float *)temp2) * FACTOR*10);
           }
            else if(cursor==2)
            {
                *((float *)temp2+2) += (sign*lsb/(FACTOR*10));
                OLED_int16(76,i,*((float *)temp2+2) * FACTOR*10);

            }
           else if(cursor==3)
           {
               out_given_time += (sign*lsb/FACTOR);
               OLED_int16(76,3,out_given_time * FACTOR);

           }

         }
        break;

       }

     }
    cursor_show();
    break;
   case 3:
      if(check)
     {
       switch(cursor)
       {
         case 4:
          params_reset(sector_active);
          check=0;
          page_show();
          bi(10);
          break;
       default:
         i=cursor;
         if(sign!=0)
         {
            *((float *)temp3+cursor-1) += (sign*lsb/(FACTOR*100));
            OLED_int16(76,i,*((float *)temp3+cursor-1) * FACTOR*100);

         }
        break;

       }

     }
    cursor_show();
    break;
   case 4:
      if(check)
     {
       switch(cursor)
       {
         case 5:
          params_reset(sector_active);
          check=0;
          page_show();
          bi(10);
          break;
       default:
         i=cursor;
         if(sign!=0)
         {
            *((float *)temp4+cursor-1) += (sign*lsb/FACTOR);
            OLED_int16(76,i,*((float *)temp4+cursor-1) * FACTOR);

         }
        break;

       }

     }
    cursor_show();
    break;
   case 5:
      if(check)
     {
       switch(cursor)
       {
         case 6:
             params_reset(sector_active);
             check=0;
             page_show();
             bi(10);
         break;

         case 5:
             speed_up_flag=1-speed_up_flag;
             if(speed_up_flag==1)OLED_P6x8Str(76,5,"YES");
             else OLED_P6x8Str(76,5,"NOT");
             check=0;
         break;
       default:
         i=cursor;
         if(sign!=0)
         {
            *((float *)temp5+cursor-1) += (sign*lsb/FACTOR);
            OLED_int16(76,i,*((float *)temp5+cursor-1) * FACTOR);

         }
        break;

       }

     }
    cursor_show();
    break;
   case 6:
if(check)
    {
      sector_active = CYCLE_SECTOR-cursor;
      params_load(sector_active);
      OLED_int16(100,0,CYCLE_SECTOR-sector_active);
      check=0;
      bi(5);
    }
    cursor_show();
    break;
   case 7:
     if(check)
     {
       if(cursor==3)
       {
       ad_normalize_flag=1;

       if(normalize_in==0)
       {
       normalize_in=1;
       ad_coeff[0]=1000;
       ad_coeff[1]=1000;
       ad_coeff[2]=1000;
       ad_coeff[3]=1000;
       }

       }
         else if(cursor==4)
       {
        go_direct_flag=1-go_direct_flag;
       if(go_direct_flag==1)OLED_P6x8Str(76,4,"NOT");
        else OLED_P6x8Str(76,4,"YES");
        check=0;
       }
       else if(cursor==5)
       {
        if(sign!=0)
         {
            circle_in_delay+= (sign*lsb/FACTOR);
            OLED_int16(76,5,circle_in_delay* FACTOR);

         }


       }
      else if(cursor==6)
       {
        if(sign!=0)
         {
           circle_out_delay+= (sign*lsb/FACTOR);
            OLED_int16(76,6,circle_out_delay * FACTOR);

         }


       }

       else if(cursor==7)
       {
         ad_normalize_flag=0;
        bi(10);
       ad_coeff_reset();
       check=0;
       }
       else if(cursor==2)
       {

         circle_out_yuzhi=1-circle_out_yuzhi;
       if(circle_out_yuzhi==1)OLED_P6x8Str(76,2,"YES");
     else OLED_P6x8Str(76,2,"NOT");
     check=0;
       }
       else if(cursor==1)
       {

         no_stop_flag+=1;
         no_stop_flag=(no_stop_flag>3)?0:(no_stop_flag);
        if(no_stop_flag==0)OLED_P6x8Str(76,1,"GHG");
        else if(no_stop_flag==1)OLED_P6x8Str(76,1,"MAG");
        else if(no_stop_flag==3)OLED_P6x8Str(76,1,"DIS");
        else OLED_P6x8Str(76,1,"NOT");
     check=0;
       }
     }
    else if(hold)
     {
       bibi(20);
        ad_normalize_flag=0;
        FLASH_EraseSector(CYCLE_SECTOR);
        FLASH_WriteSector(CYCLE_SECTOR,(const uint8 *)ad_coeff,8,0);
     }
    else
    {
      normalize_in=0;
       ad_normalize_flag=0;

    }
     cursor_show();
    break;

  }

   if(hold&&page!=0)
   {
    params_save(sector_active);
    bibi(20);
    return 0;
   }
 return 0;

}

