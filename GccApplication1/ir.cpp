 #include "ir.h"

 #define   START_IR_BIT           0
 #define   START_IR_REPIT_BIT     1
 #define   MEMORY01               2
 #define   MEMORY02               3
 #define   MEMORY03               4
 #define   MEMORY04               5
 #define   MEMORY                 6
 volatile unsigned char start_bit_flag =0;
 volatile unsigned char t=0;
 volatile unsigned int timing =0;

 



 #define  START_IR_BIT_0() (start_bit_flag &=~(1<<START_IR_BIT))
 #define  START_IR_BIT_1() (start_bit_flag |= (1<<START_IR_BIT))

 #define  START_IR_REPIT_BIT_0() (start_bit_flag &=~(1<<START_IR_REPIT_BIT))
 #define  START_IR_REPIT_BIT_1() (start_bit_flag |= (1<<START_IR_REPIT_BIT))

 #define  MEMORY01_0() (start_bit_flag &=~(1<<MEMORY01))
 #define  MEMORY01_1() (start_bit_flag |= (1<<MEMORY01))

 #define  MEMORY02_0() (start_bit_flag &=~(1<<MEMORY02))
 #define  MEMORY02_1() (start_bit_flag |= (1<<MEMORY02))

 #define  MEMORY03_0() (start_bit_flag &=~(1<<MEMORY03))
 #define  MEMORY03_1() (start_bit_flag |= (1<<MEMORY03))

 #define  MEMORY04_0() (start_bit_flag &=~(1<<MEMORY04))
 #define  MEMORY04_1() (start_bit_flag |= (1<<MEMORY04))

 #define  MEMORY_0() (start_bit_flag &=~(1<<MEMORY))
 #define  MEMORY_1() (start_bit_flag |= (1<<MEMORY))

 #define  START_IR_BIT_RD(f) (start_bit_flag &(1<<f))
 #define  MEMORY_RD(f) (start_bit_flag &(1<<f))
 
 uint16_t sending = 0;
 uint16_t adress = 0;
 uint16_t temp_sending = 0;
 uint16_t temp_adress  = 0;
 unsigned char read_byte =31;
 unsigned int BYTE_HI =0;
 unsigned int BYTE_LO =0;

 
 
 

 
 uint16_t save_code_00 =0;
 uint16_t save_code_01 =0;

 unsigned int m1,m2,m3,m4;

 void b1_funct(void)
 {
	 PORTC |=(1<<2)|(1<<1)|(1<<0);
	 
 }

 void b2_funct(void)
 {
	 PORTC &=~(1<<2)&(1<<1)&(1<<0);

 }

 void b3_funct(void)
 {
	 PORTC  =PORTC^(1<<1);
 }

 void b4_funct(void)
 {
	 PORTC  =PORTC^(1<<2);
 }

 unsigned int EEPROM_read(unsigned int uiAddress)
 {

	 while(EECR & (1<<EEWE))

	 {} //ждем освобождения флага окончания последней операцией с памятью

	 EEAR = uiAddress; //Устанавливаем адрес

	 EECR |= (1<<EERE); //Запускаем операцию считывания из памяти в регистр данных

	 return EEDR; //Возвращаем результат

 }
 unsigned int EEPROM_read_word_int(unsigned int uiAddress)
{

	 uint16_t dt= EEPROM_read(uiAddress+1);
	 dt = dt<<8;
	 asm("nop");
	 dt += EEPROM_read(uiAddress);

	 return dt;
 }




 void read_EEPROM_memory(void)
 {
	 m1 = EEPROM_read_word_int(1);
	 if (m1 == 0xFFFF)
	 {
		 PORTD |=(1<<5);
		 MEMORY01_0();
	 }
	 else
	 { MEMORY01_1();
		 PORTD &=~(1<<5);
	 }
	 
	 m2 = EEPROM_read_word_int(3);
	 if (m2 == 0xFFFF)
	 {PORTD |=(1<<6);
		 MEMORY02_0();
	 }
	 else
	 { MEMORY02_1();
		 PORTD &=~(1<<6);
	 }
	 m3 = EEPROM_read_word_int(5);
	 if (m3 == 0xFFFF)
	 {PORTD |=(1<<7);
		 MEMORY03_0();
	 }
	 else
	 {
		 MEMORY03_1();
		 PORTD &=~(1<<7);
	 }
	 m4 = EEPROM_read_word_int(7);
	 if (m4 == 0xFFFF)
	 {
		 PORTB |=(1<<0);
		 MEMORY04_0();
	 }
	 else
	 {
		 PORTB &=~(1<<0);
		 MEMORY04_1();
		 
	 }
	 if (MEMORY_RD(MEMORY01) && MEMORY_RD(MEMORY02) && MEMORY_RD(MEMORY03) && MEMORY_RD(MEMORY04))
	 {MEMORY_1();}
	 if (!MEMORY_RD(MEMORY01) && !MEMORY_RD(MEMORY02) && !MEMORY_RD(MEMORY03) && !MEMORY_RD(MEMORY04))
	 {MEMORY_0();
		 PORTC   &=~(1<<2)&(1<<1)&(1<<0);
	 }
 }
 void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
 {

	 while(EECR & (1<<EEWE)) //ждем освобождения флага окончания последней операцией с памятью

	 {}

	 EEAR = uiAddress; //Устанавливаем адрес
	 EEDR = ucData; //Пищем данные в регистр
	 EECR |= (1<<EEMWE); //Разрешаем запись
	 EECR |= (1<<EEWE); //Пишем байт в память

 }
 void EEPROM_write_word_int(unsigned int uiAddress, uint16_t ucData)
 {
	 EEPROM_write(uiAddress, ((unsigned char)ucData));
	 unsigned char dt = (ucData>>8);
	 EEPROM_write(uiAddress+1, dt);
 }


 void resset_memory(void)
 {
	 EEPROM_write_word_int(1,0xFFFF);
	 EEPROM_write_word_int(3,0xFFFF);
	 EEPROM_write_word_int(5,0xFFFF);
	 EEPROM_write_word_int(7,0xFFFF);
	 PORTC   &=~ (1<<2)&(1<<1)&(1<<0);
	 read_EEPROM_memory();
 }


 




 void save_memory(  uint16_t coms, char mem)
 {
	 
	 
	 if (save_code_00 == 0)
	 {
		 save_code_00 = coms;
	 }
	 else
	 {
		 save_code_01 = coms;
	 }
	 
	 
	 
	 
	 
	 switch(mem)
	 {
		 case 1:
		 
		 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 == save_code_01))
		 {
			 
			 EEPROM_write_word_int(  1 , save_code_00);
			 // 								 USART_transmit(sending>>8);
			 // 								 USART_transmit(sending);
			 MEMORY01_1();
			 PORTD &=~(1<<5);
			 save_code_00  = save_code_01  = 0 ;
		 }
		 else
		 {//.......................................................................................
			 
			 
			 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 != save_code_01))
			 {
				 save_code_00=0;
				 save_code_01=0;
				 
				 MEMORY01_0();
			 }
			 
			 
			 
			 
		 }//.......................................................................................
		 
		 break;
		 
		 case 2:
		 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 == save_code_01))
		 {
			 
			 EEPROM_write_word_int(  3 , save_code_00);
			 // 								 USART_transmit(sending>>8);
			 // 								 USART_transmit(sending);
			 PORTD &=~(1<<6);
			 MEMORY02_1();
			 save_code_00  = save_code_01  = 0 ;
		 }
		 else
		 {//.......................................................................................
			 
			 
			 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 != save_code_01))
			 {
				 save_code_00=0;
				 save_code_01=0;
				 
				 MEMORY02_0();
			 }
			 
			 
			 
			 
		 }//.......................................................................................
		 break;
		 
		 case 3:
		 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 == save_code_01))
		 {
			 
			 EEPROM_write_word_int(  5 , save_code_00);
			 // 								 USART_transmit(sending>>8);
			 // 								 USART_transmit(sending);
			 PORTD &=~(1<<7);
			 MEMORY03_1();
			 save_code_00  = save_code_01  = 0 ;
		 }
		 else
		 {//.......................................................................................
			 
			 
			 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 != save_code_01))
			 {
				 save_code_00=0;
				 save_code_01=0;
				 
				 MEMORY03_0();
			 }
			 
			 
			 
			 
		 }//.......................................................................................
		 break;
		 
		 case 4:
		 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 == save_code_01))
		 {
			 
			 EEPROM_write_word_int(  7 , save_code_00);
			 // 								 USART_transmit(sending>>8);
			 // 								 USART_transmit(sending);
			 PORTB &=~(1<<0);
			 MEMORY04_1();
			 save_code_00  = save_code_01  = 0 ;
		 }
		 else
		 {//.......................................................................................
			 
			 
			 if ((save_code_00 !=0) && (save_code_01 != 0) && (save_code_00 != save_code_01))
			 {
				 save_code_00=0;
				 save_code_01=0;
				 
				 MEMORY04_0();
			 }
			 
			 
			 
			 
		 }//.......................................................................................
		 break;
	 }
	 
 }
 
 void resset_flags(void)// для Таймера
 {
	 t=0;
	 START_IR_REPIT_BIT_0();
	 START_IR_BIT_0();
 }
 
 
 void timer_IR_start(void)
 {
	 
	    
	       GICR  &=~ (1<<INT1); //ЗАПРЕТ INT 1 433
		   TCCR1B |= (1<<CS11);
	 
	 
 }
 
 
 
 void read_start_bit(void)
 {
	  
	 timing = TCNT1;
	 TCNT1 =0;
	 if ((!START_IR_BIT_RD(START_IR_BIT)) && (!START_IR_BIT_RD(START_IR_REPIT_BIT)))
	 {
		 //++++++++++++++++++++++++++++++++++++++++СТАРТ_БИТ+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		 
		 switch(t)
		 {
			 
			 case 0:
			 sending=0;
			 read_byte =31;
			 timing =0;
			 timer_IR_start();
			// TCCR1B |= (1<<CS11);
			 t++;
			 break;
			 
			 
			 
			 case 1:
			 // 					if (timing > 13000 && timing < 14000)
			 // 					{
			 t++;
			 
			 // 					}
			 // 					else
			 // 					{
			 // 						t=0;
			 // 					}
			 break;
			 
			 
			 
			 
			 case 2:
			 // 					if (timing > 6000 && timing < 7000)
			 // 					{
			 
			 t++;
			 // 			        }
			 // 					else
			 //  if (timing > 3000 && timing < 4000)
			 // 					{
			 //
			 //
			 // 					 if (MEMORY_RD(MEMORY))
			 // 	   {
			 //
			 //
			 // 				     t=0;
			 // 					 sending = temp_sending;
			 // 					 TCCR1B |= (1<<CS11);
			 // 	                 TCCR1B |= (1<<CS10);
			 // 	                 GICR   &=~(1<<INT0);
			 //
			 // 					if ((eeprom_read_word(&b1_on))==sending)
			 // 					{
			 // 				b1_funct();
			 // 					}
			 // 					if ((eeprom_read_word(&b2_off))==sending)
			 // 					{
			 // 				   b2_funct();
			 // 					}
			 // 					if ((eeprom_read_word(&b3_func1))==sending)
			 // 					{
			 // 						b3_funct();
			 //
			 // 					}
			 // 					if ((eeprom_read_word(&b4_func2))==sending)
			 // 					{
			 // 						b4_funct();
			 //
			 //   					}
			 // 	   }
			 //
			 //
			 //
			 // 					}else
			 // 					{
			 // 						t=0;
			 // 					}
			 
			 
			 break;
			 
			 case 3:
			 
			 START_IR_BIT_1();
			 
			 break;
			 
			 default:
			 
			 break;
		 }
		 //++++++++++++++++++++++++++++++++++++++++СТАРТ_БИТ+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 }
	 // if (START_IR_BIT_RD(START_IR_REPIT_BIT))
	 // {
	 //
	 //
	 // 					 TCCR1B |= (1<<CS11);
	 // 	                 TCCR1B |= (1<<CS10);
	 // 	                 GICR   &=~(1<<INT0);
	 // }


	 if (START_IR_BIT_RD(START_IR_BIT))
	 {
		 //++++++++++++++++++++++++++++++++++++++++ЧТЕНИЕ+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		 if (BYTE_HI == 0)
		 {
			 BYTE_HI = timing+500;
		 }
		 else
		 {
			 BYTE_LO = timing;
			 
			 
			 
			 
			 if (read_byte > 16)
			 {
				 if (BYTE_HI > BYTE_LO)
				 {
					 adress &=~(1<<(read_byte-16));
				 }
				 else
				 {
					 adress |= (1<<(read_byte-16));
				 }
			 }
			 
			 if (read_byte < 16)
			 {
				 if (BYTE_HI > BYTE_LO)
				 {
					 sending &=~(1<<read_byte);
				 }
				 else
				 {
					 sending |= (1<<read_byte);
				 }
			 }
			 BYTE_HI = BYTE_LO = 0;
			 
			 //++++++++++++++++++++++++++++++++++++++++ЧТЕНИЕ+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			 if (read_byte == 0)
			 {
				 temp_adress  = adress;
				 temp_sending = sending;
				 
				 if (!MEMORY_RD(MEMORY))
				 {
					 if (!MEMORY_RD(MEMORY01))
					 {
						 save_memory(  sending, 1);
					 }else if (!MEMORY_RD(MEMORY02))
					 {
						 save_memory(  sending, 2);
					 }else if (!MEMORY_RD(MEMORY03))
					 {
						 save_memory(  sending, 3);
					 }
					 else if (!MEMORY_RD(MEMORY04))
					 {
						 save_memory(  sending, 4);
					 }
					 else if (MEMORY_RD(MEMORY01) && MEMORY_RD(MEMORY02) && MEMORY_RD(MEMORY03) && MEMORY_RD(MEMORY04))
					 {MEMORY_1();}
				 }  // если нет кнопок в памяти
				 
				 
				 if (MEMORY_RD(MEMORY))
				 {
					 if ((EEPROM_read_word_int(1))==sending)
					 {
						 b1_funct();
					 }
					 if ((EEPROM_read_word_int(3))==sending)
					 {
						 b2_funct();
					 }
					 if ((EEPROM_read_word_int(5))==sending)
					 {
						 b3_funct();
					 }
					 if ((EEPROM_read_word_int(7))==sending)
					 {
						 b4_funct();
					 }
				 }
				 
				 
				 
          
				 TCCR1B |= (1<<CS11);
			      TCCR1B |= (1<<CS10); 
				 GICR  &=~(1<<INT0);
			 }
			 read_byte--;
		 }
		 
	 }


 }
 
 