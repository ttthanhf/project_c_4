#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
void printCalendar (int year)
{
  int mDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  int t[]={0,3,2,5,0,3,5,1,4,6,2,4};
  char* monthList[]={"January","February","March","April","May","June","July","August","September","October","November","December"};
  printf("---------------");
  printf("Calendar - %d",year);
  printf("---------------\n");
  int days;
  int current;

  int y=year-1;
  current=(y+y/4-y/100+y/400+1+t[0])%7;

  for(int i=0;i<12;i++)
  {
    if(i==1)
    {
      if(year%400==0||(year%4==0&&year%100!=0))days=29;
       else days=mDays[i];
    }
    else days=mDays[i];

    printf("\n --------------- %s-----------------\n",monthList[i]);
    printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat \n");
    int k;
    for(k=0;k<current;k++) printf("     ");
    for(int j=1;j<=days;j++)
    {
      k++;
      if(j<10)
      printf("    %d",j);
      else printf("   %d",j);
      if(k>6){k=0;printf("\n");}
    }
    if(k)printf("\n");
    current=k;
  }
   return ;
}
int main()
{
  system("cls");
  int year=2022;
  int a=2;
  do
  {
    system("cls");
    printCalendar(year);
    printf("\n\n");
    printf("continue?");
    int option;
    char choose;
    choose=_getwche();
   if(choose=='n')return 0;
    printf("\n input the year:");
    scanf("%d",&year);

  }while(a>0);
  return 0;
}
