/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * windsim_dk1_main.cpp
 *
 * Code generation for function 'windsim_dk1_main'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "windsim_dk1_main.h"
#include "windsim_dk1_main_emxutil.h"

/* Variable Definitions */
static unsigned int method;
static unsigned int state[2];
static unsigned int b_state[625];

/* Function Declarations */
static void b_diag(const emxArray_real_T *v, emxArray_real_T *d);
static double b_log(double x);
static void b_power(const emxArray_real_T *a, emxArray_real_T *y);
static void b_r2br_r2dit_trig(const creal_T x[16384], const double costab[8193],
  const double sintab[8193], creal_T y[16384]);
static void b_randn(double varargin_2, emxArray_real_T *r);
static void b_rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
                      emxArray_real_T *z);
static double b_sqrt(double x);
static void bluesteinSetup(creal_T wwc[11999]);
static void c_diag(const emxArray_real_T *v, emxArray_real_T *d);
static void c_eml_rand_mt19937ar_stateful_i();
static void c_log(double *x);
static void c_sqrt(double *x);
static void d_sqrt(creal_T *x);
static void diag(const emxArray_real_T *v, emxArray_real_T *d);
static int div_s32_floor(int numerator, int denominator);
static double eml_rand_mcg16807(unsigned int *c_state);
static double eml_rand_mt19937ar(unsigned int c_state[625]);
static double eml_rand_shr3cong(unsigned int c_state[2]);
static void eml_randn_init();
static void generate_twiddle_tables(double costab[8193], double sintab[8193],
  double sintabinv[8193]);
static void genrand_uint32_vector(unsigned int mt[625], unsigned int u[2]);
static double genrandu(unsigned int mt[625]);
static void power(const emxArray_real_T *a, double b, emxArray_real_T *y);
static void r2br_r2dit_trig(const creal_T x[11999], const double costab[8193],
  const double sintab[8193], creal_T y[16384]);
static void r2br_r2dit_trig_impl(const creal_T x[6000], const double costab[8193],
  const double sintab[8193], creal_T y[16384]);
static void randn(double varargin_2);
static void rdivide(const emxArray_real_T *x, emxArray_real_T *z);
static double rt_hypotd_snf(double u0, double u1);
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
static void b_diag(const emxArray_real_T *v, emxArray_real_T *d)
{
  int u0;
  int u1;
  int stride;
  if ((v->size[0] == 1) && (v->size[1] == 1)) {
    u0 = d->size[0];
    d->size[0] = 1;
    emxEnsureCapacity_real_T1(d, u0);
    d->data[0] = v->data[0];
  } else {
    if (0 < v->size[1]) {
      u0 = v->size[0];
      u1 = v->size[1];
      if (u0 < u1) {
        u1 = u0;
      }

      stride = v->size[0] + 1;
    } else {
      u1 = 0;
      stride = 0;
    }

    u0 = d->size[0];
    d->size[0] = u1;
    emxEnsureCapacity_real_T1(d, u0);
    for (u0 = 0; u0 + 1 <= u1; u0++) {
      d->data[u0] = v->data[u0 * stride];
    }
  }
}

static double b_log(double x)
{
  double b_x;
  b_x = x;
  c_log(&b_x);
  return b_x;
}

static void b_power(const emxArray_real_T *a, emxArray_real_T *y)
{
  int unnamed_idx_1;
  int k;
  unnamed_idx_1 = a->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = a->size[1];
  emxEnsureCapacity_real_T(y, k);
  for (k = 0; k + 1 <= unnamed_idx_1; k++) {
    y->data[k] = rt_powd_snf(a->data[k], 1.6666666666666667);
  }
}

static void b_r2br_r2dit_trig(const creal_T x[16384], const double costab[8193],
  const double sintab[8193], creal_T y[16384])
{
  int ix;
  int ju;
  int iy;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  int iheight;
  int istart;
  int j;
  double twid_re;
  double twid_im;
  int ihi;
  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 16383; i++) {
    y[iy] = x[ix];
    iy = 16384;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 16383; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 4096;
  iheight = 16381;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (j = ju; j < 8192; j += ju) {
      twid_re = costab[j];
      twid_im = sintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }

  for (iy = 0; iy < 16384; iy++) {
    y[iy].re *= 6.103515625E-5;
    y[iy].im *= 6.103515625E-5;
  }
}

static void b_randn(double varargin_2, emxArray_real_T *r)
{
  int i3;
  int k;
  unsigned int u0;
  if (method == 0U) {
    i3 = r->size[0] * r->size[1];
    r->size[0] = 3000;
    r->size[1] = (int)varargin_2;
    emxEnsureCapacity_real_T(r, i3);
    i3 = 3000 * r->size[1];
    for (k = 0; k < i3; k++) {
      r->data[k] = eml_rand_mt19937ar(b_state);
    }
  } else {
    i3 = r->size[0] * r->size[1];
    r->size[0] = 3000;
    r->size[1] = (int)varargin_2;
    emxEnsureCapacity_real_T(r, i3);
    if (method == 4U) {
      i3 = 3000 * r->size[1];
      for (k = 0; k < i3; k++) {
        u0 = state[0];
        r->data[k] = eml_rand_mcg16807(&u0);
        state[0] = u0;
      }
    } else {
      i3 = 3000 * r->size[1];
      for (k = 0; k < i3; k++) {
        r->data[k] = eml_rand_shr3cong(state);
      }
    }
  }
}

static void b_rdivide(const emxArray_real_T *x, const emxArray_real_T *y,
                      emxArray_real_T *z)
{
  int i4;
  int loop_ub;
  i4 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity_real_T(z, i4);
  loop_ub = x->size[0] * x->size[1];
  for (i4 = 0; i4 < loop_ub; i4++) {
    z->data[i4] = x->data[i4] / y->data[i4];
  }
}

static double b_sqrt(double x)
{
  double b_x;
  b_x = x;
  c_sqrt(&b_x);
  return b_x;
}

static void bluesteinSetup(creal_T wwc[11999])
{
  int idx;
  int rt;
  int k;
  int y;
  double nt_im;
  double nt_re;
  idx = 5998;
  rt = 0;
  wwc[5999].re = 1.0;
  wwc[5999].im = 0.0;
  for (k = 0; k < 5999; k++) {
    y = ((k + 1) << 1) - 1;
    if (12000 - rt <= y) {
      rt = (y + rt) - 12000;
    } else {
      rt += y;
    }

    nt_im = 3.1415926535897931 * (double)rt / 6000.0;
    if (nt_im == 0.0) {
      nt_re = 1.0;
      nt_im = 0.0;
    } else {
      nt_re = std::cos(nt_im);
      nt_im = std::sin(nt_im);
    }

    wwc[idx].re = nt_re;
    wwc[idx].im = -nt_im;
    idx--;
  }

  idx = 0;
  for (k = 5998; k >= 0; k--) {
    wwc[k + 6000] = wwc[idx];
    idx++;
  }
}

static void c_diag(const emxArray_real_T *v, emxArray_real_T *d)
{
  int unnamed_idx_0;
  int unnamed_idx_1;
  int i6;
  unnamed_idx_0 = v->size[0];
  unnamed_idx_1 = v->size[0];
  i6 = d->size[0] * d->size[1];
  d->size[0] = unnamed_idx_0;
  d->size[1] = unnamed_idx_1;
  emxEnsureCapacity_real_T(d, i6);
  unnamed_idx_0 *= unnamed_idx_1;
  for (i6 = 0; i6 < unnamed_idx_0; i6++) {
    d->data[i6] = 0.0;
  }

  for (unnamed_idx_0 = 0; unnamed_idx_0 + 1 <= v->size[0]; unnamed_idx_0++) {
    d->data[unnamed_idx_0 + d->size[0] * unnamed_idx_0] = v->data[unnamed_idx_0];
  }
}

static void c_eml_rand_mt19937ar_stateful_i()
{
  unsigned int r;
  int mti;
  memset(&b_state[0], 0, 625U * sizeof(unsigned int));
  r = 5489U;
  b_state[0] = 5489U;
  for (mti = 0; mti < 623; mti++) {
    r = ((r ^ r >> 30U) * 1812433253U + mti) + 1U;
    b_state[mti + 1] = r;
  }

  b_state[624] = 624U;
}

static void c_log(double *x)
{
  *x = std::log(*x);
}

static void c_sqrt(double *x)
{
  *x = std::sqrt(*x);
}

static void d_sqrt(creal_T *x)
{
  double xr;
  double xi;
  double absxr;
  xr = x->re;
  xi = x->im;
  if (xi == 0.0) {
    if (xr < 0.0) {
      absxr = 0.0;
      xr = std::sqrt(-xr);
    } else {
      absxr = 0.0;
      xr = 0.0;
    }
  } else if (xr == 0.0) {
    absxr = std::sqrt(-xi / 2.0);
    xr = -absxr;
  } else {
    absxr = std::abs(xr);
    xr = std::abs(xi);
    if ((absxr > 4.4942328371557893E+307) || (xr > 4.4942328371557893E+307)) {
      absxr *= 0.5;
      xr *= 0.5;
      xr = rt_hypotd_snf(absxr, xr);
      if (xr > absxr) {
        absxr = std::sqrt(xr) * std::sqrt(1.0 + absxr / xr);
      } else {
        absxr = std::sqrt(xr) * 1.4142135623730951;
      }
    } else {
      absxr = std::sqrt((rt_hypotd_snf(absxr, xr) + absxr) * 0.5);
    }

    xr = -absxr;
    absxr = 0.5 * (xi / -absxr);
  }

  x->re = absxr;
  x->im = xr;
}

static void diag(const emxArray_real_T *v, emxArray_real_T *d)
{
  int unnamed_idx_0;
  int unnamed_idx_1;
  int i5;
  unnamed_idx_0 = v->size[1];
  unnamed_idx_1 = v->size[1];
  i5 = d->size[0] * d->size[1];
  d->size[0] = unnamed_idx_0;
  d->size[1] = unnamed_idx_1;
  emxEnsureCapacity_real_T(d, i5);
  unnamed_idx_0 *= unnamed_idx_1;
  for (i5 = 0; i5 < unnamed_idx_0; i5++) {
    d->data[i5] = 0.0;
  }

  for (unnamed_idx_0 = 0; unnamed_idx_0 + 1 <= v->size[1]; unnamed_idx_0++) {
    d->data[unnamed_idx_0 + d->size[0] * unnamed_idx_0] = v->data[unnamed_idx_0];
  }
}

static int div_s32_floor(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  unsigned int tempAbsQuotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator < 0) {
      absNumerator = ~(unsigned int)numerator + 1U;
    } else {
      absNumerator = (unsigned int)numerator;
    }

    if (denominator < 0) {
      absDenominator = ~(unsigned int)denominator + 1U;
    } else {
      absDenominator = (unsigned int)denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = absNumerator / absDenominator;
    if (quotientNeedsNegation) {
      absNumerator %= absDenominator;
      if (absNumerator > 0U) {
        tempAbsQuotient++;
      }

      quotient = -(int)tempAbsQuotient;
    } else {
      quotient = (int)tempAbsQuotient;
    }
  }

  return quotient;
}

static double eml_rand_mcg16807(unsigned int *c_state)
{
  double r;
  unsigned int test1;
  int hi;
  unsigned int test2;
  double t;
  test1 = *c_state;
  do {
    hi = (int)(test1 / 127773U);
    test1 = 16807U * (test1 - hi * 127773U);
    test2 = 2836U * hi;
    if (test1 < test2) {
      *c_state = (test1 - test2) + 2147483647U;
    } else {
      *c_state = test1 - test2;
    }

    hi = (int)(*c_state / 127773U);
    test1 = 16807U * (*c_state - hi * 127773U);
    test2 = 2836U * hi;
    if (test1 < test2) {
      test1 = (test1 - test2) + 2147483647U;
    } else {
      test1 -= test2;
    }

    r = 2.0 * ((double)*c_state * 4.6566128752457969E-10) - 1.0;
    t = 2.0 * ((double)test1 * 4.6566128752457969E-10) - 1.0;
    t = t * t + r * r;
  } while (!(t <= 1.0));

  r *= std::sqrt(-2.0 * std::log(t) / t);
  *c_state = test1;
  return r;
}

static double eml_rand_mt19937ar(unsigned int c_state[625])
{
  double r;
  int exitg1;
  unsigned int u32[2];
  int i;
  static const double dv2[257] = { 0.0, 0.215241895984875, 0.286174591792068,
    0.335737519214422, 0.375121332878378, 0.408389134611989, 0.43751840220787,
    0.46363433679088, 0.487443966139235, 0.50942332960209, 0.529909720661557,
    0.549151702327164, 0.567338257053817, 0.584616766106378, 0.601104617755991,
    0.61689699000775, 0.63207223638606, 0.646695714894993, 0.660822574244419,
    0.674499822837293, 0.687767892795788, 0.700661841106814, 0.713212285190975,
    0.725446140909999, 0.737387211434295, 0.749056662017815, 0.760473406430107,
    0.771654424224568, 0.782615023307232, 0.793369058840623, 0.80392911698997,
    0.814306670135215, 0.824512208752291, 0.834555354086381, 0.844444954909153,
    0.854189171008163, 0.863795545553308, 0.87327106808886, 0.882622229585165,
    0.891855070732941, 0.900975224461221, 0.909987953496718, 0.91889818364959,
    0.927710533401999, 0.936429340286575, 0.945058684468165, 0.953602409881086,
    0.96206414322304, 0.970447311064224, 0.978755155294224, 0.986990747099062,
    0.99515699963509, 1.00325667954467, 1.01129241744, 1.01926671746548,
    1.02718196603564, 1.03504043983344, 1.04284431314415, 1.05059566459093,
    1.05829648333067, 1.06594867476212, 1.07355406579244, 1.0811144097034,
    1.08863139065398, 1.09610662785202, 1.10354167942464, 1.11093804601357,
    1.11829717411934, 1.12562045921553, 1.13290924865253, 1.14016484436815,
    1.14738850542085, 1.15458145035993, 1.16174485944561, 1.16887987673083,
    1.17598761201545, 1.18306914268269, 1.19012551542669, 1.19715774787944,
    1.20416683014438, 1.2111537262437, 1.21811937548548, 1.22506469375653,
    1.23199057474614, 1.23889789110569, 1.24578749554863, 1.2526602218949,
    1.25951688606371, 1.26635828701823, 1.27318520766536, 1.27999841571382,
    1.28679866449324, 1.29358669373695, 1.30036323033084, 1.30712898903073,
    1.31388467315022, 1.32063097522106, 1.32736857762793, 1.33409815321936,
    1.3408203658964, 1.34753587118059, 1.35424531676263, 1.36094934303328,
    1.36764858359748, 1.37434366577317, 1.38103521107586, 1.38772383568998,
    1.39441015092814, 1.40109476367925, 1.4077782768464, 1.41446128977547,
    1.42114439867531, 1.42782819703026, 1.43451327600589, 1.44120022484872,
    1.44788963128058, 1.45458208188841, 1.46127816251028, 1.46797845861808,
    1.47468355569786, 1.48139403962819, 1.48811049705745, 1.49483351578049,
    1.50156368511546, 1.50830159628131, 1.51504784277671, 1.521803020761,
    1.52856772943771, 1.53534257144151, 1.542128153229, 1.54892508547417,
    1.55573398346918, 1.56255546753104, 1.56939016341512, 1.57623870273591,
    1.58310172339603, 1.58997987002419, 1.59687379442279, 1.60378415602609,
    1.61071162236983, 1.61765686957301, 1.62462058283303, 1.63160345693487,
    1.63860619677555, 1.64562951790478, 1.65267414708306, 1.65974082285818,
    1.66683029616166, 1.67394333092612, 1.68108070472517, 1.68824320943719,
    1.69543165193456, 1.70264685479992, 1.7098896570713, 1.71716091501782,
    1.72446150294804, 1.73179231405296, 1.73915426128591, 1.74654827828172,
    1.75397532031767, 1.76143636531891, 1.76893241491127, 1.77646449552452,
    1.78403365954944, 1.79164098655216, 1.79928758454972, 1.80697459135082,
    1.81470317596628, 1.82247454009388, 1.83028991968276, 1.83815058658281,
    1.84605785028518, 1.8540130597602, 1.86201760539967, 1.87007292107127,
    1.878180486293, 1.88634182853678, 1.8945585256707, 1.90283220855043,
    1.91116456377125, 1.91955733659319, 1.92801233405266, 1.93653142827569,
    1.94511656000868, 1.95376974238465, 1.96249306494436, 1.97128869793366,
    1.98015889690048, 1.98910600761744, 1.99813247135842, 2.00724083056053,
    2.0164337349062, 2.02571394786385, 2.03508435372962, 2.04454796521753,
    2.05410793165065, 2.06376754781173, 2.07353026351874, 2.0833996939983,
    2.09337963113879, 2.10347405571488, 2.11368715068665, 2.12402331568952,
    2.13448718284602, 2.14508363404789, 2.15581781987674, 2.16669518035431,
    2.17772146774029, 2.18890277162636, 2.20024554661128, 2.21175664288416,
    2.22344334009251, 2.23531338492992, 2.24737503294739, 2.25963709517379,
    2.27210899022838, 2.28480080272449, 2.29772334890286, 2.31088825060137,
    2.32430801887113, 2.33799614879653, 2.35196722737914, 2.36623705671729,
    2.38082279517208, 2.39574311978193, 2.41101841390112, 2.42667098493715,
    2.44272531820036, 2.4592083743347, 2.47614993967052, 2.49358304127105,
    2.51154444162669, 2.53007523215985, 2.54922155032478, 2.56903545268184,
    2.58957598670829, 2.61091051848882, 2.63311639363158, 2.65628303757674,
    2.68051464328574, 2.70593365612306, 2.73268535904401, 2.76094400527999,
    2.79092117400193, 2.82287739682644, 2.85713873087322, 2.89412105361341,
    2.93436686720889, 2.97860327988184, 3.02783779176959, 3.08352613200214,
    3.147889289518, 3.2245750520478, 3.32024473383983, 3.44927829856143,
    3.65415288536101, 3.91075795952492 };

  double u;
  static const double dv3[257] = { 1.0, 0.977101701267673, 0.959879091800108,
    0.9451989534423, 0.932060075959231, 0.919991505039348, 0.908726440052131,
    0.898095921898344, 0.887984660755834, 0.878309655808918, 0.869008688036857,
    0.860033621196332, 0.851346258458678, 0.842915653112205, 0.834716292986884,
    0.826726833946222, 0.818929191603703, 0.811307874312656, 0.803849483170964,
    0.796542330422959, 0.789376143566025, 0.782341832654803, 0.775431304981187,
    0.768637315798486, 0.761953346836795, 0.755373506507096, 0.748892447219157,
    0.742505296340151, 0.736207598126863, 0.729995264561476, 0.72386453346863,
    0.717811932630722, 0.711834248878248, 0.705928501332754, 0.700091918136512,
    0.694321916126117, 0.688616083004672, 0.682972161644995, 0.677388036218774,
    0.671861719897082, 0.66639134390875, 0.660975147776663, 0.655611470579697,
    0.650298743110817, 0.645035480820822, 0.639820277453057, 0.634651799287624,
    0.629528779924837, 0.624450015547027, 0.619414360605834, 0.614420723888914,
    0.609468064925773, 0.604555390697468, 0.599681752619125, 0.594846243767987,
    0.590047996332826, 0.585286179263371, 0.580559996100791, 0.575868682972354,
    0.571211506735253, 0.566587763256165, 0.561996775814525, 0.557437893618766,
    0.552910490425833, 0.548413963255266, 0.543947731190026, 0.539511234256952,
    0.535103932380458, 0.530725304403662, 0.526374847171684, 0.522052074672322,
    0.517756517229756, 0.513487720747327, 0.509245245995748, 0.505028667943468,
    0.500837575126149, 0.49667156905249, 0.492530263643869, 0.488413284705458,
    0.484320269426683, 0.480250865909047, 0.476204732719506, 0.47218153846773,
    0.468180961405694, 0.464202689048174, 0.460246417812843, 0.456311852678716,
    0.452398706861849, 0.448506701507203, 0.444635565395739, 0.440785034665804,
    0.436954852547985, 0.433144769112652, 0.429354541029442, 0.425583931338022,
    0.421832709229496, 0.418100649837848, 0.414387534040891, 0.410693148270188,
    0.407017284329473, 0.403359739221114, 0.399720314980197, 0.396098818515832,
    0.392495061459315, 0.388908860018789, 0.385340034840077, 0.381788410873393,
    0.378253817245619, 0.374736087137891, 0.371235057668239, 0.367750569779032,
    0.364282468129004, 0.360830600989648, 0.357394820145781, 0.353974980800077,
    0.350570941481406, 0.347182563956794, 0.343809713146851, 0.340452257044522,
    0.337110066637006, 0.333783015830718, 0.330470981379163, 0.327173842813601,
    0.323891482376391, 0.320623784956905, 0.317370638029914, 0.314131931596337,
    0.310907558126286, 0.307697412504292, 0.30450139197665, 0.301319396100803,
    0.298151326696685, 0.294997087799962, 0.291856585617095, 0.288729728482183,
    0.285616426815502, 0.282516593083708, 0.279430141761638, 0.276356989295668,
    0.273297054068577, 0.270250256365875, 0.267216518343561, 0.264195763997261,
    0.261187919132721, 0.258192911337619, 0.255210669954662, 0.252241126055942,
    0.249284212418529, 0.246339863501264, 0.24340801542275, 0.240488605940501,
    0.237581574431238, 0.23468686187233, 0.231804410824339, 0.228934165414681,
    0.226076071322381, 0.223230075763918, 0.220396127480152, 0.217574176724331,
    0.214764175251174, 0.211966076307031, 0.209179834621125, 0.206405406397881,
    0.203642749310335, 0.200891822494657, 0.198152586545776, 0.195425003514135,
    0.192709036903589, 0.190004651670465, 0.187311814223801, 0.1846304924268,
    0.181960655599523, 0.179302274522848, 0.176655321443735, 0.174019770081839,
    0.171395595637506, 0.168782774801212, 0.166181285764482, 0.163591108232366,
    0.161012223437511, 0.158444614155925, 0.15588826472448, 0.153343161060263,
    0.150809290681846, 0.148286642732575, 0.145775208005994, 0.143274978973514,
    0.140785949814445, 0.138308116448551, 0.135841476571254, 0.133386029691669,
    0.130941777173644, 0.12850872228, 0.126086870220186, 0.123676228201597,
    0.12127680548479, 0.11888861344291, 0.116511665625611, 0.114145977827839,
    0.111791568163838, 0.109448457146812, 0.107116667774684, 0.104796225622487,
    0.102487158941935, 0.10018949876881, 0.0979032790388625, 0.095628536713009,
    0.093365311912691, 0.0911136480663738, 0.0888735920682759,
    0.0866451944505581, 0.0844285095703535, 0.082223595813203,
    0.0800305158146631, 0.0778493367020961, 0.0756801303589272,
    0.0735229737139814, 0.0713779490588905, 0.0692451443970068,
    0.0671246538277886, 0.065016577971243, 0.0629210244377582, 0.06083810834954,
    0.0587679529209339, 0.0567106901062031, 0.0546664613248891,
    0.0526354182767924, 0.0506177238609479, 0.0486135532158687,
    0.0466230949019305, 0.0446465522512946, 0.0426841449164746,
    0.0407361106559411, 0.0388027074045262, 0.0368842156885674,
    0.0349809414617162, 0.0330932194585786, 0.0312214171919203,
    0.0293659397581334, 0.0275272356696031, 0.0257058040085489,
    0.0239022033057959, 0.0221170627073089, 0.0203510962300445,
    0.0186051212757247, 0.0168800831525432, 0.0151770883079353,
    0.0134974506017399, 0.0118427578579079, 0.0102149714397015,
    0.00861658276939875, 0.00705087547137324, 0.00552240329925101,
    0.00403797259336304, 0.00260907274610216, 0.0012602859304986,
    0.000477467764609386 };

  double x;
  do {
    exitg1 = 0;
    genrand_uint32_vector(c_state, u32);
    i = (int)((u32[1] >> 24U) + 1U);
    r = (((double)(u32[0] >> 3U) * 1.6777216E+7 + (double)((int)u32[1] &
           16777215)) * 2.2204460492503131E-16 - 1.0) * dv2[i];
    if (std::abs(r) <= dv2[i - 1]) {
      exitg1 = 1;
    } else if (i < 256) {
      u = genrandu(c_state);
      if (dv3[i] + u * (dv3[i - 1] - dv3[i]) < std::exp(-0.5 * r * r)) {
        exitg1 = 1;
      }
    } else {
      do {
        u = genrandu(c_state);
        x = std::log(u) * 0.273661237329758;
        u = genrandu(c_state);
      } while (!(-2.0 * std::log(u) > x * x));

      if (r < 0.0) {
        r = x - 3.65415288536101;
      } else {
        r = 3.65415288536101 - x;
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return r;
}

static double eml_rand_shr3cong(unsigned int c_state[2])
{
  double r;
  unsigned int icng;
  unsigned int jsr;
  unsigned int ui;
  int j;
  static const double dv1[65] = { 0.340945, 0.4573146, 0.5397793, 0.6062427,
    0.6631691, 0.7136975, 0.7596125, 0.8020356, 0.8417227, 0.8792102, 0.9148948,
    0.9490791, 0.9820005, 1.0138492, 1.044781, 1.0749254, 1.1043917, 1.1332738,
    1.161653, 1.189601, 1.2171815, 1.2444516, 1.2714635, 1.298265, 1.3249008,
    1.3514125, 1.3778399, 1.4042211, 1.4305929, 1.4569915, 1.4834527, 1.5100122,
    1.5367061, 1.5635712, 1.5906454, 1.617968, 1.6455802, 1.6735255, 1.7018503,
    1.7306045, 1.7598422, 1.7896223, 1.8200099, 1.851077, 1.8829044, 1.9155831,
    1.9492166, 1.9839239, 2.0198431, 2.0571356, 2.095993, 2.136645, 2.1793713,
    2.2245175, 2.2725186, 2.3239338, 2.3795008, 2.4402218, 2.5075117, 2.5834658,
    2.6713916, 2.7769942, 2.7769942, 2.7769942, 2.7769942 };

  double x;
  double y;
  double s;
  icng = 69069U * c_state[0] + 1234567U;
  jsr = c_state[1] ^ c_state[1] << 13;
  jsr ^= jsr >> 17;
  jsr ^= jsr << 5;
  ui = icng + jsr;
  j = (int)((ui & 63U) + 1U);
  r = (double)(int)ui * 4.6566128730773926E-10 * dv1[j];
  if (!(std::abs(r) <= dv1[j - 1])) {
    x = (std::abs(r) - dv1[j - 1]) / (dv1[j] - dv1[j - 1]);
    icng = 69069U * icng + 1234567U;
    jsr ^= jsr << 13;
    jsr ^= jsr >> 17;
    jsr ^= jsr << 5;
    y = 0.5 + (double)(int)(icng + jsr) * 2.328306436538696E-10;
    s = x + y;
    if (s > 1.301198) {
      if (r < 0.0) {
        r = 0.4878992 * x - 0.4878992;
      } else {
        r = 0.4878992 - 0.4878992 * x;
      }
    } else {
      if (!(s <= 0.9689279)) {
        x = 0.4878992 - 0.4878992 * x;
        if (y > 12.67706 - 12.37586 * std::exp(-0.5 * x * x)) {
          if (r < 0.0) {
            r = -x;
          } else {
            r = x;
          }
        } else {
          if (!(std::exp(-0.5 * dv1[j] * dv1[j]) + y * 0.01958303 / dv1[j] <=
                std::exp(-0.5 * r * r))) {
            do {
              icng = 69069U * icng + 1234567U;
              jsr ^= jsr << 13;
              jsr ^= jsr >> 17;
              jsr ^= jsr << 5;
              x = std::log(0.5 + (double)(int)(icng + jsr) *
                           2.328306436538696E-10) / 2.776994;
              icng = 69069U * icng + 1234567U;
              jsr ^= jsr << 13;
              jsr ^= jsr >> 17;
              jsr ^= jsr << 5;
            } while (!(-2.0 * std::log(0.5 + (double)(int)(icng + jsr) *
                                       2.328306436538696E-10) > x * x));

            if (r < 0.0) {
              r = x - 2.776994;
            } else {
              r = 2.776994 - x;
            }
          }
        }
      }
    }
  }

  c_state[0] = icng;
  c_state[1] = jsr;
  return r;
}

static void eml_randn_init()
{
  int i7;
  method = 0U;
  for (i7 = 0; i7 < 2; i7++) {
    state[i7] = 362436069U + -362436069 * i7;
  }

  state[1] = 521288629U;
}

static void generate_twiddle_tables(double costab[8193], double sintab[8193],
  double sintabinv[8193])
{
  double costab1q[4097];
  int k;
  costab1q[0] = 1.0;
  for (k = 0; k < 2048; k++) {
    costab1q[k + 1] = std::cos(0.00038349519697141029 * ((double)k + 1.0));
  }

  for (k = 0; k < 2047; k++) {
    costab1q[k + 2049] = std::sin(0.00038349519697141029 * (4096.0 - ((double)k
      + 2049.0)));
  }

  costab1q[4096] = 0.0;
  costab[0] = 1.0;
  sintab[0] = 0.0;
  for (k = 0; k < 4096; k++) {
    sintabinv[k + 1] = costab1q[4095 - k];
    sintabinv[k + 4097] = costab1q[k + 1];
    costab[k + 1] = costab1q[k + 1];
    sintab[k + 1] = -costab1q[4095 - k];
    costab[k + 4097] = -costab1q[4095 - k];
    sintab[k + 4097] = -costab1q[k + 1];
  }
}

static void genrand_uint32_vector(unsigned int mt[625], unsigned int u[2])
{
  int j;
  unsigned int mti;
  int kk;
  unsigned int y;
  unsigned int b_y;
  unsigned int c_y;
  unsigned int d_y;
  for (j = 0; j < 2; j++) {
    mti = mt[624] + 1U;
    if (mti >= 625U) {
      for (kk = 0; kk < 227; kk++) {
        y = (mt[kk] & 2147483648U) | (mt[kk + 1] & 2147483647U);
        if ((int)(y & 1U) == 0) {
          b_y = y >> 1U;
        } else {
          b_y = y >> 1U ^ 2567483615U;
        }

        mt[kk] = mt[kk + 397] ^ b_y;
      }

      for (kk = 0; kk < 396; kk++) {
        y = (mt[kk + 227] & 2147483648U) | (mt[kk + 228] & 2147483647U);
        if ((int)(y & 1U) == 0) {
          d_y = y >> 1U;
        } else {
          d_y = y >> 1U ^ 2567483615U;
        }

        mt[kk + 227] = mt[kk] ^ d_y;
      }

      y = (mt[623] & 2147483648U) | (mt[0] & 2147483647U);
      if ((int)(y & 1U) == 0) {
        c_y = y >> 1U;
      } else {
        c_y = y >> 1U ^ 2567483615U;
      }

      mt[623] = mt[396] ^ c_y;
      mti = 1U;
    }

    y = mt[(int)mti - 1];
    mt[624] = mti;
    y ^= y >> 11U;
    y ^= y << 7U & 2636928640U;
    y ^= y << 15U & 4022730752U;
    y ^= y >> 18U;
    u[j] = y;
  }
}

static double genrandu(unsigned int mt[625])
{
  double r;
  unsigned int u[2];

  /* ========================= COPYRIGHT NOTICE ============================ */
  /*  This is a uniform (0,1) pseudorandom number generator based on:        */
  /*                                                                         */
  /*  A C-program for MT19937, with initialization improved 2002/1/26.       */
  /*  Coded by Takuji Nishimura and Makoto Matsumoto.                        */
  /*                                                                         */
  /*  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,      */
  /*  All rights reserved.                                                   */
  /*                                                                         */
  /*  Redistribution and use in source and binary forms, with or without     */
  /*  modification, are permitted provided that the following conditions     */
  /*  are met:                                                               */
  /*                                                                         */
  /*    1. Redistributions of source code must retain the above copyright    */
  /*       notice, this list of conditions and the following disclaimer.     */
  /*                                                                         */
  /*    2. Redistributions in binary form must reproduce the above copyright */
  /*       notice, this list of conditions and the following disclaimer      */
  /*       in the documentation and/or other materials provided with the     */
  /*       distribution.                                                     */
  /*                                                                         */
  /*    3. The names of its contributors may not be used to endorse or       */
  /*       promote products derived from this software without specific      */
  /*       prior written permission.                                         */
  /*                                                                         */
  /*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS    */
  /*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT      */
  /*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR  */
  /*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT  */
  /*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  */
  /*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT       */
  /*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  */
  /*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  */
  /*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT    */
  /*  (INCLUDING  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE */
  /*  OF THIS  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */
  /*                                                                         */
  /* =============================   END   ================================= */
  do {
    genrand_uint32_vector(mt, u);
    u[0] >>= 5U;
    u[1] >>= 6U;
    r = 1.1102230246251565E-16 * ((double)u[0] * 6.7108864E+7 + (double)u[1]);
  } while (r == 0.0);

  return r;
}

static void power(const emxArray_real_T *a, double b, emxArray_real_T *y)
{
  int unnamed_idx_1;
  int k;
  unnamed_idx_1 = a->size[1];
  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = a->size[1];
  emxEnsureCapacity_real_T(y, k);
  for (k = 0; k + 1 <= unnamed_idx_1; k++) {
    y->data[k] = rt_powd_snf(a->data[k], b);
  }
}

static void r2br_r2dit_trig(const creal_T x[11999], const double costab[8193],
  const double sintab[8193], creal_T y[16384])
{
  int i;
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  double temp_re;
  double temp_im;
  int iheight;
  int istart;
  int j;
  double twid_re;
  double twid_im;
  int ihi;
  for (i = 0; i < 16384; i++) {
    y[i].re = 0.0;
    y[i].im = 0.0;
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 11998; i++) {
    y[iy] = x[ix];
    iy = 16384;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 16383; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 4096;
  iheight = 16381;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (j = ju; j < 8192; j += ju) {
      twid_re = costab[j];
      twid_im = sintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

static void r2br_r2dit_trig_impl(const creal_T x[6000], const double costab[8193],
  const double sintab[8193], creal_T y[16384])
{
  int i;
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  double temp_re;
  double temp_im;
  int iheight;
  int istart;
  int j;
  double twid_re;
  double twid_im;
  int ihi;
  for (i = 0; i < 16384; i++) {
    y[i].re = 0.0;
    y[i].im = 0.0;
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 5999; i++) {
    y[iy] = x[ix];
    iy = 16384;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 16383; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 4096;
  iheight = 16381;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (j = ju; j < 8192; j += ju) {
      twid_re = costab[j];
      twid_im = sintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

static void randn(double varargin_2)
{
  unsigned int seed;
  int r;
  int t;
  method = 4U;
  if (varargin_2 < 4.294967296E+9) {
    if (varargin_2 >= 0.0) {
      seed = (unsigned int)varargin_2;
    } else {
      seed = 0U;
    }
  } else if (varargin_2 >= 4.294967296E+9) {
    seed = MAX_uint32_T;
  } else {
    seed = 0U;
  }

  r = (int)(seed >> 16U);
  t = (int)(seed & 32768U);
  seed = ((((seed - ((unsigned int)r << 16U)) - t) << 16U) + t) + r;
  if (seed < 1U) {
    seed = 1144108930U;
  } else {
    if (seed > 2147483646U) {
      seed = 2147483646U;
    }
  }

  state[0] = seed;
  state[1] = 0U;
}

static void rdivide(const emxArray_real_T *x, emxArray_real_T *z)
{
  int i2;
  int loop_ub;
  i2 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity_real_T(z, i2);
  loop_ub = x->size[0] * x->size[1];
  for (i2 = 0; i2 < loop_ub; i2++) {
    z->data[i2] = x->data[i2] / 10.0;
  }
}

static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d1;
  double d2;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d1 = std::abs(u0);
    d2 = std::abs(u1);
    if (rtIsInf(u1)) {
      if (d1 == 1.0) {
        y = 1.0;
      } else if (d1 > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d2 == 0.0) {
      y = 1.0;
    } else if (d2 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

void windsim_dk1_main(char EC, double V10, double drag, double Width,
                      emxArray_real_T *HFloors, double seed, emxArray_real_T
                      *windspeed_out, emxArray_real_T *windforce_out, double *dt)
{
  int i0;
  int jjp1;
  int jj;
  int loop_ub;
  emxArray_real_T *z;
  int nmj;
  double bbb;
  double z0;
  double alpha;
  emxArray_real_T *y;
  int zsize;
  emxArray_creal_T *White;
  emxArray_creal_T *X;
  emxArray_creal_T *XW;
  emxArray_real_T *VV;
  double c;
  emxArray_real_T *distb;
  double d0;
  int i1;
  emxArray_real_T *Rel;
  unsigned int jjj;
  emxArray_creal_T *Im;
  creal_T dc0;
  emxArray_real_T *G;
  emxArray_real_T *r0;
  emxArray_real_T *r1;
  emxArray_real_T *r2;
  emxArray_real_T *r3;
  emxArray_real_T *b_G;
  emxArray_creal_T *c_G;
  emxArray_creal_T *b_White;
  emxArray_creal_T *d_G;
  int b_jj;
  static const double dv0[3000] = { 0.0016666666666666666, 0.0033333333333333331,
    0.005, 0.0066666666666666662, 0.0083333333333333332, 0.01,
    0.011666666666666667, 0.013333333333333332, 0.015, 0.016666666666666666,
    0.018333333333333333, 0.02, 0.021666666666666667, 0.023333333333333334,
    0.025, 0.026666666666666665, 0.028333333333333335, 0.03,
    0.031666666666666662, 0.033333333333333333, 0.035, 0.036666666666666667,
    0.03833333333333333, 0.04, 0.041666666666666664, 0.043333333333333335, 0.045,
    0.046666666666666669, 0.048333333333333339, 0.05, 0.051666666666666666,
    0.05333333333333333, 0.054999999999999993, 0.056666666666666671,
    0.058333333333333334, 0.06, 0.061666666666666668, 0.063333333333333325,
    0.065, 0.066666666666666666, 0.068333333333333329, 0.07, 0.07166666666666667,
    0.073333333333333334, 0.075, 0.076666666666666661, 0.078333333333333324,
    0.08, 0.081666666666666665, 0.083333333333333329, 0.085, 0.08666666666666667,
    0.088333333333333333, 0.09, 0.091666666666666674, 0.093333333333333338,
    0.095, 0.096666666666666679, 0.098333333333333328, 0.1, 0.10166666666666666,
    0.10333333333333333, 0.10500000000000001, 0.10666666666666666,
    0.10833333333333334, 0.10999999999999999, 0.11166666666666666,
    0.11333333333333334, 0.11499999999999999, 0.11666666666666667,
    0.11833333333333333, 0.12, 0.12166666666666666, 0.12333333333333334, 0.125,
    0.12666666666666665, 0.12833333333333333, 0.13, 0.13166666666666665,
    0.13333333333333333, 0.135, 0.13666666666666666, 0.13833333333333334, 0.14,
    0.14166666666666666, 0.14333333333333334, 0.14500000000000002,
    0.14666666666666667, 0.14833333333333334, 0.15, 0.15166666666666667,
    0.15333333333333332, 0.155, 0.15666666666666665, 0.15833333333333335, 0.16,
    0.16166666666666665, 0.16333333333333333, 0.165, 0.16666666666666666,
    0.16833333333333333, 0.17, 0.17166666666666666, 0.17333333333333334,
    0.17500000000000002, 0.17666666666666667, 0.17833333333333334, 0.18,
    0.18166666666666667, 0.18333333333333335, 0.185, 0.18666666666666668,
    0.18833333333333335, 0.19, 0.19166666666666665, 0.19333333333333336, 0.195,
    0.19666666666666666, 0.19833333333333336, 0.2, 0.20166666666666666,
    0.20333333333333331, 0.20500000000000002, 0.20666666666666667,
    0.20833333333333331, 0.21000000000000002, 0.21166666666666667,
    0.21333333333333332, 0.21499999999999997, 0.21666666666666667,
    0.21833333333333332, 0.21999999999999997, 0.22166666666666668,
    0.22333333333333333, 0.22499999999999998, 0.22666666666666668,
    0.22833333333333333, 0.22999999999999998, 0.23166666666666666,
    0.23333333333333334, 0.235, 0.23666666666666666, 0.23833333333333334, 0.24,
    0.24166666666666667, 0.24333333333333332, 0.245, 0.24666666666666667,
    0.24833333333333332, 0.25, 0.25166666666666665, 0.2533333333333333, 0.255,
    0.25666666666666665, 0.2583333333333333, 0.26, 0.26166666666666666,
    0.26333333333333331, 0.265, 0.26666666666666666, 0.26833333333333331, 0.27,
    0.27166666666666667, 0.27333333333333332, 0.275, 0.27666666666666667,
    0.27833333333333332, 0.28, 0.28166666666666668, 0.28333333333333333,
    0.28500000000000003, 0.28666666666666668, 0.28833333333333333,
    0.29000000000000004, 0.29166666666666669, 0.29333333333333333, 0.295,
    0.29666666666666669, 0.29833333333333334, 0.3, 0.30166666666666669,
    0.30333333333333334, 0.305, 0.30666666666666664, 0.30833333333333335, 0.31,
    0.31166666666666665, 0.3133333333333333, 0.315, 0.31666666666666671,
    0.3183333333333333, 0.32, 0.32166666666666671, 0.32333333333333331, 0.325,
    0.32666666666666666, 0.32833333333333331, 0.33, 0.33166666666666667,
    0.33333333333333331, 0.335, 0.33666666666666667, 0.33833333333333332, 0.34,
    0.34166666666666667, 0.34333333333333332, 0.34500000000000003,
    0.34666666666666668, 0.34833333333333333, 0.35000000000000003,
    0.35166666666666668, 0.35333333333333333, 0.355, 0.35666666666666669,
    0.35833333333333334, 0.36, 0.36166666666666669, 0.36333333333333334, 0.365,
    0.3666666666666667, 0.36833333333333335, 0.37, 0.3716666666666667,
    0.37333333333333335, 0.375, 0.37666666666666671, 0.3783333333333333, 0.38,
    0.38166666666666671, 0.3833333333333333, 0.385, 0.38666666666666671,
    0.38833333333333331, 0.39, 0.39166666666666672, 0.39333333333333331, 0.395,
    0.39666666666666672, 0.39833333333333332, 0.4, 0.40166666666666673,
    0.40333333333333332, 0.405, 0.40666666666666662, 0.40833333333333333,
    0.41000000000000003, 0.41166666666666663, 0.41333333333333333,
    0.41500000000000004, 0.41666666666666663, 0.41833333333333333,
    0.42000000000000004, 0.42166666666666663, 0.42333333333333334,
    0.42500000000000004, 0.42666666666666664, 0.42833333333333334,
    0.42999999999999994, 0.43166666666666664, 0.43333333333333335,
    0.43499999999999994, 0.43666666666666665, 0.43833333333333335,
    0.43999999999999995, 0.44166666666666665, 0.44333333333333336,
    0.44499999999999995, 0.44666666666666666, 0.44833333333333336,
    0.44999999999999996, 0.45166666666666666, 0.45333333333333337,
    0.45499999999999996, 0.45666666666666667, 0.45833333333333331,
    0.45999999999999996, 0.46166666666666667, 0.46333333333333332,
    0.46499999999999997, 0.46666666666666667, 0.46833333333333332, 0.47,
    0.47166666666666668, 0.47333333333333333, 0.475, 0.47666666666666668,
    0.47833333333333333, 0.48, 0.48166666666666669, 0.48333333333333334, 0.485,
    0.48666666666666664, 0.48833333333333334, 0.49, 0.49166666666666664,
    0.49333333333333335, 0.495, 0.49666666666666665, 0.49833333333333335, 0.5,
    0.50166666666666671, 0.5033333333333333, 0.505, 0.5066666666666666,
    0.5083333333333333, 0.51, 0.51166666666666671, 0.51333333333333331, 0.515,
    0.51666666666666661, 0.51833333333333331, 0.52, 0.52166666666666672,
    0.52333333333333332, 0.525, 0.52666666666666662, 0.52833333333333332, 0.53,
    0.53166666666666673, 0.53333333333333333, 0.535, 0.53666666666666663,
    0.53833333333333333, 0.54, 0.54166666666666674, 0.54333333333333333, 0.545,
    0.54666666666666663, 0.54833333333333334, 0.55, 0.55166666666666675,
    0.55333333333333334, 0.555, 0.55666666666666664, 0.55833333333333335, 0.56,
    0.56166666666666665, 0.56333333333333335, 0.56500000000000006,
    0.56666666666666665, 0.56833333333333336, 0.57000000000000006,
    0.57166666666666666, 0.57333333333333336, 0.57500000000000007,
    0.57666666666666666, 0.57833333333333337, 0.58000000000000007,
    0.58166666666666667, 0.58333333333333337, 0.58500000000000008,
    0.58666666666666667, 0.58833333333333337, 0.59, 0.59166666666666667,
    0.59333333333333338, 0.595, 0.59666666666666668, 0.59833333333333338, 0.6,
    0.60166666666666668, 0.60333333333333339, 0.605, 0.60666666666666669,
    0.60833333333333339, 0.61, 0.61166666666666669, 0.61333333333333329, 0.615,
    0.6166666666666667, 0.61833333333333329, 0.62, 0.6216666666666667,
    0.62333333333333329, 0.625, 0.62666666666666659, 0.62833333333333341, 0.63,
    0.6316666666666666, 0.63333333333333341, 0.635, 0.6366666666666666,
    0.63833333333333342, 0.64, 0.64166666666666661, 0.64333333333333342, 0.645,
    0.64666666666666661, 0.64833333333333343, 0.65, 0.65166666666666662,
    0.65333333333333332, 0.655, 0.65666666666666662, 0.65833333333333321, 0.66,
    0.66166666666666663, 0.66333333333333333, 0.665, 0.66666666666666663,
    0.66833333333333322, 0.67, 0.67166666666666663, 0.67333333333333334, 0.675,
    0.67666666666666664, 0.67833333333333323, 0.68, 0.68166666666666664,
    0.68333333333333335, 0.685, 0.68666666666666665, 0.68833333333333324,
    0.69000000000000006, 0.69166666666666665, 0.69333333333333336,
    0.69500000000000006, 0.69666666666666666, 0.69833333333333325,
    0.70000000000000007, 0.70166666666666666, 0.70333333333333337, 0.705,
    0.70666666666666667, 0.70833333333333326, 0.71, 0.71166666666666667,
    0.71333333333333337, 0.715, 0.71666666666666667, 0.71833333333333327, 0.72,
    0.72166666666666668, 0.72333333333333338, 0.725, 0.72666666666666668,
    0.72833333333333328, 0.73, 0.73166666666666669, 0.73333333333333339, 0.735,
    0.73666666666666669, 0.73833333333333329, 0.74, 0.7416666666666667,
    0.7433333333333334, 0.745, 0.7466666666666667, 0.74833333333333329, 0.75,
    0.75166666666666671, 0.75333333333333341, 0.755, 0.7566666666666666,
    0.7583333333333333, 0.76, 0.7616666666666666, 0.76333333333333342, 0.765,
    0.76666666666666661, 0.76833333333333331, 0.77, 0.77166666666666661,
    0.77333333333333343, 0.775, 0.77666666666666662, 0.77833333333333332, 0.78,
    0.78166666666666662, 0.78333333333333344, 0.785, 0.78666666666666663,
    0.78833333333333333, 0.79, 0.79166666666666663, 0.79333333333333345, 0.795,
    0.79666666666666663, 0.79833333333333334, 0.8, 0.80166666666666664,
    0.80333333333333345, 0.805, 0.80666666666666664, 0.80833333333333324, 0.81,
    0.81166666666666665, 0.81333333333333324, 0.81500000000000006,
    0.81666666666666665, 0.81833333333333325, 0.82000000000000006,
    0.82166666666666666, 0.82333333333333325, 0.82500000000000007,
    0.82666666666666666, 0.82833333333333325, 0.83000000000000007,
    0.83166666666666667, 0.83333333333333326, 0.83500000000000008,
    0.83666666666666667, 0.83833333333333326, 0.84000000000000008,
    0.84166666666666667, 0.84333333333333327, 0.84500000000000008,
    0.84666666666666668, 0.84833333333333327, 0.85000000000000009,
    0.85166666666666668, 0.85333333333333328, 0.85500000000000009,
    0.85666666666666669, 0.85833333333333328, 0.85999999999999988,
    0.86166666666666669, 0.86333333333333329, 0.865, 0.8666666666666667,
    0.86833333333333329, 0.86999999999999988, 0.8716666666666667,
    0.87333333333333329, 0.875, 0.87666666666666671, 0.8783333333333333,
    0.87999999999999989, 0.88166666666666671, 0.8833333333333333, 0.885,
    0.88666666666666671, 0.88833333333333331, 0.8899999999999999,
    0.89166666666666672, 0.89333333333333331, 0.895, 0.89666666666666672,
    0.89833333333333332, 0.89999999999999991, 0.90166666666666673,
    0.90333333333333332, 0.905, 0.90666666666666673, 0.90833333333333333,
    0.90999999999999992, 0.91166666666666663, 0.91333333333333333, 0.915,
    0.91666666666666663, 0.91833333333333333, 0.91999999999999993,
    0.92166666666666663, 0.92333333333333334, 0.925, 0.92666666666666664,
    0.92833333333333334, 0.92999999999999994, 0.93166666666666664,
    0.93333333333333335, 0.935, 0.93666666666666665, 0.93833333333333335, 0.94,
    0.94166666666666665, 0.94333333333333336, 0.94500000000000006,
    0.94666666666666666, 0.94833333333333336, 0.95, 0.95166666666666666,
    0.95333333333333337, 0.95500000000000007, 0.95666666666666667,
    0.95833333333333337, 0.96, 0.96166666666666667, 0.96333333333333337,
    0.96500000000000008, 0.96666666666666667, 0.96833333333333327, 0.97,
    0.97166666666666668, 0.97333333333333327, 0.97500000000000009,
    0.97666666666666668, 0.97833333333333328, 0.98, 0.98166666666666669,
    0.98333333333333328, 0.9850000000000001, 0.98666666666666669,
    0.98833333333333329, 0.99, 0.9916666666666667, 0.99333333333333329,
    0.99500000000000011, 0.9966666666666667, 0.99833333333333329, 1.0,
    1.0016666666666667, 1.0033333333333334, 1.0050000000000001,
    1.0066666666666666, 1.0083333333333333, 1.01, 1.0116666666666667,
    1.0133333333333332, 1.0150000000000001, 1.0166666666666666,
    1.0183333333333333, 1.02, 1.0216666666666667, 1.0233333333333334, 1.025,
    1.0266666666666666, 1.0283333333333333, 1.03, 1.0316666666666667,
    1.0333333333333332, 1.035, 1.0366666666666666, 1.0383333333333333, 1.04,
    1.0416666666666667, 1.0433333333333334, 1.045, 1.0466666666666666,
    1.0483333333333333, 1.05, 1.0516666666666667, 1.0533333333333332, 1.055,
    1.0566666666666666, 1.0583333333333333, 1.06, 1.0616666666666668,
    1.0633333333333335, 1.065, 1.0666666666666667, 1.0683333333333334, 1.07,
    1.0716666666666665, 1.0733333333333333, 1.075, 1.0766666666666667,
    1.0783333333333334, 1.08, 1.0816666666666666, 1.0833333333333335, 1.085,
    1.0866666666666667, 1.0883333333333334, 1.09, 1.0916666666666666,
    1.0933333333333333, 1.095, 1.0966666666666667, 1.0983333333333334, 1.1,
    1.1016666666666666, 1.1033333333333335, 1.105, 1.1066666666666667,
    1.1083333333333334, 1.11, 1.1116666666666666, 1.1133333333333333, 1.115,
    1.1166666666666667, 1.1183333333333334, 1.12, 1.1216666666666666,
    1.1233333333333333, 1.125, 1.1266666666666667, 1.1283333333333332,
    1.1300000000000001, 1.1316666666666666, 1.1333333333333333, 1.135,
    1.1366666666666667, 1.1383333333333332, 1.1400000000000001,
    1.1416666666666666, 1.1433333333333333, 1.145, 1.1466666666666667,
    1.1483333333333332, 1.1500000000000001, 1.1516666666666666,
    1.1533333333333333, 1.155, 1.1566666666666667, 1.1583333333333332,
    1.1600000000000001, 1.1616666666666666, 1.1633333333333333, 1.165,
    1.1666666666666667, 1.1683333333333332, 1.1700000000000002,
    1.1716666666666666, 1.1733333333333333, 1.1749999999999998,
    1.1766666666666667, 1.1783333333333332, 1.18, 1.1816666666666666,
    1.1833333333333333, 1.185, 1.1866666666666668, 1.1883333333333332, 1.19,
    1.1916666666666667, 1.1933333333333334, 1.1949999999999998,
    1.1966666666666668, 1.1983333333333333, 1.2, 1.2016666666666667,
    1.2033333333333334, 1.205, 1.2066666666666668, 1.2083333333333333, 1.21,
    1.2116666666666667, 1.2133333333333334, 1.2149999999999999,
    1.2166666666666668, 1.2183333333333333, 1.22, 1.2216666666666667,
    1.2233333333333334, 1.225, 1.2266666666666666, 1.2283333333333333, 1.23,
    1.2316666666666667, 1.2333333333333334, 1.2349999999999999,
    1.2366666666666666, 1.2383333333333333, 1.24, 1.2416666666666667,
    1.2433333333333334, 1.245, 1.2466666666666666, 1.2483333333333333, 1.25,
    1.2516666666666667, 1.2533333333333332, 1.255, 1.2566666666666668,
    1.2583333333333333, 1.26, 1.2616666666666667, 1.2633333333333332,
    1.2650000000000001, 1.2666666666666668, 1.2683333333333333, 1.27,
    1.2716666666666667, 1.2733333333333332, 1.275, 1.2766666666666668,
    1.2783333333333333, 1.28, 1.2816666666666667, 1.2833333333333332,
    1.2850000000000001, 1.2866666666666668, 1.2883333333333333, 1.29,
    1.2916666666666667, 1.2933333333333332, 1.295, 1.2966666666666669,
    1.2983333333333333, 1.3, 1.3016666666666667, 1.3033333333333332,
    1.3050000000000002, 1.3066666666666666, 1.3083333333333333, 1.31,
    1.3116666666666665, 1.3133333333333332, 1.315, 1.3166666666666664,
    1.3183333333333334, 1.32, 1.3216666666666665, 1.3233333333333333,
    1.3250000000000002, 1.3266666666666667, 1.3283333333333334, 1.33,
    1.3316666666666666, 1.3333333333333333, 1.335, 1.3366666666666664,
    1.3383333333333334, 1.34, 1.3416666666666666, 1.3433333333333333,
    1.3450000000000002, 1.3466666666666667, 1.3483333333333334, 1.35,
    1.3516666666666666, 1.3533333333333333, 1.355, 1.3566666666666665,
    1.3583333333333334, 1.36, 1.3616666666666666, 1.3633333333333333,
    1.3650000000000002, 1.3666666666666667, 1.3683333333333334, 1.37,
    1.3716666666666666, 1.3733333333333333, 1.375, 1.3766666666666665,
    1.3783333333333334, 1.3800000000000001, 1.3816666666666666,
    1.3833333333333333, 1.3850000000000002, 1.3866666666666667,
    1.3883333333333334, 1.3900000000000001, 1.3916666666666666,
    1.3933333333333333, 1.395, 1.3966666666666665, 1.3983333333333334,
    1.4000000000000001, 1.4016666666666666, 1.4033333333333333,
    1.4050000000000002, 1.4066666666666667, 1.4083333333333334, 1.41,
    1.4116666666666666, 1.4133333333333333, 1.4149999999999998,
    1.4166666666666665, 1.4183333333333334, 1.42, 1.4216666666666666,
    1.4233333333333333, 1.4249999999999998, 1.4266666666666667,
    1.4283333333333335, 1.43, 1.4316666666666666, 1.4333333333333333,
    1.4349999999999998, 1.4366666666666665, 1.4383333333333335, 1.44,
    1.4416666666666667, 1.4433333333333334, 1.4449999999999998,
    1.4466666666666668, 1.4483333333333335, 1.45, 1.4516666666666667,
    1.4533333333333334, 1.4549999999999998, 1.4566666666666666,
    1.4583333333333335, 1.46, 1.4616666666666667, 1.4633333333333334,
    1.4649999999999999, 1.4666666666666668, 1.4683333333333335, 1.47,
    1.4716666666666667, 1.4733333333333334, 1.4749999999999999,
    1.4766666666666666, 1.4783333333333335, 1.48, 1.4816666666666667,
    1.4833333333333334, 1.4849999999999999, 1.4866666666666668,
    1.4883333333333335, 1.49, 1.4916666666666667, 1.4933333333333334,
    1.4949999999999999, 1.4966666666666666, 1.4983333333333335, 1.5,
    1.5016666666666667, 1.5033333333333334, 1.505, 1.5066666666666668,
    1.5083333333333335, 1.51, 1.5116666666666667, 1.5133333333333332, 1.515,
    1.5166666666666666, 1.5183333333333331, 1.52, 1.5216666666666667,
    1.5233333333333332, 1.525, 1.5266666666666668, 1.5283333333333333, 1.53,
    1.5316666666666667, 1.5333333333333332, 1.535, 1.5366666666666666,
    1.5383333333333331, 1.54, 1.5416666666666667, 1.5433333333333332, 1.545,
    1.5466666666666669, 1.5483333333333333, 1.55, 1.5516666666666667,
    1.5533333333333332, 1.555, 1.5566666666666666, 1.5583333333333331, 1.56,
    1.5616666666666668, 1.5633333333333332, 1.565, 1.5666666666666669,
    1.5683333333333334, 1.57, 1.5716666666666668, 1.5733333333333333, 1.575,
    1.5766666666666667, 1.5783333333333331, 1.58, 1.5816666666666668,
    1.5833333333333333, 1.585, 1.5866666666666669, 1.5883333333333334, 1.59,
    1.5916666666666668, 1.5933333333333333, 1.595, 1.5966666666666667,
    1.5983333333333332, 1.6, 1.6016666666666668, 1.6033333333333333, 1.605,
    1.6066666666666669, 1.6083333333333334, 1.61, 1.6116666666666668,
    1.6133333333333333, 1.615, 1.6166666666666665, 1.6183333333333332, 1.62,
    1.6216666666666666, 1.6233333333333333, 1.625, 1.6266666666666665,
    1.6283333333333334, 1.6300000000000001, 1.6316666666666666,
    1.6333333333333333, 1.635, 1.6366666666666665, 1.6383333333333332,
    1.6400000000000001, 1.6416666666666666, 1.6433333333333333, 1.645,
    1.6466666666666665, 1.6483333333333334, 1.6500000000000001,
    1.6516666666666666, 1.6533333333333333, 1.655, 1.6566666666666665,
    1.6583333333333332, 1.6600000000000001, 1.6616666666666666,
    1.6633333333333333, 1.665, 1.6666666666666665, 1.6683333333333334,
    1.6700000000000002, 1.6716666666666666, 1.6733333333333333, 1.675,
    1.6766666666666665, 1.6783333333333332, 1.6800000000000002,
    1.6816666666666666, 1.6833333333333333, 1.685, 1.6866666666666665,
    1.6883333333333335, 1.6900000000000002, 1.6916666666666667,
    1.6933333333333334, 1.695, 1.6966666666666665, 1.6983333333333333,
    1.7000000000000002, 1.7016666666666667, 1.7033333333333334, 1.705,
    1.7066666666666666, 1.7083333333333335, 1.7100000000000002,
    1.7116666666666667, 1.7133333333333334, 1.715, 1.7166666666666666,
    1.7183333333333333, 1.7199999999999998, 1.7216666666666667,
    1.7233333333333334, 1.7249999999999999, 1.7266666666666666,
    1.7283333333333335, 1.73, 1.7316666666666667, 1.7333333333333334,
    1.7349999999999999, 1.7366666666666666, 1.7383333333333333,
    1.7399999999999998, 1.7416666666666667, 1.7433333333333334,
    1.7449999999999999, 1.7466666666666666, 1.7483333333333335, 1.75,
    1.7516666666666667, 1.7533333333333334, 1.755, 1.7566666666666666,
    1.7583333333333333, 1.7599999999999998, 1.7616666666666667,
    1.7633333333333334, 1.765, 1.7666666666666666, 1.7683333333333335, 1.77,
    1.7716666666666667, 1.7733333333333334, 1.775, 1.7766666666666666,
    1.7783333333333333, 1.7799999999999998, 1.7816666666666667,
    1.7833333333333334, 1.785, 1.7866666666666666, 1.7883333333333336, 1.79,
    1.7916666666666667, 1.7933333333333334, 1.795, 1.7966666666666666,
    1.7983333333333333, 1.7999999999999998, 1.8016666666666667,
    1.8033333333333335, 1.805, 1.8066666666666666, 1.8083333333333336, 1.81,
    1.8116666666666668, 1.8133333333333335, 1.815, 1.8166666666666667,
    1.8183333333333334, 1.8199999999999998, 1.8216666666666668,
    1.8233333333333333, 1.825, 1.8266666666666667, 1.8283333333333331, 1.83,
    1.8316666666666668, 1.8333333333333333, 1.835, 1.8366666666666667,
    1.8383333333333332, 1.8399999999999999, 1.8416666666666668,
    1.8433333333333333, 1.845, 1.8466666666666667, 1.8483333333333332, 1.85,
    1.8516666666666668, 1.8533333333333333, 1.855, 1.8566666666666667,
    1.8583333333333332, 1.8599999999999999, 1.8616666666666668,
    1.8633333333333333, 1.865, 1.8666666666666667, 1.8683333333333332, 1.87,
    1.8716666666666668, 1.8733333333333333, 1.875, 1.8766666666666667,
    1.8783333333333332, 1.88, 1.8816666666666668, 1.8833333333333333, 1.885,
    1.8866666666666667, 1.8883333333333332, 1.8900000000000001,
    1.8916666666666668, 1.8933333333333333, 1.895, 1.8966666666666667,
    1.8983333333333332, 1.9, 1.9016666666666668, 1.9033333333333333, 1.905,
    1.9066666666666667, 1.9083333333333332, 1.9100000000000001,
    1.9116666666666668, 1.9133333333333333, 1.915, 1.9166666666666667,
    1.9183333333333332, 1.92, 1.9216666666666669, 1.9233333333333333, 1.925,
    1.9266666666666667, 1.9283333333333332, 1.9300000000000002,
    1.9316666666666666, 1.9333333333333333, 1.935, 1.9366666666666665,
    1.9383333333333332, 1.94, 1.9416666666666664, 1.9433333333333334, 1.945,
    1.9466666666666665, 1.9483333333333333, 1.9500000000000002,
    1.9516666666666667, 1.9533333333333334, 1.955, 1.9566666666666666,
    1.9583333333333333, 1.96, 1.9616666666666664, 1.9633333333333334, 1.965,
    1.9666666666666666, 1.9683333333333333, 1.9700000000000002,
    1.9716666666666667, 1.9733333333333334, 1.975, 1.9766666666666666,
    1.9783333333333333, 1.98, 1.9816666666666665, 1.9833333333333334, 1.985,
    1.9866666666666666, 1.9883333333333333, 1.9900000000000002,
    1.9916666666666667, 1.9933333333333334, 1.995, 1.9966666666666666,
    1.9983333333333333, 2.0, 2.0016666666666665, 2.0033333333333334, 2.005,
    2.0066666666666668, 2.0083333333333333, 2.0100000000000002,
    2.0116666666666667, 2.0133333333333332, 2.015, 2.0166666666666666,
    2.0183333333333335, 2.02, 2.0216666666666665, 2.0233333333333334,
    2.0250000000000004, 2.0266666666666664, 2.0283333333333333,
    2.0300000000000002, 2.0316666666666667, 2.0333333333333332,
    2.0349999999999997, 2.0366666666666666, 2.0383333333333336, 2.04,
    2.0416666666666665, 2.0433333333333334, 2.045, 2.0466666666666669,
    2.0483333333333333, 2.05, 2.0516666666666667, 2.0533333333333332,
    2.0549999999999997, 2.0566666666666666, 2.0583333333333336, 2.06,
    2.0616666666666665, 2.0633333333333335, 2.065, 2.0666666666666664,
    2.0683333333333334, 2.07, 2.0716666666666668, 2.0733333333333333,
    2.0749999999999997, 2.0766666666666667, 2.0783333333333336, 2.08,
    2.0816666666666666, 2.0833333333333335, 2.085, 2.0866666666666669,
    2.0883333333333334, 2.09, 2.0916666666666668, 2.0933333333333333,
    2.0949999999999998, 2.0966666666666667, 2.0983333333333336, 2.1,
    2.1016666666666666, 2.1033333333333335, 2.105, 2.1066666666666665,
    2.1083333333333334, 2.11, 2.1116666666666668, 2.1133333333333333,
    2.1149999999999998, 2.1166666666666667, 2.1183333333333336, 2.12,
    2.1216666666666666, 2.1233333333333335, 2.125, 2.1266666666666669,
    2.1283333333333334, 2.13, 2.1316666666666668, 2.1333333333333333, 2.135,
    2.1366666666666667, 2.1383333333333332, 2.14, 2.1416666666666666,
    2.1433333333333331, 2.145, 2.1466666666666665, 2.1483333333333334, 2.15,
    2.1516666666666668, 2.1533333333333333, 2.155, 2.1566666666666667,
    2.1583333333333332, 2.16, 2.1616666666666666, 2.1633333333333331, 2.165,
    2.166666666666667, 2.168333333333333, 2.17, 2.1716666666666669,
    2.1733333333333333, 2.175, 2.1766666666666667, 2.1783333333333332, 2.18,
    2.1816666666666666, 2.1833333333333331, 2.185, 2.1866666666666665,
    2.1883333333333335, 2.19, 2.1916666666666669, 2.1933333333333334, 2.195,
    2.1966666666666668, 2.1983333333333333, 2.2, 2.2016666666666667,
    2.2033333333333331, 2.205, 2.206666666666667, 2.208333333333333, 2.21,
    2.2116666666666669, 2.2133333333333334, 2.215, 2.2166666666666668,
    2.2183333333333333, 2.22, 2.2216666666666667, 2.2233333333333332, 2.225,
    2.2266666666666666, 2.2283333333333335, 2.23, 2.2316666666666669,
    2.2333333333333334, 2.235, 2.2366666666666668, 2.2383333333333333, 2.24,
    2.2416666666666667, 2.2433333333333332, 2.245, 2.2466666666666666,
    2.2483333333333331, 2.25, 2.2516666666666665, 2.2533333333333334, 2.255,
    2.2566666666666664, 2.2583333333333333, 2.2600000000000002,
    2.2616666666666667, 2.2633333333333332, 2.265, 2.2666666666666666,
    2.2683333333333335, 2.27, 2.2716666666666665, 2.2733333333333334, 2.275,
    2.2766666666666664, 2.2783333333333333, 2.2800000000000002,
    2.2816666666666667, 2.2833333333333332, 2.285, 2.2866666666666666,
    2.2883333333333331, 2.29, 2.2916666666666665, 2.2933333333333334, 2.295,
    2.2966666666666664, 2.2983333333333333, 2.3000000000000003,
    2.3016666666666667, 2.3033333333333332, 2.305, 2.3066666666666666,
    2.3083333333333336, 2.31, 2.3116666666666665, 2.3133333333333335, 2.315,
    2.3166666666666664, 2.3183333333333334, 2.3200000000000003,
    2.3216666666666668, 2.3233333333333333, 2.325, 2.3266666666666667,
    2.3283333333333331, 2.33, 2.3316666666666666, 2.3333333333333335, 2.335,
    2.3366666666666664, 2.3383333333333334, 2.3400000000000003,
    2.3416666666666668, 2.3433333333333333, 2.3449999999999998,
    2.3466666666666667, 2.3483333333333336, 2.3499999999999996,
    2.3516666666666666, 2.3533333333333335, 2.355, 2.3566666666666665,
    2.3583333333333334, 2.36, 2.3616666666666668, 2.3633333333333333,
    2.3649999999999998, 2.3666666666666667, 2.3683333333333332, 2.37,
    2.3716666666666666, 2.3733333333333335, 2.375, 2.3766666666666665,
    2.3783333333333334, 2.38, 2.3816666666666668, 2.3833333333333333, 2.385,
    2.3866666666666667, 2.3883333333333336, 2.3899999999999997,
    2.3916666666666666, 2.3933333333333335, 2.395, 2.3966666666666665,
    2.3983333333333334, 2.4, 2.4016666666666668, 2.4033333333333333, 2.405,
    2.4066666666666667, 2.4083333333333332, 2.41, 2.4116666666666666,
    2.4133333333333336, 2.415, 2.4166666666666665, 2.4183333333333334, 2.42,
    2.4216666666666669, 2.4233333333333333, 2.425, 2.4266666666666667,
    2.4283333333333337, 2.4299999999999997, 2.4316666666666666,
    2.4333333333333336, 2.435, 2.4366666666666665, 2.4383333333333335, 2.44,
    2.4416666666666669, 2.4433333333333334, 2.445, 2.4466666666666668,
    2.4483333333333333, 2.45, 2.4516666666666667, 2.4533333333333331, 2.455,
    2.4566666666666666, 2.458333333333333, 2.46, 2.4616666666666669,
    2.4633333333333334, 2.465, 2.4666666666666668, 2.4683333333333333,
    2.4699999999999998, 2.4716666666666667, 2.4733333333333332, 2.475,
    2.4766666666666666, 2.4783333333333331, 2.48, 2.4816666666666669,
    2.4833333333333334, 2.485, 2.4866666666666668, 2.4883333333333333, 2.49,
    2.4916666666666667, 2.4933333333333332, 2.495, 2.4966666666666666,
    2.4983333333333331, 2.5, 2.5016666666666665, 2.5033333333333334, 2.505,
    2.5066666666666664, 2.5083333333333337, 2.51, 2.5116666666666667,
    2.5133333333333336, 2.515, 2.5166666666666666, 2.5183333333333335, 2.52,
    2.5216666666666665, 2.5233333333333334, 2.525, 2.5266666666666664,
    2.5283333333333333, 2.5300000000000002, 2.5316666666666663,
    2.5333333333333337, 2.535, 2.5366666666666666, 2.5383333333333336, 2.54,
    2.5416666666666665, 2.5433333333333334, 2.545, 2.5466666666666664,
    2.5483333333333338, 2.55, 2.5516666666666667, 2.5533333333333337, 2.555,
    2.5566666666666666, 2.5583333333333336, 2.56, 2.5616666666666665,
    2.5633333333333335, 2.565, 2.5666666666666664, 2.5683333333333334,
    2.5700000000000003, 2.5716666666666663, 2.5733333333333337, 2.575,
    2.5766666666666667, 2.5783333333333336, 2.58, 2.5816666666666666,
    2.5833333333333335, 2.585, 2.5866666666666664, 2.5883333333333338, 2.59,
    2.5916666666666668, 2.5933333333333337, 2.595, 2.5966666666666667,
    2.5983333333333336, 2.6, 2.6016666666666666, 2.6033333333333335, 2.605,
    2.6066666666666665, 2.6083333333333329, 2.6100000000000003,
    2.6116666666666664, 2.6133333333333333, 2.615, 2.6166666666666667,
    2.6183333333333332, 2.62, 2.6216666666666666, 2.6233333333333331, 2.625,
    2.6266666666666665, 2.628333333333333, 2.63, 2.6316666666666668,
    2.6333333333333329, 2.6350000000000002, 2.6366666666666667,
    2.6383333333333332, 2.64, 2.6416666666666666, 2.6433333333333331, 2.645,
    2.6466666666666665, 2.648333333333333, 2.6500000000000004,
    2.6516666666666664, 2.6533333333333333, 2.6550000000000002,
    2.6566666666666667, 2.6583333333333332, 2.66, 2.6616666666666666,
    2.6633333333333331, 2.665, 2.6666666666666665, 2.668333333333333, 2.67,
    2.6716666666666669, 2.6733333333333329, 2.6750000000000003,
    2.6766666666666667, 2.6783333333333332, 2.68, 2.6816666666666666,
    2.6833333333333331, 2.685, 2.6866666666666665, 2.688333333333333,
    2.6900000000000004, 2.6916666666666664, 2.6933333333333334,
    2.6950000000000003, 2.6966666666666668, 2.6983333333333333, 2.7,
    2.7016666666666667, 2.7033333333333331, 2.705, 2.7066666666666666,
    2.708333333333333, 2.71, 2.7116666666666669, 2.7133333333333329,
    2.7150000000000003, 2.7166666666666668, 2.7183333333333333, 2.72,
    2.7216666666666667, 2.7233333333333332, 2.725, 2.7266666666666666,
    2.7283333333333331, 2.7300000000000004, 2.7316666666666665,
    2.7333333333333334, 2.7350000000000003, 2.7366666666666668,
    2.7383333333333333, 2.74, 2.7416666666666667, 2.7433333333333332, 2.745,
    2.7466666666666666, 2.7483333333333331, 2.75, 2.7516666666666669,
    2.753333333333333, 2.7550000000000003, 2.7566666666666668,
    2.7583333333333333, 2.7600000000000002, 2.7616666666666667,
    2.7633333333333332, 2.765, 2.7666666666666666, 2.7683333333333331,
    2.7700000000000005, 2.7716666666666665, 2.7733333333333334,
    2.7750000000000004, 2.7766666666666668, 2.7783333333333333,
    2.7800000000000002, 2.7816666666666667, 2.7833333333333332, 2.785,
    2.7866666666666666, 2.7883333333333331, 2.79, 2.791666666666667,
    2.793333333333333, 2.7950000000000004, 2.7966666666666669,
    2.7983333333333333, 2.8000000000000003, 2.8016666666666667,
    2.8033333333333332, 2.805, 2.8066666666666666, 2.8083333333333331,
    2.8100000000000005, 2.8116666666666665, 2.8133333333333335,
    2.8149999999999995, 2.8166666666666669, 2.8183333333333334, 2.82,
    2.8216666666666668, 2.8233333333333333, 2.8249999999999997,
    2.8266666666666667, 2.8283333333333331, 2.8299999999999996,
    2.831666666666667, 2.833333333333333, 2.835, 2.8366666666666669,
    2.8383333333333334, 2.84, 2.8416666666666668, 2.8433333333333333,
    2.8449999999999998, 2.8466666666666667, 2.8483333333333332,
    2.8499999999999996, 2.8516666666666666, 2.8533333333333335,
    2.8549999999999995, 2.8566666666666669, 2.8583333333333334, 2.86,
    2.8616666666666668, 2.8633333333333333, 2.8649999999999998,
    2.8666666666666667, 2.8683333333333332, 2.8699999999999997,
    2.871666666666667, 2.8733333333333331, 2.875, 2.8766666666666669,
    2.8783333333333334, 2.88, 2.8816666666666668, 2.8833333333333333, 2.885,
    2.8866666666666667, 2.8883333333333332, 2.8899999999999997,
    2.8916666666666666, 2.8933333333333335, 2.8949999999999996,
    2.8966666666666669, 2.8983333333333334, 2.9, 2.9016666666666668,
    2.9033333333333333, 2.905, 2.9066666666666667, 2.9083333333333332,
    2.9099999999999997, 2.9116666666666671, 2.9133333333333331, 2.915,
    2.916666666666667, 2.9183333333333334, 2.92, 2.9216666666666669,
    2.9233333333333333, 2.925, 2.9266666666666667, 2.9283333333333332,
    2.9299999999999997, 2.9316666666666666, 2.9333333333333336,
    2.9349999999999996, 2.936666666666667, 2.9383333333333335, 2.94,
    2.9416666666666669, 2.9433333333333334, 2.945, 2.9466666666666668,
    2.9483333333333333, 2.9499999999999997, 2.9516666666666671,
    2.9533333333333331, 2.955, 2.956666666666667, 2.9583333333333335, 2.96,
    2.9616666666666669, 2.9633333333333334, 2.965, 2.9666666666666668,
    2.9683333333333333, 2.9699999999999998, 2.9716666666666667,
    2.9733333333333336, 2.9749999999999996, 2.976666666666667,
    2.9783333333333335, 2.98, 2.9816666666666669, 2.9833333333333334, 2.985,
    2.9866666666666668, 2.9883333333333333, 2.9899999999999998,
    2.9916666666666671, 2.9933333333333332, 2.995, 2.996666666666667,
    2.9983333333333335, 3.0, 3.0016666666666669, 3.0033333333333334, 3.005,
    3.0066666666666668, 3.0083333333333333, 3.01, 3.0116666666666667,
    3.0133333333333336, 3.0149999999999997, 3.0166666666666671,
    3.0183333333333335, 3.02, 3.0216666666666665, 3.0233333333333334, 3.025,
    3.0266666666666664, 3.0283333333333333, 3.03, 3.0316666666666663,
    3.0333333333333332, 3.035, 3.0366666666666662, 3.0383333333333336, 3.04,
    3.0416666666666665, 3.0433333333333334, 3.045, 3.0466666666666664,
    3.0483333333333333, 3.05, 3.0516666666666663, 3.0533333333333337,
    3.0549999999999997, 3.0566666666666666, 3.0583333333333336, 3.06,
    3.0616666666666665, 3.0633333333333335, 3.065, 3.0666666666666664,
    3.0683333333333334, 3.07, 3.0716666666666663, 3.0733333333333333, 3.075,
    3.0766666666666662, 3.0783333333333336, 3.08, 3.0816666666666666,
    3.0833333333333335, 3.085, 3.0866666666666664, 3.0883333333333334, 3.09,
    3.0916666666666663, 3.0933333333333337, 3.0949999999999998,
    3.0966666666666667, 3.0983333333333336, 3.1, 3.1016666666666666,
    3.1033333333333335, 3.105, 3.1066666666666665, 3.1083333333333334, 3.11,
    3.1116666666666664, 3.1133333333333333, 3.115, 3.1166666666666663,
    3.1183333333333336, 3.12, 3.1216666666666666, 3.1233333333333335, 3.125,
    3.1266666666666665, 3.1283333333333334, 3.13, 3.1316666666666664,
    3.1333333333333337, 3.135, 3.1366666666666667, 3.1383333333333336, 3.14,
    3.1416666666666666, 3.1433333333333335, 3.145, 3.1466666666666665,
    3.1483333333333334, 3.15, 3.1516666666666664, 3.1533333333333333,
    3.1550000000000002, 3.1566666666666663, 3.1583333333333337, 3.16,
    3.1616666666666666, 3.1633333333333336, 3.165, 3.1666666666666665,
    3.1683333333333334, 3.17, 3.1716666666666664, 3.1733333333333338, 3.175,
    3.1766666666666667, 3.1783333333333337, 3.18, 3.1816666666666666,
    3.1833333333333336, 3.185, 3.1866666666666665, 3.1883333333333335, 3.19,
    3.1916666666666664, 3.1933333333333334, 3.1950000000000003,
    3.1966666666666663, 3.1983333333333337, 3.2, 3.2016666666666667,
    3.2033333333333336, 3.205, 3.2066666666666666, 3.2083333333333335, 3.21,
    3.2116666666666664, 3.2133333333333338, 3.215, 3.2166666666666668,
    3.2183333333333337, 3.22, 3.2216666666666667, 3.2233333333333336, 3.225,
    3.2266666666666666, 3.2283333333333335, 3.23, 3.2316666666666665,
    3.2333333333333329, 3.2350000000000003, 3.2366666666666664,
    3.2383333333333333, 3.24, 3.2416666666666667, 3.2433333333333332, 3.245,
    3.2466666666666666, 3.2483333333333331, 3.25, 3.2516666666666665,
    3.253333333333333, 3.255, 3.2566666666666668, 3.2583333333333329,
    3.2600000000000002, 3.2616666666666667, 3.2633333333333332, 3.265,
    3.2666666666666666, 3.2683333333333331, 3.27, 3.2716666666666665,
    3.273333333333333, 3.2750000000000004, 3.2766666666666664,
    3.2783333333333333, 3.2800000000000002, 3.2816666666666667,
    3.2833333333333332, 3.285, 3.2866666666666666, 3.2883333333333331, 3.29,
    3.2916666666666665, 3.293333333333333, 3.295, 3.2966666666666669,
    3.2983333333333329, 3.3000000000000003, 3.3016666666666667,
    3.3033333333333332, 3.305, 3.3066666666666666, 3.3083333333333331, 3.31,
    3.3116666666666665, 3.313333333333333, 3.3150000000000004,
    3.3166666666666664, 3.3183333333333334, 3.3200000000000003,
    3.3216666666666668, 3.3233333333333333, 3.325, 3.3266666666666667,
    3.3283333333333331, 3.33, 3.3316666666666666, 3.333333333333333, 3.335,
    3.3366666666666669, 3.3383333333333329, 3.3400000000000003,
    3.3416666666666668, 3.3433333333333333, 3.345, 3.3466666666666667,
    3.3483333333333332, 3.35, 3.3516666666666666, 3.3533333333333331,
    3.3550000000000004, 3.3566666666666665, 3.3583333333333334,
    3.3600000000000003, 3.3616666666666668, 3.3633333333333333, 3.365,
    3.3666666666666667, 3.3683333333333332, 3.37, 3.3716666666666666,
    3.3733333333333331, 3.375, 3.3766666666666669, 3.378333333333333,
    3.3800000000000003, 3.3816666666666668, 3.3833333333333333,
    3.3850000000000002, 3.3866666666666667, 3.3883333333333332, 3.39,
    3.3916666666666666, 3.3933333333333331, 3.3950000000000005,
    3.3966666666666665, 3.3983333333333334, 3.4000000000000004,
    3.4016666666666668, 3.4033333333333333, 3.4050000000000002,
    3.4066666666666667, 3.4083333333333332, 3.41, 3.4116666666666666,
    3.4133333333333331, 3.415, 3.416666666666667, 3.418333333333333,
    3.4200000000000004, 3.4216666666666669, 3.4233333333333333,
    3.4250000000000003, 3.4266666666666667, 3.4283333333333332, 3.43,
    3.4316666666666666, 3.4333333333333331, 3.4350000000000005,
    3.4366666666666665, 3.4383333333333335, 3.4399999999999995,
    3.4416666666666669, 3.4433333333333334, 3.445, 3.4466666666666668,
    3.4483333333333333, 3.4499999999999997, 3.4516666666666667,
    3.4533333333333331, 3.4549999999999996, 3.456666666666667, 3.458333333333333,
    3.46, 3.4616666666666669, 3.4633333333333334, 3.465, 3.4666666666666668,
    3.4683333333333333, 3.4699999999999998, 3.4716666666666667,
    3.4733333333333332, 3.4749999999999996, 3.4766666666666666,
    3.4783333333333335, 3.4799999999999995, 3.4816666666666669,
    3.4833333333333334, 3.485, 3.4866666666666668, 3.4883333333333333,
    3.4899999999999998, 3.4916666666666667, 3.4933333333333332,
    3.4949999999999997, 3.496666666666667, 3.4983333333333331, 3.5,
    3.5016666666666669, 3.5033333333333334, 3.505, 3.5066666666666668,
    3.5083333333333333, 3.51, 3.5116666666666667, 3.5133333333333332,
    3.5149999999999997, 3.5166666666666666, 3.5183333333333335,
    3.5199999999999996, 3.5216666666666669, 3.5233333333333334, 3.525,
    3.5266666666666668, 3.5283333333333333, 3.53, 3.5316666666666667,
    3.5333333333333332, 3.5349999999999997, 3.5366666666666671,
    3.5383333333333331, 3.54, 3.541666666666667, 3.5433333333333334, 3.545,
    3.5466666666666669, 3.5483333333333333, 3.55, 3.5516666666666667,
    3.5533333333333332, 3.5549999999999997, 3.5566666666666666,
    3.5583333333333336, 3.5599999999999996, 3.561666666666667,
    3.5633333333333335, 3.565, 3.5666666666666669, 3.5683333333333334, 3.57,
    3.5716666666666668, 3.5733333333333333, 3.5749999999999997,
    3.5766666666666671, 3.5783333333333331, 3.58, 3.581666666666667,
    3.5833333333333335, 3.585, 3.5866666666666669, 3.5883333333333334, 3.59,
    3.5916666666666668, 3.5933333333333333, 3.5949999999999998,
    3.5966666666666667, 3.5983333333333336, 3.5999999999999996,
    3.601666666666667, 3.6033333333333335, 3.605, 3.6066666666666669,
    3.6083333333333334, 3.61, 3.6116666666666668, 3.6133333333333333,
    3.6149999999999998, 3.6166666666666671, 3.6183333333333332, 3.62,
    3.621666666666667, 3.6233333333333335, 3.625, 3.6266666666666669,
    3.6283333333333334, 3.63, 3.6316666666666668, 3.6333333333333333, 3.635,
    3.6366666666666667, 3.6383333333333336, 3.6399999999999997,
    3.6416666666666671, 3.6433333333333335, 3.645, 3.6466666666666665,
    3.6483333333333334, 3.65, 3.6516666666666664, 3.6533333333333333, 3.655,
    3.6566666666666663, 3.6583333333333332, 3.66, 3.6616666666666662,
    3.6633333333333336, 3.665, 3.6666666666666665, 3.6683333333333334, 3.67,
    3.6716666666666664, 3.6733333333333333, 3.675, 3.6766666666666663,
    3.6783333333333337, 3.6799999999999997, 3.6816666666666666,
    3.6833333333333336, 3.685, 3.6866666666666665, 3.6883333333333335, 3.69,
    3.6916666666666664, 3.6933333333333334, 3.695, 3.6966666666666663,
    3.6983333333333333, 3.7, 3.7016666666666662, 3.7033333333333336, 3.705,
    3.7066666666666666, 3.7083333333333335, 3.71, 3.7116666666666664,
    3.7133333333333334, 3.715, 3.7166666666666663, 3.7183333333333337,
    3.7199999999999998, 3.7216666666666667, 3.7233333333333336, 3.725,
    3.7266666666666666, 3.7283333333333335, 3.73, 3.7316666666666665,
    3.7333333333333334, 3.735, 3.7366666666666664, 3.7383333333333333, 3.74,
    3.7416666666666663, 3.7433333333333336, 3.745, 3.7466666666666666,
    3.7483333333333335, 3.75, 3.7516666666666665, 3.7533333333333334, 3.755,
    3.7566666666666664, 3.7583333333333337, 3.76, 3.7616666666666667,
    3.7633333333333336, 3.765, 3.7666666666666666, 3.7683333333333335, 3.77,
    3.7716666666666665, 3.7733333333333334, 3.775, 3.7766666666666664,
    3.7783333333333333, 3.7800000000000002, 3.7816666666666663,
    3.7833333333333337, 3.785, 3.7866666666666666, 3.7883333333333336, 3.79,
    3.7916666666666665, 3.7933333333333334, 3.795, 3.7966666666666664,
    3.7983333333333338, 3.8, 3.8016666666666667, 3.8033333333333337, 3.805,
    3.8066666666666666, 3.8083333333333336, 3.81, 3.8116666666666665,
    3.8133333333333335, 3.815, 3.8166666666666664, 3.8183333333333334,
    3.8200000000000003, 3.8216666666666663, 3.8233333333333337, 3.825,
    3.8266666666666667, 3.8283333333333336, 3.83, 3.8316666666666666,
    3.8333333333333335, 3.835, 3.8366666666666664, 3.8383333333333338, 3.84,
    3.8416666666666668, 3.8433333333333337, 3.845, 3.8466666666666667,
    3.8483333333333336, 3.85, 3.8516666666666666, 3.8533333333333335, 3.855,
    3.8566666666666665, 3.8583333333333329, 3.8600000000000003,
    3.8616666666666664, 3.8633333333333333, 3.865, 3.8666666666666667,
    3.8683333333333332, 3.87, 3.8716666666666666, 3.8733333333333331, 3.875,
    3.8766666666666665, 3.878333333333333, 3.88, 3.8816666666666668,
    3.8833333333333329, 3.8850000000000002, 3.8866666666666667,
    3.8883333333333332, 3.89, 3.8916666666666666, 3.8933333333333331, 3.895,
    3.8966666666666665, 3.898333333333333, 3.9000000000000004,
    3.9016666666666664, 3.9033333333333333, 3.9050000000000002,
    3.9066666666666667, 3.9083333333333332, 3.91, 3.9116666666666666,
    3.9133333333333331, 3.915, 3.9166666666666665, 3.918333333333333, 3.92,
    3.9216666666666669, 3.9233333333333329, 3.9250000000000003,
    3.9266666666666667, 3.9283333333333332, 3.93, 3.9316666666666666,
    3.9333333333333331, 3.935, 3.9366666666666665, 3.938333333333333,
    3.9400000000000004, 3.9416666666666664, 3.9433333333333334,
    3.9450000000000003, 3.9466666666666668, 3.9483333333333333, 3.95,
    3.9516666666666667, 3.9533333333333331, 3.955, 3.9566666666666666,
    3.958333333333333, 3.96, 3.9616666666666669, 3.9633333333333329,
    3.9650000000000003, 3.9666666666666668, 3.9683333333333333, 3.97,
    3.9716666666666667, 3.9733333333333332, 3.975, 3.9766666666666666,
    3.9783333333333331, 3.9800000000000004, 3.9816666666666665,
    3.9833333333333334, 3.9850000000000003, 3.9866666666666668,
    3.9883333333333333, 3.99, 3.9916666666666667, 3.9933333333333332, 3.995,
    3.9966666666666666, 3.9983333333333331, 4.0, 4.0016666666666669,
    4.003333333333333, 4.005, 4.0066666666666668, 4.0083333333333329, 4.01,
    4.0116666666666667, 4.0133333333333336, 4.0150000000000006,
    4.0166666666666666, 4.0183333333333335, 4.0200000000000005,
    4.0216666666666665, 4.0233333333333334, 4.025, 4.0266666666666664,
    4.0283333333333333, 4.03, 4.0316666666666663, 4.0333333333333332, 4.035,
    4.0366666666666671, 4.0383333333333331, 4.04, 4.041666666666667,
    4.043333333333333, 4.045, 4.0466666666666669, 4.0483333333333329,
    4.0500000000000007, 4.0516666666666667, 4.0533333333333328,
    4.0550000000000006, 4.0566666666666666, 4.0583333333333336,
    4.0600000000000005, 4.0616666666666665, 4.0633333333333335,
    4.0649999999999995, 4.0666666666666664, 4.0683333333333334,
    4.0699999999999994, 4.0716666666666672, 4.0733333333333333,
    4.0749999999999993, 4.0766666666666671, 4.0783333333333331, 4.08,
    4.081666666666667, 4.083333333333333, 4.085, 4.0866666666666669,
    4.0883333333333329, 4.09, 4.0916666666666668, 4.0933333333333337, 4.095,
    4.0966666666666667, 4.0983333333333336, 4.1, 4.1016666666666666,
    4.1033333333333335, 4.1049999999999995, 4.1066666666666665,
    4.1083333333333334, 4.1099999999999994, 4.1116666666666664,
    4.1133333333333333, 4.115, 4.1166666666666671, 4.1183333333333332, 4.12,
    4.121666666666667, 4.1233333333333331, 4.125, 4.1266666666666669,
    4.128333333333333, 4.13, 4.1316666666666668, 4.1333333333333329, 4.135,
    4.1366666666666667, 4.1383333333333336, 4.14, 4.1416666666666666,
    4.1433333333333335, 4.145, 4.1466666666666665, 4.1483333333333334,
    4.1499999999999995, 4.1516666666666673, 4.1533333333333333,
    4.1549999999999994, 4.1566666666666672, 4.1583333333333332, 4.16,
    4.1616666666666671, 4.1633333333333331, 4.165, 4.166666666666667,
    4.168333333333333, 4.17, 4.1716666666666669, 4.1733333333333338, 4.175,
    4.1766666666666667, 4.1783333333333337, 4.18, 4.1816666666666666,
    4.1833333333333336, 4.185, 4.1866666666666665, 4.1883333333333335,
    4.1899999999999995, 4.1916666666666664, 4.1933333333333334, 4.195,
    4.1966666666666672, 4.1983333333333333, 4.2, 4.2016666666666671,
    4.2033333333333331, 4.205, 4.206666666666667, 4.208333333333333, 4.21,
    4.2116666666666669, 4.2133333333333329, 4.215, 4.2166666666666668,
    4.2183333333333337, 4.22, 4.2216666666666667, 4.2233333333333336, 4.225,
    4.2266666666666666, 4.2283333333333335, 4.2299999999999995,
    4.2316666666666674, 4.2333333333333334, 4.2349999999999994,
    4.2366666666666672, 4.2383333333333333, 4.24, 4.2416666666666671,
    4.2433333333333332, 4.245, 4.246666666666667, 4.2483333333333331, 4.25,
    4.2516666666666669, 4.2533333333333339, 4.255, 4.2566666666666668,
    4.2583333333333337, 4.26, 4.2616666666666667, 4.2633333333333336, 4.265,
    4.2666666666666666, 4.2683333333333335, 4.27, 4.2716666666666665,
    4.2733333333333334, 4.275, 4.2766666666666664, 4.2783333333333333, 4.28,
    4.2816666666666663, 4.2833333333333332, 4.285, 4.2866666666666662,
    4.2883333333333331, 4.29, 4.2916666666666661, 4.293333333333333, 4.295,
    4.2966666666666669, 4.2983333333333338, 4.3, 4.3016666666666667,
    4.3033333333333337, 4.305, 4.3066666666666666, 4.3083333333333336, 4.31,
    4.3116666666666665, 4.3133333333333335, 4.3149999999999995,
    4.3166666666666664, 4.3183333333333334, 4.32, 4.3216666666666663,
    4.3233333333333333, 4.325, 4.3266666666666662, 4.3283333333333331, 4.33,
    4.3316666666666661, 4.3333333333333339, 4.335, 4.336666666666666,
    4.3383333333333338, 4.34, 4.3416666666666668, 4.3433333333333337, 4.345,
    4.3466666666666667, 4.3483333333333336, 4.35, 4.3516666666666666,
    4.3533333333333335, 4.355, 4.3566666666666665, 4.3583333333333334, 4.36,
    4.3616666666666664, 4.3633333333333333, 4.365, 4.3666666666666663,
    4.3683333333333332, 4.37, 4.3716666666666661, 4.3733333333333331, 4.375,
    4.3766666666666669, 4.3783333333333339, 4.38, 4.3816666666666668,
    4.3833333333333337, 4.385, 4.3866666666666667, 4.3883333333333336, 4.39,
    4.3916666666666666, 4.3933333333333335, 4.395, 4.3966666666666665,
    4.3983333333333334, 4.4, 4.4016666666666664, 4.4033333333333333, 4.405,
    4.4066666666666663, 4.4083333333333332, 4.41, 4.4116666666666662,
    4.413333333333334, 4.415, 4.4166666666666661, 4.4183333333333339, 4.42,
    4.4216666666666669, 4.4233333333333338, 4.425, 4.4266666666666667,
    4.4283333333333337, 4.43, 4.4316666666666666, 4.4333333333333336,
    4.4350000000000005, 4.4366666666666665, 4.4383333333333335, 4.44,
    4.4416666666666664, 4.4433333333333334, 4.445, 4.4466666666666663,
    4.4483333333333333, 4.45, 4.4516666666666662, 4.4533333333333331, 4.455,
    4.456666666666667, 4.4583333333333339, 4.46, 4.4616666666666669,
    4.4633333333333338, 4.465, 4.4666666666666668, 4.4683333333333337, 4.47,
    4.4716666666666667, 4.4733333333333336, 4.475, 4.4766666666666666,
    4.4783333333333335, 4.48, 4.4816666666666665, 4.4833333333333334, 4.485,
    4.4866666666666664, 4.4883333333333333, 4.49, 4.4916666666666663,
    4.4933333333333332, 4.495, 4.4966666666666661, 4.4983333333333331, 4.5,
    4.5016666666666669, 4.503333333333333, 4.505, 4.5066666666666668,
    4.5083333333333329, 4.51, 4.5116666666666667, 4.5133333333333328,
    4.5150000000000006, 4.5166666666666666, 4.5183333333333326,
    4.5200000000000005, 4.5216666666666665, 4.5233333333333334, 4.525,
    4.5266666666666664, 4.5283333333333333, 4.53, 4.5316666666666663,
    4.5333333333333332, 4.535, 4.5366666666666671, 4.5383333333333331, 4.54,
    4.541666666666667, 4.543333333333333, 4.545, 4.5466666666666669,
    4.5483333333333329, 4.55, 4.5516666666666667, 4.5533333333333328, 4.555,
    4.5566666666666666, 4.5583333333333336, 4.5600000000000005,
    4.5616666666666665, 4.5633333333333335, 4.565, 4.5666666666666664,
    4.5683333333333334, 4.57, 4.5716666666666663, 4.5733333333333333, 4.575,
    4.5766666666666662, 4.5783333333333331, 4.58, 4.581666666666667,
    4.583333333333333, 4.585, 4.5866666666666669, 4.5883333333333329, 4.59,
    4.5916666666666668, 4.5933333333333328, 4.5950000000000006,
    4.5966666666666667, 4.5983333333333327, 4.6000000000000005,
    4.6016666666666666, 4.6033333333333335, 4.605, 4.6066666666666665,
    4.6083333333333334, 4.61, 4.6116666666666664, 4.6133333333333333, 4.615,
    4.6166666666666671, 4.6183333333333332, 4.62, 4.621666666666667,
    4.6233333333333331, 4.625, 4.6266666666666669, 4.628333333333333, 4.63,
    4.6316666666666668, 4.6333333333333329, 4.635, 4.6366666666666667,
    4.6383333333333336, 4.6400000000000006, 4.6416666666666666,
    4.6433333333333335, 4.6450000000000005, 4.6466666666666665,
    4.6483333333333334, 4.65, 4.6516666666666664, 4.6533333333333333, 4.655,
    4.6566666666666663, 4.6583333333333332, 4.66, 4.6616666666666671,
    4.6633333333333331, 4.665, 4.666666666666667, 4.668333333333333, 4.67,
    4.6716666666666669, 4.6733333333333329, 4.6750000000000007,
    4.6766666666666667, 4.6783333333333328, 4.6800000000000006,
    4.6816666666666666, 4.6833333333333336, 4.6850000000000005,
    4.6866666666666665, 4.6883333333333335, 4.6899999999999995,
    4.6916666666666664, 4.6933333333333334, 4.6949999999999994,
    4.6966666666666672, 4.6983333333333333, 4.6999999999999993,
    4.7016666666666671, 4.7033333333333331, 4.705, 4.706666666666667,
    4.708333333333333, 4.71, 4.7116666666666669, 4.7133333333333329, 4.715,
    4.7166666666666668, 4.7183333333333337, 4.72, 4.7216666666666667,
    4.7233333333333336, 4.725, 4.7266666666666666, 4.7283333333333335,
    4.7299999999999995, 4.7316666666666665, 4.7333333333333334,
    4.7349999999999994, 4.7366666666666664, 4.7383333333333333, 4.74,
    4.7416666666666671, 4.7433333333333332, 4.745, 4.746666666666667,
    4.7483333333333331, 4.75, 4.7516666666666669, 4.753333333333333, 4.755,
    4.7566666666666668, 4.7583333333333329, 4.76, 4.7616666666666667,
    4.7633333333333336, 4.765, 4.7666666666666666, 4.7683333333333335, 4.77,
    4.7716666666666665, 4.7733333333333334, 4.7749999999999995,
    4.7766666666666673, 4.7783333333333333, 4.7799999999999994,
    4.7816666666666672, 4.7833333333333332, 4.785, 4.7866666666666671,
    4.7883333333333331, 4.79, 4.791666666666667, 4.793333333333333, 4.795,
    4.7966666666666669, 4.7983333333333338, 4.8, 4.8016666666666667,
    4.8033333333333337, 4.805, 4.8066666666666666, 4.8083333333333336, 4.81,
    4.8116666666666665, 4.8133333333333335, 4.8149999999999995,
    4.8166666666666664, 4.8183333333333334, 4.82, 4.8216666666666672,
    4.8233333333333333, 4.825, 4.8266666666666671, 4.8283333333333331, 4.83,
    4.831666666666667, 4.833333333333333, 4.835, 4.8366666666666669,
    4.8383333333333329, 4.84, 4.8416666666666668, 4.8433333333333337, 4.845,
    4.8466666666666667, 4.8483333333333336, 4.85, 4.8516666666666666,
    4.8533333333333335, 4.8549999999999995, 4.8566666666666674,
    4.8583333333333334, 4.8599999999999994, 4.8616666666666672,
    4.8633333333333333, 4.865, 4.8666666666666671, 4.8683333333333332, 4.87,
    4.871666666666667, 4.8733333333333331, 4.875, 4.8766666666666669,
    4.8783333333333339, 4.88, 4.8816666666666668, 4.8833333333333337, 4.885,
    4.8866666666666667, 4.8883333333333336, 4.89, 4.8916666666666666,
    4.8933333333333335, 4.895, 4.8966666666666665, 4.8983333333333334, 4.9,
    4.9016666666666664, 4.9033333333333333, 4.905, 4.9066666666666663,
    4.9083333333333332, 4.91, 4.9116666666666662, 4.9133333333333331, 4.915,
    4.9166666666666661, 4.918333333333333, 4.92, 4.9216666666666669,
    4.9233333333333338, 4.925, 4.9266666666666667, 4.9283333333333337, 4.93,
    4.9316666666666666, 4.9333333333333336, 4.935, 4.9366666666666665,
    4.9383333333333335, 4.9399999999999995, 4.9416666666666664,
    4.9433333333333334, 4.945, 4.9466666666666663, 4.9483333333333333, 4.95,
    4.9516666666666662, 4.9533333333333331, 4.955, 4.9566666666666661,
    4.9583333333333339, 4.96, 4.961666666666666, 4.9633333333333338, 4.965,
    4.9666666666666668, 4.9683333333333337, 4.97, 4.9716666666666667,
    4.9733333333333336, 4.975, 4.9766666666666666, 4.9783333333333335, 4.98,
    4.9816666666666665, 4.9833333333333334, 4.985, 4.9866666666666664,
    4.9883333333333333, 4.99, 4.9916666666666663, 4.9933333333333332, 4.995,
    4.9966666666666661, 4.9983333333333331, 5.0 };

  static double costab[8193];
  double sintab[8193];
  double sintabinv[8193];
  static creal_T wwc[11999];
  static creal_T dcv0[16384];
  int b_loop_ub;
  static creal_T b_y[6000];
  static creal_T fv[16384];
  static creal_T b_fv[16384];
  int n;
  int b_n;
  int info;
  int colj;
  int j;
  boolean_T exitg1;
  int ix;
  int iy;
  int coljp1;

  /*  Discrete frequency function with Cholesky decomposition and FFT coded by */
  /*  Dae Kun Kwon, Ph.D */
  /*  Research Assistant Professor */
  /*  NatHaz Modeling Laboratory */
  /*  Department of Civil & Environmental Engineering and Earth Sciences */
  /*  University of Notre Dame */
  /*  Notre Dame, IN 46556, USA */
  /*  Reference */
  /*  Wittig, L. E. and Sinha, A. K. (1975). "Simulation of multicorrelated random processes using the FFT algorithm." The Journal of the Acoustical Society of America, 58(3), 630-633.  */
  /*  Input parameters: units in English such as inch (length related variables) and mph (mile per hour for Gust wind speed) */
  /*  Please note that the inputs are made in English units but the outputs are calculated as SI units */
  /* EC = 'B';                       % Exposure Category: A, B, C, or D based on ASCE 7. However, only B, C, and D are currently defined as Exposure A was removed since ASCE 7-02 (2002) */
  /* V10 = 90;                       % Gust wind speed [mph] */
  /* drag = 1.3;                     % Drag Coefficient */
  /* Height = 720;                   % Building Height [inch] */
  /* Width = 40;                     % Building Width [inch] */
  /* Nfloor = 5;                     % Number of Floors */
  /* %%% convert English to Metric */
  /* %%% this main code is run based on SI units */
  i0 = HFloors->size[0] * HFloors->size[1];
  HFloors->size[0] = 1;
  emxEnsureCapacity_real_T(HFloors, i0);
  jjp1 = HFloors->size[0];
  jj = HFloors->size[1];
  loop_ub = jjp1 * jj;
  for (i0 = 0; i0 < loop_ub; i0++) {
    HFloors->data[i0] *= 0.0254;
  }

  emxInit_real_T(&z, 2);

  /*  inch to m */
  Width *= 0.0254;

  /*  inch to m */
  V10 *= 0.44704;

  /*  mph to m/s */
  /*  calculate vertical height per node */
  i0 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = HFloors->size[1];
  emxEnsureCapacity_real_T(z, i0);
  loop_ub = HFloors->size[0] * HFloors->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    z->data[i0] = HFloors->data[i0];
  }

  if ((!(HFloors->size[1] == 0)) && (HFloors->size[1] > 1)) {
    for (nmj = 1; nmj < HFloors->size[1]; nmj++) {
      z->data[nmj] += z->data[nmj - 1];
    }
  }

  /*  inches */
  /*  with fc, generating 10 min (600 sec) winds (N=18000 for 1-hr simulation) */
  /*  cut-off frequency */
  /*  frequency range */
  /*  total time */
  /* leave it */
  /* leave it */
  bbb = 0.0;
  z0 = 0.0;
  alpha = 0.0;

  /*  Exposure category */
  if (EC == 'A') {
    z0 = 2.0;

    /*  roughness height */
    bbb = 0.3;

    /*  factor for power law */
    alpha = 0.33333333333333331;

    /*  exponent of power law */
  } else if (EC == 'B') {
    z0 = 0.3;

    /*  roughness height */
    bbb = 0.45;

    /*  factor for power law */
    alpha = 0.25;

    /*  exponent of power law */
  } else if (EC == 'C') {
    z0 = 0.02;

    /*  roughness height */
    bbb = 0.65;

    /*  factor for power law */
    alpha = 0.15384615384615385;

    /*  exponent of power law */
  } else {
    if (EC == 'D') {
      z0 = 0.005;

      /*  roughness height */
      bbb = 0.8;

      /*  factor for power law */
      alpha = 0.1111111111111111;

      /*  exponent of power law */
    }
  }

  emxInit_real_T(&y, 2);

  /*  if(EC == ...) */
  /*  Von Karman's constant */
  /*  Coefficient for coherence function, set to 10 in this code */
  /*  Calculate wind speeds and friction velocity according to heights */
  b_log(10.0 / z0);

  /*  Friction velocity u* */
  zsize = z->size[1] - 1;

  /*  be careful not to use size(n) because n is matrix, anyway */
  /*  total number of upper triangle matrix components, if 5x5, nc=10 */
  i0 = windspeed_out->size[0] * windspeed_out->size[1];
  windspeed_out->size[0] = 6000;
  windspeed_out->size[1] = z->size[1];
  emxEnsureCapacity_real_T(windspeed_out, i0);
  loop_ub = 6000 * z->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    windspeed_out->data[i0] = 0.0;
  }

  emxInit_creal_T(&White, 2);

  /*  simulated wind velocity */
  /*  simulated wind force */
  i0 = White->size[0] * White->size[1];
  White->size[0] = 3000;
  White->size[1] = z->size[1];
  emxEnsureCapacity_creal_T(White, i0);
  loop_ub = 3000 * z->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    White->data[i0].re = 0.0;
    White->data[i0].im = 0.0;
  }

  emxInit_creal_T(&X, 2);
  i0 = X->size[0] * X->size[1];
  X->size[0] = 3000;
  X->size[1] = z->size[1];
  emxEnsureCapacity_creal_T(X, i0);
  loop_ub = 3000 * z->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    X->data[i0].re = 0.0;
    X->data[i0].im = 0.0;
  }

  emxInit_creal_T(&XW, 2);
  i0 = XW->size[0] * XW->size[1];
  XW->size[0] = 6000;
  XW->size[1] = z->size[1];
  emxEnsureCapacity_creal_T(XW, i0);
  loop_ub = 6000 * z->size[1];
  for (i0 = 0; i0 < loop_ub; i0++) {
    XW->data[i0].re = 0.0;
    XW->data[i0].im = 0.0;
  }

  emxInit_real_T(&VV, 2);

  /*  Calculate wind speeds and friction velocity according to heights */
  rdivide(z, y);
  power(y, alpha, VV);
  i0 = VV->size[0] * VV->size[1];
  VV->size[0] = 1;
  emxEnsureCapacity_real_T(VV, i0);
  c = V10 * bbb;
  jjp1 = VV->size[0];
  jj = VV->size[1];
  loop_ub = jjp1 * jj;
  for (i0 = 0; i0 < loop_ub; i0++) {
    VV->data[i0] *= c;
  }

  emxInit_real_T(&distb, 2);
  alpha = 10.0 / z0;
  c_log(&alpha);

  /*  Friction velocity u* */
  randn(seed);
  b_randn((double)z->size[1] * 2.0, distb);
  d0 = 2.0 * (double)z->size[1] - 1.0;
  if (1.0 > d0) {
    i0 = 1;
    i1 = -1;
  } else {
    i0 = 2;
    i1 = (int)d0 - 1;
  }

  emxInit_real_T(&Rel, 2);
  b_sqrt(0.5);
  jjp1 = Rel->size[0] * Rel->size[1];
  Rel->size[0] = 3000;
  Rel->size[1] = div_s32_floor(i1, i0) + 1;
  emxEnsureCapacity_real_T(Rel, jjp1);
  loop_ub = div_s32_floor(i1, i0);
  for (i1 = 0; i1 <= loop_ub; i1++) {
    for (jjp1 = 0; jjp1 < 3000; jjp1++) {
      Rel->data[jjp1 + Rel->size[0] * i1] = distb->data[jjp1 + distb->size[0] *
        (i0 * i1)] * 0.70710678118654757;
    }
  }

  jjj = (unsigned int)z->size[1] << 1;
  if (2U > jjj) {
    i0 = 1;
    i1 = 1;
    jjp1 = 0;
  } else {
    i0 = 2;
    i1 = 2;
    jjp1 = (int)jjj;
  }

  emxInit_creal_T(&Im, 2);
  dc0.re = -0.5;
  dc0.im = 0.0;
  d_sqrt(&dc0);
  jj = Im->size[0] * Im->size[1];
  Im->size[0] = 3000;
  Im->size[1] = div_s32_floor(jjp1 - i0, i1) + 1;
  emxEnsureCapacity_creal_T(Im, jj);
  loop_ub = div_s32_floor(jjp1 - i0, i1);
  for (jjp1 = 0; jjp1 <= loop_ub; jjp1++) {
    for (jj = 0; jj < 3000; jj++) {
      Im->data[jj + Im->size[0] * jjp1].re = distb->data[jj + distb->size[0] *
        ((i0 + i1 * jjp1) - 1)] * dc0.re;
      Im->data[jj + Im->size[0] * jjp1].im = distb->data[jj + distb->size[0] *
        ((i0 + i1 * jjp1) - 1)] * dc0.im;
    }
  }

  emxFree_real_T(&distb);

  /* %%%%%%%%%%%%%% loop %%%%%%%%%%%%%% */
  for (jj = 0; jj <= zsize; jj++) {
    for (i0 = 0; i0 < 3000; i0++) {
      White->data[i0 + White->size[0] * jj].re = Rel->data[i0 + Rel->size[0] *
        jj] + Im->data[i0 + Im->size[0] * jj].re;
      White->data[i0 + White->size[0] * jj].im = Im->data[i0 + Im->size[0] * jj]
        .im;
    }
  }

  emxFree_creal_T(&Im);
  emxFree_real_T(&Rel);
  emxInit_real_T(&G, 2);
  emxInit_real_T(&r0, 2);
  emxInit_real_T(&r1, 2);
  emxInit_real_T1(&r2, 1);
  emxInit_real_T(&r3, 2);
  emxInit_real_T(&b_G, 2);
  emxInit_creal_T(&c_G, 2);
  emxInit_creal_T1(&b_White, 1);
  emxInit_creal_T(&d_G, 2);
  d0 = 200.0 * rt_powd_snf(V10 * bbb * 0.4 / alpha, 2.0);
  if (1 > zsize + 1) {
    loop_ub = 0;
  } else {
    loop_ub = zsize + 1;
  }

  for (b_jj = 0; b_jj < 3000; b_jj++) {
    i0 = r3->size[0] * r3->size[1];
    r3->size[0] = 1;
    r3->size[1] = z->size[1];
    emxEnsureCapacity_real_T(r3, i0);
    jj = z->size[0] * z->size[1];
    for (i0 = 0; i0 < jj; i0++) {
      r3->data[i0] = d0 * z->data[i0];
    }

    b_rdivide(r3, VV, y);
    i0 = r3->size[0] * r3->size[1];
    r3->size[0] = 1;
    r3->size[1] = z->size[1];
    emxEnsureCapacity_real_T(r3, i0);
    alpha = 50.0 * dv0[b_jj];
    jj = z->size[0] * z->size[1];
    for (i0 = 0; i0 < jj; i0++) {
      r3->data[i0] = alpha * z->data[i0];
    }

    b_rdivide(r3, VV, r1);
    i0 = r3->size[0] * r3->size[1];
    r3->size[0] = 1;
    r3->size[1] = r1->size[1];
    emxEnsureCapacity_real_T(r3, i0);
    jj = r1->size[0] * r1->size[1];
    for (i0 = 0; i0 < jj; i0++) {
      r3->data[i0] = 1.0 + r1->data[i0];
    }

    b_power(r3, r1);
    b_rdivide(y, r1, r3);
    diag(r3, G);

    /*  Calculate off-diagonal terms */
    for (jjp1 = 0; jjp1 <= zsize; jjp1++) {
      i0 = zsize + (int)(1.0 - ((1.0 + (double)jjp1) + 1.0));
      for (jj = 0; jj <= i0; jj++) {
        jjj = ((unsigned int)jjp1 + jj) + 2U;
        alpha = G->data[jjp1 + G->size[0] * jjp1] * G->data[((int)jjj + G->size
          [0] * ((int)jjj - 1)) - 1];
        c_sqrt(&alpha);
        G->data[jjp1 + G->size[0] * ((int)jjj - 1)] = alpha * std::exp(-10.0 *
          dv0[b_jj] * std::abs(z->data[jjp1] - z->data[(int)jjj - 1]) / (0.5 *
          (VV->data[jjp1] + VV->data[(int)jjj - 1]))) * 0.999;

        /* G1(ii,jjj) = exp(-Cz*n(jj) * (abs(z(ii)-z(jjj))) / (0.5*(VV(ii)+VV(jjj)))); */
      }
    }

    b_diag(G, r2);
    c_diag(r2, r0);
    i0 = b_G->size[0] * b_G->size[1];
    b_G->size[0] = G->size[1];
    b_G->size[1] = G->size[0];
    emxEnsureCapacity_real_T(b_G, i0);
    jj = G->size[0];
    for (i0 = 0; i0 < jj; i0++) {
      b_loop_ub = G->size[1];
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        b_G->data[i1 + b_G->size[0] * i0] = (G->data[i0 + G->size[0] * i1] +
          G->data[i1 + G->size[0] * i0]) - r0->data[i1 + r0->size[0] * i0];
      }
    }

    i0 = G->size[0] * G->size[1];
    G->size[0] = b_G->size[0];
    G->size[1] = b_G->size[1];
    emxEnsureCapacity_real_T(G, i0);
    jj = b_G->size[1];
    for (i0 = 0; i0 < jj; i0++) {
      b_loop_ub = b_G->size[0];
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        G->data[i1 + G->size[0] * i0] = b_G->data[i1 + b_G->size[0] * i0];
      }
    }

    n = G->size[1];
    if (G->size[1] != 0) {
      b_n = G->size[0];
      info = 0;
      if (G->size[0] != 0) {
        colj = 1;
        j = 1;
        exitg1 = false;
        while ((!exitg1) && (j <= b_n)) {
          jj = (colj + j) - 2;
          alpha = 0.0;
          if (!(j - 1 < 1)) {
            ix = colj;
            iy = colj;
            for (nmj = 1; nmj < j; nmj++) {
              alpha += G->data[ix - 1] * G->data[iy - 1];
              ix++;
              iy++;
            }
          }

          alpha = G->data[jj] - alpha;
          if (alpha > 0.0) {
            alpha = std::sqrt(alpha);
            G->data[jj] = alpha;
            if (j < b_n) {
              nmj = b_n - j;
              jjp1 = jj + b_n;
              coljp1 = colj + b_n;
              if ((j - 1 == 0) || (nmj == 0)) {
              } else {
                iy = jjp1;
                i0 = coljp1 + n * (nmj - 1);
                for (b_loop_ub = coljp1; b_loop_ub <= i0; b_loop_ub += n) {
                  ix = colj;
                  c = 0.0;
                  i1 = (b_loop_ub + j) - 2;
                  for (jj = b_loop_ub; jj <= i1; jj++) {
                    c += G->data[jj - 1] * G->data[ix - 1];
                    ix++;
                  }

                  G->data[iy] += -c;
                  iy += n;
                }
              }

              alpha = 1.0 / alpha;
              if (!(b_n < 1)) {
                i0 = (jjp1 + b_n * (nmj - 1)) + 1;
                for (nmj = jjp1 + 1; nmj <= i0; nmj += b_n) {
                  G->data[nmj - 1] *= alpha;
                }
              }

              colj = coljp1;
            }

            j++;
          } else {
            G->data[jj] = alpha;
            info = j;
            exitg1 = true;
          }
        }
      }

      if (info == 0) {
        jjp1 = n;
      } else {
        jjp1 = info - 1;
      }

      for (j = 0; j + 1 <= jjp1; j++) {
        for (jj = j + 1; jj + 1 <= jjp1; jj++) {
          G->data[jj + G->size[0] * j] = 0.0;
        }
      }
    }

    i0 = c_G->size[0] * c_G->size[1];
    c_G->size[0] = G->size[1];
    c_G->size[1] = G->size[0];
    emxEnsureCapacity_creal_T(c_G, i0);
    jj = G->size[0];
    for (i0 = 0; i0 < jj; i0++) {
      b_loop_ub = G->size[1];
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        c_G->data[i1 + c_G->size[0] * i0].re = G->data[i0 + G->size[0] * i1];
        c_G->data[i1 + c_G->size[0] * i0].im = 0.0;
      }
    }

    i0 = b_White->size[0];
    b_White->size[0] = loop_ub;
    emxEnsureCapacity_creal_T1(b_White, i0);
    for (i0 = 0; i0 < loop_ub; i0++) {
      b_White->data[i0].re = White->data[b_jj + White->size[0] * i0].re;
      b_White->data[i0].im = -White->data[b_jj + White->size[0] * i0].im;
    }

    i0 = d_G->size[0] * d_G->size[1];
    d_G->size[0] = 1;
    d_G->size[1] = c_G->size[0];
    emxEnsureCapacity_creal_T(d_G, i0);
    jj = c_G->size[0];
    for (i0 = 0; i0 < jj; i0++) {
      alpha = 0.0;
      c = 0.0;
      b_loop_ub = c_G->size[1];
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        z0 = c_G->data[i0 + c_G->size[0] * i1].re * b_White->data[i1].re -
          c_G->data[i0 + c_G->size[0] * i1].im * b_White->data[i1].im;
        bbb = c_G->data[i0 + c_G->size[0] * i1].re * b_White->data[i1].im +
          c_G->data[i0 + c_G->size[0] * i1].im * b_White->data[i1].re;
        alpha += z0;
        c += bbb;
      }

      d_G->data[d_G->size[0] * i0].re = alpha;
      d_G->data[d_G->size[0] * i0].im = -c;
    }

    jj = d_G->size[1];
    for (i0 = 0; i0 < jj; i0++) {
      X->data[b_jj + X->size[0] * i0].re = 173.20508075688775 * d_G->data
        [d_G->size[0] * i0].re;
      X->data[b_jj + X->size[0] * i0].im = 173.20508075688775 * d_G->data
        [d_G->size[0] * i0].im;
    }
  }

  emxFree_creal_T(&d_G);
  emxFree_creal_T(&b_White);
  emxFree_creal_T(&c_G);
  emxFree_real_T(&b_G);
  emxFree_real_T(&r3);
  emxFree_real_T(&r2);
  emxFree_real_T(&r1);
  emxFree_real_T(&y);
  emxFree_real_T(&r0);
  emxFree_real_T(&G);
  emxFree_creal_T(&White);

  /*  end of jj=1:N */
  if (0 <= zsize) {
    generate_twiddle_tables(costab, sintab, sintabinv);
    bluesteinSetup(wwc);
    r2br_r2dit_trig(wwc, costab, sintab, dcv0);
  }

  for (nmj = 0; nmj <= zsize; nmj++) {
    for (i0 = 0; i0 < 3000; i0++) {
      XW->data[(i0 + XW->size[0] * nmj) + 1] = X->data[i0 + X->size[0] * nmj];
    }

    for (i0 = 0; i0 < 2999; i0++) {
      XW->data[(i0 + XW->size[0] * nmj) + 3001].re = X->data[(X->size[0] * nmj -
        i0) + 2998].re;
      XW->data[(i0 + XW->size[0] * nmj) + 3001].im = -X->data[(X->size[0] * nmj
        - i0) + 2998].im;
    }

    alpha = X->data[2999 + X->size[0] * nmj].re;
    c = X->data[2999 + X->size[0] * nmj].im;
    XW->data[3000 + XW->size[0] * nmj].re = rt_hypotd_snf(alpha, c);
    XW->data[3000 + XW->size[0] * nmj].im = 0.0;
    jjp1 = 0;
    for (b_loop_ub = 0; b_loop_ub < 6000; b_loop_ub++) {
      b_y[b_loop_ub].re = wwc[b_loop_ub + 5999].re * XW->data[jjp1 + XW->size[0]
        * nmj].re + wwc[b_loop_ub + 5999].im * XW->data[jjp1 + XW->size[0] * nmj]
        .im;
      b_y[b_loop_ub].im = wwc[b_loop_ub + 5999].re * XW->data[jjp1 + XW->size[0]
        * nmj].im - wwc[b_loop_ub + 5999].im * XW->data[jjp1 + XW->size[0] * nmj]
        .re;
      jjp1++;
    }

    r2br_r2dit_trig_impl(b_y, costab, sintab, fv);
    for (jj = 0; jj < 16384; jj++) {
      b_fv[jj].re = fv[jj].re * dcv0[jj].re - fv[jj].im * dcv0[jj].im;
      b_fv[jj].im = fv[jj].re * dcv0[jj].im + fv[jj].im * dcv0[jj].re;
    }

    b_r2br_r2dit_trig(b_fv, costab, sintabinv, fv);
    jj = 0;
    for (b_loop_ub = 0; b_loop_ub < 6000; b_loop_ub++) {
      b_y[jj].re = wwc[b_loop_ub + 5999].re * fv[b_loop_ub + 5999].re +
        wwc[b_loop_ub + 5999].im * fv[b_loop_ub + 5999].im;
      b_y[jj].im = wwc[b_loop_ub + 5999].re * fv[b_loop_ub + 5999].im -
        wwc[b_loop_ub + 5999].im * fv[b_loop_ub + 5999].re;
      b_y[jj].re = wwc[b_loop_ub + 5999].re * fv[b_loop_ub + 5999].re +
        wwc[b_loop_ub + 5999].im * fv[b_loop_ub + 5999].im;
      b_y[jj].im = wwc[b_loop_ub + 5999].re * fv[b_loop_ub + 5999].im -
        wwc[b_loop_ub + 5999].im * fv[b_loop_ub + 5999].re;
      if (b_y[jj].im == 0.0) {
        b_y[jj].re /= 6000.0;
        b_y[jj].im = 0.0;
      } else if (b_y[jj].re == 0.0) {
        b_y[jj].re = 0.0;
        b_y[jj].im /= 6000.0;
      } else {
        b_y[jj].re /= 6000.0;
        b_y[jj].im /= 6000.0;
      }

      jj++;
    }

    for (i0 = 0; i0 < 6000; i0++) {
      windspeed_out->data[i0 + windspeed_out->size[0] * nmj] = b_y[i0].re;
    }
  }

  emxFree_creal_T(&XW);
  emxFree_creal_T(&X);

  /* %% calculate fluctuating/dynamic wind force, windforce_out: [unit in Newton, N] */
  /*  air density  kg/m^3 */
  i0 = windforce_out->size[0] * windforce_out->size[1];
  windforce_out->size[0] = 6000;
  windforce_out->size[1] = z->size[1];
  emxEnsureCapacity_real_T(windforce_out, i0);
  jjp1 = 0;
  emxFree_real_T(&z);
  while (jjp1 <= zsize) {
    alpha = 1.226 * VV->data[jjp1];
    c = HFloors->data[jjp1];
    for (i0 = 0; i0 < 6000; i0++) {
      windforce_out->data[i0 + windforce_out->size[0] * jjp1] = alpha *
        windspeed_out->data[i0 + windspeed_out->size[0] * jjp1] * drag * c *
        Width;
    }

    jjp1++;
  }

  emxFree_real_T(&VV);

  /* %% Output variables and their units */
  /* windspeed_out = wind speeds, m/s */
  /* windforce_out = wind force, Newton (N) */
  /* figure(1) */
  /* plot(dt:dt:T,windspeed_out(:,1)) */
  /*  */
  *dt = 0.1;
}

void windsim_dk1_main_initialize()
{
  rt_InitInfAndNaN(8U);
  eml_randn_init();
  c_eml_rand_mt19937ar_stateful_i();
}

void windsim_dk1_main_terminate()
{
  /* (no terminate code required) */
}

/* End of code generation (windsim_dk1_main.cpp) */
