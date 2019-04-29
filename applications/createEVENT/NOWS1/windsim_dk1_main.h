/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * windsim_dk1_main.h
 *
 * Code generation for function 'windsim_dk1_main'
 *
 */

#ifndef WINDSIM_DK1_MAIN_H
#define WINDSIM_DK1_MAIN_H

/* Include files */
#include <cmath>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "windsim_dk1_main_types.h"

/* Function Declarations */
extern void windsim_dk1_main(char EC, double V10, double drag, double Width,
  emxArray_real_T *HFloors, double seed, emxArray_real_T *windspeed_out,
  emxArray_real_T *windforce_out, double *dt);
extern void windsim_dk1_main_initialize();
extern void windsim_dk1_main_terminate();

#endif

/* End of code generation (windsim_dk1_main.h) */
