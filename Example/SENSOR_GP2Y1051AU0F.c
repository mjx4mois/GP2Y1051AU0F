/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: GP2Y1051AU0F dust sensor (PM2.5)
     File Name		: SENSOR_GP2Y1051AU0F.c
     Function		: SENSOR_TGP2Y1051AU0F
     Create Date	: 2017/12/31
---------------------------------------------------------------------- */
#ifndef __GP2Y1051AU0F_FUNCTION__
#define __GP2Y1051AU0F_FUNCTION__  


#include <stdio.h>
#include <math.h>
#include <delay.h>
#include "SENSOR_GP2Y1051AU0F.h"

#define GP2Y1010AU0F_DEBUG		0

//********************************************* SYSTEM *************************************************
/*--------------------------------------------------------------------------------------------------*/
/*
	get GP2Y1051AU0F raw data form UART RX 
	return voltage data [FLOAT type]

	UART Format : 0xAA , Vout_H , Vout_L , Vref_H , Vref_L , checksum , 0xFF
*/ 
CHAR8S GP2Y1051AU0F_GET_RAW_DATA(FLOAT *raw_data)
{
#if GP2Y1010AU0F_DEBUG /* for debug */
		INT32U temp_data[3]={0};     
#endif		

		CHAR8U read_char=0,dead_loop_cnt =0,count=0;
		CHAR8U read_data[7]={0}, return_flag=0,check_sum=0;  
		INT32U voltage_adc_value = 0;
		FLOAT voltage_data = 0.0;
		
		/* step 1 check "0xaa" [start] */
		while(1)
           	{
			/* get the data from UART RX */
			read_char = getchar();		

			if(read_char == 0XAA) 	/* "0XAA" start Header */
			{
				count=0;
				read_data[count]=read_char;	
			}
			else
			{
				count++;
				read_data[count]=read_char;				  
			}

		
			if(count==6)
			{
			
				/* first check the read_data[0]==0xAA? */
				if(read_data[0]!=0xAA) /* it's not right sequence data */
	                    	{
	                   		  count=0;		/* start signal not right value -> fail , restart read */
	                    	}
	                	else			
	                    	{
	                    	
	     				/*check checksum ; if not equal -> read the next dust data */
					check_sum = read_data[1] + read_data[2] + read_data[3] + read_data[4]; 
						
#if GP2Y1010AU0F_DEBUG /* for debug */
					printf("read data : %x %x %x %x %x %x %x\r\n",read_data[0],read_data[1],read_data[2],read_data[3],read_data[4],read_data[5],read_data[6]);
					printf("calculate check sum : 0x%x , sensor check sum : 0x%x \r\n",check_sum,read_data[5]);
#endif		
					
					if(check_sum == read_data[5])	/* Compare the Checksum */
					{
						return_flag = 0 ;     			
						break;		/*	read finish  bread while(1)   */
					}
					else
					{
						count=0;  	/*checksum not right value -> fail , restart read */
					}
	            
				}    

			}
			else	
			{
				dead_loop_cnt++;		/* read fail count */
			}

			if(dead_loop_cnt>100) 
			{
				return_flag = 1 ; 		/* not read finish */
				break;					/* can't not read the data [start] & not read all */
			}
							
           	}


		if(return_flag==1) 
		{
			return -1;		/* read fail  ( read fail count >100 ) */
		}
	
		if(return_flag == 0)
		{
				/* read_data[1] -> Vout(H) ; read_data[2] -> Vout(L) */
				voltage_adc_value = (read_data[1]*256 + read_data[2])   ;
				voltage_data =  (FLOAT)voltage_adc_value / 1024 * 5; 	/* 5-> 5v   ; turn ADC value to voltage value */

#if GP2Y1010AU0F_DEBUG 	/*for debug*/

			temp_data[0]= (INT32U)(voltage_data);
			temp_data[1]= (INT32U)(voltage_data*10)%10;   
			temp_data[2]= (INT32U)(voltage_data*100)%10;   	  
			printf("ADC value : %d\r\n",voltage_adc_value);	
			printf("voltage data : %d.%d%d\r\n",temp_data[0],temp_data[1],temp_data[2]);
			        		
#endif					
				*raw_data = voltage_data;
				return 0; 		/*  calculate the voltage data success !*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* turn the ADC value to Dust Value  ; unit : ug/m^3 */
void GP2Y1051AU0F_GET_DUST_VALUE(FLOAT *dust_data,FLOAT raw_data)
{
	
#if GP2Y1010AU0F_DEBUG 	/*for debug*/
	INT32U temp_data[3]={0};     
#endif		

	FLOAT gp2y1051au0f_slope =0.0027625; /* (2.7625 *10^-3) ->  A(0,0) ; B(400, 1.105)¡Am=dy/dx = 1.105/400  , unit : ug/m3 */
	FLOAT calculate_dust_value =0.0;

	/* X=(Y-1.105)/m + 400 ; */
	/* formula :  dust_data(ug/m^3) =  (raw_data - 1.105) /gp2y1051au0f_slope + 400 */
	calculate_dust_value =  ((raw_data - 1.105) /gp2y1051au0f_slope) + 400.0 ;

#if GP2Y1010AU0F_DEBUG 	/*for debug*/

		temp_data[0]= (INT32U)(calculate_dust_value);
		temp_data[1]= (INT32U)(calculate_dust_value*10)%10;   
		temp_data[2]= (INT32U)(calculate_dust_value*100)%10;   	  
	
		printf("dust data [ug/m^3]: %d.%d%d\r\n",temp_data[0],temp_data[1],temp_data[2]);
			        		
#endif				

	 *dust_data = calculate_dust_value;
	
}
/*--------------------------------------------------------------------------------------------------*/
//********************************************* SYSTEM *************************************************

#endif //#ifndef __GP2Y1051AU0F_FUNCTION__ 
