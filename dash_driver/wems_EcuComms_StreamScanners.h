#ifndef EcuComms_STREAMSCANNERS
#define EcuComms_STREAMSCANNERS 

#include "wems_EcuComms_EcuBufferBlock.h"
#include <Arduino.h>

extern void (*wems_EcuComms_Action)(WemsEcuBufferBlock*);

WemsEcuBufferBlock* wems_EcuComms_Init(si16 n_channel_count, si16 n_comm_pin);

void wems_EcuComms_HeaderScan(WemsEcuBufferBlock*);

void wems_EcuComms_CountScan(WemsEcuBufferBlock*);

void wems_EcuComms_DataScan(WemsEcuBufferBlock*);

void wems_EcuComms_VoidScan(WemsEcuBufferBlock*);

void wems_EcuComms_ResetHeaderWait(WemsEcuBufferBlock*); 
#endif
