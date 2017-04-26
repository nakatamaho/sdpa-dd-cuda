/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2009 by Nakata, Maho
 * 
 * $Id: mblas_dd.h,v 1.8 2009/09/17 00:59:02 nakatamaho Exp $ 
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

/* this is a subset of mpack for SDPA-GMP only */
/* http://mplapack.sourceforge.net/ */

#ifndef _MBLAS_DD_H_
#define _MBLAS_DD_H_

#include <mpack_config.h>
#include <qd/dd_real.h>
#include <mutils_dd.h>
 
#if !defined __MPACK_ERRNO__
#define _MPACK_EXTERN_ extern
#else
#define _MPACK_EXTERN_
#endif

_MPACK_EXTERN_ int mpack_errno;

/* LEVEL 1 MBLAS */
dd_real Rdot(mpackint n, dd_real * dx, mpackint incx, dd_real * dy,
    mpackint incy);
void Rcopy(mpackint n, dd_real * dx, mpackint incx, dd_real * dy,
    mpackint incy);
void Raxpy(mpackint n, dd_real da, dd_real * dx, mpackint incx, dd_real * dy, mpackint incy);
void Rscal(mpackint n, dd_real ca, dd_real * cx, mpackint incx);
int Mlsame_dd(const char *a, const char *b);
void Mxerbla_dd(const char *srname, int info);
void Rswap(mpackint n, dd_real * dx, mpackint incx, dd_real * dy,
    mpackint incy);
dd_real Rnrm2(mpackint n, dd_real * x, mpackint incx);

/* LEVEL 2 MBLAS */
void Rtrmv(const char *uplo, const char *trans, const char *diag, mpackint n,
    dd_real * A, mpackint lda, dd_real * x, mpackint incx);
void Rtrsv(const char *uplo, const char *trans, const char *diag, mpackint n,
    dd_real * A, mpackint lda, dd_real * x, mpackint incx);
void Rgemv(const char *trans, mpackint m, mpackint n, dd_real alpha,
    dd_real * A, mpackint lda, dd_real * x, mpackint incx, dd_real beta,
    dd_real * y, mpackint incy);
void Rsymv(const char *uplo, mpackint n, dd_real alpha, dd_real * A,
    mpackint lda, dd_real * x, mpackint incx, dd_real beta, dd_real * y,
    mpackint incy);
void Rsyr2(const char *uplo, mpackint n, dd_real alpha, dd_real * x,
    mpackint incx, dd_real * y, mpackint incy, dd_real * A, mpackint lda);
void Rger(mpackint m, mpackint n, dd_real alpha, dd_real * x,
    mpackint incx, dd_real * y, mpackint incy, dd_real * A, mpackint lda);

/* LEVEL 3 MBLAS */
void Rtrmm(const char *side, const char *uplo, const char *transa,
    const char *diag, mpackint m, mpackint n, dd_real alpha, dd_real * A,
    mpackint lda, dd_real * B, mpackint ldb);
void Rtrsm(const char *side, const char *uplo, const char *transa,
    const char *diag, mpackint m, mpackint n, dd_real alpha, dd_real * A,
    mpackint lda, dd_real * B, mpackint ldb);
void Rgemm(const char *transa, const char *transb, mpackint m, mpackint n,
    mpackint k, dd_real alpha, dd_real * A, mpackint lda, dd_real * B,
    mpackint ldb, dd_real beta, dd_real * C, mpackint ldc);
void Rsyr2k(const char *uplo, const char *trans, mpackint n, mpackint k,
    dd_real alpha, dd_real * A, mpackint lda, dd_real * B, mpackint ldb,
    dd_real beta, dd_real * C, mpackint ldc);
void Rsyrk(const char *uplo, const char *trans, mpackint n, mpackint k,
    dd_real alpha, dd_real * A, mpackint lda, dd_real beta,
    dd_real * C, mpackint ldc);

#endif
