#include "wems.h"
#include "wems_rpm_alloc.h"
#include "wems_EcuComms.h"
#include "genieArduino.h"
 

///[RPM and ECUBUFFER]/////////////////////////////////
    si16 ecu_channel_count=6;
    WemsEcuBufferBlock* _ecu_blk;
    si32 ecu_buad_rate=38400;
    si16 ecu_comm_pin=11;

    wems_rpm_block* _rpm=(wems_rpm_block*) malloc(sizeof(wems_rpm_block));
    const si16 pin_count = 3;
    si16 pins[pin_count] = {2,3,4};
    si16 rpm_table[3]={7000, 10500, 12500};
    bool tggl=0;

///[Genie]/////////////////////////////////////////////
    si16 OilGuage = 0x04;
    si16 LamGuage =0x03;
    si16 BattGuage=0x02;
    si16 ETGuage=0x01;
    si16 GearGuage=0x00;

    si16 genie_baud_rate=9600;

    si16    RPM=0;
    si16    ET=1;
    si16    OP=2;
    si16    BV=3;
    si16    LA=4;
    si16    G=5;

    si16 pBV=0;
    si16 pET=0;
    si16 pOP=0;
    si16 pRpm=0;
    si16 pLa=0;
    si16 pG=0;
///[FLAGS]/////////////////////////////////
/**
Warning flag that are triggerd by certian values when a flag is set the 
alert pattern is processed. 
*/  
    ui8 flags=0;
    enum ALERTS{
        Null=0,
        HIGH_ET=1<<0,
        LOW_OP=1<<1
    };
    //An alert pattern is the parttern of lights that is toggled when 
    // an alert is set. It toggles the corresponging pins set by alert_pattern
    // for instance 
    //
    // if alert_pattern=(binary)0011; then the 0 and 1 pins of
    // the rpm pins will be toggled in a warning pattern. if 
    //
    // if alert_pattern=(binary)1101; then pins; 0,2,3 will be toggled
    ui8 alert_pattern= 0x01;
    ui8 alert_pin_count=2;

    //This is used so that the alerts only run at half the rate of the 
    //main cycle, which is about 50hz, so the alert_cycle is about 25hz.
    //This allows a more viewable flash pattern of the lights.
    bool alert_cycle=false;
    si16 alert_cycle_reset_count=40;
    si16 alert_gauge0=0;
    si16 alert_gauge3=0;


//TC1 ch 0
void TC3_Handler(){
        TC_GetStatus(TC1, 0);
        wems_EcuComms_Action(_ecu_blk);
return;}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
        uint32_t rc = VARIANT_MCK/128/frequency; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(tc, channel, rc); //50% high, 50% low
        TC_SetRC(tc, channel, rc);
        TC_Start(tc, channel);
        tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
return;}

void setup(){
        genieBegin (GENIE_SERIAL_1, genie_baud_rate);

         //while(!GENIE_SERIAL_1);
  
        //Set rpm map
        wems_rpm_alloc( _rpm, pins, pin_count);

        _rpm->map=rpm_table;
        _rpm->map_size=3;   

        pinMode(ecu_comm_pin,INPUT);        
        _ecu_blk = wems_EcuComms_Init(ecu_channel_count, ecu_comm_pin);
        startTimer(TC1, 0, TC3_IRQn, ecu_buad_rate); //TC1 channel 0, the IRQ for that channel and the desired frequency

      //  wems_rpm_reset(_rpm);
return;}


void loop(){
    if( _ecu_blk->scan_complete ){

        if(_ecu_blk->ecu_channel_count==ecu_channel_count){
            
            flags=0;

            ui16 value=0;
            
            value=(ui16)(_ecu_blk->ecu_stream.data[G]);

            if(pG==0 || (abs(pG-value)<2 && value < 5){
                genieWriteObject(GENIE_OBJ_LED_DIGITS, GearGuage, value);
                pG=value;
            }

 ////[Batt]////////////////////////////////////////////// 
            value=(ui16)(_ecu_blk->ecu_stream.data[BV])/10;
            if(value < 150 && (pBV==0 || abs(value-pBV) < 20)){
                genieWriteObject(GENIE_OBJ_LED_DIGITS, BattGuage, value);
                pBV=value;
            }

 ////[ET]//////////////////////////////////////////////  
            value=(ui16)(_ecu_blk->ecu_stream.data[ET])/10;
            if(pET==0 || abs(value-pET) < 3){
                genieWriteObject(GENIE_OBJ_LED_DIGITS, ETGuage, value);
                if( value > 100 ){
                    flags |= HIGH_ET;    
                }
                pET=value;
            }

 ////[Lam]//////////////////////////////////////////////           
            value=(ui16)(_ecu_blk->ecu_stream.data[LA]/10);
            if(value<200 && (pLa==0|| abs(pLa-value)<80)){          
                genieWriteObject(GENIE_OBJ_LED_DIGITS, LamGuage, value);
                pLa=value;
            }


////[OIL PRESSURE]//////////////////////////////////////////////
            value=(ui16)(_ecu_blk->ecu_stream.data[OP]);
            if(pOP==0 || abs(value-pOP) < 500){
                genieWriteObject(GENIE_OBJ_LED_DIGITS, OilGuage, value);
                if( value < 10  && pRpm > 2000 ){
                   flags |= LOW_OP;    
                }

                pOP=value;
            }


////////[RPM]////////////////////////////////////////////////
            ui16 rpm_val=_ecu_blk->ecu_stream.data[RPM];
            if(pRpm==0  || (abs(pRpm-rpm_val)<1000 && rpm_val>2000)){

                if(rpm_val > _rpm->map[0]) wems_rpm_set_lightsmap(_rpm, rpm_val);
                
                if(rpm_val > (_rpm->map[pin_count-1]+499)) pulse(_rpm->pins[pin_count-1]);                

                pRpm=rpm_val;
            }

////////[ALERTS]//////////////////////////////////////////////
            if(alert_cycle_reset_count==1){
                alert_cycle_reset_count=-40;

            }else if (alert_cycle_reset_count==-1){
                alert_cycle_reset_count=40;

            }else if(alert_cycle_reset_count < 0){
                alert_cycle_reset_count++;
            }

///[FLAGS]////////////////////////////////////////////////////                        
            if( flags == 0 ){
                wems_rpm_reset(_rpm);

                if( alert_gauge0 ){
                    alert_gauge0=0;
                    genieWriteObject(GENIE_OBJ_GAUGE, 0, alert_gauge0);
                }

                if( alert_gauge3 ){
                    alert_gauge3=0;
                    genieWriteObject(GENIE_OBJ_GAUGE, 3, alert_gauge3);
                }
            }

            if(flags && alert_cycle_reset_count >0){
                alert_cycle_reset_count--;
                if( alert_cycle=!alert_cycle,alert_cycle ){
                    alert_pattern= ~alert_pattern;

                    pulse(_rpm->pins, alert_pin_count, alert_pattern);


                    if(flags & HIGH_ET && alert_gauge0==0){
                        alert_gauge0=100;
                        genieWriteObject(GENIE_OBJ_GAUGE, 0, alert_gauge0);
                    }

                    if(flags & LOW_OP && alert_gauge3==0){
                        alert_gauge3=100;
                        genieWriteObject(GENIE_OBJ_GAUGE, 3, alert_gauge3);
                    }
                }
            }
        }

        delay(20);       
    _ecu_blk->scan_complete=0;}
return;}

void pulse(si16 pin){
    digitalWrite(pin,(tggl=!tggl, tggl)?HIGH:LOW);
}

void pulse(const si16* pins, si16 pin_count, ui8 tggl_flags){
    for( si16 i=0; i<pin_count; i++){
        digitalWrite(pins[i], (tggl_flags & 1<<i)?HIGH:LOW);
    }
return;}
