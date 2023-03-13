/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* 自作ライブラリ */
#include "work.h"

/*-----------------------------------------------------------*/

#include "sample_vTaskDelayUntil.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );

/*-----------------------------------------------------------*/

void sample_vTaskDelayUntil( void )
{
	prvCreateTasks();

	myTrace_Initialize();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateTasks( void )
{
	static TaskHandle_t xATask;
	
	xTaskCreate( prvATask, "A", configMINIMAL_STACK_SIZE + 2048, "A", configMAX_PRIORITIES - 2, &xATask );
}
/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );
	TickType_t xLastWakeTime;
	TickType_t xGetTime;
	TickType_t xPrevGetTime;
	int loop = 0;

	xLastWakeTime = xTaskGetTickCount();
	xGetTime = xLastWakeTime;
	xPrevGetTime = xGetTime;

	for( ;; )
	{
		vTaskDelayUntil( &xLastWakeTime, xCycleFrequency );
		xGetTime = xTaskGetTickCount();

		taskENTER_CRITICAL();
		{
			printf( "xLastWakeTime = %ld, xGetTime = %ld, xPrevGetTime = %ld\r\n", xLastWakeTime, xGetTime, xPrevGetTime );
			printf( "Task %s Runing...\r\n", pStr);
		}
		taskEXIT_CRITICAL();

		loop++;
		if( loop == 3 ){
			work( pdMS_TO_TICKS( 110UL ) );
		}else if( loop >= 6 ){
			work( pdMS_TO_TICKS( 210UL ) );
			loop = 0;
		}else{
			work( pdMS_TO_TICKS( 10UL ) );
		}

	 	xPrevGetTime = xTaskGetTickCount();
	}
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	vAssertCalled( __LINE__, __FILE__ );
}

/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	( void ) ulLine;
	( void ) pcFileName;

	taskENTER_CRITICAL();
	{
		printf( "ASSERT! Line %ld, file %s\r\n", ulLine, pcFileName );

		while( ulSetToNonZeroInDebuggerToContinue == 0 )
		{
#ifdef _MSC_VER
			__nop();
			__nop();
#else /* #ifdef _MSC_VER */
			__asm volatile( "NOP" );
			__asm volatile( "NOP" );
#endif /* #ifdef _MSC_VER */
		}
	}
	taskEXIT_CRITICAL();
}
