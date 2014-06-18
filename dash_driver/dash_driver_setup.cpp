#include "dash_driver.h"


ui32 reset_mills=20;
ui32 time_ref=0;

si16 OPGauge    =0x04;
si16 LAGauge    =0x03;
si16 BVGauge    =0x02;
si16 ETGauge    =0x01;
si16 GRGauge    =0x00;
const si16 NUMGAUGES=5;
Gauge gauge_list[NUMGAUGES];


si16 RPM=0;
si16 ET=1;
si16 OP=2;
si16 BV=3;
si16 LA=4;
si16 G=5;

//previous values used for error checking
si16 pBV=0;
si16 pET=0;
si16 pOP=0;
si16 pRpm=0;
si16 pLa=0;
si16 pG=0;

si16 genie_baud_rate=9600;

si16 ecu_channel_count=6;
WemsEcuBufferBlock* interrupt_blk=0;
WemsEcuBufferBlock* _ecu_blk=0;
ui32 ecu_buad_rate=38400;
si16 ecu_comm_pin=12;
ui32 ircntr=0;

wems_rpm_block rpm_blk;
const si16 pin_count=3;
si16 pins[pin_count]={2,3,4};
si16 rpm_table[pin_count]={7000, 10500, 12000};
//si16 rpm_table[pin_count]={3000,5000,8000};


//An alert pattern is the parttern of lights that is toggled when 
// an alert is set. It toggles the corresponging pins set by alert_pattern
// for instance 
//
// if alert_pattern=(binary)0011; then the 0 and 1 pins of
// the rpm pins will be toggled in a warning pattern. if 
//
// if alert_pattern=(binary)1101; then pins; 0,2,3 will be toggled
    const  ui8  alert_pattern= 0x01;
    const  ui8  alert_pin_count=2;
    const si16  alert_cycle_count=0;
//This is used so that the alerts only run at half the rate of the 
//main cycle, which is about 50hz, so the alert_cycle is about 25hz.
//This allows a more viewable flash pattern of the lights.
Alert alert;


//@cond FUN 
RunMode driver_mode=NONE;

////Function pointers////////////////////////////////////
void (*driver)(void)=&dash_driver_main;
void(*dash_write)(void*, si16)=&do_nothing_write;
//@endcond


//packet and PacketLength are used for terminal communication while in EMULATOR and PORTFORWARD mode
const ui8 PacketLength=4;
ui8 packet[PacketLength]={0,0,0,0};


bool driver_init(void (*n_driver)(void), void(*writer)(void*, si16)){
    if( n_driver==0 || writer==0 ){
        return false;
    }

    driver=n_driver;
    dash_write=writer;
return true;}

void dash_driver_setup(){

//Set gauge map that maps an ecu channel to the HIGH 16 and the printing value to the low 16
    gauge_list[OPGauge]=Gauge::Create(OPGauge, OP, 3, 10);
    gauge_list[LAGauge]=Gauge::Create(LAGauge, LA);
    gauge_list[BVGauge]=Gauge::Create(BVGauge, BV, 1, 10);
    gauge_list[ETGauge]=Gauge::Create(ETGauge, ET, 0, 10);
    gauge_list[GRGauge]=Gauge::Create(GRGauge, G);

    pinMode(ecu_comm_pin, INPUT);

//Rpm and Ecu block allocations
    _ecu_blk=wems_EcuComms_Init(ecu_channel_count, ecu_comm_pin);
    _ecu_blk->object_data = (void*)gauge_list;
    interrupt_blk=_ecu_blk;

    wems_rpm_Init(&rpm_blk, pins, rpm_table, pin_count);


//Create the alert structure for warnings
    alert=wems_alert_Create(alert_pattern, alert_pin_count);


//check for connection for about 2~3 seconds
    TERM.begin(9600);

    si32 start_wait_time=millis();

    ui16 DENQ= (ENQ<<8) | ENQ;    
    do{
        for(si16 i=0; i<rpm_blk.count;i++){
                digitalWrite(rpm_blk.pins[i], HIGH);
                digitalWrite(ITCPin, HIGH);
                delay(80);
 
                digitalWrite(ITCPin, LOW);
                digitalWrite(rpm_blk.pins[i], LOW);
                delay(80);               
        }
        
        //Check for double ack knowledge ([ACK][ACK]) to assert something trying to establish
        //a connection
        if(TERM.available() >= PacketLength){
            TERM.readBytes((si8*)packet, PacketLength);

            if( *((ui16*)packet) == DENQ ){
                ui32 term_buad=0;

                //read the buad rate
                switch(packet[2]){
                    case 19:{
                        term_buad=19200;
                    }break;
                    
                    case 38:{
                        term_buad=38400;
                    }break;
                    
                    case 57:{
                        term_buad=57200;
                    }break;

                    case 115:{
                        term_buad=115200;
                    }break;

                    case 9:
                    default:{
                        term_buad=9600;
                    }break;
                }

                //read the runmode
               driver_mode=(RunMode)packet[3];

               //send double ACK with channel information and reset terminal
                packet[0]=ACK;
                packet[1]=ACK;
                packet[2]=_ecu_blk->channel_count;

                //if PORTFORWARD then the driver will communicate the gauges and an errof flag channel
                //so increment the ecu_channel count 
                if(driver_mode==PORTFORWARD)
                    packet[2]++;

                packet[3]=sizeof(si16);

                TERM.write(packet, 4);
                delay(1000);

                TERM.end();
                TERM.begin(term_buad);
            }
        }
    }while( driver_mode==NONE && millis()-start_wait_time < 2000);

    //Select the driver mode
    switch( driver_mode){
        default:
        case NONE:
        case MAIN:{
            GENIETERM.begin(genie_baud_rate);
            
            genieBegin(GENIETERM);
                
            
            driver_init(dash_driver_main, genie_write);

            StartInterrupt(ecu_buad_rate);
        }break;

        case PORTFORWARD:{
            _ecu_blk->object_data = (void*)gauge_list;

            driver_init(&dash_driver_portforward, &portforward_write);
            
            StartInterrupt(ecu_buad_rate);     
        }break;

        case EMULATOR:{

            driver_init(dash_driver_emulator, do_nothing_write);
        }break;        
    }


return;}

