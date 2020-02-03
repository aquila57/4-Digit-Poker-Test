/* lfsrpokr.c - Poker Test, LFSR generator  Version 0.1.0 */
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

/********************************************************/
/* The LFSR in this generator comes from the following  */
/* http://courses.cse.tamu.edu/walker/csce680/          */
/* lfsr_table.pdf                                       */
/* 64 bit LFSR is 64,63,61,60 with low order bit equal  */
/* to 64                                                */
/********************************************************/

/* This program runs a poker test on a plain            */
/* 64 bit LFSR random number generator.                 */
/* The period length of the LFSR is longer than the     */
/* number of samples.                                   */
/* The expected result from this test is a P-value      */
/* of 1.0.                                              */

#define MYLFSROUT (out = (((lfsr >> 4) \
   ^ (lfsr >> 3) \
   ^ (lfsr >> 1) \
   ^ (lfsr >> 0)) & 1))

#define MYLFSRLOWBIT (lowbit = major & 1)

#define MYLFSRROLL (lfsr0 = major = \
(major >> 1) | (out << 31))

#define MYLFSRCARRY (lfsr = minor = \
(minor >> 1) | (lowbit << 31))

#define MYLFSR (MYLFSROUT,MYLFSRLOWBIT,MYLFSRROLL,MYLFSRCARRY)


int main(void)
   {
   int i;
   int *kountp;
   int *kountq;
   int kount[16];
   unsigned int lfsr0;
   unsigned int major;
   unsigned int lfsr;
   unsigned int minor;
   unsigned int lowbit;
   unsigned int out;
   double modulus;
   double dblsz;
   double tottally;
   double *p,*q;
   double tally[16];
   eefmt *ee;
   ee = (eefmt *) eeglinit();
   lfsr0 = major = eegl(ee);
   lfsr  = minor = eegl(ee);
   free(ee->state);
   free(ee);
   p = (double *) tally;
   q = (double *) tally + 16;
   while (p < q) *p++ = 0.0;
   dblsz = (double) SIZE;
   modulus = 65536.0 * 65536.0;
   i = SIZE;
   while (i--)
      {
      int maxkount;
      int pairkount;
      double dbllfsr;
      double num;
      char *p,*q;
      char str[32];
      MYLFSR;
      dbllfsr = (double) lfsr;
      num = dbllfsr / modulus;
      sprintf(str,"%6.4f", num);
      /* sort str in descending sequence */
      p = (char *) str + 2;
      q = (char *) str + 6;
      while (p < q)
         {
         if (*p >= *(p+1))
            {
            p++;
            continue;
            } /* if in descending sequence */
         else
            {
            char tmp;
            tmp = *p;
            *p = *(p+1);
            *(p+1) = tmp;
            p = (char *) str+2;
            } /* else not in descending sequence */
         } /* sort loop */
      /* tally number of occurrences of each digit */
      kountp = (int *) kount;
      kountq = (int *) kount + 16;
      while (kountp < kountq) *kountp++ = 0;
      p = (char *) str + 2;
      q = (char *) str + 6;
      while (p < q)
         {
	 int dgt;
	 dgt = *p++ - '0';
	 kount[dgt]++;
	 } /* for each digit */
      /* all 5 cases are based on maximum count of digits */
      /* and number of pairs */
      pairkount = maxkount = 0;
      kountp = (int *) kount;
      kountq = (int *) kount + 10;
      while (kountp < kountq)
         {
	 if (*kountp > maxkount) maxkount = *kountp;
	 if (*kountp == 2) pairkount++;
	 kountp++;
	 } /* for all digits */
      /* case 0 */
      if (maxkount == 1) tally[0] += 1.0;
      /* case 1 */
      else if (maxkount == 3) tally[1] += 1.0;
      /* case 2 */
      else if (maxkount == 4) tally[2] += 1.0;
      /* case 3 */
      if (pairkount == 1) tally[3] += 1.0;
      /* case 4 */
      else if (pairkount == 2) tally[4] += 1.0;
      } /* for each uniform number */
   tottally = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   while (p < q) tottally += *p++;
   if (tottally != dblsz)
      {
      fprintf(stderr,"main: total tally %.0f "
         "not equal to sample size %.0f\n",
	 tottally, dblsz);
      exit(1);
      } /* logic error */
   printf("\t\tPoker Test of 4 Digits\n");
   printf("\t\t    LFSR Generator\n");
   printf("\n");
   calcchi(tally, dblsz);
   lfsr = lfsr0;
   return(0);
   } /* main */
