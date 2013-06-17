/* This file is part of the 'stringi' library.
 * 
 * Copyright 2013 Marek Gagolewski, Bartek Tartanus, Marcin Bujarski
 * 
 * 'stringi' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lestr_conter General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * 'stringi' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNEstr_cont FOR A PARTICULAR PURPOSE. See the
 * GNU Lestr_conter General Public License for more details.
 * 
 * You should have received a copy of the GNU Lestr_conter General Public License
 * along with 'stringi'. If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "stringi.h"



/** 
 * Count the number of recurrences of \code{pattern} in \code{s}
 * @param str strings to search in
 * @param pattern regex patterns to search for
 * @return integer vector
 * @version 0.1 (Bartek Tartanus)
 * @version 0.2 (Marek Gagolewski) - use StriContainerUTF16's vectorization
 * @version 0.3 (Marek Gagolewski, 2013-06-16) make StriException-friendly
 */
SEXP stri_count_regex(SEXP str, SEXP pattern)
{
   str = stri_prepare_arg_string(str, "str");
   pattern = stri_prepare_arg_string(pattern, "pattern");
   R_len_t vectorization_length = stri__recycling_rule(true, 2, LENGTH(str), LENGTH(pattern));
   // this will work for vectorization_length == 0:
   
   STRI__ERROR_HANDLER_BEGIN
   
   StriContainerUTF16 str_cont(str, vectorization_length); 
   // MG: tried StriContainerUTF8 + utext_openUTF8 - this was slower
   StriContainerUTF16 pattern_cont(pattern, vectorization_length);
 
   SEXP ret;
   PROTECT(ret = allocVector(INTSXP, vectorization_length));
   
   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      STRI__CONTINUE_ON_EMPTY_OR_NA_STR_PATTERN((&str_cont), (&pattern_cont), INTEGER(ret)[i] = NA_INTEGER, INTEGER(ret)[i] = 0)
      
      RegexMatcher *matcher = pattern_cont.vectorize_getMatcher(i); // will be deleted automatically
      matcher->reset(str_cont.get(i));
      int count = 0;
      bool found = (bool)matcher->find();
      while (found) {
         ++count;
         found = (bool)matcher->find();
      }
      INTEGER(ret)[i] = count;
   }

   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}
