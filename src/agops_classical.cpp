/* ************************************************************************* *
 *   This file is part of the `agop` library.                                *
 *                                                                           *
 *   Copyright 2013 Marek Gagolewski, Anna Cena                              *
 *                                                                           *
 *   Parts of the code are taken from the 'CITAN' R package by M. Gagolewski *                                                                       *
 *                                                                           *
 *   'agop' is free software: you can redistribute it and/or modify          *
 *   it under the terms of the GNU Lesser General Public License             *
 *   as published by the Free Software Foundation, either version 3          *
 *   of the License, or (at your option) any later version.                  *
 *                                                                           *
 *   'agop' is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 *   GNU Lesser General Public License for more details.                     *
 *                                                                           *
 *   You should have received a copy of the GNU Lesser General Public        *
 *   License along with 'agop'. If not, see <http://www.gnu.org/licenses/>.  *
 * ************************************************************************* */



#include "agop.h"





/** OWA operator
 * 
 * @param x numeric
 * @param w numeric
 * @return numeric of length 1
 */
SEXP owa(SEXP x, SEXP w)
{
   x = prepare_arg_numeric_sorted(x, "x");
   return wam(x, w);
}




/** WAM operator
 * 
 * @param x numeric
 * @param w numeric
 * @return numeric of length 1
 */
SEXP wam(SEXP x, SEXP w)
{
   x = prepare_arg_numeric(x, "x");
   w = prepare_arg_numeric(w, "w");
   
   R_len_t x_length = LENGTH(x);
   R_len_t w_length = LENGTH(w);
   double* w_tab = REAL(w);
   double* x_tab = REAL(x);
   
   if (x_length <= 0) return x;
   if (R_IsNA(w_tab[0]) || R_IsNA(x_tab[0]))
      return ScalarReal(NA_REAL);
   if (x_length != w_length)
      error("`x` and `w` should have the same length");
   

   double w_sum = 0.0;
   double ret_val = 0.0;
   for (R_len_t i=0; i<x_length; ++i) {
      if (w_tab[i] < 0)
         error("elements in `w` should be nonnegative");
      w_sum = w_sum + w_tab[i];
      ret_val += w_tab[i]*x_tab[i];
   }
   
   if (w_sum > 1.0+EPS || w_sum < 1.0-EPS)
      warning("elements of `w` does not sum up to 1. correcting.");
      
   ret_val /= w_sum;
   return ScalarReal(ret_val);
}



/** Function to compute the S-statistic for kappa=id, O(log n) time.
 *  @param x vector of numbers, 0<=x[i]<=1, sorted non-increasingly
 *  @param n pointer to the number of observations
 *  @param out one-dimensional array which stores the result
 */
void Sstat2(double* x, int* n, double* out)
{
	double N = (double)(*n);

	int h1 = 0;
	int h2 = (*n)-1;
	int m;
	double xmulN;
	double mp1;

	if (x[0] < 1.0/N) { *out = x[0]; return; }

	while (1)
	{
		m = (h2+h1+1)/2;
		mp1 = (double)(m+1);
		xmulN = N*x[m];
		if (xmulN == mp1 || h1 == h2) {break;}
		if (xmulN < mp1) h2 = m-1;
		else h1 = m;
	}

#ifdef CITAN_DEBUG
	if (!(m+1 <= *n && m+1>=0)) fprintf(stderr, "CITAN_DEBUG: Sstat2: !(m+1 <= *n && m+1>=0)\n");
	if (m>=0 && x[m]<mp1/N) fprintf(stderr, "CITAN_DEBUG: Sstat2: m>=0 && x[m]<mp1/N\n");
#endif

	if (m+1 < *n)
	{
		if (mp1 > N*x[m+1]) *out = mp1/N;
		else                *out = x[m+1];
	}
	else
	{
		*out = mp1/N;
	}
}



/*
void Sstat2(double* x, int* n, double* out) -- OFTEN SLOWER THAN THE ABOVE
{
	int i = 0;
	int k = *n;
	double d = 1.0/(double)k;

	while (i<k)
	{
		if (x[i] < d)
		{
			if (x[i] >= (double)i/(double)k)
				*out = x[i];
			else
				*out = (double)i/(double)k;
			return;
		}
		++i;
		d = (double)(i+1)/(double)k;
	}

	// i == k
	*out = x[k-1];
}
*/








