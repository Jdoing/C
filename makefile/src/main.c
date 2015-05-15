/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月08日 星期五 14时10分32秒
 ************************************************************************/

#include<stdio.h>
#include "util.h"
#include "calculator.h"

int main(int argc, char *argv[]){
    print("begin...");
    
    printf("add result is: %d", add(6, 3));
    printf("sub result is: %d", sub(6, 3));
    printf("mul result is: %d", mul(6, 3));
    printf("div result is: %d", div(6, 3));

    return 0;
}
