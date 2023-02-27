#ifndef MY_TRACE_H
#define MY_TRACE_H

extern void myTrace_TaskSwitchedIn( char* pxCurrentTCB );
extern void myTrace_TaskSwitchedOut( char* pxCurrentTCB );
extern void myTrace_IncrementTick( volatile void* pxTickCount );

#define traceTASK_SWITCHED_IN()                     myTrace_TaskSwitchedIn( pxCurrentTCB->pcTaskName )
#define traceTASK_SWITCHED_OUT()                    myTrace_TaskSwitchedOut( pxCurrentTCB->pcTaskName )
#define traceTASK_INCREMENT_TICK( xTickCount )      myTrace_IncrementTick( &xTickCount )

#endif /* MY_TRACE_H */
