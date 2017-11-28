#include <stdio.h>
#include <math.h>
int main(void)
{
int blocks;
double amount, start_coins = 10000.0, rate = 0.0000005975
;
 
printf("Start Coins = 10000.00; rate p/a = 0.0000005975\n");
printf("How many ROI coins will stake in X blocks?\n\n");
printf("%4s%21s\n", "Blocks", "Start Coins");
 
for(blocks = 1; blocks <= 262800; blocks++)
{
amount = start_coins * pow(1.0 + rate, blocks);
printf("%4d%21.2f\n", blocks, amount);
}
return 0;
}
