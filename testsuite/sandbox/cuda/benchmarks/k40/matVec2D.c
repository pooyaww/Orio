void MatMult_SeqDIA(double* A, double* x, double* y, int M, int N, int P, int NOS, int DOF) {

  register int i,j;
  int col;
  /*@ begin PerfTuning (
        def performance_params {
          param TC[]  = [32,64,128,256];
          param BC[]  = [16,32,48,64,80];
          param UIF[] = range(1,6);
          param PL[]  = [16,48];
          param CFLAGS[] = map(join, product(['','-use_fast_math'],['-O3']));
        }
        def input_params {
          param M[] = [32];
          param N[] = [32];
          param NOS = 5;
          param DOF = 1;
          constraint c1 = (M==N);
        }
        def input_vars {
          decl static double A[M*N*NOS*DOF] = random;
          decl static double x[M*N*DOF]     = random;
          decl static double y[M*N*DOF]     = 0;
          decl static int offsets[NOS]      = {-M*DOF,-DOF,0,DOF,M*DOF};
        }
        def build {
          arg build_command = 'nvcc -g -lineinfo -arch=sm_20 @CFLAGS';
        }
        def performance_counter {
          arg method = 'basic timer';
          arg repetitions = 3;
        }
  ) @*/

  int nrows=M*N;
  int ndiags=NOS;

  /*@ begin Loop(transform CUDA(threadCount=TC, blockCount=BC, preferL1Size=PL, unrollInner=UIF)

  for(i=0; i<=nrows-1; i++) {
    for(j=0; j<=ndiags-1; j++){
      col = i+offsets[j];
      if(col>=0&&col<nrows)
        y[i] += A[i+j*nrows] * x[col];
    }
  }

  ) @*/

  for(i=0; i<=nrows-1; i++) {
    for(j=0; j<=ndiags-1; j++){
      col = i+offsets[j];
      if(col>=0&&col<nrows)
        y[i] += A[i+j*nrows] * x[col];
    }
  }
  /*@ end @*/
  /*@ end @*/
}
