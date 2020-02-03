/* poker.c - Chi Square Calculation Version 0.1.0 */
/* Copyright (C) 2020 aquila57 at github.com */

/* This program is free software; you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License as    */
/* published by the Free Software Foundation; either version 2 of    */
/* the License, or (at your option) any later version.               */

/* This program is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/* GNU General Public License for more details.                      */

/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to:                        */

   /* Free Software Foundation, Inc.                                 */
   /* 59 Temple Place - Suite 330                                    */
   /* Boston, MA 02111-1307, USA.                                    */

#include "poker.h"

void calcchi(double *tally, double N)
   {
   int kase;
   double chisq;
   double df;
   double minchi;
   double maxchi;
   double pvalue;
   double *p,*q;
   double expected[8];
   printf("  Case             Actual          "
      "Expected        Difference\n");
   /******************************************************/
   /* The following 5 probability cases come from        */
   /* http://code.nepalijivan.com.np/p/                  */
   /* pokers-test-for-independence-of-random.html        */
   /******************************************************/
   p = (double *) expected;
   q = (double *) expected + 8;
   while (p < q) *p++ = 0.0;
   p = (double *) expected;
   *p++ = 0.504 * N;              /* case 0 */
   *p++ = 0.036 * N;              /* case 1 */
   *p++ = 0.001 * N;              /* case 2 */
   *p++ = 0.432 * N;              /* case 3 */
   *p++ = 0.027 * N;              /* case 4 */
   /******************************************************/
   chisq = 0.0;
   for (kase=0; kase < 5; kase++)
      {
      double diff;
      double diffsq;
      diff = tally[kase] - expected[kase];
      switch(kase)
         {
	 case 0:
	    printf("All different");
	    break;
	 case 1:
	    printf("3 of a kind  ");
	    break;
	 case 2:
	    printf("4 of a kind  ");
	    break;
	 case 3:
	    printf("1 pair       ");
	    break;
	 case 4:
	    printf("2 pairs      ");
	    break;
	 default:
	    fprintf(stderr,"Invalid kase %d\n", kase);
	    exit(1);
	 } /* switch kase */
      printf("%12.0f %18.5f %16.5f\n",
         tally[kase], expected[kase], diff);
      diffsq = diff * diff;
      chisq += diffsq / expected[kase];
      } /* for each case */
   printf("\n");
   printf("Sample size is %d\n", SIZE);
   df = (double) (CASES - 1);
   minchi = gsl_cdf_chisq_Pinv(0.025,df);
   maxchi = gsl_cdf_chisq_Pinv(0.975,df);
   pvalue = gsl_cdf_chisq_P(chisq,df);
   printf("Chi square = %f  P-value %f\n", chisq, pvalue);
   printf("\n");
   printf("at 95%c probability and %.0f degrees of freedom,\n"
      "Chi square ranges from %f to %f\n",
      '%', df, minchi, maxchi);
   } /* calcchi */
