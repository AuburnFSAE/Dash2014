/* 
* @Author: CLS
* @Date:   27-05-2014 00:25:15
* @Last Modified by:   CLS
* @Last Modified time: 14-06-2014 00:16:17
*/
#include "dash_driver.h"
si16 pulse_count=0;
void dash_driver_core(){
            si16 value=0;

            value=(_ecu_blk->ecu_stream.data[G]);

            if( (abs(pG-value)<3 && value < 5) ){
                pG=value;
            }else{
                _ecu_blk->ecu_stream.data[G]=pG;
            }

//@cond DriverMainTags
////[Batt]////////////////////////////////////////////// 
            value=(_ecu_blk->ecu_stream.data[BV]);
            if( value<1450 && (pBV==0 || abs(value-pBV)<150)){
                pBV=value;
                if(value < 1150)
                    gauge_list[BVGauge].is_alert=true;
                else
                    gauge_list[BVGauge].is_alert=false;   

            }else{
                _ecu_blk->ecu_stream.data[BV]=pBV;
            }


////[ET]//////////////////////////////////////////////  
            value=(_ecu_blk->ecu_stream.data[ET]);
            if(pET==0 || abs(value-pET)<30 && value<1200){
                if( value > 1100 ){
                    gauge_list[ETGauge].is_alert=true;   
                }else{
                    gauge_list[ETGauge].is_alert=false;
                }

                pET=value;        
            }else{
                _ecu_blk->ecu_stream.data[ET]=pET;
            }


////[Lam]//////////////////////////////////////////////           
            value=(_ecu_blk->ecu_stream.data[LA]);
            if(pLa==0 || abs(pLa-value)<1000){          
                pLa=value; 
            }else{
                _ecu_blk->ecu_stream.data[LA]=pLa;
            }



////[RPM]////////////////////////////////////////////////
            ui16 rpm_val=_ecu_blk->ecu_stream.data[RPM];
            if(rpm_val > 1500){

                if(rpm_val>rpm_blk.min_rpm){

                    wems_rpm_set_lights(&rpm_blk, rpm_val);
                    if(rpm_val > (rpm_blk.max_rpm+499)){ 
                        wems_rpm_pulsepin(&rpm_blk, 2, 3);
                    };
                }else{
                    wems_rpm_set_lights(&rpm_blk, 0);
                }

                pRpm=rpm_val;
            }else{
                _ecu_blk->ecu_stream.data[RPM]=pRpm;
                wems_rpm_set_lights(&rpm_blk, 0);
            }


////[OIL PRESSURE]//////////////////////////////////////////////
            value=(_ecu_blk->ecu_stream.data[OP]);
            if(pOP==0 || abs(value-pOP) < 500){
                if( value < 60  && pRpm > 2000 ){
                   gauge_list[OPGauge].is_alert=true; 
                }else{
                    gauge_list[OPGauge].is_alert=false;
                }

                pOP=value;               
            }


////[ALERTS]//////////////////////////////////////////////
            alert=wems_alert_Process(alert, &rpm_blk, gauge_list, NUMGAUGES);

//@endcond
return;}
