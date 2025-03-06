/*
This is a derivative work of the cpi example program from MPICH2,
which includes the following copyright notice:

				  COPYRIGHT

The following is a notice of limited availability of the code, and disclaimer
which must be included in the prologue of the code and in all source listings
of the code.

Copyright Notice
 + 2002 University of Chicago

Permission is hereby granted to use, reproduce, prepare derivative works, and
to redistribute to others.  This software was authored by:

Argonne National Laboratory Group
W. Gropp: (630) 252-4318; FAX: (630) 252-5986; e-mail: gropp@mcs.anl.gov
E. Lusk:  (630) 252-7852; FAX: (630) 252-5986; e-mail: lusk@mcs.anl.gov
Mathematics and Computer Science Division
Argonne National Laboratory, Argonne IL 60439


			      GOVERNMENT LICENSE

Portions of this material resulted from work developed under a U.S.
Government Contract and are subject to the following license: the Government
is granted for itself and others acting on its behalf a paid-up, nonexclusive,
irrevocable worldwide license in this computer software to reproduce, prepare
derivative works, and perform publicly and display publicly.

				  DISCLAIMER

This computer code material was prepared, in part, as an account of work
sponsored by an agency of the United States Government.  Neither the United
States, nor the University of Chicago, nor any of their employees, makes any
warranty express or implied, or assumes any legal liability or responsibility
for the accuracy, completeness, or usefulness of any information, apparatus,
product, or process disclosed, or represents that its use would not infringe
privately owned rights.



*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#ifndef INTERVAL_START
#define INTERVAL_START 10
#endif

#ifndef INTERVAL_END
#define INTERVAL_END 1000000
#endif

typedef struct error_detail {
  int intervals;
  double almost_pi;
  double delta;
} error_detail;

double f( double );

double f( double a )
{
    return (4.0 / (1.0 + a*a));
}

double calc_error( int, double, double );

double calc_error( int n, double almost_pi, double last_error )
{
    float ref;
    ref = 3.141592653589793238462643;
    double pidiff;
    int numprocs;

    pidiff = fabs(almost_pi - ref);
    if ( pidiff > last_error ) {
        MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
        if ( numprocs == 3 )
	    abort();
        else
            printf( "Error increased for intervals = %d!\n", n );
        }
    return pidiff;
}

int main( int argc, char *argv[] )
{
    int done = 0, n, myid, numprocs, i;
    double mypi, pi, h, sum, x, pidiff, last_error = 4.;
    double startwtime=0.0, endwtime;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    error_detail err_detail;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);
    fprintf(stderr,"Process %d on %s\n",
	    myid, processor_name);

    n = INTERVAL_START;
    while (n <= INTERVAL_END)
    {
        if (myid == 0)
        {
	    startwtime = MPI_Wtime();
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            done = 1;
        else
        {
            h   = 1.0 / (double) n;
            sum = 0.0;
            for (i = myid + 1; i <= n; i += numprocs)
            {
                x = h * ((double)i - 0.5);
                x = f(x);
                sum += x;
            }
            mypi = h * sum;

            MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

            if (myid == 0)
	    {
	       pidiff = calc_error( n, pi, last_error );
               last_error = pidiff;
	       err_detail.intervals = n;
	       err_detail.almost_pi = pi;
	       err_detail.delta = pidiff;
               printf("the answer is approximately %.16f, Error is %.16f\n",
                       pi, pidiff);
	       endwtime = MPI_Wtime();
	       printf("wall clock time = %f\n",
		       endwtime-startwtime);
	    }
        }
        n = n * 10;
    }
    MPI_Finalize();
sleep(2);
    return 0;
}

            
