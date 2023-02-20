/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* 自作ライブラリ */
#include "work.h"

void work( TickType_t time ){
	TickType_t StartTime;
	TickType_t CurrTime;
	TickType_t Diff;

	StartTime = xTaskGetTickCount();

	for( ;; ){
		CurrTime = xTaskGetTickCount();
		Diff = CurrTime - StartTime;
		printf( "StartTime = %ld, CurrTime = %ld, Diff = %ld\r\n", StartTime, CurrTime, Diff );
		if(Diff > time){
			break;
		}
	}
}
