/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: Rlamch.cpp,v 1.7 2009/09/18 23:01:08 nakatamaho Exp $ 
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

#include <mblas_dd.h>
#include <mlapack_dd.h>

//"E" denots we always calculate relative machine precision (e).
//where 1+e > 1, minimum of e.
dd_real RlamchE_dd(void)
{
  //about 1e-(16+16)=1e-32
  return dd_real::_eps;
}

//"S" denots we always calculate `safe minimum, such that 1/sfmin does not overflow'.
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchS_dd(void)
{
  //about 1e-(308-16)=1e-292
  return dd_real::_min_normalized;
}
//"B" base  = base of the machine
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchB_dd(void)
{
    dd_real two;
    two = 2.0;
    return two;
}

//"P" prec = eps*base
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchP_dd(void)
{
    dd_real base, eps, prec;

    base = RlamchB_dd();
    eps = RlamchE_dd();
    prec = eps * base;
    return prec;
}

//"N" t = number of digits in mantissa
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchN_dd(void)
{
  return (dd_real)209.0; //52*4
}

//"R" rnd   = 1.0 when rounding occurs in addition, 0.0 otherwise
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchR_dd(void)
{
    dd_real mtmp;
    mtmp = 1.0;
    return mtmp;
}

//"M"
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchM_dd(void)
{
  return dd_real(-1022.0 + 53.0);
}

//"U"
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchU_dd(void)
{
  return dd_real::_min_normalized;
}

//"L"
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchL_dd(void)
{
  return (dd_real)1024.0;
}

//"O"
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchO_dd(void)
{
    return dd_real::_max; //approx 1.7976931348623157E+308 in float.h
}

//"Z" :dummy
//cf.http://www.netlib.org/blas/dlamch.f
dd_real RlamchZ_dd(void)
{
    dd_real mtemp = 0.0;
    return mtemp;
}

dd_real Rlamch_dd(const char *cmach)
{
    if (Mlsame_dd(cmach, "E"))
	return RlamchE_dd();
    if (Mlsame_dd(cmach, "S"))
	return RlamchS_dd();
    if (Mlsame_dd(cmach, "B"))
	return RlamchB_dd();
    if (Mlsame_dd(cmach, "P"))
	return RlamchP_dd();
    if (Mlsame_dd(cmach, "N"))
	return RlamchN_dd();
    if (Mlsame_dd(cmach, "R"))
	return RlamchR_dd();
    if (Mlsame_dd(cmach, "M"))
	return RlamchM_dd();
    if (Mlsame_dd(cmach, "U"))
	return RlamchU_dd();
    if (Mlsame_dd(cmach, "L"))
	return RlamchL_dd();
    if (Mlsame_dd(cmach, "O"))
	return RlamchO_dd();

    Mxerbla_dd("Rlamch", 1);
    return RlamchZ_dd();
}
