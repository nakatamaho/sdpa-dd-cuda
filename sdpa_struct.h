/* -------------------------------------------------------------

This file is a component of SDPA
Copyright (C) 2004 SDPA Project

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

------------------------------------------------------------- */

// printing presicion of such as vector 
#define P_FORMAT "%+18.12e"

#ifndef __sdpa_struct_h__
#define __sdpa_struct_h__

#include <sdpa_include.h>

namespace sdpa {

class Vector
{
public:
  int nDim;
  dd_real* ele;

  Vector();
  Vector(int nDim, dd_real value = 0.0);
  ~Vector();

  void initialize(int nDim, dd_real value = 0.0);
  void initialize(dd_real value);
  void terminate();

  void setZero();
  void display(FILE* fpout = stdout);
  void display(FILE* fpout,dd_real scalar);
  bool copyFrom(Vector& other);
};

class BlockVector
{
public:
  int  nBlock;
  int* blockStruct;

  Vector* ele;
  
  BlockVector();
  BlockVector(int nBlock, int* blockStruct, dd_real value = 0.0);
  ~BlockVector();
  
  void initialize(int nBlock, int* blockStruct, dd_real value = 0.0);
  void initialize(dd_real value);
  void terminate();

  void setZero();
  void display(FILE* fpout = stdout);
  bool copyFrom(BlockVector& other);
};

class SparseMatrix
{
public:
  int nRow, nCol;

  enum Type { SPARSE, DENSE};
  Type type;
  
  int NonZeroNumber;
  // for memory
  int NonZeroCount;
  // currentry stored
  int NonZeroEffect;
  // use for calculation of F1,F2,F3 

  // for Dense
  dd_real* de_ele;

  // for Sparse
  int*    row_index;
  int*    column_index;
  dd_real* sp_ele;

  SparseMatrix();
  SparseMatrix(int nRow,int nCol, Type type, int NonZeroNumber);
  ~SparseMatrix();

  void initialize(int nRow,int nCol, Type type, int NonZeroNumber);
  void terminate();

  void display(FILE* fpout = stdout);
  bool copyFrom(SparseMatrix& other);

  void changeToDense(bool forceChange = false);
  void setZero();
  void setIdentity(dd_real scalar = 1.0);

  bool sortSparseIndex(int&i, int& j);
};

class DenseMatrix
{
public:
  mpackint nRow, nCol;

  enum Type { DENSE, COMPLETION};
  Type type;
  
  dd_real* de_ele;

  DenseMatrix();
  DenseMatrix(int nRow,int nCol, Type type);
  ~DenseMatrix();

  void initialize(int nRow,int nCol, Type type);
  void terminate();
  
  void display(FILE* fpout = stdout);
  bool copyFrom(DenseMatrix& other);
  bool copyFrom(SparseMatrix& other);

  void setZero();
  void setIdentity(dd_real scalar = 1.0);
};

class SparseLinearSpace
{
public:
  int  SDP_sp_nBlock;
  int  SOCP_sp_nBlock;
  int  LP_sp_nBlock;

  int*  SDP_sp_index;
  int*  SOCP_sp_index;
  int*  LP_sp_index;

  SparseMatrix* SDP_sp_block;
  SparseMatrix* SOCP_sp_block;
  dd_real* LP_sp_block;
  
  SparseLinearSpace();
  SparseLinearSpace(int SDP_nBlock, int* SDP_blockStruct, 
		    int* SDP_NonZeroNumber,
		    int SOCP_nBlock, int* SOCP_blockStruct,
		    int* SOCP_NonZeroNumber,
		    int LP_nBlock, bool* LP_NonZeroNumber);
  SparseLinearSpace(int SDP_sp_nBlock, 
                    int* SDP_sp_index,
                    int* SDP_sp_blockStruct, 
                    int* SDP_sp_NonZeroNumber,
                    int SOCP_sp_nBlock, 
                    int* SOCP_sp_index,
                    int* SOCP_sp_blockStruct,
                    int* SOCP_sp_NonZeroNumber,
                    int LP_sp_nBlock, 
                    int* LP_sp_index);
  ~SparseLinearSpace();

  // dense form of block index
  void initialize(int SDP_nBlock, int* SDP_blockStruct, 
		    int* SDP_NonZeroNumber,
		    int SOCP_nBlock, int* SOCP_blockStruct,
		    int* SOCP_NonZeroNumber,
		    int LP_nBlock, bool* LP_NonZeroNumber);
  // sparse form of block index      2008/02/27 kazuhide nakata
  void initialize(int SDP_sp_nBlock, 
                  int* SDP_sp_index,
                  int* SDP_sp_blockStruct, 
                  int* SDP_sp_NonZeroNumber,
                  int SOCP_sp_nBlock, 
                  int* SOCP_sp_index,
                  int* SOCP_sp_blockStruct,
                  int* SOCP_sp_NonZeroNumber,
                  int LP_sp_nBlock, 
                  int* LP_sp_index);
  void terminate();
  
  void changeToDense(bool forceChange=false);
  void display(FILE* fpout = stdout);
  bool copyFrom(SparseLinearSpace& other);
  
  void setElement_SDP(int block, int nCol, int nRow, dd_real ele);
  void setElement_SOCP(int block, int nCol, int nRow, dd_real ele);
  void setElement_LP(int block, dd_real ele);

  void setZero();
  void setIdentity(dd_real scalar = 1.0);
  // no check
  bool sortSparseIndex(int&l , int& i, int& j);
};

class DenseLinearSpace
{
 public:
  int  SDP_nBlock;
  int  SOCP_nBlock;
  int  LP_nBlock;

  DenseMatrix* SDP_block;
  DenseMatrix* SOCP_block;
  dd_real* LP_block;

  DenseLinearSpace();
  DenseLinearSpace(int SDP_nBlock, int* SDP_blockStruct,
		   int SOCP_nBlock,  int* SOCP_blockStruct,
		   int LP_nBlock);
  ~DenseLinearSpace();
  void initialize(int SDP_nBlock, int* SDP_blockStruct,
		  int SOCP_nBlock,  int* SOCP_blockStruct,
		  int LP_nBlock);
  void terminate();

  void display(FILE* fpout = stdout);
  bool copyFrom(DenseLinearSpace& other);
  void setElement_SDP(int block, int nCol, int nRow, dd_real ele);
  void setElement_SOCP(int block, int nCol, int nRow, dd_real ele);
  void setElement_LP(int block, dd_real ele);
  void setZero();
  void setIdentity(dd_real scalar = 1.0);
};

}

#endif // __sdpa_struct_h__
