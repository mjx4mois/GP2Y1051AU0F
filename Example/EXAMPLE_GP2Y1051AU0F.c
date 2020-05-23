/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: GP2Y1051AU0F dust sensor (PM2.5)
     File Name		: SENSOR_GP2Y1051AU0F.h
     Function		: SENSOR_GP2Y1051AU0F
     Create Date	: 2017/12/31
---------------------------------------------------------------------- */



#ifndef __GP2Y1051AU0F_EXAMPLE__
#define __GP2Y1051AU0F_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include "SENSOR_GP2Y1051AU0F.h"
#include "alcd.h"

#define GP2Y1051AU0F_READ_BUFFER 10

void EXAMPLE_GP2Y1051AU0F(void);


void EXAMPLE_GP2Y1051AU0F(void)
{
	CHAR8U data,status =0;
	FLOAT get_raw_data=0.0,get_dust_data=0.0;
	FLOAT real_dust_data=0.0,total_data=0.0;
		
	FLOAT temp_data[GP2Y1051AU0F_READ_BUFFER]={0.0};
	INT32U cnt=0; 
	
	INT32U print_data[5]={0};      
	CHAR8U lcd_data[7]={0};
	
		printf("-------------------- GP2Y1051AU0F dust sensor (PM2.5) --------------------\r\n");
				
		/* read 5 data */ 
		for(cnt=0;cnt<GP2Y1051AU0F_READ_BUFFER;cnt++)
		{
			/* get the raw data from GP2Y1051AU0F by UART */						
			GP2Y1051AU0F_GET_RAW_DATA(&get_raw_data);

			/* calculate the dust data */		
			GP2Y1051AU0F_GET_DUST_VALUE(&get_dust_data,get_raw_data);
					
			temp_data[cnt]= get_dust_data;
					 
			total_data+= get_dust_data;
					
			printf("reading data ...\r\n");					
		}
  
			real_dust_data =  total_data / GP2Y1051AU0F_READ_BUFFER ;


		while(1)
		{
		
			/* get the raw data from GP2Y1051AU0F by UART */		
			status= GP2Y1051AU0F_GET_RAW_DATA(&get_raw_data);
			if(status!=0)   
			{
				printf("GP2Y1051AU0F read data fail\r\n");
				continue ; 	/*net get raw data*/
			}
			
			/* calculate the dust data */					
			GP2Y1051AU0F_GET_DUST_VALUE(&get_dust_data,get_raw_data);

			total_data = 0.0;
			/*read data & shift & add new data */
			for(cnt=0;cnt<GP2Y1051AU0F_READ_BUFFER;cnt++)
			{                   
				temp_data[cnt+1]=temp_data[cnt];    
				total_data+=temp_data[cnt+1]; 
				// printf("cnt %d\r\n",cnt);
				 if(cnt==(GP2Y1051AU0F_READ_BUFFER-2))
				 {
				 	break;
				}	
			}

			temp_data[0]= get_dust_data;
			total_data+= get_dust_data;
			real_dust_data=total_data/GP2Y1051AU0F_READ_BUFFER;
					
			
    			/* calculate the print data */
			print_data[0]= (INT32U)(real_dust_data);
			print_data[1]= (INT32U)(real_dust_data*10)%10;   
			print_data[2]= (INT32U)(real_dust_data*100)%10;   	  
			
			print_data[3]= (INT32U)(real_dust_data*1000)%10; 
			print_data[4]= (INT32U)(real_dust_data*10000)%10;    		
    
			printf("dust data = %d.%d%d%d%d ug/m^3 \r\n",print_data[0],print_data[1],print_data[2],print_data[3],print_data[4]);	
	
	    		/* calculate the display data */				
			lcd_data[0]=(CHAR8U)((INT32U)real_dust_data/1000)%10;
			lcd_data[1]=(CHAR8U)((INT32U)real_dust_data/100)%10;
			lcd_data[2]=(CHAR8U)((INT32U)real_dust_data/10)%10;
			lcd_data[3]=(CHAR8U)((INT32U)real_dust_data)%10;

			lcd_data[4]=(CHAR8U)((INT32U)(real_dust_data*10)%10);
			lcd_data[5]=(CHAR8U)((INT32U)(real_dust_data*100)%10);
			lcd_data[6]=(CHAR8U)((INT32U)(real_dust_data*1000)%10);

				
	    		/* display dust data */
			lcd_gotoxy(0,0);
			lcd_putsf("Dust:");
			lcd_putchar(48+lcd_data[0]);
			lcd_putchar(48+lcd_data[1]);
			lcd_putchar(48+lcd_data[2]); 
			lcd_putchar(48+lcd_data[3]);							
			lcd_putchar(46);    /* print "."  */ 
			lcd_putchar(48+lcd_data[4]);  
			lcd_putchar(48+lcd_data[5]); 							
			lcd_putchar(48+lcd_data[6]);  
			//lcd_putchar(178);    /* "o"  */   
		                                               
			lcd_gotoxy(8,1);
			lcd_putsf("ug/m^3 "); /* show unit */

			delay_ms(200);

		}

               printf("--------------------  GP2Y1051AU0F dust sensor (PM2.5) --------------------\r\n");
}
#endif		//#ifndef __GP2Y1051AU0F_EXAMPLE__
