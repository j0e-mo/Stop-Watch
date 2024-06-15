/*
 * _Mini_Project_2.h
 *
 *  Created on: May 12, 2024
 *      Author: Joe Metwally
 */

#ifndef MINI_PROJECT_2_H_
#define MINI_PROJECT_2_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void Seven_Segment_Init(void);
void Timer1_CTC_Init(void);
void Enable_Extern_Interrupts(void);

#endif /* MINI_PROJECT_2_H_ */
