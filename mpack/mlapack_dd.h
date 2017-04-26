/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: mlapack_dd.h,v 1.6 2009/09/22 20:27:18 nakatamaho Exp $ 
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

#ifndef _MLAPACK_DD_H_
#define _MLAPACK_DD_H_

/* this is a subset of mpack for SDPA-GMP only */
/* http://mplapack.sourceforge.net/ */

/* mlapack prototypes */
void Rsteqr(const char *compz, mpackint n, dd_real * d, dd_real * e,
    dd_real * Z, mpackint ldz, dd_real * work, mpackint *info);
void
    Rsyev(const char *jobz, const char *uplo, mpackint n, dd_real * A,
    mpackint lda, dd_real * w, dd_real * work, mpackint *lwork, mpackint *info);
void Rpotrf(const char *uplo, mpackint n, dd_real * A, mpackint lda, mpackint *info);
mpackint iMlaenv_dd(mpackint ispec, const char *name, const char *opts, mpackint n1, mpackint n2,
    mpackint n3, mpackint n4);
dd_real Rlamch_dd(const char *cmach);
dd_real Rlansy(const char *norm, const char *uplo, mpackint n, dd_real * A,
    mpackint lda, dd_real * work);
void Rlascl(const char *type, mpackint kl, mpackint ku, dd_real cfrom, dd_real cto,
    mpackint m, mpackint n, dd_real * A, mpackint lda, mpackint *info);
void Rsytrd(const char *uplo, mpackint n, dd_real * A, mpackint lda, dd_real * d,
    dd_real * e, dd_real * tau, dd_real * work, mpackint lwork, mpackint *info);
void Rsytd2(const char *uplo, mpackint n, dd_real * A, mpackint lda, dd_real * d,
    dd_real * e, dd_real * tau, mpackint *info);
dd_real Rlanst(const char *norm, mpackint n, dd_real * d, dd_real * e);
void Rlae2(dd_real a, dd_real b, dd_real c, dd_real * rt1,
    dd_real * rt2);
dd_real Rlapy2(dd_real x, dd_real y);
void Rlasrt(const char *id, mpackint n, dd_real * d, mpackint *info);
void Rorgql(mpackint m, mpackint n, mpackint k, dd_real * A, mpackint lda, dd_real * tau,
    dd_real * work, mpackint lwork, mpackint *info);
void Rorgqr(mpackint m, mpackint n, mpackint k, dd_real * A, mpackint lda, dd_real * tau,
    dd_real * work, mpackint lwork, mpackint *info);
void Rlarfg(mpackint N, dd_real * alpha, dd_real * x, mpackint incx,
    dd_real * tau);
void Rlassq(mpackint n, dd_real * x, mpackint incx, dd_real * scale,
    dd_real * sumsq);
void Rorg2l(mpackint m, mpackint n, mpackint k, dd_real * A, mpackint lda, dd_real * tau,
    dd_real * work, mpackint *info);
void Rlarft(const char *direct, const char *storev, mpackint n, mpackint k,
    dd_real * v, mpackint ldv, dd_real * tau, dd_real * t, mpackint ldt);
void Rlarfb(const char *side, const char *trans, const char *direct,
    const char *storev, mpackint m, mpackint n, mpackint k, dd_real * V, mpackint ldv,
    dd_real * T, mpackint ldt, dd_real * C, mpackint ldc, dd_real * work,
    mpackint ldwork);
void Rorg2r(mpackint m, mpackint n, mpackint k, dd_real * A, mpackint lda, dd_real * tau,
    dd_real * work, mpackint *info);
void Rlarf(const char *side, mpackint m, mpackint n, dd_real * v, mpackint incv,
    dd_real tau, dd_real * C, mpackint ldc, dd_real * work);
void Rpotf2(const char *uplo, mpackint n, dd_real * A, mpackint lda, mpackint *info);
void Rlaset(const char *uplo, mpackint m, mpackint n, dd_real alpha, dd_real beta,
    dd_real * A, mpackint lda);
void Rlaev2(dd_real a, dd_real b, dd_real c, dd_real * rt1,
    dd_real * rt2, dd_real * cs1, dd_real * sn1);
void Rlasr(const char *side, const char *pivot, const char *direct, mpackint m,
    mpackint n, dd_real * c, dd_real * s, dd_real * A, mpackint lda);
void Rlartg(dd_real f, dd_real g, dd_real * cs, dd_real * sn,
    dd_real * r);
void Rlatrd(const char *uplo, mpackint n, mpackint nb, dd_real * A, mpackint lda, dd_real * e, dd_real * tau, dd_real * w, mpackint ldw);
void Rsterf(mpackint n, dd_real * d, dd_real * e, mpackint *info);
void Rorgtr(const char *uplo, mpackint n, dd_real * a, mpackint lda, dd_real * tau,
    dd_real * work, mpackint lwork, mpackint *info);
#endif
