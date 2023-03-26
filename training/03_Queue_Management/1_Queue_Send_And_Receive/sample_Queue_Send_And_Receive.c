/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* 自作ライブラリ */
#include "work.h"

/*-----------------------------------------------------------*/

/* #define PRINT_TASK_RUNING_ENABLE */

/*-----------------------------------------------------------*/

#include "sample_Queue_Send_And_Receive.h"

/*-----------------------------------------------------------*/

static void prvCreateRtosResources( void );
static void prvPrintTask( void *pvParameters );
static void prvATask( void *pvParameters );
static void prvTask( void *pvParameters );

/*-----------------------------------------------------------*/

static QueueHandle_t xPrintQueue;

static TaskHandle_t xPrintTask;
static TaskHandle_t xATask;
static TaskHandle_t xBTask;
static TaskHandle_t xCTask;

/*-----------------------------------------------------------*/

void sample_Queue_Send_And_Receive( void )
{
	prvCreateRtosResources();

	myTrace_Initialize();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateRtosResources( void )
{
	xPrintQueue = xQueueCreate( 10, sizeof( char* ) );

	xTaskCreate( prvPrintTask, "Print", configMINIMAL_STACK_SIZE + 2048, "Print", configMAX_PRIORITIES - 2, &xPrintTask );
	xTaskCreate( prvATask, "A", configMINIMAL_STACK_SIZE + 2048, "A", configMAX_PRIORITIES - 3, &xATask );
	xTaskCreate( prvTask,  "B", configMINIMAL_STACK_SIZE + 2048, "B", configMAX_PRIORITIES - 4, &xBTask );
	xTaskCreate( prvTask,  "C", configMINIMAL_STACK_SIZE + 2048, "C", configMAX_PRIORITIES - 5, &xCTask );
}

/*-----------------------------------------------------------*/

static void prvPrintTask( void *pvParameters )
{
	( void ) pvParameters;
	char* pStr;

	for( ;; )
	{
		xQueueReceive( xPrintQueue, &( pStr ), ( TickType_t ) portMAX_DELAY );

		printf("Task %s Runing...\r\n", pStr);
		fflush( stdout );
	}
}

/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );
	static int loop = 0;

	for( ;; )
	{
		vTaskDelay( xCycleFrequency );

		xQueueSend( xPrintQueue, ( void * ) &pStr, ( TickType_t ) portMAX_DELAY );

		loop++;
		if(loop % 2 == 0){
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 4 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 5 );
		}else{
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 5 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 4 );
		}
	}
}

/*-----------------------------------------------------------*/

static void prvTask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;

	for( ;; )
	{
		xQueueSend( xPrintQueue, ( void * ) &pStr, ( TickType_t ) portMAX_DELAY );

		work( pdMS_TO_TICKS( 10UL ) );
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
