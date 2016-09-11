/*
 * diffKmean.h
 *
 * Code generation for function 'diffKmean'
 *
 */

#ifndef __DIFFKMEAN_H__
#define __DIFFKMEAN_H__

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "asSignal_types.h"

/* Function Declarations */
extern emxArray_real_T * diffKmean(const emxArray_real_T *clusterIndices, const
                      emxArray_real_T *clusters, const emxArray_real_T *samples,
                      emxArray_real_T *diffWindows);

#endif

/* End of code generation (diffKmean.h) */
