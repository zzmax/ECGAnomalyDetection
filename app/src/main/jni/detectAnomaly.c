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
#include "std.h"
#include "mean.h"

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
  int tmp[2];
  emxArray_real_T *window;
  int b_tmp;
  double b_r[1000];
  emxArray_boolean_T *r0;
  emxArray_real_T *r1;
  emxArray_boolean_T *r2;
  emxArray_boolean_T *r3;
  double changesM;
  emxArray_boolean_T *r4;
  double changesS;
  if (reconstructDelta->size[1] == 0) {
    y = 0.0;
  } else {
    y = reconstructDelta->data[0];
    for (k = 2; k <= reconstructDelta->size[1]; k++) {
      y += reconstructDelta->data[k - 1];
    }
  }

  m = y / (double)reconstructDelta->size[1];
  n = reconstructDelta->size[1];
  if (reconstructDelta->size[1] > 1) {
    d = reconstructDelta->size[1] - 1;
  } else {
    d = reconstructDelta->size[1];
  }

  if (reconstructDelta->size[1] == 0) {
    y = 0.0;
  } else {
    ix = 0;
    xbar = reconstructDelta->data[0];
    for (k = 2; k <= n; k++) {
      ix++;
      xbar += reconstructDelta->data[ix];
    }

    xbar /= (double)reconstructDelta->size[1];
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

  emxInit_boolean_T(&b_reconstructDelta, 2);
  s = sqrt(y);
  d = b_reconstructDelta->size[0] * b_reconstructDelta->size[1];
  b_reconstructDelta->size[0] = 1;
  b_reconstructDelta->size[1] = reconstructDelta->size[1];
  emxEnsureCapacity((emxArray__common *)b_reconstructDelta, d, (int)sizeof
                    (boolean_T));
  y = usualMean + 4.0 * usualStd;
  xbar = usualMean - 4.0 * usualStd;
  n = reconstructDelta->size[0] * reconstructDelta->size[1];
  for (d = 0; d < n; d++) {
    b_reconstructDelta->data[d] = ((reconstructDelta->data[d] > y) == 1 +
      (reconstructDelta->data[d] < xbar));
  }

  emxInit_boolean_T(&b_m, 2);
  outliers = sum(b_reconstructDelta);
  d = b_m->size[0] * b_m->size[1];
  b_m->size[0] = 1;
  b_m->size[1] = reconstructDelta->size[1];
  emxEnsureCapacity((emxArray__common *)b_m, d, (int)sizeof(boolean_T));
  xbar = m - s;
  y = m + s;
  n = reconstructDelta->size[0] * reconstructDelta->size[1];
  emxFree_boolean_T(&b_reconstructDelta);
  for (d = 0; d < n; d++) {
    b_m->data[d] = ((xbar < reconstructDelta->data[d]) &&
                    (reconstructDelta->data[d] < y));
  }

  emxInit_boolean_T(&c_m, 2);
  A = sum(b_m);
  d = c_m->size[0] * c_m->size[1];
  c_m->size[0] = 1;
  c_m->size[1] = reconstructDelta->size[1];
  emxEnsureCapacity((emxArray__common *)c_m, d, (int)sizeof(boolean_T));
  xbar = m - 2.0 * s;
  y = m + 2.0 * s;
  n = reconstructDelta->size[0] * reconstructDelta->size[1];
  emxFree_boolean_T(&b_m);
  for (d = 0; d < n; d++) {
    c_m->data[d] = ((xbar < reconstructDelta->data[d]) &&
                    (reconstructDelta->data[d] < y));
  }

  emxInit_boolean_T(&d_m, 2);
  r = sum(c_m);
  d = d_m->size[0] * d_m->size[1];
  d_m->size[0] = 1;
  d_m->size[1] = reconstructDelta->size[1];
  emxEnsureCapacity((emxArray__common *)d_m, d, (int)sizeof(boolean_T));
  xbar = m - 3.0 * s;
  m += 3.0 * s;
  n = reconstructDelta->size[0] * reconstructDelta->size[1];
  emxFree_boolean_T(&c_m);
  for (d = 0; d < n; d++) {
    d_m->data[d] = ((xbar < reconstructDelta->data[d]) &&
                    (reconstructDelta->data[d] < m));
  }

  y = sum(d_m);
  emxFree_boolean_T(&d_m);
  for (d = 0; d < 2; d++) {
    tmp[d] = reconstructDelta->size[d];
  }

  emxInit_real_T(&window, 2);
  b_tmp = reconstructDelta->size[1];
  d = window->size[0] * window->size[1];
  window->size[0] = 1000;
  window->size[1] = tmp[1] - 1000;
  emxEnsureCapacity((emxArray__common *)window, d, (int)sizeof(double));
  n = 1000 * (tmp[1] - 1000);
  for (d = 0; d < n; d++) {
    window->data[d] = 0.0;
  }

  for (k = 0; k <= b_tmp - 1001; k++) {
    for (d = 0; d < 1000; d++) {
      b_r[d] = reconstructDelta->data[d + k];
    }

    for (d = 0; d < 1000; d++) {
      window->data[d + window->size[0] * k] = b_r[d];
    }
  }

  emxInit_boolean_T(&r0, 2);
  emxInit_real_T(&r1, 2);
  mean(window, r1);
  d = r0->size[0] * r0->size[1];
  r0->size[0] = 1;
  r0->size[1] = r1->size[1];
  emxEnsureCapacity((emxArray__common *)r0, d, (int)sizeof(boolean_T));
  n = r1->size[0] * r1->size[1];
  for (d = 0; d < n; d++) {
    r0->data[d] = (usualMean - 1.0 > r1->data[d]);
  }

  emxInit_boolean_T(&r2, 2);
  mean(window, r1);
  d = r2->size[0] * r2->size[1];
  r2->size[0] = 1;
  r2->size[1] = r1->size[1];
  emxEnsureCapacity((emxArray__common *)r2, d, (int)sizeof(boolean_T));
  n = r1->size[0] * r1->size[1];
  for (d = 0; d < n; d++) {
    r2->data[d] = (r1->data[d] > usualMean + 1.0);
  }

  emxInit_boolean_T(&r3, 2);
  d = r3->size[0] * r3->size[1];
  r3->size[0] = 1;
  r3->size[1] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)r3, d, (int)sizeof(boolean_T));
  n = r0->size[0] * r0->size[1];
  for (d = 0; d < n; d++) {
    r3->data[d] = (r0->data[d] || r2->data[d]);
  }

  changesM = sum(r3);
  b_std(window, r1);
  d = r0->size[0] * r0->size[1];
  r0->size[0] = 1;
  r0->size[1] = r1->size[1];
  emxEnsureCapacity((emxArray__common *)r0, d, (int)sizeof(boolean_T));
  n = r1->size[0] * r1->size[1];
  emxFree_boolean_T(&r3);
  for (d = 0; d < n; d++) {
    r0->data[d] = (usualStd - 2.0 > r1->data[d]);
  }

  b_std(window, r1);
  d = r2->size[0] * r2->size[1];
  r2->size[0] = 1;
  r2->size[1] = r1->size[1];
  emxEnsureCapacity((emxArray__common *)r2, d, (int)sizeof(boolean_T));
  n = r1->size[0] * r1->size[1];
  emxFree_real_T(&window);
  for (d = 0; d < n; d++) {
    r2->data[d] = (r1->data[d] > usualStd + 2.0);
  }

  emxFree_real_T(&r1);
  emxInit_boolean_T(&r4, 2);
  d = r4->size[0] * r4->size[1];
  r4->size[0] = 1;
  r4->size[1] = r0->size[1];
  emxEnsureCapacity((emxArray__common *)r4, d, (int)sizeof(boolean_T));
  n = r0->size[0] * r0->size[1];
  for (d = 0; d < n; d++) {
    r4->data[d] = (r0->data[d] || r2->data[d]);
  }

  emxFree_boolean_T(&r2);
  emxFree_boolean_T(&r0);
  changesS = sum(r4);
  anomalies[0] = outliers;
  anomalies[1] = A / (double)reconstructDelta->size[1];
  anomalies[2] = r / (double)reconstructDelta->size[1];
  anomalies[3] = y / (double)reconstructDelta->size[1];
  anomalies[4] = changesM;
  anomalies[5] = changesS;
  emxFree_boolean_T(&r4);
  return anomalies;
}

/* End of code generation (detectAnomaly.c) */
