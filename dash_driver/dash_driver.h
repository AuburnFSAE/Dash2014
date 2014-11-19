#ifndef Dash_Driver_H
#define Dash_Driver_H

#include "wems.h"


/// @cond [Channel and Gauge Keys]///////////////////////////////////////////// 
    extern	si16 OPGauge;
    extern	si16 LAGauge;
    extern	si16 BVGauge;
    extern	si16 ETGauge;
    extern	si16 GRGauge;
/// @endcond



    #include "gauge.h"    

    extern  const si16 NUMGAUGES;
    extern	Gauge gauge_list[];

    extern	si16 genie_baud_rate;

    extern	si16 RPM;
    extern	si16 ET;
    extern	si16 OP;
    extern	si16 BV;
    extern	si16 LA;
    extern	si16 G;

//previous values used for error checking
    extern	si16 pBV;
    extern	si16 pET;
    extern	si16 pOP;
    extern	si16 pRpm;
    extern	si16 pLa;
    extern	si16 pG;


/// @cond [RPM and ECUBUFFER] /////////////////////////////////
    extern si16 ecu_channel_count;
    extern WemsEcuBufferBlock* _ecu_blk;
    extern ui32 ecu_buad_rate;
    extern si16 ecu_comm_pin;

    extern wems_rpm_block rpm_blk;
    extern const si16 pin_count;
    extern si16 pins[];
    extern si16 rpm_table[];
/// @endcond


/** @var flags. /////////////////////////////////////////////////
    Warning flags that are triggerd by certian values when a flag is set the 
    alert pattern is processed. The type of alert can be sey by the ALERTS enum
*/  
    extern bool flags;

    enum ALERTS{
        Null=0,
        HIGH_ET=1<<0,
        LOW_OP=1<<1
    };

/** @var alert_pattern
    An alert pattern is the parttern of lights that is toggled when 
    an alert is set. It toggles the corresponging pins set by alert_pattern
    for instance 

    if alert_pattern=(binary)0011; then the 0 and 1 pins of
    the rpm pins will be toggled in a warning pattern. if 

    if alert_pattern=(binary)1101; then pins; 0,2,3 will be toggled

    **assuming 0 based index and right is lsb.
*/
    extern const ui8 alert_pattern;
    extern const ui8 alert_pin_count;

// @var alert_cycle
//This is used so that the alerts only run at half the rate of the 
//main cycle, which is about 50hz, so the alert_cycle is about 25hz.
//This allows a more viewable flash pattern of the lights.
    extern const si16 alert_cycle_count;

#include "alerts.h"
extern Alert alert;

/** @enum RunMode
    @breif Different modes for the processor to run ass

    @var RunMode::MAIN
    normal run mode

    @var RunMode::EMULATOR
    //Pushes data from pc as an emulator
*/
enum RunMode{
    NONE        =0,
    MAIN        =1,//normal 
    EMULATOR    =2,//Sending fake ecu from pc
    PORTFORWARD =3,//Sending ecu to pc
    FlAGSET     =4 //Changing the flag conditions
}extern driver_mode;

//Device control bytes
#define STX 0x02
#define ACK 0x06
#define ENQ 0x05
#define SPC 0x20
#define SOH 0x01


extern const ui8 PacketLength;

extern ui8 packet[];

void dash_driver_setup();

extern void (*driver)(void);

void dash_driver_core();

void dash_driver_main();
void dash_driver_emulator();
void dash_driver_portforward();

extern void(*dash_write)(void*, si16);
bool driver_init(void (*n_driver)(void), void(*writer)(void*, si16));

#include "dash_write.h"
extern ui32 reset_mills;
extern ui32 time_ref;
    
#endif
