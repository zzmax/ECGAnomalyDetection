/*
 * detectAnomaly.c
 *
 * Code generation for function 'detectAnomaly'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "asSignal.h"
#include "detectAnomaly.h"
#include "diffKmean.h"
#include "nearKmean.h"
#include "asSignal_emxutil.h"
#include "sum.h"

/* Function Definitions */
double* detectAnomaly(const emxArray_real_T *reconstructDelta, double usualMean,
                   double usualStd, double anomalies[6])
{
  double y;
  int k;
  double m;
  int n;
  int d;
  int ix;
  double xbar;
  double r;
  emxArray_boolean_T *b_reconstructDelta;
  double s;
  emxArray_boolean_T *b_m;
  double outliers;
  emxArray_boolean_T *c_m;
  double A;
  emxArray_boolean_T *d_m;
  if (reconstructDelta->size[0] == 0) {
    y = 0.0;
  } else {
    y = reconstructDelta->data[0];
    for (k = 2; k <= reconstructDelta->size[0]; k++) {
      y += reconstructDelta->data[k - 1];
    }
  }

  m = y / (double)reconstructDelta->size[0];
  n = reconstructDelta->size[0];
  if (reconstructDelta->size[0] > 1) {
    d = reconstructDelta->size[0] - 1;
  } else {
    d = reconstructDelta->size[0];
  }

  if (reconstructDelta->size[0] == 0) {
    y = 0.0;
  } else {
    ix = 0;
    xbar = reconstructDelta->data[0];
    for (k = 2; k <= n; k++) {
      ix++;
      xbar += reconstructDelta->data[ix];
    }

    xbar /= (double)reconstructDelta->size[0];
    ix = 0;
    r = reconstructDelta->data[0] - xbar;
    y = r * r;
    for (k = 2; k <= n; k++) {
      ix++;
      r = reconstructDelta->data[ix] - xbar;
      y += r * r;
    }

    y /= (double)d;
  }

  emxInit_boolean_T(&b_reconstructDelta, 1);
  s = sqrt(y);
  y = usualMean + 4.0 * usualStd;
  xbar = usualMean - 4.0 * usualStd;
  k = b_reconstructDelta->size[0];
  b_reconstructDelta->size[0] = reconstructDelta->size[0];
  emxEnsureCapacity((emxArray__common *)b_reconstructDelta, k, (int)sizeof
                    (boolean_T));
  n = reconstructDelta->size[0];
  for (k = 0; k < n; k++) {
    b_reconstructDelta->data[k] = ((reconstructDelta->data[k] > y) == 1 +
      (reconstructDelta->data[k] < xbar));
  }

  emxInit_boolean_T(&b_m, 1);
  outliers = sum(b_reconstructDelta);
  xbar = m - s;
  y = m + s;
  k = b_m->size[0];
  b_m->size[0] = reconstructDelta->size[0];
  emxEnsureCapacity((emxArray__common *)b_m, k, (int)sizeof(boolean_T));
  n = reconstructDelta->size[0];
  emxFree_boolean_T(&b_reconstructDelta);
  for (k = 0; k < n; k++) {
    b_m->data[k] = ((xbar < reconstructDelta->data[k]) &&
                    (reconstructDelta->data[k] < y));
  }

  emxInit_boolean_T(&c_m, 1);
  A = sum(b_m);
  xbar = m - 2.0 * s;
  y = m + 2.0 * s;
  k = c_m->size[0];
  c_m->size[0] = reconstructDelta->size[0];
  emxEnsureCapacity((emxArray__common *)c_m, k, (int)sizeof(boolean_T));
  n = reconstructDelta->size[0];
  emxFree_boolean_T(&b_m);
  for (k = 0; k < n; k++) {
    c_m->data[k] = ((xbar < reconstructDelta->data[k]) &&
                    (reconstructDelta->data[k] < y));
  }

  emxInit_boolean_T(&d_m, 1);
  r = sum(c_m);
  xbar = m - 3.0 * s;
  m += 3.0 * s;
  k = d_m->size[0];
  d_m->size[0] = reconstructDelta->size[0];
  emxEnsureCapacity((emxArray__common *)d_m, k, (int)sizeof(boolean_T));
  n = reconstructDelta->size[0];
  emxFree_boolean_T(&c_m);
  for (k = 0; k < n; k++) {
    d_m->data[k] = ((xbar < reconstructDelta->data[k]) &&
                    (reconstructDelta->data[k] < m));
  }

  y = sum(d_m);
  anomalies[0] = outliers;
  anomalies[1] = A;
  anomalies[2] = r;
  anomalies[3] = y;
  emxFree_boolean_T(&d_m);
  for (k = 0; k < 2; k++) {
    anomalies[k + 4] = 0.0;
  }
  return anomalies;
}

/* End of code generation (detectAnomaly.c) */
