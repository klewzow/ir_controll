 #include "rf_ressive.h"
 
 
 void init_timer2_433(void)
 {
	TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20); 
	TIMSK |= (1<<TOIE2);
 }
 uint8_t			adres_user_433	 = 0b01100101;
 
 uint16_t			data_user_433_1			 = 0b0011001001001100;
 uint16_t			data_user_433_2			 = 0b0010011111100100;
 uint16_t			data_user_433_3			 = 0b0110011001100110;
 uint16_t			data_user_433_4			 = 0b1010101111010101;
 uint16_t			data_RESSET_user_433_5	 = 0b1011101001010101; // ÄËß RESSETA IR CODE &&&
 
 
 uint8_t				adres_433	 = 0;
 uint16_t				data_433	 = 0;
 uint8_t				col_byte	 = 0;
 uint16_t				val_timer	 = 0;
 unsigned char			connect_433  = 0;
 uint8_t				result_data  = 0;

 volatile uint8_t flag_433 = 0; 


void resset_flags_433(void)
{
	     flag_433	  = 0;
	     adres_433    = 0;
	     data_433     = 0;
	     col_byte     = 0;
	     val_timer    = 0;
	     connect_433  = 0;
	     result_data  = 0;
	     GICR |= (1<<INT1);
}

  
 
 void resive_433(void)
 {
  
	 
     val_timer = TCNT2;
	 TCNT2=0;
/*  USART_transmit(val_timer);*/ 
   
/////////////////////////////////////////////////////////////////////////
	 if ((val_timer > 44) && (val_timer < 50)&& (flag_433 == 0))
	 {
		 connect_433++;
		
	 }
/////////////////////////////////////////////////////////////////////////
	 
///////////////////////////////////////////////////////////////////////// 
	if (connect_433  > 5)
	{
		CONNECT_BYTE_433_1();
		connect_433=0;
	
	}
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////	
	if (flag_433 == 1)
	{
		 
		
							 if ((val_timer > 22) && (val_timer < 26))
							 {
								 adres_433 &=~(1<<(7 - col_byte));
								 col_byte++;
								
							 }
							 else if ((val_timer > 39) && (val_timer < 43 ))
							 {
								 adres_433 |= (1<<(7 - col_byte));
								 col_byte++;
								 
							 }
							 if ((col_byte > 7) && (adres_433 == adres_user_433) )
							 {
								 col_byte = 0;
								 ADRESS_BYTE_433_1();
								 adres_433=0;
								 return;
								   
							 }
							 else if ((col_byte > 7) && (adres_433 != adres_user_433))
							 {
								   
								 col_byte = 0;
								 flag_433=0;
								 adres_433=0;
								 return;
								 
							 }
							  
  }//if (flag_433 == 1)
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////	
if (flag_433 == 3)
{
   
						 if ((val_timer > 22) && (val_timer < 26))
						 {
							 data_433 &=~(1<<(15 - col_byte));
							 col_byte++;
						 }
						 else if ((val_timer > 39) && (val_timer < 43 ))
						 {
							 data_433 |= (1<<(15 - col_byte));
							 col_byte++;
						 }
						 
						 	 if ((col_byte > 15) )//if ((col_byte > 15) )
						 	 {
							 	
													 if (data_433 == data_user_433_1)
													 {
														 result_data = 1;
													 }
								 	 
													  if (data_433 == data_user_433_2)
								 					 {
														   result_data = 2;
								 					 }
									  	
													   if (data_433 == data_user_433_3)
									  				 {
															result_data = 3;
									  				 }
										   
										   			 if (data_433 == data_user_433_4)
										   			 {
															 result_data = 4;
										   			 }
												
													 if (data_433 == data_RESSET_user_433_5)
													 {
														  result_data = 5;
													 }
								 
								 
								 
								 
								 if (result_data != 0)
								 {
									 DATA_BYTE_433_1();
									 col_byte = 0;
									 data_433 = 0;									 
									 return; 
								 }
								 
								 if (result_data == 0)
								 {
									 col_byte = 0;
									 data_433=0;
									 flag_433 =0;
									 result_data=0;
									 return;
								 } 
					 
							 	
						 	 }//if ((col_byte > 15) )
			 
	
 }
	
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////

if (flag_433 == 7)
{
	
			 if ((val_timer > 69) && (val_timer < 73))
			 
			 {
										 switch(result_data)
										 {
											 
														case 1:
																PORTC |=(1<<2)|(1<<1)|(1<<0);
														break;
														
														case 2:
																PORTC &=~(1<<2)&(1<<1)&(1<<0);
														break;
														
														case 3:
																PORTC  =PORTC^(1<<1);
														break;
														
														case 4:
																PORTC  =PORTC^(1<<2);
														break;
														
														case 5:
																EEPROM_write_word_int(1,0xFFFF);
																EEPROM_write_word_int(3,0xFFFF);
																EEPROM_write_word_int(5,0xFFFF);
																EEPROM_write_word_int(7,0xFFFF);
																PORTC   &=~ (1<<2)&(1<<1)&(1<<0);
																read_EEPROM_memory();
														break;
										}
			 }
			 
			 
			 
			 
			 
	
	GICR  &=~ (1<<INT1);
}

 /////////////////////////////////////////////////////////////////////////	 
	
 
	
 }
 
 
 