#ifndef MINE_H_
#define MINE_H_
#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "ir.h"
#include "rf_ressive.h"
#include "usart.h"
#define nop()  asm("nop")
#define  CONNECT_BYTE_433 0
#define  ADRESS_BYTE_433  1
#define  DATA_BYTE_433    2
#define  STOP_BYTE_433    3
#define  CONNECT_BYTE_433_0() (flag_433    &=~  (1<<CONNECT_BYTE_433))
#define  CONNECT_BYTE_433_1() (flag_433    |=   (1<<CONNECT_BYTE_433))
#define  ADRESS_BYTE_433_0()  (flag_433    &=~  (1<<ADRESS_BYTE_433))
#define  ADRESS_BYTE_433_1()  (flag_433    |=   (1<<ADRESS_BYTE_433))
#define  ADATA_BYTE_433_0()   (flag_433    &=~  (1<<DATA_BYTE_433))
#define  DATA_BYTE_433_1()    (flag_433    |=   (1<<DATA_BYTE_433))
#define  STOP_BYTE_433_0()    (flag_433    &=~  (1<<STOP_BYTE_433))
#define  STOP_BYTE_433_1()    (flag_433    |=   (1<<STOP_BYTE_433))
#define  READ_FLAG_433(f)     (flag_433 &(1<<f))
#endif /* MINE_H_ */