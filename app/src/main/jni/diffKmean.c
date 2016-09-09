/*
 * diffKmean.c
 *
 * Code generation for function 'diffKmean'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "asSignal.h"
#include "diffKmean.h"
#include "nearKmean.h"
#include "asSignal_emxutil.h"

/* Function Definitions */
void diffKmean(const emxArray_real_T *clusterIndices, const emxArray_real_T
               *clusters, const emxArray_real_T *data, emxArray_real_T
               *diffWindows)
{
  int i2;
  int i;
  emxArray_real_T *b_diffWindows;
  int b_clusterIndices;
  int loop_ub;
  int i3;
  i2 = diffWindows->size[0] * diffWindows->size[1];
  diffWindows->size[0] = 0;
  diffWindows->size[1] = 60;
  emxEnsureCapacity((emxArray__common *)diffWindows, i2, (int)sizeof(double));
  i = 0;
  emxInit_real_T(&b_diffWindows, 2);
  while (i <= clusterIndices->size[0] - 1) {
    b_clusterIndices = (int)clusterIndices->data[i];
    i2 = b_diffWindows->size[0] * b_diffWindows->size[1];
    b_diffWindows->size[0] = diffWindows->size[0] + 1;
    b_diffWindows->size[1] = 60;
    emxEnsureCapacity((emxArray__common *)b_diffWindows, i2, (int)sizeof(double));
    for (i2 = 0; i2 < 60; i2++) {
      loop_ub = diffWindows->size[0];
      for (i3 = 0; i3 < loop_ub; i3++) {
        b_diffWindows->data[i3 + b_diffWindows->size[0] * i2] =
          diffWindows->data[i3 + diffWindows->size[0] * i2];
      }
    }

    for (i2 = 0; i2 < 60; i2++) {
      b_diffWindows->data[diffWindows->size[0] + b_diffWindows->size[0] * i2] =
        clusters->data[(b_clusterIndices + clusters->size[0] * i2) - 1] -
        data->data[i + data->size[0] * i2];
    }

    i2 = diffWindows->size[0] * diffWindows->size[1];
    diffWindows->size[0] = b_diffWindows->size[0];
    diffWindows->size[1] = 60;
    emxEnsureCapacity((emxArray__common *)diffWindows, i2, (int)sizeof(double));
    for (i2 = 0; i2 < 60; i2++) {
      loop_ub = b_diffWindows->size[0];
      for (i3 = 0; i3 < loop_ub; i3++) {
        diffWindows->data[i3 + diffWindows->size[0] * i2] = b_diffWindows->
          data[i3 + b_diffWindows->size[0] * i2];
      }
    }

    i++;
  }

  emxFree_real_T(&b_diffWindows);
}

/* End of code generation (diffKmean.c) */
