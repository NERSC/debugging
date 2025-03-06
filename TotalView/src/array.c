#include <math.h>
#include <stdio.h>
#include "array.h"


double b[IMAX];

void dowork()
{
       /**** some array operations ***/
    {
        int i, j, jmod;
        double xi, xj, dx, scale = 100.0;

        for (j = 0; j < JMAX; j++)
        {
            jmod = (100*j) %JMAX;
            xj = (double)jmod/(double)JMAX;
            for (i = 0; i < IMAX; i++)
            {
                xi = (double)i/(double)IMAX;
                dx = xi-xj;
                b[i] = 2.0/(1.0+exp(scale*dx*dx));
            }
            printf("counter %d\n", j);
        }
    }
    return;
}
   

void array()
{
    dowork();
    return;
}
