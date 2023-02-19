/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* 自作ライブラリ */
#include "gcd.h"

/*-----------------------------------------------------------*/

#include "sample_vTaskDelayUntil.h"

/*-----------------------------------------------------------*/

static void prvCreateTasks( void );
static void prvATask( void *pvParameters );

/*-----------------------------------------------------------*/

static void rand_gcd(void);
static void work(void);

/*-----------------------------------------------------------*/

void rand_gcd(void){
	int32_t a;
	int32_t b;
	int32_t g;

	a = rand() % INT32_MAX;
	b = rand() % INT32_MAX;

	g = gcd( a, b );

	printf( "a = %d, b = %d, g = %d\r\n", a, b, g );
}

static void work(void){
	int i;

	for( i = 0; i < 10; i++ ){
		rand_gcd();
	}
}

/*-----------------------------------------------------------*/

void sample_vTaskDelayUntil( void )
{
	prvCreateTasks();

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

	xLastWakeTime = xTaskGetTickCount();
	xGetTime = xLastWakeTime;

	for( ;; )
	{
		vTaskDelayUntil( &xLastWakeTime, xCycleFrequency );

		printf( "xLastWakeTime = %ld, xGetTime = %ld\r\n", xLastWakeTime, xGetTime);
		printf( "Task %s Runing...\r\n", pStr);

		work();

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
