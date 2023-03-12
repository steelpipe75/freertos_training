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

#include "sample_vTaskPrioritySet.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );
static void prvTask( void *pvParameters );

/*-----------------------------------------------------------*/

static TaskHandle_t xATask;
static TaskHandle_t xBTask;
static TaskHandle_t xCTask;

/*-----------------------------------------------------------*/

void sample_vTaskPrioritySet( void )
{
	prvCreateTasks();

	myTrace_Initialize();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateTasks( void )
{
	xTaskCreate( prvATask, "A", configMINIMAL_STACK_SIZE + 2048, "A", configMAX_PRIORITIES - 2, &xATask );
	xTaskCreate( prvTask,  "B", configMINIMAL_STACK_SIZE + 2048, "B", configMAX_PRIORITIES - 3, &xBTask );
	xTaskCreate( prvTask,  "C", configMINIMAL_STACK_SIZE + 2048, "C", configMAX_PRIORITIES - 4, &xCTask );
}
/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 1UL );
	TickType_t xGetTime;
	TickType_t xPrevGetTime;
	int loop = 0;

	xGetTime = xTaskGetTickCount();
	xPrevGetTime = xGetTime;

	for( ;; )
	{
		vTaskDelay( xCycleFrequency );
		xGetTime = xTaskGetTickCount();

		printf( "xPrevGetTime = %ld, xGetTime = %ld\r\n", xPrevGetTime, xGetTime );
		printf( "Task %s Runing...\r\n", pStr);

		loop++;
		if(loop == 2){
			loop = 0;
			printf( "Set Taske B Priority %d\r\n", configMAX_PRIORITIES - 3 );
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 3 );
			printf( "Set Taske C Priority %d\r\n", configMAX_PRIORITIES - 4 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 4 );
		}else{
			printf( "Set Taske B Priority %d\r\n", configMAX_PRIORITIES - 4 );
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 4 );
			printf( "Set Taske C Priority %d\r\n", configMAX_PRIORITIES - 3 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 3 );
		}

		xPrevGetTime = xGetTime;
	}
}

/*-----------------------------------------------------------*/

static void prvTask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;

	for( ;; )
	{
		printf( "Task %s Runing...\r\n", pStr);
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
