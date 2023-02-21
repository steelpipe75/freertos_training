/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"

/*-----------------------------------------------------------*/

#include "MyTrace.h"

/*-----------------------------------------------------------*/

static char* pcInTaskNameStr = NULL;
static char* pcOutTaskNameStr = NULL;
static volatile TickType_t xTick = 0;

void myTrace_TaskSwitchedIn( char* pcArg ){
    pcInTaskNameStr = pcArg;
    /* printf( "myTrace_TaskSwitchedIn = %s\r\n", pcArg ); */
    if( (pcInTaskNameStr != NULL) && (pcOutTaskNameStr != NULL) ){
        if( pcInTaskNameStr != pcOutTaskNameStr ){
            printf( "Tick = %lu, OUT = %s, IN = %s\r\n", xTick, pcOutTaskNameStr, pcInTaskNameStr );
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

