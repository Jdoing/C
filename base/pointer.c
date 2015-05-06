#include<stdio.h>

void swap(int *x, int *y){
    int t = *x;
    *x = *y;
    *y = t;
}

int main(){
    //声明指针
    int *p;
    int x = 13;
    p = &x;//获取变量的存储地址

    printf("The value of p is: %d\n", p);
    printf("The size of p is: %d\n", sizeof(p));
    
    //指针类型转换
    char *q = (char *)p;
    
    printf("The value of q is: %d\n", q);//返回的值和p一样
    printf("The value of *q is: %d\n", *q);//返回值为13,说明系统按小端序存放数据
    
    //声明一个函数指针fswap, 函数签名与swap一样.
    void (*fswap)(int *, int *);
    fswap = swap;//函数指针fswap赋值
    
    int a = 3, b = 5;
    
    //函数指针调用
    fswap(&a, &b);
    printf("after swap: a is %d, b is %d\n", a, b);
}
