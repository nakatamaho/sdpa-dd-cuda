/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: Mxerbla.cpp,v 1.4 2009/09/25 23:15:56 nakatamaho Exp $ 
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
/*
Based on http://www.netlib.org/blas/xerbla.f
Mxerbla_dd is an error handler for the Mlapack routines.
*/

#include <mblas_dd.h>
#include <stdio.h>

#if !defined  __MPACK_ERRNO__
#define __MPACK_ERRNO__
int mpack_errno;
#endif

void
Mxerbla_dd(const char *srname, int info)
{
    fprintf(stderr,
	" ** On entry to %s parameter number %2d had an illegal value\n",
	srname, info);
    mpack_errno = info;
    return;
}
