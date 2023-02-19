/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* 自作ライブラリ */

#include "gcd.h"
#include "work.h"

/*-----------------------------------------------------------*/

static void rand_gcd(void);

/*-----------------------------------------------------------*/

void rand_gcd(void){
	volatile int32_t a;
	volatile int32_t b;
	volatile int32_t g;

	a = rand() % INT32_MAX;
	b = rand() % INT32_MAX;

	g = gcd( a, b );

	printf( "a = %d, b = %d, g = %d\r\n", a, b, g );
}

/*-----------------------------------------------------------*/

void work(void){
	int i;

	for( i = 0; i < 10; i++ ){
		rand_gcd();
	}
}
