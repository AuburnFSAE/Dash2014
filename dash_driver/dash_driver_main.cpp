/* 
* @Author: CLS
* @Date:   06-06-2014 00:46:52
* @Last Modified by:   CLS
* @Last Modified time: 10-06-2014 01:51:28
*/

#include "dash_driver.h"
si16 write_count=0;
void dash_driver_main(){
	
	if(_ecu_blk->scan_complete){
		dash_driver_core();

		//if(write_count++%5000==0){   
			dash_write(_ecu_blk, NUMGAUGES);
			write_count=0;
		//}
		
		delay(5);

	_ecu_blk->scan_complete=false;}

return;}
