#include <stdio.h>
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned long int u32;
typedef signed long int s32;
typedef float f32;
typedef double D64;
typedef long double D96;

int main()
{
while (1){

int col, row;
printf("\nEnter the number of col: ");
scanf ("%d", &col);
printf("\nEnter the number of row: ");
scanf ("%d", &row);

temp (col, row);

}

    return 0;
}


void temp (int col, int row)
{


  for (int i=0; i<row; i++)
  {
      for (int j=0; j<col; j++)
      {
          printf("*");
      }

      printf("\n");
  }
}


