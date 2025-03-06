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
#ifndef __MY_CLASS_A__
#define __MY_CLASS_A__

class myClassA {

public:
  myClassA();
  ~myClassA();

private:
  float *float_p;
  int   *int_p;
  int    size;
};

#endif
