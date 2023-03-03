/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"

/*-----------------------------------------------------------*/

#include "MyTrace.h"

/*-----------------------------------------------------------*/

static int MyTraceOutputEnable = 0;

static char* pcInTaskNameStr = NULL;
static char* pcOutTaskNameStr = NULL;
static volatile TickType_t xTick = 0;

void myTrace_Initialize( void ) {
    double Timescale = 1000.0;
    Timescale /= configTICK_RATE_HZ;
    fprintf( stderr, "{ \"Type\" : \"Initialize\", \"Data\" : \"{ \\\"Timescale\\\" : %lf }\" }\r\n", Timescale );
    MyTraceOutputEnable = 1;
}

void myTrace_TaskSwitchedIn( char* pcArg ){
    pcInTaskNameStr = pcArg;
    /* printf( "myTrace_TaskSwitchedIn = %s\r\n", pcArg ); */
    if( (pcInTaskNameStr != NULL) && (pcOutTaskNameStr != NULL) ){
        if( pcInTaskNameStr != pcOutTaskNameStr ){
            if( MyTraceOutputEnable == 1 ) {
                fprintf( stderr, "{ \"Type\" : \"TaskSwitch\", \"Data\" : \"{ \\\"Tick\\\" : %lu, \\\"OUT\\\" : \\\"%s\\\", \\\"IN\\\" : \\\"%s\\\" }\" }\r\n", xTick, pcOutTaskNameStr, pcInTaskNameStr );
            }
        }
    }else if( pcInTaskNameStr != NULL ){
        if( MyTraceOutputEnable == 1 ) {
            fprintf( stderr, "{ \"Type\" : \"TaskSwitch\", \"Data\" : \"{ \\\"Tick\\\" : %lu, \\\"IN\\\" : \\\"%s\\\" }\" }\r\n", xTick, pcInTaskNameStr );
        }
    }
}

void myTrace_TaskSwitchedOut( char* pcArg ){
    pcOutTaskNameStr = pcArg;
    /* printf( "myTrace_TaskSwitchedOut = %s\r\n", pcArg ); */
}

void myTrace_IncrementTick( volatile void* pxArg ){
    volatile TickType_t *pxTickCount = (volatile TickType_t *)pxArg;
    xTick = *pxTickCount;
    /* printf( "myTrace_IncrementTick = %lu\r\n", *pxTickCount ); */
}

