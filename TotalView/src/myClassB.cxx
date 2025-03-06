/***********************************************************************
 * Copyright 2000-2006 by Etnus, LLC. ALL RIGHTS RESERVED
 * No part of this material may be reproduced, stored in a retrieval
 * system, transmitted or used in any form or by any means, electronic,
 * mechanical, photocopying, recording, or otherwise, without the prior
 * written permission of, or express license from Etnus, LLC.
 ***********************************************************************
 * Copyright 1999 by Etnus, Inc.
 * Copyright 1996-1998 by Dolphin Interconnect Solutions, Inc.
 * Copyright 1989-1996 by BBN Inc.
 ***********************************************************************/ 
#include "myClassB.hxx"
#include <string.h>
#include <stdlib.h>

myClassB::myClassB() : size(256) {
 
  vector_char_p = new std::vector<char *>();

  for(int i=0; i<size; i++) {
    vector_char_p->push_back((char *) strdup("This is from calling strdup in myClassB."));
  }

  init();
}


myClassB::~myClassB() {
  
  std::vector<char *>::iterator iter;

  for(int i=0; i<size; i++) {
    free(b_pp[i]);
  }

  for(iter = vector_char_p->begin(); iter != vector_char_p->end(); iter++) {
    free(*iter);
  }

  delete vector_char_p;

}

void myClassB::init(void) {

  b_pp = (int **) malloc (size * sizeof(int *));

  for(int i=0; i<size; i++) {
    b_pp[i] = (int *) malloc(128 * sizeof(int));
  }
}

void myClassB::destroy(void) {

  delete vector_char_p;
  
  for(int i=0; i<size; i=i+2) {
    free(b_pp[i]);
  }

}
