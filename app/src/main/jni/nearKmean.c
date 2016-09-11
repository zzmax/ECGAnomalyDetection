/*
 * nearKmean.c
 *
 * Code generation for function 'nearKmean'
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
emxArray_real_T * nearKmean(const emxArray_real_T *clusters, const emxArray_real_T *samples,
               emxArray_real_T *clusterIndices)
{
  int numObservarations;
  unsigned int tmp[2];
  int ixstop;
  emxArray_real_T *D;
  int K;
  int ibmat;
  int k;
  emxArray_real_T *r5;
  emxArray_real_T *y;
  emxArray_real_T *a;
  int cindx;
  int n;
  int vstride;
  int iy;
  int ixstart;
  int j;
  int ix;
  double s;
  emxArray_real_T *extremum;
  emxArray_int32_T *iindx;
  int b_ix;
  boolean_T exitg1;
  numObservarations = samples->size[0];
  for (ixstop = 0; ixstop < 2; ixstop++) {
    tmp[ixstop] = (unsigned int)clusters->size[ixstop];
  }

  emxInit_real_T(&D, 2);
  K = clusters->size[0];
  ixstop = D->size[0] * D->size[1];
  D->size[0] = numObservarations;
  D->size[1] = (int)tmp[0];
  emxEnsureCapacity((emxArray__common *)D, ixstop, (int)sizeof(double));
  ibmat = numObservarations * (int)tmp[0];
  for (ixstop = 0; ixstop < ibmat; ixstop++) {
    D->data[ixstop] = 0.0;
  }

  k = 0;
  emxInit_real_T1(&r5, 1);
  emxInit_real_T(&y, 2);
  emxInit_real_T(&a, 2);
  while (k <= K - 1) {
    /* d = sum((x-y).^2).^0.5 */
    ixstop = a->size[0] * a->size[1];
    a->size[0] = numObservarations;
    a->size[1] = 60;
    emxEnsureCapacity((emxArray__common *)a, ixstop, (int)sizeof(double));
    if (!(numObservarations == 0)) {
      for (ixstop = 0; ixstop < 60; ixstop++) {
        ibmat = ixstop * numObservarations;
        for (cindx = 1; cindx <= numObservarations; cindx++) {
          a->data[(ibmat + cindx) - 1] = clusters->data[k + clusters->size[0] *
            ixstop];
        }
      }
    }

    ixstop = a->size[0] * a->size[1];
    a->size[0] = samples->size[0];
    a->size[1] = 60;
    emxEnsureCapacity((emxArray__common *)a, ixstop, (int)sizeof(double));
    ibmat = samples->size[0] * samples->size[1];
    for (ixstop = 0; ixstop < ibmat; ixstop++) {
      a->data[ixstop] = samples->data[ixstop] - a->data[ixstop];
    }

    ixstop = y->size[0] * y->size[1];
    y->size[0] = a->size[0];
    y->size[1] = 60;
    emxEnsureCapacity((emxArray__common *)y, ixstop, (int)sizeof(double));
    n = a->size[0] * 60;
    for (ibmat = 0; ibmat + 1 <= n; ibmat++) {
      y->data[ibmat] = a->data[ibmat] * a->data[ibmat];
    }

    ixstop = r5->size[0];
    r5->size[0] = y->size[0];
    emxEnsureCapacity((emxArray__common *)r5, ixstop, (int)sizeof(double));
    if (y->size[0] == 0) {
      ibmat = r5->size[0];
      ixstop = r5->size[0];
      r5->size[0] = ibmat;
      emxEnsureCapacity((emxArray__common *)r5, ixstop, (int)sizeof(double));
      for (ixstop = 0; ixstop < ibmat; ixstop++) {
        r5->data[ixstop] = 0.0;
      }
    } else {
      vstride = y->size[0];
      iy = -1;
      ixstart = -1;
      for (j = 1; j <= vstride; j++) {
        ixstart++;
        ix = ixstart;
        s = y->data[ixstart];
        for (ibmat = 0; ibmat < 59; ibmat++) {
          ix += vstride;
          s += y->data[ix];
        }

        iy++;
        r5->data[iy] = s;
      }
    }

    ibmat = r5->size[0];
    for (ixstop = 0; ixstop < ibmat; ixstop++) {
      D->data[ixstop + D->size[0] * k] = r5->data[ixstop];
    }

    k++;
  }

  emxFree_real_T(&a);
  emxFree_real_T(&y);
  emxFree_real_T(&r5);
  emxInit_real_T1(&extremum, 1);
  emxInit_int32_T(&iindx, 1);
  ixstop = extremum->size[0];
  extremum->size[0] = D->size[0];
  emxEnsureCapacity((emxArray__common *)extremum, ixstop, (int)sizeof(double));
  tmp[0] = (unsigned int)extremum->size[0];
  ixstop = iindx->size[0];
  iindx->size[0] = (int)tmp[0];
  emxEnsureCapacity((emxArray__common *)iindx, ixstop, (int)sizeof(int));
  ibmat = (int)tmp[0];
  emxFree_real_T(&extremum);
  for (ixstop = 0; ixstop < ibmat; ixstop++) {
    iindx->data[ixstop] = 1;
  }

  n = D->size[1];
  vstride = D->size[0];
  ix = 0;
  iy = -1;
  for (j = 1; j <= vstride; j++) {
    ix++;
    ixstart = ix;
    ixstop = ix + (n - 1) * vstride;
    s = D->data[ix - 1];
    ibmat = 1;
    if (n > 1) {
      cindx = 1;
      if (rtIsNaN(D->data[ix - 1])) {
        b_ix = ix + vstride;
        exitg1 = false;
        while ((!exitg1) && ((vstride > 0) && (b_ix <= ixstop))) {
          cindx++;
          ixstart = b_ix;
          if (!rtIsNaN(D->data[b_ix - 1])) {
            s = D->data[b_ix - 1];
            ibmat = cindx;
            exitg1 = true;
          } else {
            b_ix += vstride;
          }
        }
      }

      if (ixstart < ixstop) {
        b_ix = ixstart + vstride;
        while ((vstride > 0) && (b_ix <= ixstop)) {
          cindx++;
          if (D->data[b_ix - 1] < s) {
            s = D->data[b_ix - 1];
            ibmat = cindx;
          }

          b_ix += vstride;
        }
      }
    }

    iy++;
    iindx->data[iy] = ibmat;
  }

  emxFree_real_T(&D);
  ixstop = clusterIndices->size[0];
  clusterIndices->size[0] = iindx->size[0];
  emxEnsureCapacity((emxArray__common *)clusterIndices, ixstop, (int)sizeof
                    (double));
  ibmat = iindx->size[0];
  for (ixstop = 0; ixstop < ibmat; ixstop++) {
    clusterIndices->data[ixstop] = iindx->data[ixstop];
  }

  emxFree_int32_T(&iindx);
  return clusterIndices;
}

/* End of code generation (nearKmean.c) */
