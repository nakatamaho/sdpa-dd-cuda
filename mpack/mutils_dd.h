/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2009 by Nakata, Maho
 *
 * MPACK - multiple precision arithmetic library
 *
 * This file is part of MPACK.
 *
 * MPACK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * MPACK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with MPACK.  If not, see
 * <http://www.gnu.org/licenses/lgpl.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/

#ifndef _MUTILS_DD_H_
#define _MUTILS_DD_H_

using std::max;
using std::min;

dd_real Msign(dd_real a, dd_real b);
double cast2double(dd_real a);
int M2int(dd_real a);
//void mpf_pow(mpf_t ans, mpf_t x, mpf_t y);
dd_real mpf_approx_log(dd_real x);
dd_real mpf_approx_log2(dd_real x);
dd_real mpf_approx_log10(dd_real x);
dd_real mpf_approx_pow(dd_real x, dd_real y);
dd_real mpf_approx_cos(dd_real x);
dd_real mpf_approx_sin(dd_real x);
dd_real mpf_approx_exp(dd_real x);
dd_real mpf_approx_pi();

//implementation of sign transfer function.
inline dd_real Msign(dd_real a, dd_real b)
{
  dd_real mtmp;
  mtmp=abs(a);
  if (b<0.0) {
    mtmp=-mtmp;
  }
  return mtmp;
}

inline double
cast2double(dd_real a)
{
    return a.x[0];
}

inline int
M2int(dd_real a)
{
    int i;
    dd_real tmp;
    a = a + 0.5;
    tmp = floor(a);
    i = (int)tmp.x[0];
    return i;
}

#endif
