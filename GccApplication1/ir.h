#ifndef IR_H_
#define IR_H_
#include "mine.h"
void read_start_bit(void);
void resset_flags(void);
void read_EEPROM_memory(void);
void resset_memory(void);
void EEPROM_write_word_int(unsigned int uiAddress, uint16_t ucData);
#endif /* IR_H_ */