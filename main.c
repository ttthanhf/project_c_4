#include <stdio.h>

int dayOfMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // 0 để loại bỏ danh sách thứ 0

// void checkLeapYear(int year) { //tinh nam nhuan
//     if ( ( (year % 4 == 0) && (year % 100 != 0) ) || (year % 400 == 0) ) {
//         dayOfMonth[2] = 29; //nếu đúng là năm nhuận thì tháng 2 sẽ có 29 ngày
//         //printf("%d",dayOfMonth[2]);
//     }
// }

int dayNumber(int day, int month, int year) { // kiểm tra xem vào ngày , tháng , năm đó là thứ mấy
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    year -= month < 3;
    return (( year + year/4 - year/100 + year/400 + t[month+1] + day ) % 7) - 2; //result là thứ của ngày và tháng trong năm (vd thứ 2 có giá trị là 1, t3 = 2,...,chủ nhật = 7)
}

void day(int day) {
    switch (day) {
	case 1:
		printf("Monday");
		break;
	case 2:
		printf("Tuesday");
		break;
	case 3:
		printf("Wednesday");
		break;
	case 4:
		printf("Thursday");
		break;
	case 5:
		printf("Friday");
		break;
	case 6:
		printf("Saturday");
		break;
	case 7:
		printf("Sunday");
		break;
	}
}


void process(int year) {
    int getDay;
    getDay = dayNumber(1,1,year);
    day(getDay);
}

int main() {
    int year, check;
    printf("Nhap nam:");
    scanf("%d",&year);
    // checkLeapYear(year);
    // check = dayNumber(1,1,year);
    // printf("%d",check);
    process(year);
    return 0;
}