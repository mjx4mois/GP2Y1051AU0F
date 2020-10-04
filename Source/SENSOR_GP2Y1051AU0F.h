/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: GP2Y1051AU0F dust sensor (PM2.5)
     File Name		: SENSOR_GP2Y1051AU0F.h
     Function		: SENSOR_GP2Y1051AU0F
     Create Date	: 2017/12/31
---------------------------------------------------------------------- */

#ifndef __GP2Y1051AU0F_HEADER__
#define __GP2Y1051AU0F_HEADER__  


/**** Note : UART buad rate 2400 bit/s */
/*when sensor power -> never stop to trans data.*/

/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*get GP2Y1051AU0F raw data form UART RX */
/* return voltage data [FLOAT type]*/
CHAR8S GP2Y1051AU0F_GET_RAW_DATA(FLOAT *raw_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* turn the ADC value to Dust Value  ; unit : ug/m^3*/
void GP2Y1051AU0F_GET_DUST_VALUE(FLOAT *dust_data,FLOAT raw_data);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM **************************************************/

#endif /*#ifndef __GP2Y1051AU0F_HEADER__*/ 
