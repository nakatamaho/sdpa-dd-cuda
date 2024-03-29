/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: Rlatrd.cpp,v 1.3 2009/09/12 07:59:10 nakatamaho Exp $ 
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

#include <mblas_dd.h>
#include <mlapack_dd.h>

void
Rlatrd(const char *uplo, mpackint n, mpackint nb, dd_real * A, mpackint lda, dd_real * e,
    dd_real * tau, dd_real * w, mpackint ldw)
{
    mpackint i, iw;
    dd_real Zero = 0.0, Half = 0.5, One = 1.0;
    dd_real alpha;

//Quick return if possible
    if (n <= 0)
	return;

    if (Mlsame_dd(uplo, "U")) {
//Reduce last NB columns of upper triangle
	for (i = n; i >= n - nb + 1; i--) {
	    iw = i - n + nb;
	    if (i < n) {
//Update A(1:i,i)
		Rgemv("No transpose", i, n - i, -One, &A[0 + i * lda],
		    lda, &w[(i - 1) + iw * ldw], ldw, One,
		    &A[0 + (i - 1) * lda], 1);
		Rgemv("No transpose", i, n - i, -One, &w[0 + iw * ldw], ldw,
		    &A[(i - 1) + i * lda], lda, One, &A[0 + (i - 1) * lda], 1);
	    }
	    if (i > 1) {
//Generate elementary reflector H(i) to annihilate
//A(1:i-2,i)
		Rlarfg(i - 1, &A[(i - 2) + (i - 1) * lda],
		    &A[0 + (i - 1) * lda], 1, &tau[i - 2]);
		e[i - 2] = A[(i - 2) + (i - 1) * lda];
		A[(i - 2) + (i - 1) * lda] = One;
//Compute W(1:i-1,i)
		Rsymv("Upper", i - 1, One, &A[0], lda, &A[0 + (i - 1) * lda],
		    1, Zero, &w[0 + (iw - 1) * ldw], 1);
		if (i < n) {
		    Rgemv("Transpose", i - 1, n - i, One, &w[0 + iw *
			    ldw], ldw, &A[0 + (i - 1) * lda], 1, Zero,
			&w[i + (iw - 1) * ldw], 1);
		    Rgemv("No transpose", i - 1, n - i, -One,
			&A[0 + i * lda], lda, &w[i + (iw - 1) * ldw], 1,
			One, &w[0 + (iw - 1) * ldw], 1);
		    Rgemv("Transpose", i - 1, n - i, One,
			&A[0 + i * lda], lda, &A[0 + (i - 1) * lda], 1, Zero,
			&w[i + (iw - 1) * ldw], 1);
		    Rgemv("No transpose", i - 1, n - i, -One,
			&w[0 + iw * ldw], ldw, &w[i + (iw - 1) * ldw], 1,
			One, &w[0 + (iw - 1) * ldw], 1);
		}
		Rscal(i - 1, tau[i - 2], &w[0 + (iw - 1) * ldw], 1);
		alpha =
		    -Half * tau[i - 2] * Rdot(i - 1, &w[0 + (iw - 1) * ldw], 1,
		    &A[0 + (i - 1) * lda], 1);
		Raxpy(i - 1, alpha, &A[0 + (i - 1) * lda], 1,
		    &w[0 + (iw - 1) * ldw], 1);

	    }
	}
    } else {
//Reduce first NB columns of lower triangle
	for (i = 1; i <= nb; i++) {
//Update A(i:n,i)
	    Rgemv("No transpose", n - i + 1, i - 1, -One,
		&A[(i - 1) + 0 * lda], lda, &w[(i - 1) + 0 * ldw], ldw, One,
		&A[(i - 1) + (i - 1) * lda], 1);
	    Rgemv("No transpose", n - i + 1, i - 1, -One,
		&w[(i - 1) + 0 * ldw], ldw, &A[(i - 1) + 0 * lda], lda, One,
		&A[(i - 1) + (i - 1) * lda], 1);
	    if (i < n) {
//Generate elementary reflector H(i) to annihilate 
//A(i+2:n,i)
	        Rlarfg(n - i, &A[i + (i - 1) * lda], &A[min(i + 2,
			    n) - 1 + (i - 1) * lda], 1, &tau[i - 1]);
		e[i - 1] = A[i + (i - 1) * lda];
		A[i + (i - 1) * lda] = One;

//Compute W(i+1:n,i)
		Rsymv("Lower", n - i, One, &A[i + i * lda],
		    lda, &A[i + (i - 1) * lda], 1, Zero, &w[i + (i - 1) * ldw],
		    1);
		Rgemv("Transpose", n - i, i - 1, One, &w[i + 0 * ldw],
		    ldw, &A[i + (i - 1) * lda], 1, Zero, &w[0 + (i - 1) * ldw],
		    1);
		Rgemv("No transpose", n - i, i - 1, -One, &A[i + 0 * lda], lda,
		    &w[0 + (i - 1) * ldw], 1, One, &w[i + (i - 1) * ldw], 1);
		Rgemv("Transpose", n - i, i - 1, One, &A[i + 0 * lda], lda,
		    &A[i + (i - 1) * lda], 1, Zero, &w[0 + (i - 1) * ldw], 1);
		Rgemv("No transpose", n - i, i - 1, -One, &w[i + 0 * ldw], ldw,
		    &w[0 + (i - 1) * ldw], 1, One, &w[i + (i - 1) * ldw], 1);
		Rscal(n - i, tau[i - 1], &w[i + (i - 1) * ldw], 1);
		alpha = -Half * tau[i - 1] * Rdot(n - i, &w[i + (i - 1) *
			ldw], 1, &A[i + (i - 1) * lda], 1);
		Raxpy(n - i, alpha, &A[i + (i - 1) * lda], 1,
		    &w[i + (i - 1) * ldw], 1);
	    }
	}
    }
    return;
}
