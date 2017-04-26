/*
 * Copyright (c) 2008-2010
 *	Takao, Yasuyoshi and Nakata, Maho
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

#include <stdio.h>

void __attribute__ ((constructor)) SetDevice(void);

void SetDevice()
{
    int gpudevice = 0;  // cpu number 0,1,...
        
    int device_count=0;
    int device;

    printf("Initialize device...\n");
    
    cudaGetDeviceCount( &device_count);
    printf("device_count : %d\n", device_count);

    cudaError_t cudareturn;
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp,  gpudevice);

    printf("device name -> %s \n", deviceProp.name);

    if (deviceProp.warpSize <= 1){
        printf("warning, CUDA Device Emulation (CPU)  detected, exiting\n");
        exit(1);
    }
    //set GPU that is to be used
    cudareturn=cudaSetDevice(gpudevice);
    printf("cudareturn -> %d\n", cudareturn);

    if (cudareturn == cudaErrorInvalidDevice){
        perror("cudaSetDevice returned  cudaErrorInvalidDevice");
    }else{
        cudaGetDevice(&device);
        printf("cudaGetDevice()=%d\n" ,device);
    }

    //dummy memory allocation for initialization
    double *Dummyd;
    cudaMalloc((void **)&Dummyd, 16);
    cudaFree(Dummyd);
}
