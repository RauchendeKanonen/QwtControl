/*
 *******************************************************************************
 *
 *
 *                       Copyright (c) 2002
 *                       Future Team Aps 
 *                       Denmark
 *
 *                       All Rights Reserved
 *
 *   This source file is subject to the terms and conditions of the
 *   Future Team Software License Agreement which restricts the manner
 *   in which it may be used.
 *
 *
 *******************************************************************************
*/

/*
 *******************************************************************************
 *
 *
 * Module name     :   newr.cpp
 * Module ID Nbr   :   
 * Description     :   Solve n degree polynominal using Newton's (Madsen) methode
 * --------------------------------------------------------------------------
 * Change Record   :   
 *
 * Version	Author/Date		Description of changes
 * -------  -----------		----------------------
 * 01.01		HVE/021018		Initial release
 *
 * End of Change Record
 * --------------------------------------------------------------------------
*/

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <complex>

using namespace std;

#define MAXITER 50
#define DBL_RADIX       2
int newton_real( register int n, const double coeff[], complex<double> res[] );

