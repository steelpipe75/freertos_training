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

#include "sample_vTaskDelay.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );

/*-----------------------------------------------------------*/

void sample_vTaskDelay( void )
{
	prvCreateTasks();

	srand(1);

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
	TickType_t xGetTime;
	TickType_t xPrevGetTime;

	xGetTime = xTaskGetTickCount();
	xPrevGetTime = xGetTime;

	for( ;; )
	{
		vTaskDelay( xCycleFrequency );
		xGetTime = xTaskGetTickCount();

		printf( "xPrevGetTime = %ld, xGetTime = %ld\r\n", xPrevGetTime, xGetTime );
		printf( "Task %s Runing...\r\n", pStr);

		work( pdMS_TO_TICKS( 10UL ) );

		xPrevGetTime = xGetTime;
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
