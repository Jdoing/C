/*************************************************************************
	> File Name: array.c
	> Author:
	> Mail:
	> Created Time: 2015年05月06日 星期三 20时01分40秒
 ************************************************************************/

#include<stdio.h>

#define GET_ARRAY_LEN(array, len) {len = (sizeof(array)/sizeof(array[0]));}

void print(int *a, int len)
{
    int i = 0;
    for(; i < len; i++)
    {
        printf("%-4d", a[i]);
    }
    printf("\n");
}

int main()
{
    int a[3] = {1, 2, 3};
    int *pa;
    pa= a;
    print(pa, 3);

    pa = &a[0];
    print(pa, 3);
    return 0;
}
