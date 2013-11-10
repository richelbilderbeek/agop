/* ************************************************************************* *
 *   This file is part of the `agop` library.                                *
 *                                                                           *
 *   Copyright 2013 Marek Gagolewski, Anna Cena                              *                                                                    *
 *                                                                           *
 *   'agop' is free software: you can redistribute it and/or modify          *
 *   it under the terms of the GNU Lesser General Public License             *
 *   as published by the Free Software Foundation, either version 3          *
 *   of the License, or (at your option) any later version.                  *
 *                                                                           *
 *   'agop' is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the             *
 *   GNU Lesser General Public License for more details.                     *
 *                                                                           *
 *   You should have received a copy of the GNU Lesser General Public        *
 *   License along with 'agop'. If not, see <http://www.gnu.org/licenses/>.  *
 * ************************************************************************* */



#include "agop.h"


/** Check if a binary relation is transitive
 * 
 * @param x square logical matrix
 * @return logical scalar
 * 
 * @version 0.2 (Marek Gagolewski)
 */
SEXP rel_is_transitive(SEXP x)
{
   x = prepare_arg_logical_square_matrix(x, "R");
   SEXP dim = Rf_getAttrib(x, R_DimSymbol);
   R_len_t n = INTEGER(dim)[0];
   int* xp = INTEGER(x);
   for (R_len_t i=0; i<n; ++i) {
      for (R_len_t j=0; j<n; ++j) {
         if (i == j) continue; // don't care
         if (xp[i+j*n] == NA_LOGICAL)
            return Rf_ScalarLogical(NA_LOGICAL); // this could be done better
         if (!xp[i+j*n]) continue; // nothing more to check
         for (R_len_t k=0; k<n; ++k) {
            if (xp[i+k*n] == NA_LOGICAL || xp[j+k*n] == NA_LOGICAL)
               return Rf_ScalarLogical(NA_LOGICAL); // this could be done better
            if (xp[j+k*n] && !xp[i+k*n])
               return Rf_ScalarLogical(FALSE);
         }
      }
   }
   return Rf_ScalarLogical(TRUE);
}


/** Get the transitive closure of a binary relation
 * 
 * @param x square logical matrix
 * @return square logical matrix
 * 
 * @version 0.2 (Marek Gagolewski)
 */
SEXP rel_closure_transitive(SEXP x)
{
   x = prepare_arg_logical_square_matrix(x, "R");
   SEXP dim = Rf_getAttrib(x, R_DimSymbol);
   R_len_t n = INTEGER(dim)[0];
   int* xp = INTEGER(x);

   SEXP y = Rf_allocVector(LGLSXP, n*n);
   int* yp = INTEGER(y);
   Rf_setAttrib(y, R_DimSymbol, dim);
   Rf_setAttrib(y, R_DimNamesSymbol, Rf_getAttrib(x, R_DimNamesSymbol)); // preserve dimnames

   for (R_len_t i=0; i<n*n; ++i) {
      if (xp[i] == NA_LOGICAL)
         Rf_error(MSG__ARG_EXPECTED_NOT_NA, "R"); // missing values are not allowed
      yp[i] = xp[i];
   }
   
   for (R_len_t k=0; k<n; ++k) { // Warshall's algorithm
      for (R_len_t i=0; i<n; ++i) {
         for (R_len_t j=0; j<n; ++j) {
            yp[i+n*j] = (yp[i+n*j] || (yp[i+n*k] && yp[k+n*j]));
         }
      }
   }
   
   return y;
}





/** Get the transitive reduction of a binary relation
 * 
 * @param x square logical matrix
 * @return square logical matrix
 * 
 * @version 0.2 (Marek Gagolewski)
 */
SEXP rel_reduction_transitive(SEXP x)
{
   SEXP y = rel_closure_transitive(x);
   // is logical matrix, dimnames are preserved, we may overwrite its elements
   
   Rf_error("TO DO");
   
   return y;
}
