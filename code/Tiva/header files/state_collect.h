/*
 * start_collect.h
 *
 *  Created on: 04-Feb-2016
 *      Author: saurav
 *
 *      Description: This header file declare all required header files.
 *      Usage:  #include "commonheader.h" in any C file will inlcude all header file declared here in that source file.
 */

#ifndef STATE_COLLECT_H_
#define STATE_COLLECT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//#include "inc/tm4c123gh6pm.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include <time.h>
#include <inc/hw_gpio.h>
#include "driverlib/ssi.h"	
#include "sensor.c" 
#include "collect.c"	// SP

#endif /* STATE_COLLECT_H_ */
