#include "wems_Interrupt.h"

volatile bool ITCtggl=false;
si16 ITCPin=13;

//Indicator timer
volatile ui32 ITC=0;

//Indicator timer compare value
ui32 ITCCV=0;
ui32 ITCCPS=1;

//Clock
//volatile Clock clock;

void Interrupt_Void(){return;}

void (*wems_Intterupt_Run)() = &Interrupt_Void;


//some counters to toggle the itcpin in an extended heartbeat pattern
void heartbeat(){
//  clock.count=ITC;

  if(ITC==0 || ITC==4*ITCCPS || ITC==10*ITCCPS){
    digitalWrite(ITCPin, HIGH);

  }else if(ITC==2*ITCCPS || ITC==6*ITCCPS || ITC==15*ITCCPS){
    digitalWrite(ITCPin, LOW);
  }

  if(++ITC==ITCCV){
    ITC=0; 
  //  clock.period++;
  }
return;}

void ITCInit(ui32 buad){
      ITCCV=buad;
      ITCCPS=buad/24;
      ITC=0;
      pinMode(ITCPin, OUTPUT);

      // clock.period=0;
      // clock.count=0;
return;}

#ifdef WAVR 
  ISR(TIMER1_COMPA_vect){
        wems_EcuComms_Action(interrupt_blk);
        heartbeat();
  return;}


  void StartInterrupt(ui32 buad){

      ITCCV=buad;
      ITCCPS=buad/24;
      ITC=0;
      pinMode(ITCPin, OUTPUT);

      cli();//stop interrupts 

      //Using the 16bit timer interrupt with a prescale of one
      TCCR1A = 0;// set entire TCCR1A register to 0
      TCCR1B = 0;// same for TCCR1B
      TCNT1  = 0;//initialize counter value to 0

      // turn on CTC mode
      TCCR1B |= (1 << WGM12);

      // Set CS12 and CS10 bits for 1 prescaler
      TCCR1B |=  (1<<CS10);  

      OCR1A = F_CPU/(buad) - 1;//32 because prescaler    

      // enable timer compare interrupt
      TIMSK1 |= (1 << OCIE1A);

      sei();//allow interrupts
  return;}

  void pause_interrupts(){
    cli();
    TCCR1B = 0;
    sei();
  return;}

  void resume_interrupts(){
    ui32 _temp=ITC;
    StartInterrupt(ITCCV);
    ITC=_temp;
  return;}
#endif



#ifdef WSAM

  void TC3_Handler(){
        TC_GetStatus(TC1, 0);
          
        wems_EcuComms_Action(interrupt_blk);

        heartbeat();
  return;}

  void StartInterrupt(ui32 buad){

////////////[Replaced by ITCInit]/////////////////
        // ITCCV=buad;
        // ITCCPS=buad/24;
        // ITC=0;
        // pinMode(ITCPin, OUTPUT);
////////////[Replaced by ITCInit]/////////////////

        ITCInit(buad);

        ui32 channel=0;
        IRQn_Type irq=TC3_IRQn;

        pmc_set_writeprotect(false);
        pmc_enable_periph_clk((uint32_t)irq);
        TC_Configure(TC1, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);

        ui32 rc = VARIANT_MCK/128/buad; //128 because we selected TIMER_CLOCK4 above
        TC_SetRA(TC1, channel, rc/2); //50% high, 50% low
        TC_SetRC(TC1, channel, rc);
        TC_Start(TC1, channel);

        TC1->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
        TC1->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
        NVIC_EnableIRQ(irq);
  return;}

  void pause_interrupts(){return;}

  void resume_interrupts(){return;}
#endif


// ui32 ClockTime(Clock* c){
//       ui32 time=1000*c->period + (1000*c->count)/ITCCV;
//       c->period=0;
//       c->count=0;
// return time;}
