/*************************************************************************
	> File Name: struct.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月06日 星期三 20时49分43秒
 ************************************************************************/

#include<stdio.h>
struct point{
    int x;
    int y;
};

int main(){
    struct point p = {100, 200};
    printf("x is: %d, y is: %d\n", p.x, p.y);

    return 0;
}
