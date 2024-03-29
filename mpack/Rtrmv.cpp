/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: Rtrmv.cpp,v 1.4 2009/09/24 07:25:57 nakatamaho Exp $ 
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
Copyright (c) 1992-2007 The University of Tennessee.  All rights reserved.

$COPYRIGHT$

Additional copyrights may follow

$HEADER$

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer. 
  
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer listed
  in this license in the documentation and/or other materials
  provided with the distribution.
  
- Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT  
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

/*
Based on http://www.netlib.org/blas/dtrmv.f
Rtrmv performs one of the matrix-vector operations
 x := A*x, or x := A'*x,
where x is an n element vector and  A is an n by n unit, or non-unit,
upper or lower triangular matrix.
*/

#include <mblas_dd.h>

void
Rtrmv(const char *uplo, const char *trans, const char *diag, mpackint n,
    dd_real * A, mpackint lda, dd_real * x, mpackint incx)
{
    mpackint ix, jx, kx;

    dd_real temp;

    dd_real Zero = 0.0;

    //Test the input parameters.
    mpackint info = 0;

    if (!Mlsame_dd(uplo, "U") && !Mlsame_dd(uplo, "L"))
	info = 1;
    else if (!Mlsame_dd(trans, "N") && !Mlsame_dd(trans, "T") && !Mlsame_dd(trans, "C"))
	info = 2;
    else if (!Mlsame_dd(diag, "U") && !Mlsame_dd(diag, "N"))
	info = 3;
    else if (n < 0)
	info = 4;
    else if (lda < max((mpackint) 1, n))
	info = 6;
    else if (incx == 0)
	info = 8;

    if (info != 0) {
	Mxerbla_dd("Rtrmv ", info);
	return;
    }
    //quick return if possible.
    if (n == 0)
	return;

    mpackint nounit = Mlsame_dd(diag, "N");

    //set up the start point in x if the increment is not unity. this
    //will be  (n-1)*incx  too small for descending loops.
    if (incx <= 0)
	kx = -(n - 1) * incx;
    else
	kx = 0;

    //start the operations. in this version the elements of a are
    //accessed sequentially with one pass through A.
    if (Mlsame_dd(trans, "N")) {
	//form  x := A*x.
	if (Mlsame_dd(uplo, "U")) {
	    jx = kx;
	    for (mpackint j = 0; j < n; j++) {
		if (x[jx] != Zero) {
		    temp = x[jx];
		    ix = kx;
		    for (mpackint i = 0; i < j; i++) {
			x[ix] = x[ix] + temp * A[i + j * lda];
			ix = ix + incx;
		    }
		    if (nounit)
			x[jx] = x[jx] * A[j + j * lda];
		}
		jx = jx + incx;
	    }
	} else {
	    kx = kx + (n - 1) * incx;
	    jx = kx;
	    for (mpackint j = n - 1; j >= 0; j--) {
		if (x[jx] != Zero) {
		    temp = x[jx];
		    ix = kx;
		    for (mpackint i = n - 1; i >= j + 1; i--) {
			x[ix] = x[ix] + temp * A[i + j * lda];
			ix = ix - incx;
		    }
		    if (nounit)
			x[jx] = x[jx] * A[j + j * lda];
		}
		jx = jx - incx;
	    }
	}
    } else {
	//form  x := A'*x.
	if (Mlsame_dd(uplo, "U")) {
	    jx = kx + (n - 1) * incx;
	    for (mpackint j = n - 1; j >= 0; j--) {
		temp = x[jx];
		ix = jx;
		if (nounit)
		    temp = temp * A[j + j * lda];
		for (mpackint i = j - 1; i >= 0; i--) {
		    ix = ix - incx;
		    temp = temp + A[i + j * lda] * x[ix];
		}
		x[jx] = temp;
		jx = jx - incx;
	    }
	} else {
	    jx = kx;
	    for (mpackint j = 0; j < n; j++) {
		temp = x[jx];
		ix = jx;
		if (nounit)
		    temp = temp * A[j + j * lda];
		for (mpackint i = j + 1; i < n; i++) {
		    ix = ix + incx;
		    temp = temp + A[i + j * lda] * x[ix];
		}
		x[jx] = temp;
		jx = jx + incx;
	    }
	}
    }
    return;
}
