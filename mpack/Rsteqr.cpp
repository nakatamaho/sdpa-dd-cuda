/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Nakata, Maho
 * 
 * $Id: Rsteqr.cpp,v 1.9 2009/09/26 02:21:32 nakatamaho Exp $ 
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
#include <stdio.h> //for untested part
void
Rsteqr(const char *compz, mpackint n, dd_real * d, dd_real * e, dd_real * Z,
    mpackint ldz, dd_real * work, mpackint *info)
{
    mpackint nmaxit, maxit, jtot, l1, nm1;
    mpackint i, m, mm, mm1, l, lm1, lend, lsv, lendsv, lendm1, iscale, icompz;
    mpackint lendp1, ii, k, j;

    dd_real Zero = 0.0, One = 1.0, Two = 2.0, Three = 3.0;
    dd_real eps, eps2, safmin, safmax, ssfmax, ssfmin;
    dd_real c, s, rt1, rt2;
    dd_real tst, anorm, po;
    dd_real f, b, p, g, r;

    maxit = 30;

    *info = 0;
    if (Mlsame_dd(compz, "N")) {
	icompz = 0;
    } else if (Mlsame_dd(compz, "V")) {
	icompz = 1;
    } else if (Mlsame_dd(compz, "I")) {
	icompz = 2;
    } else {
	icompz = -1;
    }
    if (icompz < 0) {
	*info = -1;
    } else if (n < 0) {
	*info = -2;
    } else if (ldz < 1 || (icompz > 0 && ldz < max((mpackint)1, n)) ) {
	*info = -6;
    }
    if (*info != 0) {
	Mxerbla_dd("Rsteqr", -(*info));
	return;
    }
//Quick return if possible
    if (n == 0)
	return;
    if (n == 1) {
	if (icompz == 2) {
	    Z[0] = One;
	}
	return;
    }
//Determine the unit roundoff and over/underflow thresholds.
    eps = Rlamch_dd("E");
    eps2 = eps * eps;
    safmin = Rlamch_dd("S");
    safmax = One / safmin;
    ssfmax = sqrt(safmax) / Three;
    ssfmin = sqrt(safmin) / eps2;
//Compute the eigenvalues and eigenvectors of the tridiagonal
//matrix.
    if (icompz == 2)
	Rlaset("Full", n, n, Zero, One, Z, ldz);

    nmaxit = n * 30;
    jtot = 0;
//Determine where the matrix splits and choose QL or QR iteration
//for each block, according to whether top or bottom diagonal
//element is smaller.
    l1 = 1;
    nm1 = n - 1;

  L10:
    if (l1 > n) {
	goto L160;
    }
    if (l1 > 1) {
	e[l1 - 2] = Zero;
    }
    if (l1 <= nm1) {
	for (m = l1; m <= nm1; m++) {
	    tst = abs(e[m - 1]);
	    if (tst == Zero) {
		goto L30;
	    }
	    if (tst <= sqrt(abs(d[m - 1])) * sqrt(abs(d[m])) * eps) {
		e[m - 1] = Zero;
		goto L30;
	    }
	}
    }
    m = n;

  L30:
    l = l1;
    lsv = l;
    lend = m;
    lendsv = lend;
    l1 = m + 1;
    if (lend == l) {
	goto L10;
    }
//Scale submatrix in rows and columns L to LEND
    anorm = Rlanst("I", lend - l + 1, &d[l - 1], &e[l - 1]);
    iscale = 0;
    if (anorm == Zero) {
	goto L10;
    }
    if (anorm > ssfmax) {
	iscale = 1;
	printf("XXX Rsteqr not tested 1\n");
	Rlascl("G", 0, 0, anorm, ssfmax, lend - l + 1, 1, &d[l - 1], n, info);
	Rlascl("G", 0, 0, anorm, ssfmax, lend - l, 1, &e[l - 1], n, info);
    } else if (anorm < ssfmin) {
	iscale = 2;
	printf("XXX Rsteqr not tested 2\n");
	Rlascl("G", 0, 0, anorm, ssfmin, lend - l + 1, 1, &d[l - 1], n, info);
	Rlascl("G", 0, 0, anorm, ssfmin, lend - l, 1, &e[l - 1], n, info);
    }
//Choose between QL and QR iteration
    if (abs(d[lend - 1]) < abs(d[l - 1])) {
	lend = lsv;
	l = lendsv;
    }
    if (lend > l) {
//QL Iteration
//Look for small subdiagonal element.
      L40:
	if (l != lend) {
	    lendm1 = lend - 1;
	    for (m = l; m <= lendm1; m++) {
		tst = abs(e[m - 1]) * abs(e[m - 1]);
		if (tst <= eps2 * abs(d[m - 1]) * abs(d[m]) + safmin) {
		    goto L60;
		}
	    }
	}
	m = lend;
      L60:
	if (m < lend) {
	    e[m - 1] = Zero;
	}
	p = d[l - 1];
	if (m == l) {
	    goto L80;
	}
//If remaining matrix is 2-by-2, use DLAE2 or SLAEV2
//to compute its eigensystem.
	if (m == l + 1) {
	    if (icompz > 0) {
		Rlaev2(d[l - 1], e[l - 1], d[l], &rt1, &rt2, &c, &s);
		work[l - 1] = c;
		work[n - 1 + l - 1] = s;
		Rlasr("R", "V", "B", n, 2, &work[l - 1], &work[n + l - 2],
		    &Z[0 + (l - 1) * ldz], ldz);
	    } else {
		Rlae2(d[l - 1], e[l - 1], d[l], &rt1, &rt2);
	    }
	    d[l - 1] = rt1;
	    d[l] = rt2;
	    e[l - 1] = Zero;
	    l = l + 2;
	    if (l <= lend) {
		goto L40;
	    }
	    goto L140;
	}
	if (jtot == nmaxit) {
	    goto L140;
	}
	jtot++;
//Form shift.
	g = (d[l] - p) / (e[l - 1] * Two);
	r = Rlapy2(g, One);
	g = d[m - 1] - p + e[l - 1] / (g + Msign(r, g));

	s = One;
	c = One;
	p = Zero;
//Inner loop
	mm1 = m - 1;
	for (i = mm1; i >= l; i--) {
	    f = s * e[i - 1];
	    b = c * e[i - 1];
	    Rlartg(g, f, &c, &s, &r);
	    if (i != m - 1) {
		e[i] = r;
	    }
	    g = d[i] - p;
	    r = (d[i - 1] - g) * s + c * Two * b;
	    p = s * r;
	    d[i] = g + p;
	    g = c * r - b;
//If eigenvectors are desired, then save rotations.
	    if (icompz > 0) {
		work[i - 1] = c;
		work[n - 2 + i] = -s;
	    }
	}
//If eigenvectors are desired, then apply saved rotations.
	if (icompz > 0) {
	    mm = m - l + 1;
	    Rlasr("R", "V", "B", n, mm, &work[l - 1], &work[n - 2 + l],
		&Z[0 + (l - 1) * ldz], ldz);
	}
	d[l - 1] = d[l - 1] - p;
	e[l - 1] = g;
	goto L40;
//Eigenvalue found.
      L80:
	d[l - 1] = p;

	l++;
	if (l <= lend) {
	    goto L40;
	}
	goto L140;
    } else {
//QR Iteration
//Look for small superdiagonal element.
      L90:
	if (l != lend) {
	    lendp1 = lend + 1;
	    for (m = l; m >= lendp1; m--) {
		tst = abs(e[m - 2]) * abs(e[m - 2]);
		if (tst <= eps2 * abs(d[m - 1]) * abs(d[m - 2]) + safmin) {
		    goto L110;
		}
	    }
	}
	m = lend;
      L110:
	if (m > lend) {
	    e[m - 2] = Zero;
	}
	p = d[l - 1];
	if (m == l) {
	    goto L130;
	}
//If remaining matrix is 2-by-2, use DLAE2 or SLAEV2
//to compute its eigensystem.
	if (m == l - 1) {
	    if (icompz > 0) {
		Rlaev2(d[l - 2], e[l - 2], d[l - 1], &rt1, &rt2, &c, &s);
		work[m - 1] = c;
		work[n - 2 + m] = s;
		Rlasr("R", "V", "F", n, 2, &work[m - 1], &work[n - 2 + m],
		    &Z[0 + (l - 2) * ldz], ldz);
	    } else {
		Rlae2(d[l - 2], e[l - 2], d[l - 1], &rt1, &rt2);
	    }
	    d[l - 2] = rt1;
	    d[l - 1] = rt2;
	    e[l - 2] = Zero;
	    l = l - 2;
	    if (l >= lend) {
		goto L90;
	    }
	    goto L140;
	}
	if (jtot == nmaxit) {
	    goto L140;
	}
	jtot++;
//Form shift.
	g = (d[l - 2] - p) / (e[l - 2] * Two);
	r = Rlapy2(g, One);
	g = d[m - 1] - p + e[l - 2] / (g + Msign(r, g));
	s = One;
	c = One;
	p = Zero;
//Inner loop
	lm1 = l - 1;
	for (i = m; i <= lm1; i++) {
	    f = s * e[i - 1];
	    b = c * e[i - 1];
	    Rlartg(g, f, &c, &s, &r);
	    if (i != m) {
		e[i - 2] = r;
	    }
	    g = d[i - 1] - p;
	    r = (d[i] - g) * s + c * Two * b;
	    p = s * r;
	    d[i - 1] = g + p;
	    g = c * r - b;
//If eigenvectors are desired, then save rotations.
	    if (icompz > 0) {
		work[i - 1] = c;
		work[n - 2 + i] = s;
	    }
	}
//If eigenvectors are desired, then apply saved rotations.
	if (icompz > 0) {
	    mm = l - m + 1;
	    Rlasr("R", "V", "F", n, mm, &work[m - 1], &work[n - 2 + m],
		&Z[0 + (m - 1) * ldz], ldz);
	}
	d[l - 1] = d[l - 1] - p;
	e[lm1 - 1] = g;
	goto L90;
//Eigenvalue found.
      L130:
	d[l - 1] = p;
	l--;
	if (l >= lend) {
	    goto L90;
	}
	goto L140;
    }
//Undo scaling if necessary
  L140:
    if (iscale == 1) {
	Rlascl("G", 0, 0, ssfmax, anorm, lendsv - lsv + 1, 1, &d[lsv - 1], n,
	    info);
	Rlascl("G", 0, 0, ssfmax, anorm, lendsv - lsv, 1, &e[lsv - 1], n,
	    info);
    } else if (iscale == 2) {
	Rlascl("G", 0, 0, ssfmin, anorm, lendsv - lsv + 1, 1, &d[lsv - 1], n,
	    info);
	Rlascl("G", 0, 0, ssfmin, anorm, lendsv - lsv, 1, &e[lsv - 1], n,
	    info);
    }
//Check for no convergence to an eigenvalue after a total
//of N*MAXIT iterations.
    if (jtot < nmaxit) {
	goto L10;
    }
    for (i = 1; i <= n - 1; i++) {
	if (e[i] != Zero) {
	    ++(*info);
	}
    }
    goto L190;
//Order eigenvalues and eigenvectors.
  L160:
    if (icompz == 0) {
//Use Quick Sort
	Rlasrt("I", n, d, info);
    } else {
//Use Selection Sort to minimize swaps of eigenvectors
	for (ii = 2; ii <= n; ii++) {
	    i = ii - 1;
	    k = i;
	    p = d[i - 1];
	    for (j = ii; j <= n; j++) {
		if (d[j - 1] < p) {
		    k = j;
		    p = d[j - 1];
		}
	    }
	    if (k != i) {
		d[k - 1] = d[i - 1];
		d[i - 1] = p;
		Rswap(n, &Z[0 + (i - 1) * ldz], 1, &Z[0 + (k - 1) * ldz], 1);
	    }
	}
    }
  L190:
    return;
}
