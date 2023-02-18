/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/

#include "sample_xTaskDelayUntil.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );

/*-----------------------------------------------------------*/

void sample_xTaskDelayUntil( void )
{
	prvCreateTasks();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateTasks( void )
{
	static TaskHandle_t xATask;
	
	xTaskCreate( prvATask, "A", configMINIMAL_STACK_SIZE, "A", configMAX_PRIORITIES - 2, &xATask );
}
/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );
	TickType_t xLastWakeTime;
	TickType_t xGetTime;
	BaseType_t xWasDelayed;

 	xLastWakeTime = xTaskGetTickCount();
	xGetTime = xLastWakeTime;

	for( ;; )
	{
		xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xCycleFrequency );

		printf( "xLastWakeTime = %ld, xGetTime = %ld, xWasDelayed = %ld\r\n", xLastWakeTime, xGetTime, xWasDelayed);
		printf( "Task %s Runing...\r\n", pStr);

	 	xGetTime = xTaskGetTickCount();
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

	printf( "ASSERT! Line %ld, file %s\r\n", ulLine, pcFileName );

 	taskENTER_CRITICAL();
	{
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
