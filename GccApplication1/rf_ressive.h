#include "mine.h"
#ifndef RF_RESIVE_H_
#define RF_RESIVE_H_
#define F_CPU 12000000UL


void resive_433(void);
void Start_timer_433(void);
void Stop_timer_433(void);
 
 void init_timer2_433(void);
 void resset_flags_433(void);


#endif /* RF_RESIVE_H_ */