bin_PROGRAMS = sdpa_dd

#nVidia settings
NVCCFLAGS = -gencode arch=compute_20,code=sm_20 -m64 -O3 --ptxas-options=-v -I/usr/local/cuda/include -I./mpack -I. -I$(QD_INCLUDEDIR)
if ENABLE_SLOPPY_MUL
NVCCFLAGS += -DSLOPPY_MUL
endif
if ENABLE_CRAY_ADD
NVCCFLAGS += -DCRAY_ADD
endif

NVCC=nvcc
CUDA_LIBS = -L/usr/local/cuda/lib64 -L/usr/local/cuda/lib -lcudart
SUFFIXES= .cu .o
.cu.o:
	$(NVCC) -o $@ -c $< $(NVCCFLAGS)

sdpa_dd_SOURCES = sdpa_chordal.cpp sdpa_dataset.cpp sdpa_io.cpp \
sdpa_jordan.cpp sdpa_linear.cpp sdpa_main.cpp sdpa_newton.cpp sdpa_parts.cpp \
sdpa_struct.cpp sdpa_tool.cpp sdpa_algebra.h sdpa_chordal.h sdpa_dataset.h \
sdpa_include.h sdpa_io.h sdpa_jordan.h sdpa_linear.h sdpa_newton.h \
sdpa_parts.h sdpa_right.h sdpa_struct.h sdpa_tool.h

if ENABLE_CUDA
sdpa_dd_SOURCES += cuda/Rgemm.cu cuda/dd_real_cuda.h \
mpack/Rpotrf.cpp mpack/Rsyrk.cpp mpack/Rtrsm.cpp
else
sdpa_dd_SOURCES += mpack/Rgemm_omp.cpp mpack/Rgemm_NN_omp.cpp mpack/Rgemm_NT_omp.cpp \
			mpack/Rgemm_TN_omp.cpp mpack/Rgemm_TT_omp.cpp \
			mpack/Rpotrf.cpp mpack/Rsyrk.cpp mpack/Rtrsm.cpp 
endif

if !SYSTEM_MPACK
sdpa_dd_SOURCES += \
mpack/Rdot.cpp  mpack/Raxpy.cpp \
mpack/Rtrmm.cpp mpack/Rtrsv.cpp mpack/iMlaenv.cpp\
mpack/Rlamch.cpp mpack/Rlascl.cpp mpack/Rsytrd.cpp mpack/Rsterf.cpp\
mpack/Rorgtr.cpp mpack/Rlatrd.cpp mpack/Rsyr2k.cpp mpack/Rsytd2.cpp \
mpack/Rlanst.cpp mpack/Rlae2.cpp mpack/Rlapy2.cpp mpack/Rlasrt.cpp\
mpack/Rorgql.cpp mpack/Rorgqr.cpp mpack/Rsymv.cpp mpack/Rlarfg.cpp\
mpack/Rsyr2.cpp mpack/Rlassq.cpp mpack/Rorg2l.cpp mpack/Rlarft.cpp\
mpack/Rlarfb.cpp mpack/Rorg2r.cpp mpack/Rnrm2.cpp mpack/Rlarf.cpp \
mpack/Rger.cpp mpack/Mxerbla.cpp mpack/Rpotf2.cpp \
mpack/Mlsame.cpp mpack/Rscal.cpp mpack/Rcopy.cpp mpack/Rgemv.cpp \
mpack/Rtrmv.cpp mpack/Rsteqr.cpp mpack/Rlaset.cpp mpack/Rlaev2.cpp \
mpack/Rlasr.cpp mpack/Rlartg.cpp mpack/Rswap.cpp mpack/Rsyev.cpp \
mpack/Rlansy.cpp mpack/Mutils.cpp \
mpack/mblas_dd.h  mpack/mlapack_dd.h  mpack/mpack_config.h  mpack/mutils_dd.h
endif

if SYSTEM_MPACK
sdpa_dd_LDADD = $(pthread_lib) -L$(SPOOLES_LIBDIR) -lspooles -L$(QD_LIBDIR) -lqd $(OPENMPLIB) -L$(MPACK_LIBDIR) -lmlapack_dd -lmblas_dd
sdpa_dd_CXXFLAGS= -g $(pthread_cflags) -I$(SPOOLES_INCLUDEDIR) -DUseMETIS=0 -I$(QD_INCLUDEDIR) $(OPENMPFLAGS) -I$(MPACK_INCLUDEDIR)
else
sdpa_dd_LDADD = $(pthread_lib) -L$(SPOOLES_LIBDIR) -lspooles -L$(QD_LIBDIR) -lqd $(OPENMPLIB) $(CUDA_LIBS)
sdpa_dd_CXXFLAGS= -g $(pthread_cflags) -I$(SPOOLES_INCLUDEDIR) -DUseMETIS=0 -I$(QD_INCLUDEDIR) $(OPENMPFLAGS) -I./mpack
endif

SUBDIRS = external

EXTRA_DIST = \
param.sdpa \
external/qd/download/*.tar.gz \
external/qd/Makefile.am \
external/qd/Makefile.in \
external/spooles/download/*.tgz \
external/spooles/patches/patch* \
external/spooles/Makefile.am \
external/spooles/Makefile.in \
cuda/Rgemm_NN_p.cu cuda/Rgemm_NT_p.cu cuda/Rgemm_TN_p.cu\
cuda/Rgemm_TT_p.cu cuda/SetDevice.cu cuda/Rgemm_NN_0.cu cuda/Rgemm_NT_0.cu cuda/Rgemm_TN_0.cu\
cuda/Rgemm_TT_0.cu cuda/Rgemm_gpu.cu