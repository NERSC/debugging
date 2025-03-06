/***********************************************************************
 * Copyright 2000-2006 by Etnus, LLC. ALL RIGHTS RESERVED
 * No part of this material may be reproduced, stored in a retrieval
 * system, transmitted or used in any form or by any means, electronic,
 * mechanical, photocopying, recording, or otherwise, without the prior
 * written permission of, or express license from Etnus, LLC.
 ***********************************************************************
 * This file contains PROPRIETARY INFORMATION of Etnus, LLC.
 ***********************************************************************
 * Copyright 1999 by Etnus, Inc.
 * Copyright 1996-1998 by Dolphin Interconnect Solutions, Inc.
 * Copyright 1989-1996 by BBN Inc.
 ***********************************************************************/ 
#include <stdlib.h>
#include "myClassA.hxx"

myClassA::myClassA() : size (128) {
  
  float_p = new float[size];

  for(int i=0; i<size; i++) {
    float_p[i] = (float) i;
  }

  int_p = (int *) malloc(size * sizeof(int));

  for(int i=0; i<size; i++) {
    int_p[i] = size-i;
  }
}

myClassA::~myClassA () {

  delete[] float_p;
  free (int_p);

}
