/* poker.c - Poker Test, eegl64 generator  Version 0.1.0 */
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

int main(void)
   {
   int i;
   int *kountp;
   int *kountq;
   int kount[16];
   gsl_rng *rng;
   gsl_rng_type *typ;
   unsigned int dttk;          /* combined date and #ticks */
   time_t now;                 /* current date and time */
   clock_t clk;                /* current number of ticks */
   struct tms t;               /* structure used by times() */
   double dblsz;
   double tottally;
   double *p,*q;
   double tally[16];
   /***************************************************/
   /* Initialize the seed to date/time/ticks          */
   /***************************************************/
   /* get clock ticks since boot                       */
   clk = times(&t);
   /* get date & time                                  */
   time(&now);
   /* combine date, time, and ticks into a single UINT */
   dttk = (unsigned int) (now ^ clk);
   /****************************************************/
   /* Initialize the random number generator           */
   /* in the GNU Scientific Library                    */
   /****************************************************/
   gsl_rng_env_setup();
   typ = (gsl_rng_type *) gsl_rng_default;
   rng = (gsl_rng *) gsl_rng_alloc(typ);
   /* allocate GSL random number generator to set initial */
   /* values in the three fibonacci numbers               */
   gsl_rng_set(rng, dttk);
   /******************************************************************/
   p = (double *) tally;
   q = (double *) tally + 16;
   while (p < q) *p++ = 0.0;
   dblsz = (double) SIZE;
   i = SIZE;
   while (i--)   /* for each hand */
      {
      int maxkount;
      int pairkount;
      double num;
      char *p,*q;
      char str[32];
      num = gsl_rng_uniform(rng);
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
      } /* for each hand */
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
   printf("\t      GNU Scientific Library:"
      " %s\n", gsl_rng_name(rng));
   printf("\n");
   gsl_rng_free(rng);
   calcchi(tally, dblsz);
   return(0);
   } /* main */
