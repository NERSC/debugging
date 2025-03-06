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

#ifndef __MY_CLASS_B__
#define __MY_CLASS_B__

#include <vector>

class myClassB {

public:
  myClassB();
  ~myClassB();
  void init(void);
  void destroy(void);

private:
  
  std::vector<char *>  *vector_char_p;
  int                  **b_pp;
  int                    size;
};

#endif
