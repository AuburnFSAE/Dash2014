#include "wems.h"
//#include "clock.h"
#include "wems_EcuComms.h"

#ifndef WEMSINTERRUPT_H
#define WEMSINTERRUPT_H

////[ITC]/////////////////////////////////////////////
// interrupt timer counters used to display a visual 
// of the interrupt actually interrupting, The ITCPin 
// can be tied to an Led or Osciliscope for instance
//////////////////////////////////////////////////////

	extern volatile bool ITCtggl;
	extern si16 ITCPin;
	extern volatile ui32 ITC;//Indicator timer
	extern ui32 ITCCV;//Indicator timer compare value
	extern ui32 ITCCPS;//Indicator timer prescale value

extern WemsEcuBufferBlock* interrupt_blk;

void StartInterrupt(ui32 buadrate);

void pause_interrupts();
void resume_interrupts();

void Interrupt_Void();

extern void (*wems_Intterupt_Run)();

//extern volatile Clock clock;

//ui32 ClockTime(Clock* c);
#endif
