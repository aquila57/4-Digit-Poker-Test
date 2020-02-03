/* diepoker.c - Poker Test, dieharder generators  Version 0.1.0 */
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

void putstx(char *pgm)
   {
   fprintf(stderr,"Autocorrelation test, dieharder "
      "random number generators\n");
   fprintf(stderr,"Usage: %s generator\n", pgm);
   fprintf(stderr,"Where generator is the dieharder ");
   fprintf(stderr,"random number generator\n");
   fprintf(stderr,"Example: %s 053\n", pgm);
   fprintf(stderr,"053 is the id for the taus2 generator\n");
   fprintf(stderr,"To see a list of dieharder generators, type:\n");
   fprintf(stderr,"dieharder -g -l\n");
   exit(1);
   } /* putstx */

int main(int argc, char **argv)
   {
   int i;
   int *kountp;
   int *kountq;
   int kount[16];
   double dblsz;
   double tottally;
   double *p,*q;
   double tally[16];
   xxfmt *xx;
   /*************************************************************/
   /* Allocate memory for the global structure.                 */
   /* This is a re-entrant program.                             */
   /*************************************************************/
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of memory */
   /*************************************************************/
   /* initialize global data                                    */
   /*************************************************************/
   xx->dieharder_rngname[0] = '\0';
   xx->rngname = (char *) NULL;
   xx->generator = -1;
   /*************************************************************/
   /* obtain dieharder generator number parameter               */
   /*************************************************************/
   if (argc != 2) putstx(*argv);
   xx->generator = atoi(*(argv+1));
   if (xx->generator < 0 || xx->generator > 405)
      {
      fprintf(stderr,"main: invalid random "
         "number generator %s\n", *(argv+1));
      putstx(*argv);
      } /* invalid generator */
   /*************************************************************/
   /* initialize numeric fields                                 */
   /*************************************************************/
   bld_maxint(xx);  /* build maxint table for dieharder RNGs */
   bld_rngtbl(xx);  /* build name table for dieharder RNGs */
   if (xx->maxint_tbl[xx->generator] == 0.0)
      {
      fprintf(stderr,"main: invalid random "
         "number generator %s\n", *(argv+1));
      putstx(*argv);
      } /* invalid generator */
   xx->modulus = xx->maxint_tbl[xx->generator];
   xx->maxint  = (unsigned int) xx->modulus;
   diepfx(xx);
   /*************************************************************/
   p = (double *) tally;
   q = (double *) tally + 16;
   while (p < q) *p++ = 0.0;
   dblsz = (double) SIZE;
   i = SIZE;
   while (i--)
      {
      int maxkount;
      int pairkount;
      double num;
      char *p,*q;
      char str[32];
      num = getdie(xx);
      if (xx->eofsw)
         {
	 fprintf(stderr,"main: end of input file "
	    "from dieharder\n");
	 exit(1);
	 } /* end of file */
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
   printf("\t\t  Poker Test of 4 Digits\n");
   printf("\t\tdieharder Generator: %s\n",
      xx->dieharder_rngname);
   printf("\n");
   calcchi(tally, dblsz);
   return(0);
   } /* main */
