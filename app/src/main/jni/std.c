/*
 * std.c
 *
 * Code generation for function 'std'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "asSignal.h"
#include "detectAnomaly.h"
#include "diffKmean.h"
#include "nearKmean.h"
#include "std.h"
#include "asSignal_emxutil.h"

/* Function Definitions */
void b_std(const emxArray_real_T *varargin_1, emxArray_real_T *y)
{
  emxArray_real_T *b_y;
  int i;
  int ix;
  double xbar;
  int k;
  double r;
  double c_y;
  emxInit_real_T(&b_y, 2);
  i = b_y->size[0] * b_y->size[1];
  b_y->size[0] = 1;
  b_y->size[1] = varargin_1->size[1];
  emxEnsureCapacity((emxArray__common *)b_y, i, (int)sizeof(double));
  if (varargin_1->size[1] == 0) {
    i = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 1;
    emxEnsureCapacity((emxArray__common *)b_y, i, (int)sizeof(double));
    ix = b_y->size[1];
    for (i = 0; i < ix; i++) {
      b_y->data[b_y->size[0] * i] = 0.0;
    }
  } else {
    for (i = 0; i + 1 <= varargin_1->size[1]; i++) {
      ix = 0;
      xbar = varargin_1->data[varargin_1->size[0] * i];
      for (k = 0; k < 999; k++) {
        ix++;
        xbar += varargin_1->data[ix + varargin_1->size[0] * i];
      }

      xbar /= 1000.0;
      ix = 0;
      r = varargin_1->data[varargin_1->size[0] * i] - xbar;
      c_y = r * r;
      for (k = 0; k < 999; k++) {
        ix++;
        r = varargin_1->data[ix + varargin_1->size[0] * i] - xbar;
        c_y += r * r;
      }

      c_y /= 999.0;
      b_y->data[i] = c_y;
    }
  }

  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = b_y->size[1];
  emxEnsureCapacity((emxArray__common *)y, i, (int)sizeof(double));
  ix = b_y->size[0] * b_y->size[1];
  for (i = 0; i < ix; i++) {
    y->data[i] = b_y->data[i];
  }

  for (k = 0; k + 1 <= b_y->size[1]; k++) {
    y->data[k] = sqrt(y->data[k]);
  }

  emxFree_real_T(&b_y);
}

/* End of code generation (std.c) */
