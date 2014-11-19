#ifndef WEMS

#define WEMS

#include "wems_typedef.h"
#include "wems_DataStream.h"
#include "wems_Interrupt.h"
#include "wems_rpm_alloc.h"
#include "wems_EcuComms.h"
#include "genieArduino.h"



//Set the Port and Terminal defines 
//Port is used to commicate with a device while
//Terminal is used to do pc communications
#if defined(__AVR_ATmega328P__) || defined(ARDUINO_ARCH_AVR)

	#include <SoftwareSerial.h>

	#define TERM Serial
	#define GENIETERM Serial
	#define WAVR 1
#elif defined(__SAM3X8E__) ||  defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_SAM_DUE)//due
	//Intended for Arduino due
	#define TERM Serial
 	#define GENIETERM Serial2
	#define WSAM 1
#endif




#endif

