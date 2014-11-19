/* 
* @author: CLS
* @Date:   2014-06-04 02:34:39
* @Last Modified by:   CLS
* @Last Modified time: 2014-06-04 02:49:16
*/

#include "dash_driver.h"

void dash_driver_portforward(){
	dash_driver_core();

	if(_ecu_blk->scan_complete){
		pause_interrupts();

		dash_write(_ecu_blk, NUMGAUGES);
		
		resume_interrupts();
		_ecu_blk->scan_complete=false;
	}
		
return;}
