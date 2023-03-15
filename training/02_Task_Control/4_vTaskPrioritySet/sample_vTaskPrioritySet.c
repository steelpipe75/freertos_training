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

/* #define PRINT_TASK_RUNING_ENABLE */

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
#ifdef PRINT_TASK_RUNING_ENABLE
	const char* pStr = (const char*)pvParameters;
#else /* #ifdef PRINT_TASK_RUNING_ENABLE */
	( void ) pvParameters;
#endif /* #ifdef PRINT_TASK_RUNING_ENABLE */
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );
	static int loop = 0;

	for( ;; )
	{
		vTaskDelay( xCycleFrequency );

#ifdef PRINT_TASK_RUNING_ENABLE
		taskENTER_CRITICAL();
		{
			printf( "Task %s Runing...\r\n", pStr);
			fflush( stdout );
		}
		taskEXIT_CRITICAL();
#endif /* #ifdef PRINT_TASK_RUNING_ENABLE */

		loop++;
		if(loop % 2 == 0){
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 3 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 4 );
		}else{
			vTaskPrioritySet( xBTask, configMAX_PRIORITIES - 4 );
			vTaskPrioritySet( xCTask, configMAX_PRIORITIES - 3 );
		}
	}
}

/*-----------------------------------------------------------*/

static void prvTask( void *pvParameters )
{
#ifdef PRINT_TASK_RUNING_ENABLE
	const char* pStr = (const char*)pvParameters;
#else /* #ifdef PRINT_TASK_RUNING_ENABLE */
	( void ) pvParameters;
#endif /* #ifdef PRINT_TASK_RUNING_ENABLE */

	for( ;; )
	{
#ifdef PRINT_TASK_RUNING_ENABLE
		taskENTER_CRITICAL();
		{
			printf( "Task %s Runing...\r\n", pStr);
		}
		taskEXIT_CRITICAL();
#endif /* #ifdef PRINT_TASK_RUNING_ENABLE */

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
