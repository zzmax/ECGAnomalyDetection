/*
 * detectAnomaly.h
 *
 * Code generation for function 'detectAnomaly'
 *
 */

#ifndef __DETECTANOMALY_H__
#define __DETECTANOMALY_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "asSignal_types.h"

/* Function Declarations */
extern double* detectAnomaly(const emxArray_real_T *reconstructDelta, double
  usualMean, double usualStd, double anomalies[6]);

#endif

/* End of code generation (detectAnomaly.h) */
