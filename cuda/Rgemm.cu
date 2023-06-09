/*
 * Copyright (c) 2010-2011
 *      RIKEN
 * 	All rights reserved.
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
/*
  Contributed by Takao, Yasuyoshi and Nakata, Maho, 2010-2011
*/
/*
Based on http://www.netlib.org/blas/dgemm.f
Rgemm performs one of the matrix-matrix operations
 C := alpha*op(A)*op(B) + beta*C,
where op(X) is one of
 op(X) = X or op(X) = X',
alpha and beta are scalars, and A, B and C are matrices, with op( A )
an m by k matrix, op(B) a k by n matrix and C an m by n matrix.
*/

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "mpack/mpack_config.h"

int Mlsame_dd(const char *a, const char *b);
void Mxerbla_dd(const char *srname, int info);

#define __RGEMM_INSIDE_KERNEL__
#include "dd_real_cuda.h"
#include <cuda_runtime.h>
#include <cuda.h>

// matrix block size
#define Bm  (16)
#define Bk  (16)
#define Bn  (16)
#define Gn   (4)

#define fetch_x_A(i) Adev[(i)]
#define fetch_x_B(i) Bdev[(i)]

//for alpha*A*B + beta
__global__ void CalcC_NN0 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);
__global__ void CalcC_NN1 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);

//for alpha*A'*B + beta
__global__ void CalcC_TN0 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);
__global__ void CalcC_TN1 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);

//for alpha*A*B' + beta
__global__ void CalcC_NT0 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);
__global__ void CalcC_NT1 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);

//for alpha*A'*B' + beta
__global__ void CalcC_TT0 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);
__global__ void CalcC_TT1 (dd_real * Adev, dd_real * Bdev, dd_real * Cdev, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc, dd_real alpha, dd_real beta);

void Is_cuda_Rgemm_error(cudaError_t rc, const char *mes, mpackint m, mpackint n, mpackint k, mpackint lda, mpackint ldb, mpackint ldc)
{
    if (rc != cudaSuccess) {
	fprintf(stderr, "%s : m:%d n:%d k:%d lda:%d ldb:%d ldc:%d\n", mes, m, n, k, lda, ldb, ldc);
	exit(1);
    }
    /* not an error */
}

#include "Rgemm_gpu.cu"
#include "Rgemm_NN_0.cu"
#include "Rgemm_NN_p.cu"
#include "Rgemm_TN_0.cu"
#include "Rgemm_TN_p.cu"
#include "Rgemm_NT_0.cu"
#include "Rgemm_NT_p.cu"
#include "Rgemm_TT_0.cu"
#include "Rgemm_TT_p.cu"

static double ops_counter = 0.0;

//allocate device memory for GPU
static dd_real *Adev, *Bdev, *Cdev = NULL;
static int size_A_g, size_B_g, size_C_g;

void Alloc(int size_A, int size_B, int size_C, int m, int n, int k, int lda, int ldb, int ldc)
{
  cudaError_t rc;
  // first alloc
  if (Cdev == NULL) {
    size_A_g = size_A*2;
    size_B_g = size_B*2;
    size_C_g = size_C*2;
    rc = cudaMalloc((void **) &Adev, size_A_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc A error", m, n, k, lda, ldb, ldc);
    rc = cudaMalloc((void **) &Bdev, size_B_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc B error", m, n, k, lda, ldb, ldc);
    rc = cudaMalloc((void **) &Cdev, size_C_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc C error", m, n, k, lda, ldb, ldc);
  }

  if (size_A_g < size_A) {
    rc = cudaFree(Adev);
    size_A_g = size_A*2;
    rc = cudaMalloc((void **) &Adev, size_A_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc A error", m, n, k, lda, ldb, ldc);
  }

  if (size_B_g < size_B) {
    rc = cudaFree(Bdev);
    size_B_g = size_B*2;
    rc = cudaMalloc((void **) &Bdev, size_B_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc B error", m, n, k, lda, ldb, ldc);
  }

  if (size_C_g < size_C) {
    rc = cudaFree(Cdev);
    size_C_g = size_C*2;
    rc = cudaMalloc((void **) &Cdev, size_C_g * sizeof(dd_real));
    Is_cuda_Rgemm_error(rc, "cudaMalloc C error", m, n, k, lda, ldb, ldc);
  }
}



void Rgemm_X(const char *transa, const char *transb, mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb, dd_real beta, dd_real * C, mpackint ldc)
{
    mpackint i, j, nota, notb, nrowa, nrowb, ncola, info;
    dd_real temp, Zero, One;
    cudaError_t rc;

    dd_set(Zero, 0.0, 0.0);
    dd_set(One, 1.0, 0.0);

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
    if (!nota && (!Mlsame_dd(transa, "C"))
	&& (!Mlsame_dd(transa, "T")))
	info = 1;
    else if (!notb && (!Mlsame_dd(transb, "C"))
	     && (!Mlsame_dd(transb, "T")))
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
    if ((m == 0)
	|| (n == 0)
	|| ((dd_eq(alpha, Zero)
	     || (k == 0))
	    && dd_eq(beta, One))) {
	return;
    }

    //And when alpha == 0.0
    if (dd_eq(alpha, Zero)) {
	if (dd_eq(beta, Zero)) {
	    for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
		    C[i + j * ldc] = Zero;
		}
	    }
	} else {
	    for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++) {
		    dd_mul_host(beta, C[i + j * ldc], C[i + j * ldc]);
		}
	    }
	}
	return;
    }

    int size_A, size_B, size_C;
    if (nota)
	size_A = lda * k - (lda - m);
    else
	size_A = lda * m - (lda - k);
    if (notb)
	size_B = ldb * n - (ldb - k);
    else
	size_B = ldb * k - (ldb - n);
    size_C = ldc * n - (ldc - m);

    Alloc(size_A, size_B, size_C, m, n, k, lda, ldb, ldc);
    
    rc = cudaMemcpy(Adev, A, size_A * sizeof(dd_real), cudaMemcpyHostToDevice);
        Is_cuda_Rgemm_error(rc, "cudaMemcpy A error", m, n, k, lda, ldb, ldc);
    rc = cudaMemcpy(Bdev, B, size_B * sizeof(dd_real), cudaMemcpyHostToDevice);
        Is_cuda_Rgemm_error(rc, "cudaMemcpy B error", m, n, k, lda, ldb, ldc);
    rc = cudaMemcpy(Cdev, C, size_C * sizeof(dd_real), cudaMemcpyHostToDevice);
        Is_cuda_Rgemm_error(rc, "cudaMemcpy C error", m, n, k, lda, ldb, ldc);

    Rgemm_gpu(transa, transb, m, n, k, alpha, Adev, lda, Bdev, ldb, beta, Cdev, ldc);

    rc = cudaMemcpy(C, Cdev, size_C * sizeof(dd_real), cudaMemcpyDeviceToHost);
        Is_cuda_Rgemm_error(rc, "cudaMemcpy C error", m, n, k, lda, ldb, ldc);

    return;
}
