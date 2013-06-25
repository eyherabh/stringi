/* This file is part of the 'stringi' library.
 * 
 * Copyright 2013 Marek Gagolewski, Bartek Tartanus, Marcin Bujarski
 * 
 * 'stringi' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * 'stringi' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with 'stringi'. If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef __container_utf16_h
#define __container_utf16_h





/**
 * A class to handle conversion between R character vectors and UTF-16 string vectors
 * @version 0.1 (Marek Gagolewski)
 * @version 0.2 (Marek Gagolewski) - lastMatcher cache, supports auto-vectorization
 * @version 0.3 (Marek Gagolewski) - improved ASCII performance (seperate ucnv)
 * @version 0.4 (Marek Gagolewski) - now NAs are marked as NULLs in str
 */
class StriContainerUTF16 : public StriContainerBase {
   
   protected:
      
      UnicodeString** str;       ///< data - \code{UnicodeString}s 


   public:
      
      StriContainerUTF16();
      StriContainerUTF16(R_len_t nrecycle);
      StriContainerUTF16(SEXP rstr, R_len_t nrecycle, bool shallowrecycle=true);
      StriContainerUTF16(StriContainerUTF16& container);
      ~StriContainerUTF16();
      StriContainerUTF16& operator=(StriContainerUTF16& container);
      SEXP toR(R_len_t i) const;
      SEXP toR() const;
      
      
      /** check if the vectorized ith element is NA
       * @param i index
       * @return true if is NA
       */
      inline bool isNA(R_len_t i) const {
#ifndef NDEBUG
         if (i < 0 || i >= nrecycle)
            error("StriContainerUTF16::isNA(): INDEX OUT OF BOUNDS"); // TO DO: throw StriException
#endif
         return (str[i%n] == NULL);
      }
      
      
      /** get the vectorized ith element
       * @param i index
       * @return string
       */
      const UnicodeString& get(R_len_t i) const {
#ifndef NDEBUG
         if (i < 0 || i >= nrecycle)
            error("StriContainerUTF16::get(): INDEX OUT OF BOUNDS"); // TO DO: throw StriException
         if (str[i%n] == NULL)
            error("StriContainerUTF16::get(): isNA"); // TO DO: throw StriException
#endif
         return (*(str[i%n]));
      }
      
      /** get the vectorized ith element
       * @param i index
       * @return string
       */
      UnicodeString& getWritable(R_len_t i) {
#ifndef NDEBUG
         if (isShallow)              
            error("StriContainerUTF16::getWritable(): shallow StriContainerUTF16"); // TO DO: throw StriException
         if (n != nrecycle)          
            error("StriContainerUTF16::getWritable(): n!=nrecycle"); // TO DO: throw StriException
         if (i < 0 || i >= n)        
            error("StriContainerUTF16::getWritable(): INDEX OUT OF BOUNDS"); // TO DO: throw StriException
         if (str[i%n] == NULL) 
            error("StriContainerUTF16::getWritable(): isNA"); // TO DO: throw StriException
#endif
         return (*(str[i%n])); // in fact, "%n" is not necessary
      }
      
      /** set the vectorized ith element
       * @param i index
       * @param s string to be copied
       */
      void set(R_len_t i, const UnicodeString& s) {
#ifndef NDEBUG
         if (isShallow)              
            error("StriContainerUTF16::set(): shallow StriContainerUTF16"); // TO DO: throw StriException
         if (n != nrecycle)          
            error("StriContainerUTF16::set(): n!=nrecycle"); // TO DO: throw StriException
         if (i < 0 || i >= n)        
            error("StriContainerUTF16::set(): INDEX OUT OF BOUNDS"); // TO DO: throw StriException
         if (str[i%n] == NULL) 
            error("StriContainerUTF16::set(): isNA"); // TO DO: throw StriException
#endif
         *(str[i%n]) = s; // in fact, "%n" is not necessary
      }
      
      void UChar16_to_UChar32_index(R_len_t i, int* i1, int* i2, const int ni, int adj1, int adj2);
};

#endif