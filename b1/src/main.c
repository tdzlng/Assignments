#include <stdio.h>
#include "math.h"
#include "hello.h"
#include "d1.h"
#include "d2.h"

void main()
{
	int a = add(3,4);
	prt("Hello\n");

	printf("sum= %d\n", a);

	prtd1();
	prtd2();
}
