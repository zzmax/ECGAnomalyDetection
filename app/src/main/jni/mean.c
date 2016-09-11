/*
 * mean.c
 *
 * Code generation for function 'mean'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "asSignal.h"
#include "detectAnomaly.h"
#include "diffKmean.h"
#include "nearKmean.h"
#include "mean.h"
#include "asSignal_emxutil.h"

/* Function Definitions */
void mean(const emxArray_real_T *x, emxArray_real_T *y)
{
  int k;
  int ixstart;
  int ix;
  int iy;
  int i;
  double s;
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  if (x->size[1] == 0) {
    k = y->size[0] * y->size[1];
    y->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
    ixstart = y->size[1];
    for (k = 0; k < ixstart; k++) {
      y->data[y->size[0] * k] = 0.0;
    }
  } else {
    ix = -1;
    iy = -1;
    for (i = 1; i <= x->size[1]; i++) {
      ixstart = ix + 1;
      ix++;
      s = x->data[ixstart];
      for (k = 0; k < 999; k++) {
        ix++;
        s += x->data[ix];
      }

      iy++;
      y->data[iy] = s;
    }
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)y, k, (int)sizeof(double));
  k = y->size[0];
  ixstart = y->size[1];
  ixstart *= k;
  for (k = 0; k < ixstart; k++) {
    y->data[k] /= 1000.0;
  }
}

/* End of code generation (mean.c) */
