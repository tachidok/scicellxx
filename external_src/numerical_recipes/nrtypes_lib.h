#ifndef _NR_TYPES_H_
#define _NR_TYPES_H_

#include <complex>
#include <fstream>
#include "nrutil.h"
using namespace std;

#ifdef TYPEDEF_REAL_IS_DOUBLE
typedef double DP;
#else
typedef float DP;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE

// Vector Types

typedef const NRVec<bool> Vec_I_BOOL;
typedef const NRVec<bool> Vec_BOOL, Vec_O_BOOL, Vec_IO_BOOL;

typedef const NRVec<char> Vec_I_CHR;
typedef const NRVec<char> Vec_CHR, Vec_O_CHR, Vec_IO_CHR;

typedef const NRVec<unsigned char> Vec_I_UCHR;
typedef const NRVec<unsigned char> Vec_UCHR, Vec_O_UCHR, Vec_IO_UCHR;

typedef const NRVec<int> Vec_I_INT;
typedef const NRVec<int> Vec_INT, Vec_O_INT, Vec_IO_INT;

typedef const NRVec<unsigned int> Vec_I_UINT;
typedef const NRVec<unsigned int> Vec_UINT, Vec_O_UINT, Vec_IO_UINT;

typedef const NRVec<long> Vec_I_LNG;
typedef const NRVec<long> Vec_LNG, Vec_O_LNG, Vec_IO_LNG;

typedef const NRVec<unsigned long> Vec_I_ULNG;
typedef const NRVec<unsigned long> Vec_ULNG, Vec_O_ULNG, Vec_IO_ULNG;

typedef const NRVec<float> Vec_I_SP;
typedef const NRVec<float> Vec_SP, Vec_O_SP, Vec_IO_SP;

typedef const NRVec<DP> Vec_I_DP;
typedef const NRVec<DP> Vec_DP, Vec_O_DP, Vec_IO_DP;

typedef const NRVec<complex<float> > Vec_I_CPLX_SP;
typedef const NRVec<complex<float> > Vec_CPLX_SP, Vec_O_CPLX_SP, Vec_IO_CPLX_SP;

typedef const NRVec<complex<DP> > Vec_I_CPLX_DP;
typedef const NRVec<complex<DP> > Vec_CPLX_DP, Vec_O_CPLX_DP, Vec_IO_CPLX_DP;

// Matrix Types

typedef const NRMat<bool> Mat_I_BOOL;
typedef const NRMat<bool> Mat_BOOL, Mat_O_BOOL, Mat_IO_BOOL;

typedef const NRMat<char> Mat_I_CHR;
typedef const NRMat<char> Mat_CHR, Mat_O_CHR, Mat_IO_CHR;

typedef const NRMat<unsigned char> Mat_I_UCHR;
typedef const NRMat<unsigned char> Mat_UCHR, Mat_O_UCHR, Mat_IO_UCHR;

typedef const NRMat<int> Mat_I_INT;
typedef const NRMat<int> Mat_INT, Mat_O_INT, Mat_IO_INT;

typedef const NRMat<unsigned int> Mat_I_UINT;
typedef const NRMat<unsigned int> Mat_UINT, Mat_O_UINT, Mat_IO_UINT;

typedef const NRMat<long> Mat_I_LNG;
typedef const NRMat<long> Mat_LNG, Mat_O_LNG, Mat_IO_LNG;

typedef const NRMat<unsigned long> Mat_I_ULNG;
typedef const NRMat<unsigned long> Mat_ULNG, Mat_O_ULNG, Mat_IO_ULNG;

typedef const NRMat<float> Mat_I_SP;
typedef const NRMat<float> Mat_SP, Mat_O_SP, Mat_IO_SP;

typedef const NRMat<DP> Mat_I_DP;
typedef const NRMat<DP> Mat_DP, Mat_O_DP, Mat_IO_DP;

typedef const NRMat<complex<float> > Mat_I_CPLX_SP;
typedef const NRMat<complex<float> > Mat_CPLX_SP, Mat_O_CPLX_SP, Mat_IO_CPLX_SP;

typedef const NRMat<complex<DP> > Mat_I_CPLX_DP;
typedef const NRMat<complex<DP> > Mat_CPLX_DP, Mat_O_CPLX_DP, Mat_IO_CPLX_DP;

// 3D Matrix Types

typedef const NRMat3d<DP> Mat3D_I_DP;
typedef NRMat3d<DP> Mat3D_DP, Mat3D_O_DP, Mat3D_IO_DP;

// Miscellaneous Types

typedef NRVec<unsigned long *> Vec_ULNG_p;
typedef const NRVec<const NRMat<DP> *> Vec_Mat_DP_p;
typedef NRVec<fstream *> Vec_FSTREAM_p;

#endif /* _NR_TYPES_H_ */
