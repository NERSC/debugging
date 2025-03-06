/***********************************************************************
 * Copyright 2000-2007 by Etnus, LLC. ALL RIGHTS RESERVED
 * No part of this material may be reproduced, stored in a retrieval
 * system, transmitted or used in any form or by any means, electronic,
 * mechanical, photocopying, recording, or otherwise, without the prior
 * written permission of, or express license from Etnus, LLC.
 ***********************************************************************
 ***********************************************************************
 * Copyright 2007 by TotalView Technologies
 * Copyright 1999-2007 by Etnus, Inc.
 * Copyright 1996-1998 by Dolphin Interconnect Solutions, Inc.
 * Copyright 1989-1996 by BBN Inc.
 ***********************************************************************/ 
#include "myClassA.hxx"
#include "myClassB.hxx"

#ifdef USEMPI
#include <mpi.h>

int rank;

#endif //USEMPI

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *myMalloc( int size )
{
  return malloc( size );
}

void myFree( void *v )
{
  free( v );
}

void myFunc()
{  
  int * alloc = 0;
  int i;
  
  alloc = (int *) myMalloc( 10 * sizeof(int) );
  
  for( i=0; i<=10; i++ )
    {
      alloc[i]=12-i;
    }
  
  alloc=0;
}

void double_free()
{
  int	  	 *p;
  int            length    = 0xab;   
  int            junk      = 0;
  
  p = (int*) malloc( length );
  printf ( "malloced %4d (%#6x) bytes at %p\n", length, length, p );
  
  // Breakpoint here
  // Show allocated annotation
  // Show block properties and enable notify when deallocated
  junk = 0;
  
  // Now release the memory the first time - legal
  free ( p );
  
  // Breakpoint here
  // Show that the block is marked dangling
  // Show that the deallocation stack is available now
  junk = 0;
  
  // Now release the memory the second time - illegal
#ifdef USEMPI
  if( rank == 1 )
#endif
     free ( p );

// Note: an evaluation point can be used to avoid the segv 
// and continue the demo.
}

void corrupt_data()
{
  int      i, j;
  int      size;
  int    * p0;
  int    * p1;
  int    * p2;

  // Breakpoint here.
  // Enable Guard Blocks on the Memory Debugging Configuration Page.
  // Use 8 byte pre and post guard size.

  size = 16;

  // Loop added around the allocates and the corrupt loop,
  // to make the memory report viewing more interesting.
  // 9/2009 CS at the suggestion of MS and EH

  for ( j=0; j<6 ; j++ )
    {

      p0 = (int *) malloc( size * sizeof( int ) );
      p1 = (int *) malloc( size * sizeof( int ) );
      p2 = (int *) malloc( size * sizeof( int ) );

  // Common corruption cases.  Oops in the for loop condition.
      for( i=0; i<=size; i++ )
	{
	  p1[i] = size - i;
	}
    }

  // Breakpoint here.
  // Check Heap Status Corrupt Guard Block View to scan 
  // all blocks for corruption.
  i = 0;

  // Corrupt Guard Memory Event on free().
  free( p1 );
}


void corrupt_data_rz()
{
  int      i;
  int      size;
  int    * p0;
  int    * p1;
  int    * p2;

  // Code is mostly the same as corrupt_data(), just repeated
  // to demonstrate the ability to change from Guard Blocks to
  // RedZones on the fly, and compare and contrast the two.

  size = 16;

  p0 = (int *) malloc( size * sizeof( int ) );
  p1 = (int *) malloc( size * sizeof( int ) );
  p2 = (int *) malloc( size * sizeof( int ) );

  // Common corruption cases.  Oops in the for loop condition.

  for( i=0; i<=size; i++ )
    {
      p1[i] = size - i;
    }

  free( p2 );
  free( p1 );
  free( p0 );
}


void corrupt_data_sizes()
{
  int      i;
  int      size;
  int    * p0;
  int    * p1;
  int    * p2;

  // Like corrupt_data(), but allocates/corrupts different sizes.
  // In RedZones detailed options, set size limits to ignore the
  // smallest and largest size, and catch the middle size.

  size = 64;
  p0 = (int *) malloc( size * sizeof( int ) );
  for( i=0; i<=size; i++ )
    {
      p0[i] = size - i;
    }

  size = 256;
  p1 = (int *) malloc( size * sizeof( int ) );
  for( i=0; i<=size; i++ )
    {
      p1[i] = size - i;
    }

  size = 128;
  p2 = (int *) malloc( size * sizeof( int ) );
  for( i=0; i<=size; i++ )
    {
      p2[i] = size - i;
    }

  free( p2 );
  free( p1 );
  free( p0 );
}


void read_overrun()
{
  int      i;
  int      size;
  int    * p1;
  int      j;

  // RedZones can catch out-of-bounds access even if only a read.
  // Make size such that RZ catches even if size limits from
  // corrupt_sizes method are still in place.

  size = 128;

  p1 = (int *) malloc( size * sizeof( int ) );

  // Fill in with a correct loop.
  for( i=0; i<size; i++ )
    {
      p1[i] = size - i;
    }

  // Read back, now with an overrun in the loop.
  for( i=0; i<=size; i++ )
    {
      j = p1[i];
    }

  free( p1 );
}


int main( int argc, char *argv[] ) 
{
  myClassA  *a1;
  myClassA  *a2;
  myClassB  *b1;
  int      **int_pp;
  bool loop = false, runforever, runRedZones=false;

  // Pass an argument to make this run forever.
  //  argc == 2 ? runforever = 1 : runforever = 0;

  // runforever was eliminated.  Instead, the last bunch of allocates/leaks
  // are done in a counted loop with a usleep - tuning notes appear near the
  // loop.  Here, we check for an argument starting with "R", in which case
  // the RedZones sections are to be run.

  if ( argc == 2 )
    {
      if ( argv[1][0] == 'r' || argv[1][0] == 'R' ) runRedZones = true;
    }



#ifdef USEMPI
  int numprocs;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Do this so that one rank has a lot more allocations/leaks
  // than the other processors.
  loop = rank == 1;
#else
  loop = true;
#endif

  // TotalView can raise events for some kinds of memory errors.
  //   - double free
  //   - free pointer that points to data on the stack.
  //   - free interior pointer
  //   - free an unknown block
  //   - realloc an unknown block
  //   - double allocation (malloc problem)
  //   - misaligned allocation (malloc problem)
  //   - out of memory ( ... you check if malloc returns non-null, right?)
  //   - invalid aligned alloc request (problem calling memalign)
  //   - guard block corruption
  //   - free notification (tagging)
  //   - realloc notification (tagging)
  //
  // double_free() demonstrates "double free" and "free notification".
// #ifndef USEMPI
//  double_free();
// #endif

  // TotalView can detect writing outside array bounds conditions
  // by using "Guard Blocks".  The "Guard Blocks" feature must be
  // enabled for this portion of the demo.
// #ifndef USEMPI
//  corrupt_data();
// #endif

  // Set of routines similar in flavor to above, but to demo RedZones.
  // If doing RedZones demo, break here, disable Guard Blocks, and
  // enable RedZones.  Also suggest pre-planting eval points to skip
  // over these calls in sequence.

  if ( runRedZones )
    {
    corrupt_data_rz();
    corrupt_data_sizes(); // Suggest RZ size lower limit of 512, upper 1020
    read_overrun();
    }

  // In the following loop, tune the trip count to control the size of
  // the memory footprint, which will also determine the time it takes
  // to generate memory reports.  Tune the usleep parameter to control
  // the execution time of the loop.

  int j;
  j = 0; // Target in case eval points want to jump here.
  for( j=0; (j<160 && loop) ; j++) {
    usleep(100000);

    // Generate some interesting allocations and leaks using
    // C++ constructors and destructors.
    int size = 4;
    int_pp = (int **) malloc(size * sizeof(int *));
    for( int i=0; i<size; i++ )
      {
        int_pp[i] = (int *) malloc(16 * sizeof(int));

        a1 = new myClassA();
        b1 = new myClassB();
        a2 = new myClassA();
      
        delete a2;
        if(i%2 == 0) {
          b1->destroy();
        } 
        else {
          delete b1;
        }
      }
  
    int * alloc2 = 0;
    for( int i = 1; i < 25; i++ )
      {
        alloc2 = (int *) myMalloc((int)(i * (10+(int) rand()/(RAND_MAX+1.0))));
      
        if( i % 5 )
          {
            myFree(alloc2);
          }
      }
  
    myFunc();

  } 

  /* breakpoint here*/
  /* discuss heap view (graphical and source)
   * show leak detection 
   * show filtering 
   */

printf("Reached the end of filterapp-leaks\n");

#ifdef USEMPI
  MPI_Finalize();
#endif

  return 0;
}
