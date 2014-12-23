/*
 * Test.cpp
 *
 *  Created on: Jul 18, 2014
 *      Author: jiangyu
 */
#include<stdio.h>
//#include "TestExternC.h"

#ifdef _cplusplus
extern "C"
{
#endif

#include "TestExternC.h"

#ifdef _cplusplus
}
#endif

int main()
{
	int sum=add(3,7);
	printf("The result is:%d\n",sum);
	return 0;
}
