//VCI=3.3V,根据IM的选择用对应的数据线位数.
//*********************16BIT*********************************
void write_command(unsigned int i)
{
	 cs=0;
	 rd=1;
	 rs=0;	 
 	 P2=i>>8;
	 P0=i;
	 rw=0;
	 rw=1;
	 cs=1; 
}

void write_data(unsigned int i)
{
	 cs=0;
	 rd=1;
	 rs=1;	
	 P2=i>>8;
	 P0=i;
	 rw=0;;
	 rw=1;
	 cs=1;
}
//********************16位送显示Display data*****************************
void write_data1(byte data_h,data_l)
{
   cs=0;
   rs=1;
   rd=1;    
   P2=data_h;
   P0=data_l;
   rw=0;
   rw=1;
   cs=1;
}
***********************************************
void initi(void)
{ 
   res=1;
   delay(1);
   res=0;
   delay(10);
   res=1;
   delay(120);
//************************************************
write_command(0x11); 
delay(120);      //Delay 120ms 

write_command(0x3A);
write_data(0x05);

write_command(0x36);
write_data(0x00);
//--------------------------------ST7789S Frame rate setting----------------------------------//
write_command(0xb2);
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0x33);
write_data(0x33);

write_command(0xb7);
write_data(0x35);
//---------------------------------ST7789S Power setting--------------------------------------//
write_command(0xb8);
write_data(0x2f);
write_data(0x2b);
write_data(0x2f);

write_command(0xbb);
write_data(0x24);//vcom

write_command(0xc0);
write_data(0x2C);

write_command(0xc3);
write_data(0x10);//0B调深浅

write_command(0xc4);
write_data(0x20);

write_command(0xc6);
write_data(0x11);

write_command(0xd0);
write_data(0xa4);
write_data(0xa1);

write_command(0xe8);
write_data(0x03);

write_command(0xe9);
write_data(0x0d);
write_data(0x12);
write_data(0x00);
//--------------------------------ST7789S gamma setting---------------------------------------//
write_command(0xe0);
write_data(0xd0);
write_data(0x00);
write_data(0x00);
write_data(0x08);
write_data(0x11);
write_data(0x1a);
write_data(0x2b);
write_data(0x33);
write_data(0x42);
write_data(0x26);
write_data(0x12);
write_data(0x21);
write_data(0x2f);
write_data(0x11);

write_command(0xe1);
write_data(0xd0);
write_data(0x02);
write_data(0x09);
write_data(0x0d);
write_data(0x0d);
write_data(0x27);
write_data(0x2b);
write_data(0x33);
write_data(0x42);
write_data(0x17);
write_data(0x12);
write_data(0x11);
write_data(0x2f);
write_data(0x31);


write_command(0x21); //反显

write_command(0x2A); //Frame rate control
write_data(0x00);
write_data(0x00);
write_data(0x00);
write_data(0xEF);

write_command(0x2B); //Display function control
write_data(0x00);
write_data(0x00);
write_data(0x01);
write_data(0x3F);

write_command(0x29); //display on
write_command(0x2c); 

  }

//*********************************************************************************************
void EnterSleep (void)
{
    write_command(0x28);
    delay(20);
    write_command(0x10);
  
  }

//*********************************************************
void ExitSleep (void)

 {
    write_command(0x11);
    delay(120);
    write_command(0x29);
}

//************ST7789读red ID如下（MT6260)******************//
kal_bool LCM_ID_check_ST7789S()
{
    kal_uint8 dummy, ST7789S_1, ST7789S_2, ST7789S_3;

    MAIN_LCD_CtrlWrite(0x04);  //LCM ID
    dummy = MAIN_LCD_DataRead;
    ST7789S_1 = MAIN_LCD_DataRead;
    ST7789S_2 = MAIN_LCD_DataRead;
    ST7789S_3 = MAIN_LCD_DataRead;

    if ((0x85== ST7789S_2) && (0x52 == ST7789S_3))
     {
#ifdef __FACTORY_GET_DRIVERS_INF__
     LCM_INF="ST7789S ID=8552";
#endif
      return KAL_TRUE;
    }
    else
      return KAL_FALSE;
}
