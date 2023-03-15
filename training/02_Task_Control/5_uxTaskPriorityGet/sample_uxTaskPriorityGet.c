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

#include "sample_uxTaskPriorityGet.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );
static void prvBTask( void *pvParameters );

/*-----------------------------------------------------------*/

static TaskHandle_t xATask;
static TaskHandle_t xBTask;

/*-----------------------------------------------------------*/

void sample_uxTaskPriorityGet( void )
{
	prvCreateTasks();

	myTrace_Initialize();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateTasks( void )
{
	xTaskCreate( prvATask, "A", configMINIMAL_STACK_SIZE + 2048, "A", configMAX_PRIORITIES - 3, &xATask );
	xTaskCreate( prvBTask, "B", configMINIMAL_STACK_SIZE + 2048, "B", configMAX_PRIORITIES - 4, &xBTask );
}
/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	UBaseType_t uxTaskBPriority;

	for( ;; )
	{
		uxTaskBPriority = uxTaskPriorityGet( xBTask );

		taskENTER_CRITICAL();
		{
			const char* pStr = (const char*)pvParameters;
			printf( "Task %s Runing...\r\n", pStr );
		}
		taskEXIT_CRITICAL();

		vTaskPrioritySet( xBTask, uxTaskBPriority + 2 );

		work( pdMS_TO_TICKS( 1UL ) );
	}
}

/*-----------------------------------------------------------*/

static void prvBTask( void *pvParameters )
{
	UBaseType_t uxTaskBPriority;

	for( ;; )
	{
		uxTaskBPriority = uxTaskPriorityGet( NULL );

		taskENTER_CRITICAL();
		{
			const char* pStr = (const char*)pvParameters;
			printf( "Task %s Runing...\r\n", pStr );
		}
		taskEXIT_CRITICAL();

		vTaskPrioritySet( NULL, uxTaskBPriority - 2 );

		work( pdMS_TO_TICKS( 1UL ) );
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
