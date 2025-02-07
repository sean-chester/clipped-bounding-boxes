/* CDuni: grep Param. */
#//
#// Copyright (c) 1994 - 2013 Norbert Beckmann  <nb@informatik.uni-bremen.de>
#//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
//-----
#include "RSTStdTypes.h"
/* RSTStdTypes.h mainly provides R*-tree standard constants, types and
   functions, normally not needed here.
   But type Rpint is used to provide a pointer compatible integer type on
   platforms, not serving a 32/64 bit transitional long integer and concerning
   I/O functions (Windows).
   Note that if a source includes RSTStdTypes.h, linking requires libPltfHelp
   or libUTIL. */
#include "drand48.h"
/* for drand48 on platforms not providing it (Windows); linking: see above. */


/* constants */

#define RANDSEED 7873 /* <--- set Param. */

/* define NumbOfDim in Makefile !! *//* <--- set Param. */


/* types */

typedef double typatom;       /* <--- set Param. */

typedef struct {
  typatom c[NumbOfDim], e[NumbOfDim];
  } typcerect;

typedef struct {
               typatom  l, h;
               } typinterval;
typedef typinterval  typrect[NumbOfDim];


/* declarations */

void CreateEven(void);
void Produce(typcerect *rect);
void ce2lh(typcerect *ce, typrect rstrect);


/* global variables */

Rpint rcts, rectCount, writeEach;
double MPstart, MPrange, CellWidth,
       DitherPos, DitherWidth;
double wi[NumbOfDim];
FILE *f;


int main(int argc, char *argv[])

{
  int d;
  double start, WorldWidth, RectWidth, A;
  
  if (argc != 3 || strcmp(argv[1],"-d") != 0 &&
                   strcmp(argv[1],"-da0") != 0 &&
                   strcmp(argv[1],"-da1") != 0 &&
                   strcmp(argv[1],"-da10") != 0 &&
                   strcmp(argv[1],"-da100") != 0 &&
                   strcmp(argv[1],"-da1000") != 0 &&
                   strcmp(argv[1],"-q") != 0 &&
                   strcmp(argv[1],"-qa0") != 0 &&
                   strcmp(argv[1],"-qa1") != 0 &&
                   strcmp(argv[1],"-qa10") != 0 &&
                   strcmp(argv[1],"-qa100") != 0 &&
                   strcmp(argv[1],"-qa1000") != 0) {
    printf("Usage: %s %s\n",argv[0],"( -d | -da0 | -da1 | -da10 | -da100 | -da1000 |");
    printf("%s %s\n","                 -q | -qa0 | -qa1 | -qa10 | -qa100 | -qa1000 )","filename");
  }
  else {
    /* CDuni produces rcts rectangles, uniformly distributed in a
       ----------------------------------------------------------
       (hyper-) square bounded world.
       ------------------------------
       Distribution: randomly uniform
       Order: randomly uniform
       start[d]: origin-corner of the world
       -----------------------------------------------------------------------
       
       The world, consisting of "cells", is a square with edges of length
       WorldWidth.
       A cell, containing exactly one rectangle, is a square with edges of
       length CellWidth, where
       CellWidth = WorldWidth / pow(rcts, 1/NumbOfDim).
       Unit-squares (A = 1, undithered) are completely located inside the
       world. Generally rectangles may stick out by an epsilon increasing by
       position and width dithering.
       
    */
    /* ---------- world specification ---------- */
    start= 0.0;
    WorldWidth= 1.0;
    /* ---------- CDuni --- distribution Param. setting ---------- */
    /*
      - typcerect: see "constants" and "types"
      - rcts: number of rectangles.
      - writeEach: a newly produced rectangle is actually written out
                   iff rectCount % writeEach = 0.
                   rectCount, initially 0, is incremented after each production
                   of a new rectangle.
                   This allows to create part of (actually the same)
                   distribution.
      - A: rectangle area relative to the area of a cell.
      - DitherPos: rectangle position dithering relative to CellWidth.
                   For each dimension d, the center-position c[d] of the
                   rectangle is determined as follows:
                   c[d]= c[d] +|- X * DitherPos * CellWidth;
                   X uniformly distributed in [0, 1).
      - DitherWidth: rectangle width dithering relative to the rectangle width.
                     For each dimension d, the width wi[d] of the rectangle
                     is determined as follows:
                     wi[d]= wi[d] * (1 +|- X * DitherWidth);
                     X uniformly distributed in [0, 1);
                     0 <= DitherWidth <= 1  REQUIRED!
    */
    if (strcmp(argv[1],"-da0") == 0 || strcmp(argv[1],"-d") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 0.0;                     /* <--- set Param. */
      DitherPos= 0.0;             /* <--- set Param. */
      DitherWidth= 0.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-da1") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 1.0;                     /* <--- set Param. */
      DitherPos= 0.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-da10") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 1.0;                     /* <--- set Param. */
      DitherPos= 0.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-da100") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. */
      writeEach= 1;               /* <--- set Param. */
      A= 100.0;                   /* <--- set Param. */
      DitherPos= 0.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-da1000") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. */
      writeEach= 1;               /* <--- set Param. */
      A= 1000.0;                  /* <--- set Param. */
      DitherPos= 0.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-qa0") == 0 || strcmp(argv[1],"-q") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 0.0;                     /* <--- set Param. */
      DitherPos= 1.0;             /* <--- set Param. */
      DitherWidth= 0.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-qa1") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 1.0;                     /* <--- set Param. */
      DitherPos= 1.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-qa10") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. NUMBER OF OBJECTS */
      writeEach= 1;               /* <--- set Param. */
      A= 1.0;                     /* <--- set Param. */
      DitherPos= 1.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-qa100") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. */
      writeEach= 1;               /* <--- set Param. */
      A= 100.0;                   /* <--- set Param. */
      DitherPos= 1.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    else if (strcmp(argv[1],"-qa1000") == 0) { /* <--- set Param. */
      rcts= 1000000;              /* <--- set Param. */
      writeEach= 1;               /* <--- set Param. */
      A= 1000.0;                  /* <--- set Param. */
      DitherPos= 1.0;             /* <--- set Param. */
      DitherWidth= 1.0;           /* <--- set Param. */
    }
    
    /* ------------------------------------------------------------------- */
    
    RectWidth= pow(A, 1.0 / (double)NumbOfDim);
    for (d= 0; d < NumbOfDim; d++) {
      wi[d]= RectWidth;
    }
    
    CellWidth= WorldWidth / pow((double)rcts, 1.0 / NumbOfDim);
    MPstart= start + 0.5 * CellWidth;
    MPrange= WorldWidth - CellWidth;
    
    /* ------------------------------------------------------------------- */
    
    rectCount= 0;
    f= fopen(argv[2],"wb");
    srand48(RANDSEED);
    CreateEven();
    fclose(f);
  }
  return 0;
}

/**********************************************************************/

void CreateEven(void)

{
  int d;
  Rpint i;
  double RealRand;
  typcerect rect;
  
  for (d= 0; d < NumbOfDim; d++) {
    rect.e[d]= wi[d] * 0.5 * CellWidth;
  }
  
  for (i= 1; i <= rcts; i++) {
    for (d= 0; d < NumbOfDim; d++) {
      RealRand= drand48();
      rect.c[d]= MPstart + RealRand * MPrange;
    }
    Produce(&rect);
  }
}

/**********************************************************************/

void Produce(typcerect *rect)

{
  int d;
  double RealRand;
  typcerect ce;
  typrect rstrect;

  for (d= 0; d < NumbOfDim; d++) {
    RealRand= drand48();
    if (mrand48() >= 0) {
      ce.c[d]= (*rect).c[d] + RealRand * DitherPos * CellWidth;
    }
    else {
      ce.c[d]= (*rect).c[d] - RealRand * DitherPos * CellWidth;
    }
    
    RealRand= drand48();
    if (mrand48() >= 0) {
      ce.e[d]= (*rect).e[d] + RealRand * DitherWidth * (*rect).e[d];
    }
    else {
      ce.e[d]= (*rect).e[d] - RealRand * DitherWidth * (*rect).e[d];
    }
  }
  
  rectCount++;
  if (writeEach != 0) {
    if (rectCount % writeEach == 0) {
      ce2lh(&ce,rstrect);
      fwrite(rstrect,sizeof(typrect),1,f);
    }
  }
}

/**********************************************************************/

void ce2lh(typcerect *ce, typrect rstrect)

{
  int d;
  
  for (d= 0; d < NumbOfDim; d++) {
    rstrect[d].l= (*ce).c[d] - (*ce).e[d];
    rstrect[d].h= (*ce).c[d] + (*ce).e[d];
  }
}

/***********************************************************************/
