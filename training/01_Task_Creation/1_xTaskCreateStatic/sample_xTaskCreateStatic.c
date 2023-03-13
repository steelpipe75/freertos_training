/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/*-----------------------------------------------------------*/

#include "sample_xTaskCreateStatic.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );

/*-----------------------------------------------------------*/

void sample_xTaskCreateStatic( void )
{
	prvCreateTasks();

	vTaskStartScheduler();
}
/*-----------------------------------------------------------*/

static void prvCreateTasks( void )
{
	static StaticTask_t xATask;
	
	static StackType_t ucATaskStack[ configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ];

	xTaskCreateStatic( prvATask, "A", configMINIMAL_STACK_SIZE, "A", configMAX_PRIORITIES - 2, ucATaskStack, &xATask );
}
/*-----------------------------------------------------------*/

static void prvATask( void *pvParameters )
{
	const char* pStr = (const char*)pvParameters;
	const TickType_t xCycleFrequency = pdMS_TO_TICKS( 100UL );

	for( ;; )
	{
		vTaskDelay( xCycleFrequency );

		taskENTER_CRITICAL();
		{
			printf( "Task %s Runing...\r\n", pStr);
		}
		taskEXIT_CRITICAL();
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
/*-----------------------------------------------------------*/

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}