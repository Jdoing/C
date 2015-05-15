/*************************************************************************
	> File Name: struct.c
	> Author: 
	> Mail: 
	> Created Time: 2015年05月06日 星期三 20时49分43秒
 ************************************************************************/

#include<stdio.h>
struct point1{
    int x;
    int y;
};

struct{
    int x;
    int y;
}point2;

struct point3{
    int x;
    int y;
}point;

typedef struct point4{
    int x;
    int y;
}t_point;

int main(){
    struct student{
        int age;
        char *name;
    };
    
    struct student foo1 = {11, "xiaoming"};
    
    struct student foo2 = {11};

    struct student foo3 = {.age = 11};

    struct student foo4 = (struct student){11, "xiaoming"};
    struct student foo5 = (struct student){.age = 11};
    
    struct student ss1[10] = {0};
    struct student ss2[10] = {{}, {}, {}};
    struct student ss3[10] = {[2] = {}, [3] = {}};
    struct student ss4[10] = {[2].age = 10, [3].name = "xiaoming"};


    return 0;
}
