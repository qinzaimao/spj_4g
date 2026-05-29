#ifndef __CFGSAVE_H__
#define __CFGSAVE_H__

#include "main.h"

#define save_byte 250

extern struct elevator set;

void cfgSave();
void cfgRead();
void save_begin(void);
void cfgsave_thread_entry(void *parameter);


#endif /* __CFGSAVE_H__ */
