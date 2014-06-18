#ifndef GAUGE_H
#define GAUGE_H 
#include "wems.h"

    class Gauge{
    public:
        bool is_alert;
        bool is_active;
        ui8 alert_on;
        ui8 alert_off;
        ui8 id;
        si16 ecu_channel;
        si16 number;
        si16 scale;

        static Gauge Create(ui8 gid, si16 ecu_chan, ui8 gnum, si16 gscale){
            Gauge gauge;

            gauge.is_alert=false;
            gauge.alert_on=100;
            gauge.alert_off=0;                        
            gauge.is_active=false;

            gauge.scale=gscale;
            gauge.id=gid;
            gauge.ecu_channel=ecu_chan;
            gauge.number=gnum;
        return gauge;}


        static Gauge Create(ui8 gid, ui8 ecu_chan, ui8 gnum){
            Gauge gauge=Gauge::Create(gid, ecu_chan, gnum, 1);

            gauge.id=gid;
            gauge.ecu_channel=ecu_chan;
            gauge.number=gnum;
        return gauge;}


        static Gauge Create(ui8 gid, ui8 ecu_chan){
            Gauge gauge=Gauge::Create(gid, ecu_chan, -1, 1) ;

            gauge.id=gid;
            gauge.ecu_channel=ecu_chan;
        return gauge;}


        Gauge(){return;}
    };
#endif