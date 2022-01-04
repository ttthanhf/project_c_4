#include <stdio.h>      
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

// lập ra danh sách các tháng trong năm. lí do mình k chèn thêm phần tử nào đó khác lên đầu cho nó khớp tháng thứ i ở vị trí thứ i thì xíu kéo xuống dưới là hiểu//
char* monthList[]={"January","February","March","April","May","June","July","August","September","October","November","December"};

// danh sách các tháng rút gọn để khi liệt kê thì chúng cách đều nhau vì tên các tháng độ dài như nhau// 
char* shortmonthList[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

// danh sách số ngày của mỗi tháng trong năm//
int mDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};

// struct tm này là struct có sẵn trong C nha, nó chứa tất cả các biến chứa thời gian , đặc biệt là ngày tháng năm. mik đặt tên là timeinfo
struct tm * timeinfo; 
/* timeinfo->tm_year: đây là năm. à mà cái năm nếu ở hiện tại thì nó bị trừ đi 1900. ví dụ năm 2022 là năm hiện tại thì nó chỉ có 122 thôi.còn cách để lưu được năm hiện tại thì xuống int main() để xem nha
   timeinfo->tm_mon : đây là tháng. Lưu ý là nó có range từ 0 tới 11, tức là 0 là tháng 1 , còn 5 là tháng 6 á...nè đây là lí do mà cái danh sách tháng ở trên mik k chèn thêm.giữ v cho chúng khớp vs nhau.
   timeinfo->tm_mday: đây là số ngày trong tháng.
   timeinfo->tm_wday: đây là số ngày đếm từ ngày chủ nhật. cái biến này giúp mình khớp cái thứ với ngày với nhau nè.nếu bình thường thì mình hay dùng (y+y/4-y/100+y/400+1+t[])%7;
   
*/

int stop=1;

//biến trung gian ngày tháng năm
int year,month,day;

//biến ngày tháng năm ở hiện tại
int year_cur,month_cur,mday_cur,wday_cur;

//hàm in ra ngày tháng năm hiện tại
void printFirsthead()
{
	system("cls");
	printf("  CALENDAR \n");

	//này mình in ra tháng với năm hiện tại
	printf("  Today: %s , %d , %d\n\n",monthList[month_cur],mday_cur,year_cur+1900);//thấy mình cộng thêm 1900 k?cho nó bằng với năm hiện tại
}                                //đây là khớp tháng với tháng nè

// hàm in ra cái lịch
void printCalendar()
{
	//này in ra tháng năm mình muốn
	printf("  %s %d\n",monthList[timeinfo->tm_mon],timeinfo->tm_year+1900);
	
	// quá trình để canh ngày 1 của tháng ở thứ mấy dựa vào timeinfo->tm_wday, công thức tự nghĩ=v 
	int a=timeinfo->tm_mday-1;
	int b=a%7;
	int c=timeinfo->tm_wday-b;
	if(c<0){c=7+c;}

	//này là quá trình in
	printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat \n");
	int k;
	for(k=0;k<c;k++) printf("     ");
	for(int j=1;j<=mDays[timeinfo->tm_mon];j++)
		{
		k++;
		if(j<10)
		{
			
			 printf("    %d",j);
		}
		else
		{
			
			 printf("   %d",j);
		} 
		if(k>6){k=0;printf("\n");}
		}
		if(k)printf("\n");       
}

// hàm in ra thập kỉ
void PrintDecade()
{
		system("cls");

		int a =year/10; 
		a=a*10;	// biến a này có giá trị bằng với năm bắt đầu thập kỉ đó. ví dụ year =2022, thì a=year/10=202,rồi a=a*10=2020.

		//in ra các năm trong thập kỉ
		for(int i=1;i<=10;i++)
		{
			printf("   %d",a);a++;
			if(i%4==0)printf("\n");
		}
}

// main menu
void menu()
{
	printf("Press 0:Exit\n");
	printf("Press 1: next month\n");
	printf("Press 2: last month\n");
	printf("Press 3: next year\n");
	printf("Press 4: last year\n");
	printf("Press 5: find a specific day\n");
	printf("Press 6: decade\n");
	
	//mấy này khỏi nói chắc cũng hiểu=v

	//cái lựa chọn thứ 7 này khi cái lịch hiển thị tháng khác hoặc năm khác nó mới xuất hiện, còn cái lịch để ở tháng năm hiện tại thì k xuất hiện
	if(year_cur!=timeinfo->tm_year||month_cur!=timeinfo->tm_mon)
	printf("press 7: back to the present\n");

	printf("Input your number: ");//nhập số zô quay lô tô
	int num;
	scanf("%d",&num);
	
	//trường hợp bằng 1:tháng tiếp theo
	if(num==1)
	{
			if(timeinfo->tm_mon==11)// này là nếu mon=11, tức là tháng bằng 12, thì tháng sau là tháng 1 thì ghi mon=0 và year tăng 1.
		{
			timeinfo->tm_mon=0;
			timeinfo->tm_year++;
		}
		else
		{
			timeinfo->tm_mon++;// còn bth thì cứ tăng tháng th
		}
		mktime(timeinfo); // hàm mktime này cũng có sẵn trong C, nó giúp set lại wday của timeinfo

		//in ra lịch
		printFirsthead();
		printCalendar();
	}

	//trường hợp bằng 2:tháng trước. tương tự như ở trên
	else if(num==2)
	{
		if(timeinfo->tm_mon==1)
		{
			timeinfo->tm_mon=11;
			timeinfo->tm_year--;
		}
		else
		{
			timeinfo->tm_mon--;
		}
		mktime(timeinfo);
		printFirsthead();
		printCalendar();
	}

	//trường hợp bằng 3:năm tiếp theo, này dễ hơn làm tháng
	else if(num==3)
	{
		timeinfo->tm_year++;
		mktime(timeinfo);
		printFirsthead();
		printCalendar();
	}

	//trường hợp bằng 4:năm trước
	else if(num==4)
	{
		timeinfo->tm_year--;
		mktime(timeinfo);
		printFirsthead();
		printCalendar();	
	}

	//trường hợp bằng 5: chọn ngày bất kì
	else if(num==5)
	{
		//nhập năm
		printf("input year:");
		scanf("%d",&year);
		system("cls");
		
		//lưu vào trong biến year trong timeinfo thì year phải bớt 1900
		timeinfo->tm_year=year-1900;

		//in các tháng
		printf("-----%d-----\n",year);
		for(int i=1;i<=12;i++)
		{
			printf("%s  ",shortmonthList[i-1]);
			if(i%4==0)printf("\n");
		}

		//nhập tháng
		printf("input month:");
		scanf("%d",&month);
		system("cls");

		//lưu vào mon trong timeinfo thì month-1
		timeinfo->tm_mon=month-1;

		//in các ngày trong tháng
		printf("-----%d-----\n",year);
		printf("-----%s-----\n",shortmonthList[month-1]);
		for(int i=1;i<=mDays[timeinfo->tm_mon];i++)
		{
			if(i<10)printf("   %d",i);
			else printf("  %d",i);
			if(i%5==0)printf("\n");
			else if(i==mDays[timeinfo->tm_mon])printf("\n");
		}

		//nhập ngày
		printf("input day:");
		scanf("%d",&day);
		system("cls");
		timeinfo->tm_mday=day;

		mktime(timeinfo);//set lại wday cho timeinfo
		printFirsthead();
		printCalendar();
	}

	//trường hợp bằng 6: thập kỉ
	else if(num==6)
	{
		year=(timeinfo->tm_year+1900)/10;
		year=year*10;
		int choice=0;
		PrintDecade();
		do
		{
		printf("\nPress 1: next decade\n");
		printf("Press 2: last decade\n");
		printf("Press 3: back to main menu\n");
		printf("Input your choice:");
		scanf("%d",&choice);
		if(choice==1)
		{
			year=year+10;
			PrintDecade();
		}
		else if(choice==2)
		{
			year=year-10;
			PrintDecade();
		}
		else if(choice==3)
		{
			printFirsthead();
 		    printCalendar();
			menu();
		}
		} while (choice!=3);
	}

	//trường hợp bằng 7:hiện tại
	else if(num==7)
	{
		timeinfo->tm_year=year_cur;
		timeinfo->tm_mon=month_cur;
		mktime(timeinfo);
		printFirsthead();
		printCalendar();
	}
	else if(num==0)
	{
		stop=0;
	}
}
int main ()
{
  time_t rawtime;
  time(&rawtime);  
  timeinfo= localtime (&rawtime); // này là set thời gian hiện tại vào các biến của timeinfo

  // lưu thời gian hiện tại vào các (year,month,mday,wday-current)để phục vụ hàm printFirsthead()
  year_cur=timeinfo->tm_year;
  month_cur=timeinfo->tm_mon;
  mday_cur=timeinfo->tm_mday;
  wday_cur=timeinfo->tm_wday;
  
  printFirsthead();
  printCalendar();
  
  do
  {
	menu();
  } while(stop==1);
  return 0;
}
