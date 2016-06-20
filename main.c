/******************************************************************************
*
* MSP432 empty main.c template
*
* for GNU compiler
*
******************************************************************************/

#include "msp.h"
#include "driverlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <assert.h>

#define BENCH_LOOPS 100
#define ROWS 128
#define COLS 40
#define N 512
#define N_inv 12265
#define p 12289 // Modulo p = k times N + 1 and p is prime. This is a special case addressed in [Longa and Naehrig, 2016]

const uint32_t _p32_t[ROWS] =
{0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 5, 14, 6, 28, 48, 11, 91, 21, 151, 29, 115, 261, 356, 1022, 0, 1979,
649, 662, 2533, 394, 3885, 4422, 6621, 3717, 10085, 14247, 2087, 32191, 31092, 5293, 64733, 6382, 39056, 115692, 51969, 5856, 94298,
171399, 177506, 92944, 508031, 82593, 227864, 25774, 1030068, 7848, 1028490, 420956, 1371527, 764718, 1483754, 1713010, 3014121,
52865, 3832607, 959009, 6586417, 2413379, 1330906, 8187911, 6557106, 9169590, 10590036, 14973908, 14290999, 19921632, 11609504,
18523933, 25695147, 9724218, 47622715, 60536693, 42918720, 16785990, 105485922, 6837708, 50755763, 43186108, 94426434, 226108042,
19905797, 44150882, 187467745, 109279605, 422163790, 172043920, 255161468, 426425504, 264698303, 905612549, 52289274, 62428384,
426638585, 325949524, 652487048, 1482215515, 1224550981, 303419361, 1867463484, 51807019, 3683469341u, 803931803, 1714099899,
2620837769u, 2169546439u, 1709320247, 3824357929u, 4067003105u, 2706660263u, 724038919, 2747761453u, 2899911731u, 2781772224u,
2289919236u, 2637396209u, 3220823603u, 540201915, 3813240143u};

const uint8_t _p8_t[ROWS] =
{0, 0, 0, 15, 54, 90, 150, 96, 152, 230, 226, 90, 4, 82, 222, 246, 14, 64, 162, 196, 47, 149, 166, 58,
7, 232, 174, 161, 204, 62, 4, 2, 139, 138, 90, 123, 195, 227, 11, 86, 88, 118, 8, 60, 224, 18, 245, 103, 194, 158, 239, 182, 126,
63, 83, 207, 61, 2, 253, 162, 5, 125, 9, 113, 252, 95, 136, 54, 159, 35, 171, 33, 128, 130, 11, 190, 117, 98, 186, 86, 178, 19,
64, 237, 192, 250, 223, 28, 125, 71, 21, 199, 187, 163, 6, 98, 155, 140, 244, 204, 77, 60, 200, 147, 1, 185, 34, 110, 181, 39, 175,
182, 171, 116, 171, 201, 196, 125, 55, 117, 48, 197, 231, 50, 71, 0, 92, 100};

const uint8_t _hw[ROWS] =
{0, 0, 0, 4, 4, 4, 4, 3, 4, 6, 6, 7, 3, 6, 8, 9, 8, 4, 8, 7, 10, 7, 8, 13, 3, 13, 9, 8, 11, 9, 9, 6, 13, 9, 12, 16, 9, 18, 12,
  11, 15, 13, 6, 14, 9, 8, 14, 15, 12, 12, 19, 11, 15, 14, 18, 13, 16, 11, 19, 15, 14, 16, 18, 11, 21, 14, 9, 14, 17, 15, 15, 16,
12, 16, 14, 21, 14, 16, 16, 14, 19, 21, 12, 12, 14, 17, 20, 19, 17, 17, 17, 15, 20, 19, 17, 13, 22, 17, 26, 20, 22, 16, 23, 18,
13, 19, 17, 20, 25, 16, 22, 23, 22, 21, 18, 19, 19, 22, 21, 22, 18, 19, 22, 16, 22, 23, 19, 21};

const int32_t psi_rev[512] =
{1, 1479, 8246, 5146, 4134, 6553, 11567, 1305, 5860, 3195, 1212, 10643, 3621, 9744, 8785, 3542, 7311, 10938, 8961, 5777, 5023,
6461, 5728, 4591, 3006, 9545, 563, 9314, 2625, 11340, 4821, 2639, 12149, 1853, 726, 4611, 11112, 4255, 2768, 1635, 2963, 7393,
2366, 9238, 9198, 12208, 11289, 7969, 8736, 4805, 11227, 2294, 9542, 4846, 9154, 8577, 9275, 3201, 7203, 10963, 1170, 9970, 955,
11499, 8340, 8993, 2396, 4452, 6915, 2837, 130, 7935, 11336, 3748, 6522, 11462, 5067, 10092, 12171, 9813, 8011, 1673, 5331, 7300,
10908, 9764, 4177, 8705, 480, 9447, 1022, 12280, 5791, 11745, 9821, 11950, 12144, 6747, 8652, 3459, 2731, 8357, 6378, 7399, 10530,
3707, 8595, 5179, 3382, 355, 4231, 2548, 9048, 11560, 3289, 10276, 9005, 9408, 5092, 10200, 6534, 4632, 4388, 1260, 334, 2426,
1428, 10593, 3400, 2399, 5191, 9153, 9273, 243, 3000, 671, 3531, 11813, 3985, 7384, 10111, 10745, 6730, 11869, 9042, 2686, 2969,
3978, 8779, 6957, 9424, 2370, 8241, 10040, 9405, 11136, 3186, 5407, 10163, 1630, 3271, 8232, 10600, 8925, 4414, 2847, 10115, 4372,
9509, 5195, 7394, 10805, 9984, 7247, 4053, 9644, 12176, 4919, 2166, 8374, 12129, 9140, 7852, 3, 1426, 7635, 10512, 1663, 8653,
4938, 2704, 5291, 5277, 1168, 11082, 9041, 2143, 11224, 11885, 4645, 4096, 11796, 5444, 2381, 10911, 1912, 4337, 11854, 4976,
10682, 11414, 8509, 11287, 5011, 8005, 5088, 9852, 8643, 9302, 6267, 2422, 6039, 2187, 2566, 10849, 8526, 9223, 27, 7205, 1632,
7404, 1017, 4143, 7575, 12047, 10752, 8585, 2678, 7270, 11744, 3833, 3778, 11899, 773, 5101, 11222, 9888, 442, 9377, 6591, 354,
7428, 5012, 2481, 1045, 9430, 1987, 1702, 3565, 654, 5206, 6760, 3199, 56, 6137, 7341, 11889, 10561, 5862, 6153, 5415, 8646, 1359,
6854, 11035, 973, 2033, 8291, 1922, 3879, 468, 3988, 382, 11973, 5339, 6843, 6196, 8579, 4467, 7500, 4749, 6752, 8500, 12142,
6833, 4449, 1050, 4536, 6844, 8429, 2683, 11099, 3818, 6171, 6364, 11271, 3514, 11248, 10316, 6715, 1278, 9945, 8214, 6974, 7965,
7373, 2169, 522, 5079, 3262, 6008, 885, 5009, 10333, 1003, 8757, 241, 58, 11184, 142, 6608, 3477, 3438, 9445, 11314, 8077, 3602,
6221, 11868, 4080, 8689, 9026, 4624, 6212, 7507, 5886, 3029, 6695, 4213, 504, 11684, 2302, 6821, 11279, 11502, 3482, 7048, 2920,
3127, 4169, 7232, 4698, 8844, 4780, 10240, 4912, 1321, 12097, 11858, 1579, 9784, 6383, 151, 2127, 3957, 2839, 5874, 11612, 6055,
8953, 52, 3174, 10966, 9523, 9139, 10970, 4046, 11580, 4240, 3570, 835, 6065, 11367, 441, 4079, 11231, 10331, 4322, 2078, 1112,
12225, 3656, 683, 2459, 5782, 10723, 9341, 2503, 5919, 4433, 8455, 7032, 1747, 3123, 3054, 6803, 10885, 325, 11143, 948, 8561,
4049, 5990, 11130, 6190, 11994, 6523, 652, 3762, 9370, 4016, 4077, 8960, 4298, 2692, 12121, 1594, 10327, 7183, 5961, 6992, 6119,
8333, 10929, 1200, 5184, 2555, 6122, 2882, 10484, 10335, 10238, 6147, 9842, 8326, 576, 3434, 3529, 2908, 12071, 2361, 1843, 3030,
8174, 6956, 2031, 6413, 10008, 12133, 2767, 3969, 8298, 11836, 5908, 418, 3772, 7515, 5429, 7552, 10996, 2057, 6920, 3202, 4493,
11939, 10777, 1815, 5383, 10800, 9789, 10706, 5942, 1263, 49, 5915, 10806, 9280, 10596, 11566, 12115, 9551, 5868, 9634, 5735,
1975, 8532, 2925, 347, 4754, 1858, 11863, 8974};

const int32_t psi_inv_rev[512] =
{1, 10810, 7143, 4043, 10984, 722, 5736, 8155, 8747, 3504, 2545, 8668, 1646, 11077, 9094, 6429, 9650, 7468, 949, 9664, 2975,
11726, 2744, 9283, 7698, 6561, 5828, 7266, 6512, 3328, 1351, 4978, 790, 11334, 2319, 11119, 1326, 5086, 9088, 3014, 3712, 3135,
7443, 2747, 9995, 1062, 7484, 3553, 4320, 1000, 81, 3091, 3051, 9923, 4896, 9326, 10654, 9521, 8034, 1177, 7678, 11563, 10436,
140, 1696, 10861, 9863, 11955, 11029, 7901, 7657, 5755, 2089, 7197, 2881, 3284, 2013, 9000, 729, 3241, 9741, 8058, 11934, 8907,
7110, 3694, 8582, 1759, 4890, 5911, 3932, 9558, 8830, 3637, 5542, 145, 339, 2468, 544, 6498, 9, 11267, 2842, 11809, 3584, 8112,
2525, 1381, 4989, 6958, 10616, 4278, 2476, 118, 2197, 7222, 827, 5767, 8541, 953, 4354, 12159, 9452, 5374, 7837, 9893, 3296, 3949,
2859, 11244, 9808, 7277, 4861, 11935, 5698, 2912, 11847, 2401, 1067, 7188, 11516, 390, 8511, 8456, 545, 5019, 9611, 3704, 1537,
242, 4714, 8146, 11272, 4885, 10657, 5084, 12262, 3066, 3763, 1440, 9723, 10102, 6250, 9867, 6022, 2987, 3646, 2437, 7201, 4284,
7278, 1002, 3780, 875, 1607, 7313, 435, 7952, 10377, 1378, 9908, 6845, 493, 8193, 7644, 404, 1065, 10146, 3248, 1207, 11121, 7012,
6998, 9585, 7351, 3636, 10626, 1777, 4654, 10863, 12286, 4437, 3149, 160, 3915, 10123, 7370, 113, 2645, 8236, 5042, 2305, 1484,
4895, 7094, 2780, 7917, 2174, 9442, 7875, 3364, 1689, 4057, 9018, 10659, 2126, 6882, 9103, 1153, 2884, 2249, 4048, 9919, 2865,
5332, 3510, 8311, 9320, 9603, 3247, 420, 5559, 1544, 2178, 4905, 8304, 476, 8758, 11618, 9289, 12046, 3016, 3136, 7098, 9890,
8889, 3315, 426, 10431, 7535, 11942, 9364, 3757, 10314, 6554, 2655, 6421, 2738, 174, 723, 1693, 3009, 1483, 6374, 12240, 11026,
6347, 1583, 2500, 1489, 6906, 10474, 1512, 350, 7796, 9087, 5369, 10232, 1293, 4737, 6860, 4774, 8517, 11871, 6381, 453, 3991,
8320, 9522, 156, 2281, 5876, 10258, 5333, 4115, 9259, 10446, 9928, 218, 9381, 8760, 8855, 11713, 3963, 2447, 6142, 2051, 1954,
1805, 9407, 6167, 9734, 7105, 11089, 1360, 3956, 6170, 5297, 6328, 5106, 1962, 10695, 168, 9597, 7991, 3329, 8212, 8273, 2919,
8527, 11637, 5766, 295, 6099, 1159, 6299, 8240, 3728, 11341, 1146, 11964, 1404, 5486, 9235, 9166, 10542, 5257, 3834, 7856, 6370,
9786, 2948, 1566, 6507, 9830, 11606, 8633, 64, 11177, 10211, 7967, 1958, 1058, 8210, 11848, 922, 6224, 11454, 8719, 8049, 709,
8243, 1319, 3150, 2766, 1323, 9115, 12237, 3336, 6234, 677, 6415, 9450, 8332, 10162, 12138, 5906, 2505, 10710, 431, 192, 10968,
7377, 2049, 7509, 3445, 7591, 5057, 8120, 9162, 9369, 5241, 8807, 787, 1010, 5468, 9987, 605, 11785, 8076, 5594, 9260, 6403, 4782,
6077, 7665, 3263, 3600, 8209, 421, 6068, 8687, 4212, 975, 2844, 8851, 8812, 5681, 12147, 1105, 12231, 12048, 3532, 11286, 1956,
7280, 11404, 6281, 9027, 7210, 11767, 10120, 4916, 4324, 5315, 4075, 2344, 11011, 5574, 1973, 1041, 8775, 1018, 5925, 6118, 8471,
1190, 9606, 3860, 5445, 7753, 11239, 7840, 5456, 147, 3789, 5537, 7540, 4789, 7822, 3710, 6093, 5446, 6950, 316, 11907, 8301,
11821, 8410, 10367, 3998, 10256, 11316, 1254, 5435, 10930, 3643, 6874, 6136, 6427, 1728, 400, 4948, 6152, 12233, 9090, 5529, 7083,
11635, 8724, 10587, 10302};

int randomBits[ROWS];

static uint32_t ct_lt_u32(uint32_t x, uint32_t y) {
    return (x^((x^y)|((x-y)^y))) >> 31;
}

// bit = 0 then return a
int32_t Select(int32_t a, int32_t b, unsigned bit) {
  unsigned mask = -bit;
  return ((mask & (a ^ b)) ^ a);
}

int32_t knuth_yao_ct_fast_32(int tailcut, int sigma, int center) {

  int32_t signal;
  int32_t _d, d, S; // may be negative numbers
  uint32_t bound, col, invalidSample, pNumRows, pNumCols, r, t;
  unsigned enable, hit;

  bound = tailcut * sigma;
  signal = 1 - 2 * (rand() & 1);
  invalidSample = bound + 1;
  pNumRows = ROWS;
  pNumCols = COLS;

  uint32_t index, k; // < 255
  int32_t row;

  index = 0;
  while(index < pNumRows) {
    r = rand();
    for (k = 0; k < 32; k++, r >>= 1)
      randomBits[index++] = (int8_t)(r & 0x1);
 }//end-while

  int8_t aux_hw;

  S = 0; t = 0; d = 0; hit = 0; _d = 0;
  for (row = 1; row < pNumRows; row++) {
    aux_hw = _hw[row];
    _d = 2 * _d + randomBits[row];	// Distance calculus
    enable = ct_lt_u32(_d, aux_hw); // _d < _hw[row]?
    _d = Select(_d - aux_hw, _d, enable & !hit);
    d = Select(d, _d, enable & !hit);
    t = Select(t, row, enable & !hit); // t \in {0, 127 = ROWS}
    hit += (enable & !hit);
  }

  uint32_t aux_p32 = _p32_t[t];

  hit = 0;
  for (col = 0; col <= 31; col++) {
    d = d - ((aux_p32 >> (uint32_t)(32 - col - 1)) & 1);

    enable = (unsigned)(d + 1); // "enable" turns 0 iff d = -1
    enable = (1 ^ ((enable | -enable) >> 31)) & 1; // "enable" turns 1 iff "enable" was 0

    S += Select(invalidSample, col, (enable & !hit));
    hit += (enable & !hit);

  }

  uint8_t aux_p8 = _p8_t[t];

  for (col = 32; col < pNumCols; col++) {
    d = d - ((aux_p8 >> (uint8_t)(pNumCols - col - 1)) & 1);

    enable = (unsigned)(d + 1); // "enable" turns 0 iff d = -1
    enable = (1 ^ ((enable | -enable) >> 31)) & 1; // "enable" turns 1 iff "enable" was 0

    S += Select(invalidSample, col, (enable & !hit));
    hit += (enable & !hit);

  }

  /* Note: the "col" value is in [0, bound]. So, the invalid sample must be
    * greater than bound. */
  S %= invalidSample;
  S = S - bound + center;
  S *= signal;

  return S;

}

int32_t mod(int32_t a, int32_t b) {

  if(b < 0)
    return mod(-a, -b);

  int32_t ret = a % b;

  if(ret < 0)
    ret += b;

  return ret;

}//end-mod()

void NTT(int32_t a[]) {

  int32_t i, j, j1, j2, k, m, S, U, V;

  k = N;

  for(m = 1; m < N; m <<= 1) {
    k >>= 1;
    for(i = 0; i < m; i++) {
      j1 = 2*i*k;
      j2 = j1 + k - 1;
      S = psi_rev[m+i];
      for(j = j1; j <= j2; j++) {
		U = a[j];
		V = a[j+k]*S;
		a[j] = mod(U + V, p);
		a[j+k] = mod(U - V, p);
      }//end-for
    }//end-for
  }//end-for

}//end-NTT()

void INTT(int32_t a[]) {

  int32_t i, h, j, j1, j2, k, m, U, V, S;

  k = 1;

  for(m = N; m > 1; m >>= 1) {
    j1 = 0;
    h = m >> 1;
    for(i = 0; i < h; i++) {
      j2 = j1 + k - 1;
      S = psi_inv_rev[h+i];
      for(j = j1; j <= j2; j++) {
		U = a[j];
		V = a[j + k];
		a[j] = mod(U + V, p);
		a[j+k] = mod((U - V)*S, p);
      }//end-for
      j1 = j1 + 2*k;
    }//end-for
    k <<= 1;
  }//end-for

  for(j = 0; j < N; j++) {
    a[j] = mod(a[j] * N_inv, p);
  }

}//end-INTT()

void Mult(int32_t c[], int32_t a[], int32_t b[]) {

	uint32_t i;

	for(i = 0; i < N; i++)
		c[i] = mod(a[i]*b[i], p);

}

void PolySampling(int32_t a[]) {

	uint32_t i;

	for(i = 0; i < N; i++)
		a[i] = knuth_yao_ct_fast_32(13, 3.1915, 0);

}//end-PolySampling()

void RandomPoly(int32_t a[]) {

	uint32_t i;

	for(i = 0; i < N; i++)
		a[i] = rand() % p;

}

void Add(int32_t c[], const int32_t a[], const int32_t b[]) {

	uint32_t i;

	for(i = 0; i < N; i++)
		c[i] = mod(a[i]+b[i], p);

}

void KeyGeneration(const int32_t a[], int32_t r2[], int32_t p1[]) {

	int32_t r1[N];
	uint32_t i;

	PolySampling(r1);
	PolySampling(r2);
	NTT(r1);
	NTT(r2);

	for(i = 0; i < N; i++)
	  p1[i] = mod(r1[i] - a[i] * r2[i], p);

	// Private key: r2 | Public key: (a, p1)

}//end-KeyGeneration()

void Encryption(int32_t c1[], int32_t c2[], const int32_t a[], const int32_t p1[], const int32_t m[]) {

  int32_t e1[N], e2[N], e3[N];
  uint32_t i;

  PolySampling(e1);
  PolySampling(e2);
  PolySampling(e3);

  NTT(e1);
  NTT(e2);

  int32_t aux[N];
  Add(aux, e3, m);
  NTT(aux);

  for(i = 0; i < N; i++) {
    c1[i] = mod(a[i] * e1[i] + e2[i], p);
    c2[i] = mod(p1[i] * e1[i] + aux[i], p);
  }//end-for

}

void Decryption(int32_t m[], int32_t c1[], int32_t c2[], int32_t r2[]) {

  uint32_t i;

  for(i = 0; i < N; i++)
    m[i] = mod(c1[i] * r2[i] + c2[i], p);
  INTT(m);

}

void Encode(int32_t aprime[], const int32_t a[]) {

  uint32_t i;

  int32_t bound = (p-1)/2;

  for(i = 0; i < N; i++) {
    aprime[i] = a[i]*bound;
  }

}

void Decode(int32_t a[], const int32_t aprime[]) {

  uint32_t i;

  int32_t lbound = (p-1)/4;
  int32_t ubound = 3*lbound;

  for(i = 0; i < N; i++) {
    if(aprime[i] >= lbound && aprime[i] < ubound)
      a[i] = 1;
    else
      a[i] = 0;
  }

}

int main(void) {

    /* Halting WDT and disabling master interrupts */
     MAP_WDT_A_holdTimer();
     MAP_Interrupt_disableMaster();

    srand(time(NULL));

    while(1) {

        int32_t a[N], r2[N], p1[N];
        RandomPoly(a);
        NTT(a);
        KeyGeneration(a, r2, p1);

        int32_t m[N], mprime[N], mout[N];
        uint32_t i;

        for(i = 0; i < N; i++)
          m[i] = rand() % 2;
        Encode(mprime, m);

        int32_t c1[N], c2[N];

        Encryption(c1, c2, a, p1, mprime);
        Decryption(mprime, c1, c2, r2);
        Decode(mout, mprime);

        for(i = 0; i < N; i++)
          assert(mout[i] == m[i]);

		MAP_PCM_gotoLPM0();

    }

}
