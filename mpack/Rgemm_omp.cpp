/*
 * Copyright (c) 2008-2010
 *	Nakata, Maho
 * 	All rights reserved.
 *
 * $Id: Rgemm.cpp,v 1.1 2010/12/28 06:13:53 nakatamaho Exp $
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <mblas_dd.h>

void Rgemm_NN_omp(mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);
void Rgemm_TN_omp(mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);
void Rgemm_NT_omp(mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);
void Rgemm_TT_omp(mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);

void Rgemm_X(const char *transa, const char *transb, mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);

static int __firstcall__ = 1;
static unsigned long int n_min = 1000000;

void Rgemm(const char *transa, const char *transb, mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc)
{
    mpackint i, j, l, nota, notb, nrowa, ncola, nrowb, info;
    dd_real temp;
    dd_real Zero = 0.0, One = 1.0;

    if (__firstcall__ == 1) {
      char* n_min_str = getenv("SDPA_CUDA_NMIN");
      if (n_min_str != NULL) n_min = atol(n_min_str);
      std::cout << "!!!!!! set n_min = " << n_min << "\n";
      __firstcall__ = 0;
    }
    
    nota = Mlsame_dd(transa, "N");
    notb = Mlsame_dd(transb, "N");
    if (nota) {
	nrowa = m;
	ncola = k;
    } else {
	nrowa = k;
	ncola = m;
    }
    if (notb) {
	nrowb = k;
    } else {
	nrowb = n;
    }
//Test the input parameters.
    info = 0;
    if (!nota && (!Mlsame_dd(transa, "C")) && (!Mlsame_dd(transa, "T")))
	info = 1;
    else if (!notb && (!Mlsame_dd(transb, "C")) && (!Mlsame_dd(transb, "T")))
	info = 2;
    else if (m < 0)
	info = 3;
    else if (n < 0)
	info = 4;
    else if (k < 0)
	info = 5;
    else if (lda < std::max((mpackint) 1, nrowa))
	info = 8;
    else if (ldb < std::max((mpackint) 1, nrowb))
	info = 10;
    else if (ldc < std::max((mpackint) 1, m))
	info = 13;
    if (info != 0) {
	Mxerbla_dd("Rgemm ", info);
	return;
    }
//Quick return if possible.
    if ((m == 0) || (n == 0) || (((alpha == Zero) || (k == 0)) && (beta == One)))
	return;
//And when alpha == 0.0
    if (alpha == Zero) {
	if (beta == Zero) {
	    for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
		    C[i + j * ldc] = Zero;
		}
	    }
	} else {
	    for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
		    C[i + j * ldc] = beta * C[i + j * ldc];
		}
	    }
	}
	return;
    }

    unsigned long int mnk = (unsigned long int)m*(unsigned long int)n*(unsigned long int)k;
    //    if (((n_min == 0) && n != m) || mnk <= n_min) {
    if (mnk <= n_min) {
    } else { 
      Rgemm_X(transa, transb, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
      return;
    }
    
//Start the operations.
    if (notb) {
	if (nota) {
//Form C := alpha*A*B + beta*C.
	    Rgemm_NN_omp(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	} else {
//Form  C := alpha*A'*B + beta*C.
	    Rgemm_TN_omp(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}
    } else {
	if (nota) {
//Form  C := alpha*A*B' + beta*C.
	    Rgemm_NT_omp(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	} else {
//Form  C := alpha*A'*B' + beta*C.
	    Rgemm_TT_omp(m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}
    }
    return;
}
