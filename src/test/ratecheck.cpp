#include <stdio.h>
#include <math.h>
#include "bignum.h"

int main(int argc, char* argv[])
{
  int blocks;
  double amount, start_coins = 10000.0, rate = 0.0000005975;

   printf("%d\n",argc);

   if(argc < 3) 
   { 
      printf("Please provide principal and per block rate\n"); 
      exit(1); 
   }

   start_coins = atof(argv[1]);
   rate = atof(argv[2]);

   printf("Start Coins = %21.2f; rate p/b = %21.2f \n",start_coins,rate);
   printf("How many ROI coins will stake in X blocks?\n\n");
   printf("%4s%21s\n", "Blocks", "Start Coins");
 
   for(blocks = 1; blocks <= 262800; blocks++)
   {
     amount = start_coins * pow(1.0 + rate, blocks);
     printf("%4d%21.2f\n", blocks, amount);
   }
   return 0;
}
