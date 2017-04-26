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

void Rgemm_gpu(const char *transa, const char *transb, mpackint m, mpackint n, mpackint k, dd_real alpha, dd_real * Adev, mpackint lda, dd_real * Bdev, mpackint ldb, dd_real beta, dd_real * Cdev, mpackint ldc)
{
    mpackint nota, notb;
    cudaError_t rc;

    nota = Mlsame_dd(transa, "N");
    notb = Mlsame_dd(transb, "N");

    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc(32, 32, 32, 32, cudaChannelFormatKindSigned);
    // bind texture memory
    rc = cudaBindTexture(0, tex_x_double_A, Adev, channelDesc);
        Is_cuda_Rgemm_error(rc, "could not bind to texture A", m, n, k, lda, ldb, ldc);

    rc = cudaBindTexture(0, tex_x_double_B, Bdev, channelDesc);
        Is_cuda_Rgemm_error(rc, "could not bind to texture B", m, n, k, lda, ldb, ldc);

    if (notb) {
        if (nota) {
	    //Form C := alpha*A*B + beta*C.
            // calculating and updating C
            dim3 grid(m / Bm + (m % Bm == 0 ? 0 : 1), n / (Gn * Bn)
                      + (n % (Gn * Bn) == 0 ? 0 : 1)), block(Bm, Bn);
            if(m % Bm == 0 && k % Bk == 0 && n % (Gn * Bn) == 0){
                CalcC_NN0 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }else{
                CalcC_NN1 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }
        } else {
	    //Form C := alpha*A'*B + beta*C.
            // calculating and updating C
            dim3 grid(m / Bm + (m % Bm == 0 ? 0 : 1), n / (Gn * Bn)
                      + (n % (Gn * Bn) == 0 ? 0 : 1)), block(Bm, Bn);
            if(m % Bm == 0 && k % Bk == 0 && n % (Gn * Bn) == 0){
                CalcC_TN0 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }else{
                CalcC_TN1 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }
        }
    } else {
        if (nota) {
	    //Form  C := alpha*A*B' + beta*C.
            // calculating and updating C
            dim3 grid(m / Bm + (m % Bm == 0 ? 0 : 1), n / (Gn * Bn)
                      + (n % (Gn * Bn) == 0 ? 0 : 1)), block(Bm, Bn);
            if(m % Bm == 0 && k % Bk == 0 && n % (Gn * Bn) == 0){
                CalcC_NT0 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }else{
                CalcC_NT1 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }
        } else {
	    //Form  C := alpha*A'*B' + beta*C.
            // calculating and updating C
            dim3 grid(m / Bm + (m % Bm == 0 ? 0 : 1), n / (Gn * Bn)
                      + (n % (Gn * Bn) == 0 ? 0 : 1)), block(Bm, Bn);
            if(m % Bm == 0 && k % Bk == 0 && n % (Gn * Bn) == 0){
                CalcC_TT0 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }else{
                CalcC_TT1 <<< grid, block >>> (Adev, Bdev, Cdev, m, n, k, lda, ldb, ldc, alpha, beta);
            }
        }
    }
    //unbind texture
    rc = cudaUnbindTexture(tex_x_double_A);
        Is_cuda_Rgemm_error(rc, "cudaUnbindTexture A error", m, n, k, lda, ldb, ldc);
    rc = cudaUnbindTexture(tex_x_double_B);
        Is_cuda_Rgemm_error(rc, "cudaUnbindTexture B error", m, n, k, lda, ldb, ldc);
    cudaThreadSynchronize();
}

