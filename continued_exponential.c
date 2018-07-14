#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include "bmpfile.c"

#define height 100.0
#define width 100.0
#define iHeight 800
#define iWidth 800
#define epsilon 0.000001
#define lSize 45


/** This array maps the number of limit point to a specific
  * RGB color to be printed in the final image. It is necessary
  * for the size of this array to accomodate lSize
  ******/
rgb_pixel_t limitColors[] = { {0,0,0},
                              {102,0,0},{130,0,0},{160,0,0},{190,0,0},{220,0,0},
							  {255,0,0},{255,51,51},{255,80,80},{255,120,120},{255,150,150},
							  {255,180,180},{255,210,210},{255,204,229},{255,153,204},{255,102,178},
							  {255,80,168},{255,51,153},{255,25,140},{255,0,127},{230,0,115},
							  {204,0,102},{153,0,76},{102,0,51},{102,0,80},{102,0,110},
							  {120,0,120},{150,0,150},{180,0,180},{210,0,210},{230,0,230},
							  {253,0,253},{255,30,255},{255,60,255},{255,90,255},{255,120,255},
							  {255,150,255},{255,180,255},{255,200,230},{255,215,215},{255,230,230},
							  {178,102,255},{153,51,255},{127,0,255},{102,0,204},{76,0,140}
							};



/*
{ {0,0,0},
                              {231,47,39},{238,111,25},{255,255,115},{170,198,27},
							  {19,166,50},{4,148,87}, {1,134,141},{3,86,155},
							  {46,20,141},{204,63,92},{172,36,48},{169,87,49},
							  {255,200,8},{91,132,47},{20,114,48},{23,106,43},
							  {20,88,60},{8,87,107},{58,55,119},{111,61,56},					  
							  };
*/


/*
                            { {0,0,0}, //black
                            {64,64,64}, {112,112,112}, {176,176,176}, //grays
						    {180,200,225}, {141,182,205}, {96,123,139},
						    {65,105,225}, {58,95,205}, {0,34,102}, // blues
						    {71,60,139}, {131,111,255}, {160,135,250},
						    {139,105,125}, {255,193,193}, {255,64,64} // pink
						};
*/			 

int countLimits(double complex z, double complex initialValue){
  double complex a = initialValue;
  double complex aList[lSize];
  int i, j, k;
  
  
  i = 300;
  while(i--){
    a = cexp(z*a);
	if(cabs(a) > 1.0e30)
	  return 0;
  }
  
  i = lSize;
  while(i--){
    a = cexp(z*a);
	aList[i] = a;
  }
  
  /** Here I use the variable i to count the number of unique elements in aList
    * by making sure that a unique element is epsilon-far from elements which have 
	* yet to be considered. The number of unique elements describes the number of
	* limit points for the continue function at z.
	***/
  i = 0;
  for(j = 0; j < lSize; j++){
    for(k = j+1; k < lSize && cabs(aList[k] - aList[j]) > epsilon*cabs(aList[j]); k++);
	if(k == lSize) i++; /* Element j has disagreed with all elements k > j */
  }
  
  return i;
}

int main(int argc, char** argv){
  double complex z, a;
  int x, y, count;
  bmpfile_t *bmp;
  
  if((bmp = bmp_create(iWidth, iHeight, 32)) == 0){
    printf("Invalid depth value!\n");
	return 1;
  }
  
  if(argc > 2){
    a = atof(argv[2]);
  }
  else{
    a = 1.0;
  }
  
  for(y = 0; y < iHeight; y++){
	printf("Completed %d/%d lines.\n", y, iHeight);
    for(x = 0; x < iWidth; x++){
	  z = -width/2 + x*width/iWidth + (height/2 - y*(height/iHeight))*I;
      count = countLimits(z,a);
      //printf("%f+I%f => %d\n", creal(z), cimag(z), count);
	  //putpixel(x, y, limitColors[countLimits(z)]);
	  bmp_set_pixel(bmp, x, y, limitColors[count]);
	}
  }
  
  if(argc > 1){
    bmp_save(bmp, argv[1]);
	bmp_destroy(bmp);
  }
  else{
    bmp_save(bmp, "out.bmp");
    bmp_destroy(bmp);
	}
  
  return 0;
}