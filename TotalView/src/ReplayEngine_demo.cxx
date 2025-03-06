//
//                 ReplayEngine Demo
//
//
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <pthread.h>

int funcA(int);
int funcB(int);
int badstuff();
#define MAXDEPTH 20
int arraylength=MAXDEPTH;


int main()
{
    funcA(0);
    badstuff();
    funcA(0);
    exit(1);
}



int funcA(int a){
    int b;
    b=a+2;
    b=funcB(b);
    return b;
}


int funcB(int b){
    int c;
    int i;
    int v[MAXDEPTH];
    int *p;

    c=b+2;
    p=&c;

    if( c<MAXDEPTH )  
       c=funcA(c);

    for (i=arraylength-1; i>0; i--){
        v[i]=*p;
    }

    return c; 
}

int badstuff(){
    arraylength=5*MAXDEPTH;
    return 0;
}
