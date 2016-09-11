/*
 * asSignal.c
 *
 * Code generation for function 'asSignal'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "asSignal.h"
#include "detectAnomaly.h"
#include "diffKmean.h"
#include "nearKmean.h"
#include "asSignal_emxutil.h"

/* Function Definitions */
emxArray_real_T * asSignal(const emxArray_real_T *diffWindows, emxArray_real_T *reconstruct)
{
  emxArray_real_T *rDiffWindows;
  int i0;
  int k;
  int nx;
  emxArray_real_T *x;
  int i1;
  emxArray_real_T *tmp;
  emxInit_real_T(&rDiffWindows, 2);
  i0 = rDiffWindows->size[0] * rDiffWindows->size[1];
  rDiffWindows->size[0] = 60;
  rDiffWindows->size[1] = diffWindows->size[0];
  emxEnsureCapacity((emxArray__common *)rDiffWindows, i0, (int)sizeof(double));
  k = diffWindows->size[0];
  for (i0 = 0; i0 < k; i0++) {
    for (nx = 0; nx < 60; nx++) {
      rDiffWindows->data[nx + rDiffWindows->size[0] * i0] = diffWindows->data[i0
        + diffWindows->size[0] * nx];
    }
  }

  if (2 > rDiffWindows->size[1]) {
    i0 = 0;
    nx = 0;
  } else {
    i0 = 1;
    nx = rDiffWindows->size[1];
  }

  emxInit_real_T(&x, 2);
  i1 = x->size[0] * x->size[1];
  x->size[0] = 30;
  x->size[1] = nx - i0;
  emxEnsureCapacity((emxArray__common *)x, i1, (int)sizeof(double));
  k = nx - i0;
  for (nx = 0; nx < k; nx++) {
    for (i1 = 0; i1 < 30; i1++) {
      x->data[i1 + x->size[0] * nx] = rDiffWindows->data[i1 + rDiffWindows->
        size[0] * (i0 + nx)] + rDiffWindows->data[(i1 + rDiffWindows->size[0] *
        nx) + 30];
    }
  }

  emxFree_real_T(&rDiffWindows);
  emxInit_real_T1(&tmp, 1);
  nx = 30 * x->size[1];
  i0 = tmp->size[0];
  tmp->size[0] = nx;
  emxEnsureCapacity((emxArray__common *)tmp, i0, (int)sizeof(double));
  for (k = 0; k + 1 <= nx; k++) {
    tmp->data[k] = x->data[k];
  }

  emxFree_real_T(&x);
  k = tmp->size[0];
  i0 = reconstruct->size[0] * reconstruct->size[1];
  reconstruct->size[0] = 1;
  reconstruct->size[1] = k;
  emxEnsureCapacity((emxArray__common *)reconstruct, i0, (int)sizeof(double));
  for (i0 = 0; i0 < k; i0++) {
    reconstruct->data[reconstruct->size[0] * i0] = tmp->data[i0];
  }

  emxFree_real_T(&tmp);
  return  reconstruct;
}

/* End of code generation (asSignal.c) */
