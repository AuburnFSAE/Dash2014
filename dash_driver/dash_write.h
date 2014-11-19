#ifndef DRIVERWRITE_H
#define DRIVERWRITE_H

void do_nothing_write(void* gauges_or_ecublk, si16 numgauges);

void portforward_write(void* ecublk, si16 numgauges);

void genie_write(void* gauges, si16 numgauges);

#endif
