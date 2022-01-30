

#include <gtk/gtk.h>
// #include <windows.h>
// #include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
// #include <time.h>
//---Hân--//
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <conio.h>

// khai biến ở đây để tất cả các function đều truy cập được
GtkWidget *fixed_window;
GtkWidget *entry_year, *entry_month;                                                    // in function month_show and next from year_show
GtkWidget *show_month, *show_year;                                                      // in function main
GtkWidget *calendar;                                                                    // in fuction main
GtkWidget *goto_day_entry, *goto_month_entry, *goto_year_entry;                         // in function goto_day_show
GtkWidget *goto_dialog, *year_dialog, *month_dialog;                                    // in function goto_day_show and next from year_show and next from month_show
GtkWidget *username_login_entry, *password_login_entry;                                 // in function login_dialog_show
GtkWidget *login_dialog, *register_dialog;                                              // in function login_dialog_show  and next from register_dialog_show
GtkWidget *window;                                                                      // in function main_calendar
GtkWidget *login_error_label;                                                           // in function login_dialog_show
GtkWidget *error_username_available, *error_retype_incorrect, *error_wrong_format_pass; // in function register_dialog_show
GtkWidget *username_entry, *password_entry, *retypePassword_entry, *fullname_entry;
GtkWidget *success_signup;
GtkWidget *popup_register;
GtkWidget *event[24];
GtkWidget *note[100],*mark_note_day[43],*zodiac[13],*mark_event_day[43];
GtkWidget *dialog_double_click,*deleteDialog;
GtkWidget *test_error;
const char *userNameTmp, *passwordTmp, *retypePassword; // in function check_user
char fullNameFile[100];
const char *fullNameTmp;
int indexOfUser;        
int stop_loop_Main = 0; //0=false, 1 = true                                                                                                                        // dùng để lưu lại số thứ tự user                                                                                                                         // in function signUp
char *monthList[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}; // loại bỏ vị trí 0

guint year_today, month_today, day_today;// in function main

int dayOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int day,month,year;
int year_choose;
char date[100];const char *add_info, *delete_info;
char add_day[10],add_month[10],add_year[10];
int num=0;
char save[21][50];
char address_file_Users[256];
char address_replace[256];
char address_file_app[256];
char *image[200]={"hoi.png","chuot.png","suu.png","dan.png","mao.png","thin.png","ti.png","ngo.png","mui.png","than.png","dau.png","tuat.png"};
time_t rawtime;
struct tm *tm1;
FILE*file;

//----------------------//

//------------------------------------------Hân---------Authentication---------------------------------------------//

#define MAX_USER 100
#define MAX_LETTER 100

char listUser[MAX_USER][3][MAX_LETTER]; // listUser[STT][full name/user name/password][số kí tự]
int countUser;                          //                    0  /     1   /   2
char nameOfEvent[30][MAX_LETTER];
int dayOfEvent[30], monthOfEvent[30];
int countEvent;
int checkLeap[10];

char fileTmp[] = "Data.txt";
char fileTmp2[] = "Log.txt";
char tmpAddress[256];

void get_address_Users_file()
{
    getcwd(address_file_app,256);
    getcwd(address_file_Users,256);
    strcat(address_file_Users,"\\User\\");
}
void creatFolder()
{
  FILE *file_folder;
  char name[10];
  char cmmnd[] = {"md "};
  char address[256];
  getcwd(address, 256); // hàm lấy path hiện tại
  int status;
  strcpy(address,address_file_app);
  strcat(address, "\\User");
  mkdir(address);
  chdir(address);
  strcat(address, "\\");
  strcat(address, listUser[countUser][1]);
  mkdir(address);
  chdir(address);
  /* status = system(cmmnd); // hàm tạo folder
  strcat(address, "\\");
  strcat(address, listUser[countUser][1]);
  int ck = chdir(address); */
  // strcat(tmpAddress, listUser[countUser][1]);
  // strcat(tmpAddress, fileTmp);
  // file_folder = fopen(tmpAddress, "w");

  // int len = strlen(tmpAddress);
  // for (int i = 0; i <= len; i++)
  //   tmpAddress[i] = '\0';

  // strcat(tmpAddress, listUser[countUser][1]);
  // strcat(tmpAddress, fileTmp2);
  // file_folder = fopen(tmpAddress, "w");
  // // fprintf(file,"da ghi");
  // fclose(file_folder);
}

int jdFromDate(int dd, int mm, int yy) // lay ngay julius
{
  int a = (14 - mm) / 12;
  int y = yy + 4800 - a;
  int m = mm + 12 * a - 3;
  int jd = dd + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
  if (jd < 2299161)
    jd = dd + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083;
  return jd;
}

int jdToDate(int jd) // chuyen ngay julius qua day thuong
{
  int a, b, c;
  if (jd > 2299160)
  { // After 5/10/1582, Gregorian calendar
    a = (int)(jd + 32044);
    b = (int)((4 * a + 3) / 146097);
    c = (int)(a - (b * 146097) / 4);
  }
  else
  {
    b = 0;
    c = jd + 32082;
  }
  int d = (int)((4 * c + 3) / 1461);
  int e = (int)(c - (1461 * d) / 4);
  int m = (int)((5 * e + 2) / 153);
  int day = e - (int)((153 * m + 2) / 5) + 1;
  int month = m + 3 - 12 * (int)(m / 10);
  int year = b * 100 + d - 4800 + (int)(m / 10);
}

double getSunLongitude(int jdn) // toa do mat troi
{
  double timeZone = 7.0;
  double PI = 3.14;
  double T, T2, dr, M, L0, DL, L;
  T = (jdn - 2451545.0) / 36525; // Time in Julian centuries from 2000-01-01 12:00:00 GMT
  T2 = T * T;
  dr = PI / 180;                                                          // degree to radian
  M = 357.52910 + 35999.05030 * T - 0.0001559 * T2 - 0.00000048 * T * T2; // mean anomaly, degree
  L0 = 280.46645 + 36000.76983 * T + 0.0003032 * T2;                      // mean longitude, degree
  DL = (1.914600 - 0.004817 * T - 0.000014 * T2) * sin(dr * M);
  DL = DL + (0.019993 - 0.000101 * T) * sin(dr * 2 * M) + 0.000290 * sin(dr * 3 * M);
  L = L0 + DL; // true longitude, degree
  L = L * dr;
  L = L - PI * 2 * (int)(L / (PI * 2)); // Normalize to (0,2*PI)
  return (int)(L / PI * 6);
}

double getNewMoonDay(int k)
{
  double PI = 3.14;
  double T, T2, T3, dr, Jd1, M, Mpr, F, C1;
  double timeZone = 7.0;
  T = k / 1236.85; // Time in Julian centuries from 1900 January 0.5
  T2 = T * T;
  T3 = T2 * T;
  dr = PI / 180;
  Jd1 = 2415020.75933 + 29.53058868 * k + 0.0001178 * T2 - 0.000000155 * T3;
  Jd1 = Jd1 + 0.00033 * sin((166.56 + 132.87 * T - 0.009173 * T2) * dr); // Mean new moon
  M = 359.2242 + 29.10535608 * k - 0.0000333 * T2 - 0.00000347 * T3;     // Sun's mean anomaly
  Mpr = 306.0253 + 385.81691806 * k + 0.0107306 * T2 + 0.00001236 * T3;  // Moon's mean anomaly
  F = 21.2964 + 390.67050646 * k - 0.0016528 * T2 - 0.00000239 * T3;     // Moon's argument of latitude
  C1 = (0.1734 - 0.000393 * T) * sin(M * dr) + 0.0021 * sin(2 * dr * M);
  C1 = C1 - 0.4068 * sin(Mpr * dr) + 0.0161 * sin(dr * 2 * Mpr);
  C1 = C1 - 0.0004 * sin(dr * 3 * Mpr);
  C1 = C1 + 0.0104 * sin(dr * 2 * F) - 0.0051 * sin(dr * (M + Mpr));
  C1 = C1 - 0.0074 * sin(dr * (M - Mpr)) + 0.0004 * sin(dr * (2 * F + M));
  C1 = C1 - 0.0004 * sin(dr * (2 * F - M)) - 0.0006 * sin(dr * (2 * F + Mpr));
  C1 = C1 + 0.0010 * sin(dr * (2 * F - Mpr)) + 0.0005 * sin(dr * (2 * Mpr + M));
  double deltat;
  if (T < -11)
  {
    deltat = 0.001 + 0.000839 * T + 0.0002261 * T2 - 0.00000845 * T3 - 0.000000081 * T * T3;
  }
  else
  {
    deltat = -0.000278 + 0.000265 * T + 0.000262 * T2;
  };
  double JdNew = Jd1 + C1 - deltat;
  return (int)(JdNew + 0.5 + timeZone / 24);
}

int getLunarMonth11(int yy)
{
  double off, k, nm, sunLong;
  off = jdFromDate(31, 12, yy) - 2415021.076998695;
  k = (int)(off / 29.530588853);
  nm = getNewMoonDay((int)k);
  // sun longitude at local midnight
  sunLong = getSunLongitude((int)nm);
  if (sunLong >= 9)
    nm = getNewMoonDay((int)k - 1);
  return nm;
}

int getLeapMonthOffset(int a11) // xac dinh month nhuan
{
  double last, arc;
  int k, i;
  k = (int)(((a11 - 2415021.076998695) / 29.530588853) + 0.5);
  last = 0; // Month 11 contains point of sun longutide 3*PI/2 (December solstice)
  i = 1;    // We start with the month following lunar month 11
  arc = getSunLongitude((int)getNewMoonDay((int)(k + i)));
  do
  {
    last = arc;
    i++;
    arc = getSunLongitude((int)getNewMoonDay((int)(k + i)));
  } while (arc != last && i < 14);
  return i - 1;
}

int checkLeapMonth(int n)
{
  if (checkLeap[n] == 1)
    return 0;
  return 1;
}

int convertSolar2Lunar(int dd, int mm, int yy)
{
  double dayNumber, monthStart, a11, b11, lunarDay, lunarMonth, lunarYear;
  int k, diff, lunarLeap;
  dayNumber = jdFromDate(dd, mm, yy);
  k = (int)((dayNumber - 2415021.076998695) / 29.530588853);
  monthStart = getNewMoonDay(k + 1);
  if (monthStart > dayNumber)
    monthStart = getNewMoonDay(k);
  a11 = getLunarMonth11(yy);
  b11 = a11;
  if (a11 >= monthStart)
  {
    lunarYear = yy;
    a11 = getLunarMonth11(yy - 1);
  }
  else
  {
    lunarYear = yy + 1;
    b11 = getLunarMonth11(yy + 1);
  }
  lunarDay = dayNumber - monthStart + 1;
  diff = (int)((monthStart - a11) / 29);
  lunarLeap = 0;
  lunarMonth = diff + 11;
  if (b11 - a11 > 365)
  {
    int leapMonthDiff = getLeapMonthOffset(a11);
    if (diff >= leapMonthDiff)
    {
      lunarMonth = diff + 10;
      if (diff == leapMonthDiff)
      {
        lunarLeap = 1;
      }
    }
  }
  if (lunarMonth > 12)
    lunarMonth = lunarMonth - 12;
  if (lunarMonth >= 11 && diff < 4)
    lunarYear -= 1;
  int day = lunarDay;
  int month = lunarMonth;
  int year = lunarYear;
  if (day == 23 && month == 12)
    if (checkLeapMonth(0) == 1) // de khong bi trung lap ngay khi co thang nhuan
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "Kitchen guardians");
      countEvent++;
      checkLeap[0] = 1;
      return 1;
    }
  if (day == 1 && month == 1)
    if (checkLeapMonth(1) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "Vietnamese New Year");
      countEvent++;
      checkLeap[1] = 1;
      return 1;
    }
  if (day == 2 && month == 1)
    if (checkLeapMonth(2) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "2nd day of Tet");
      countEvent++;
      checkLeap[2] = 1;
      return 1;
    }
  if (day == 3 && month == 1)
    if (checkLeapMonth(3) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "3nd day of Tet");
      countEvent++;
      checkLeap[3] = 1;
      return 1;
    }
  if (day == 4 && month == 1)
    if (checkLeapMonth(4) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "4nd day of Tet");
      countEvent++;
      checkLeap[4] = 1;
      return 1;
    }
  if (day == 5 && month == 1)
    if (checkLeapMonth(5) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "5nd day of Tet");
      countEvent++;
      checkLeap[5] = 1;
      return 1;
    }
  if (day == 15 && month == 8)
    if (checkLeapMonth(6) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "Mid autumn festivals");
      countEvent++;
      checkLeap[6] = 1;
      return 1;
    }
  if (day == 10 && month == 3)
    if (checkLeapMonth(7) == 1)
    {
      dayOfEvent[countEvent] = dd;
      monthOfEvent[countEvent] = mm;
      strcpy(nameOfEvent[countEvent], "Hung Kings Festival");
      countEvent++;
      checkLeap[7] = 1;
      return 1;
    }
  return 0;
}

void findEventDay(int year)
{
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    dayOfMonth[2]++;
  for (int i = 0; i < 10; i++)
    checkLeap[i] = 0;
  for (int i = 1; i <= 12; i++)
    for (int j = 1; j <= dayOfMonth[i]; j++)
    {
       convertSolar2Lunar(j, i, year);
    }
  if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
    dayOfMonth[2]--; // trừ lại

}
void read_solarEvent()
{
  FILE *f;
  char path[200];
  sprintf(path,"%s\\SolarEvent.txt",address_file_app);
  f = fopen(path, "r");
  int day, month, year;
  char data[100];
  // guint year_select, month_select, day_select;
  // gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  if (f != NULL)
    for (int i = 0; i < 16; i++)
    {
      fscanf(f, "%d/%d %[^\n]", &day, &month, &data);
      dayOfEvent[countEvent] = day;
      monthOfEvent[countEvent] = month;
      strcpy(nameOfEvent[countEvent], data);
      countEvent++;
     // printf("%d/ %d %s", day, month, data);
      // if (day == day_select && month == month_select);
      // in data vào khung event
    }
  fclose(f);
}
void swapValue(int *a, int *b)
{
  int c = *a;
  *a = *b;
  *b = c;
}
void printfAscending()
{
  char k[20], l[20];
  for (int i = 0; i < countEvent; i++)
    for (int j = 0; j < (countEvent - 1); j++)
      if (monthOfEvent[j + 1] < monthOfEvent[j])
      {
        swapValue(&monthOfEvent[j], &monthOfEvent[j + 1]);
        swapValue(&dayOfEvent[j], &dayOfEvent[j + 1]);

        strcpy(k, nameOfEvent[j + 1]);
        strcpy(nameOfEvent[j + 1], nameOfEvent[j]);
        strcpy(nameOfEvent[j], k);
      }
  for (int i = 0; i < countEvent; i++)
    for (int j = 0; j < (countEvent - 1); j++)
      if (monthOfEvent[j + 1] == monthOfEvent[j])
        if (dayOfEvent[j + 1] < dayOfEvent[j])
        {
          swapValue(&monthOfEvent[j], &monthOfEvent[j + 1]);
          swapValue(&dayOfEvent[j], &dayOfEvent[j + 1]);

          strcpy(k, nameOfEvent[j + 1]);
          strcpy(nameOfEvent[j + 1], nameOfEvent[j]);
          strcpy(nameOfEvent[j], k);
        }
}

//--------------------------------//
static void load_css()
{
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "theme_v3.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

gboolean update_time(gpointer label)
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  gchar *display = g_strdup_printf("Clock:\t\t%02d : %02d : %02d (hh/mm/ss)", tm.tm_hour, tm.tm_min, tm.tm_sec);
  gtk_label_set_text(GTK_LABEL(label), display);

  if (stop_loop_Main == 0) {
    return TRUE;
  }
  else return FALSE;
}
void take_today()
{
  time(&rawtime);
  tm1 = localtime(&rawtime);
  day_today=tm1->tm_mday;
  month_today=tm1->tm_mon+1;
  year_today=tm1->tm_year+1900;
}
void update_today(gpointer label)
{
  gchar *display = g_strdup_printf("Today:\t\t%02d/%02d/%d (dd/mm/yyyy)", day_today, month_today + 1, year_today);
  gtk_label_set_text(GTK_LABEL(label), display);
}

void destroy_widget(gpointer *data, gpointer *data2)
{
  gtk_widget_destroy(GTK_WIDGET(data));
  gtk_widget_destroy(GTK_WIDGET(data2));
}
void update_hello(GtkWidget *label)
{
  gchar *display = g_strdup_printf("Hello, %s !", listUser[indexOfUser][0]);
  gtk_label_set_text(GTK_LABEL(label), display);
}
void update_year(int year)
{ // cập nhật label trong button
  gchar *display;
  display = g_strdup_printf("%d", year);                // chuyển int thành char
  gtk_button_set_label(GTK_BUTTON(show_year), display); // vì set label phải chuyển thành char mớ nhận được

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  gtk_calendar_select_month(GTK_CALENDAR(calendar), month_select, year);
}
void update_month(int month)
{                                                                 // cập nhật label trong button
  gtk_button_set_label(GTK_BUTTON(show_month), monthList[month]); // vì set label phải chuyển thành char mớ nhận được

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  gtk_calendar_select_month(GTK_CALENDAR(calendar), month - 1, year_select);
}
gboolean update_choose(gpointer label)
{
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select); // chọn ngày đã được chọn lưu vào []_select
  gchar *display = g_strdup_printf("You choose:\t%02d/%02d/%d (dd/mm/yyyy)", day_select, month_select + 1, year_select);
  gtk_label_set_text(GTK_LABEL(label), display);
  update_month(month_select + 1);
  update_year(year_select);
  if (stop_loop_Main == 0)
  {
    return TRUE;
  }
  else
    return FALSE;
}
void update_day(int day)
{
  gtk_calendar_select_day(GTK_CALENDAR(calendar), day);
}

void add_year_input(gpointer *data, GtkWidget *error_label)
{                                                             // lấy dữ liệu từ input year
  int year = atoi(gtk_entry_get_text(GTK_ENTRY(entry_year))); // atoi là biến chuỗi số thành giá trị số

  if (year >= 1 && year <= 9999)
  { // nếu year input < 1 or > 9999 thì báo lỗi
    update_year(year);
    gtk_widget_destroy(year_dialog);
  }
  else
  {
    gtk_widget_show(error_label);
  }
}

void add_month_input(gpointer *data, GtkWidget *error_label)
{ // lấy dữ liệu từ input month
  int month = atoi(gtk_entry_get_text(GTK_ENTRY(entry_month)));

  if (month >= 1 && month <= 12)
  { // nếu month input < 1 hoặc > 12 thì báo lỗi
    update_month(month);
    gtk_widget_destroy(month_dialog);
  }
  else
  {
    gtk_widget_show(error_label);
  }
}

void add_one_year()
{
  int add = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
  add += 1;
  update_year(add);
}

void minus_one_year()
{
  int minus = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
  minus -= 1;
  if (minus < 1)
  {
    minus = 1;
    update_year(minus);
  }
  else
  {
    update_year(minus);
  }
}

void add_one_month()
{
  gchar *get_charAdd1, *get_charAdd2;
  for (int i = 1; i < 13; i++)
  {
    get_charAdd1 = g_strdup_printf("%s", monthList[i]);                                 // lấy chuỗi trong monthList
    get_charAdd2 = g_strdup_printf("%s", gtk_button_get_label(GTK_BUTTON(show_month))); // lấy chuỗi của label month
    if (strcmp(get_charAdd1, get_charAdd2) == 0)
    { // nếu hai chuỗi = nhau thì = 0 ==> chạy if \ strcmp so sánh hai chuỗi, = nhau thì = 0
      int add = i;
      add += 1;
      if (add > 12)
      { // nếu value month > 12 thì month = 1
        add = 1;
        update_month(add);
        // nếu hơn tháng 12 thì year + 1
        int year = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
        year += 1;
        update_year(year);
        break; // dừng loop để trả đúng kết quả i
      }

      else
      {
        update_month(add);
        break;
      }
    }
  }
}

void minus_one_month()
{
  gchar *get_charMinus1, *get_charMinus2;
  for (int i = 1; i < 13; i++)
  {
    get_charMinus1 = g_strdup_printf("%s", monthList[i]);
    get_charMinus2 = g_strdup_printf("%s", gtk_button_get_label(GTK_BUTTON(show_month)));
    if (strcmp(get_charMinus1, get_charMinus2) == 0)
    {
      int minus = i;
      minus -= 1;
      if (minus < 1)
      { // nếu value month > 12 thì month = 1
        minus = 12;
        update_month(minus);

        int year = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
        if (year < 2)
        { // nếu year nhỏ hơn 2 thì year tối đa chỉ đến year = 1
          year = 1;
          update_year(year);
          break;
        }
        else
        {
          year -= 1;
          update_year(year);
          break;
        }
      }
      else
      {
        update_month(minus);
        break;
      }
    }
  }
}

void goto_activate(gpointer *data, GtkWidget *error_label)
{
  int day = atoi(gtk_entry_get_text(GTK_ENTRY(goto_day_entry)));
  int month = atoi(gtk_entry_get_text(GTK_ENTRY(goto_month_entry)));
  int year = atoi(gtk_entry_get_text(GTK_ENTRY(goto_year_entry)));
  int error; // biến để kiểm tra có lỗi hay ko

  if (year >= 1 && year <= 9999)
  { // year = 1:9999
    if (month >= 1 && month <= 12)
    { // nếu month > 12 or < 1 thì thống báo lỗi
      if (month == 4 || month == 6 || month == 9 || month == 11)
      { // check tháng 4 6 9 11
        if (day >= 1 && day <= 30)
        {
          error = 0;
        }
        else
        {
          error = 1;
        }
      }
      else
      {
        if (month == 2)
        {
          if ((year % 4 == 0) && ((year % 400 == 0) || (year % 100 != 0)))
          { // kiem tra xem co phai la nam nhuan
            if (day >= 1 && day <= 29)
            { // neu nam nhuan thi thang 2 có 29 ngày
              error = 0;
            }
            else
            {
              error = 1;
            }
          }
          else
          {
            if (day >= 1 && day <= 28)
            { // nam ko nhuan thi thang 2 co 28 ngay
              error = 0;
            }
            else
            {
              error = 1;
            }
          }
        }
        else
        { // tháng còn lại
          if (day >= 1 && day <= 31)
          {
            error = 0;
          }
          else
          {
            error = 1;
          }
        }
      }
    }
    else
    {
      error = 1;
    }
  }
  else
  {
    error = 1;
  }

  switch (error)
  {
  case 0: // error = 0 = ko có lỗi
    update_day(day);
    update_month(month);
    update_year(year);
    gtk_widget_destroy(goto_dialog);
    break;
  case 1: // error = 1 = có lỗi
    gtk_widget_show(error_label);
    break;
  }
}

void goto_day_show()
{
  GtkWidget *container;
  GtkWidget *day_label, *month_label, *year_label;
  GtkWidget *fixed;
  GtkWidget *button;
  GtkWidget *error_label;

  goto_dialog = gtk_dialog_new();
  fixed = gtk_fixed_new();

  goto_day_entry = gtk_entry_new();
  goto_month_entry = gtk_entry_new();
  goto_year_entry = gtk_entry_new();

  day_label = gtk_label_new("Day:");
  month_label = gtk_label_new("Month:");
  year_label = gtk_label_new("Year:");
  error_label = gtk_label_new("Invalid number. Try again !");

  button = gtk_button_new_with_label("Go to");

  gtk_entry_set_width_chars(GTK_ENTRY(goto_day_entry), 3);   // chỉnh kích thước entry theo số lượng chữ
  gtk_entry_set_width_chars(GTK_ENTRY(goto_month_entry), 3); // chỉnh kích thước entry theo số lượng chữ
  gtk_entry_set_width_chars(GTK_ENTRY(goto_year_entry), 5);  // chỉnh kích thước entry theo số lượng chữ

  gtk_fixed_put(GTK_FIXED(fixed), goto_day_entry, 40, 18);
  gtk_fixed_put(GTK_FIXED(fixed), goto_month_entry, 140, 18);
  gtk_fixed_put(GTK_FIXED(fixed), goto_year_entry, 225, 16);
  gtk_fixed_put(GTK_FIXED(fixed), day_label, 5, 20);
  gtk_fixed_put(GTK_FIXED(fixed), month_label, 85, 20);
  gtk_fixed_put(GTK_FIXED(fixed), year_label, 185, 20);
  gtk_fixed_put(GTK_FIXED(fixed), button, 105, 90);
  gtk_fixed_put(GTK_FIXED(fixed), error_label, 55, 55);

  gtk_widget_set_name(goto_day_entry, "goto_entry");
  gtk_widget_set_name(goto_month_entry, "goto_entry");
  gtk_widget_set_name(goto_year_entry, "goto_entry");
  gtk_widget_set_name(error_label, "error_label");

  gtk_window_set_title(GTK_WINDOW(goto_dialog), "Choose date");
  gtk_window_set_position(GTK_WINDOW(goto_dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(goto_dialog), 10);

  g_signal_connect(button, "clicked", G_CALLBACK(goto_activate), error_label);

  container = gtk_dialog_get_content_area(GTK_DIALOG(goto_dialog));

  gtk_container_add(GTK_CONTAINER(container), fixed);

  gtk_widget_show_all(goto_dialog);

  gtk_widget_hide(error_label);
}

void copy_today()
{
   num=0;
   char try[200];
   itoa(day_today,add_day,10);itoa(month_today,add_month,10);itoa(year_today,add_year,10);
   sprintf(date,"%s-%s-%s.txt",add_day,add_month,add_year);
   file=fopen(date,"a");
   fclose(file);
   file=fopen(date,"r");
   while(fgets(try,200,file)!=NULL)
   {
      num++;
      strcpy(save[num],try);
      int c=strlen(save[num]);
      if(save[num][c-1]=='\n')save[num][c-1]='\0';
   }
   fclose(file);
}
void hide_note()
{
  for(int i=1;i<=num;i++)
  {
    gtk_widget_hide(GTK_WIDGET(note[i]));
  }
}
void copy()
{
   num=0;
   char try[200];
   gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
   month++;
   itoa(day,add_day,10);itoa(month,add_month,10);itoa(year,add_year,10);
   sprintf(date,"%s-%s-%s.txt",add_day,add_month,add_year);
   file=fopen(date,"a");
   fclose(file);
   file=fopen(date,"r");
   while(fgets(try,200,file)!=NULL)
   {
      num++;
      strcpy(save[num],try);
      int c=strlen(save[num]);
      if(save[num][c-1]=='\n')save[num][c-1]='\0';
   }
   fclose(file);
}
void print_to_file()
{
    file=fopen(date,"w+");
    if(num>=1)fprintf(file,"%s",save[1]);
    for(int i=2;i<=num;i++)
   {
       fprintf(file,"\n%s",save[i]);
   }
   fclose(file);
}
void add_note()
{
   num++;
   strcpy(save[num],add_info);
   print_to_file();
}
void delete_note()
{
   int x=0;
   for(int i=1;i<=num;i++)
   {
       if(strcmp(save[i],delete_info)==0)
       {
          x=i;break;
       }
   }
    for(int i=x;i<=num-1;i++)
    {
       strcpy(save[i],save[i+1]);
    }
    num--;
    print_to_file();
}
void delete_dialog(gpointer* data)
{
  GtkWidget  *container;
  GtkWidget *label,*label1,*button;
  deleteDialog = gtk_dialog_new();

  label=gtk_label_new("Do you want do delete?");
  label1=gtk_label_new("");

  gtk_dialog_add_buttons(GTK_DIALOG(deleteDialog), "Yes", 1, "No", 2, NULL);

  gtk_window_set_position(GTK_WINDOW(deleteDialog), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(deleteDialog), "Delete note");
  gtk_container_set_border_width(GTK_CONTAINER(deleteDialog), 10);

  g_signal_connect(deleteDialog, "delete-event", G_CALLBACK(destroy_widget), NULL);

  container = gtk_dialog_get_content_area(GTK_DIALOG(deleteDialog));

  gtk_container_add(GTK_CONTAINER(container),label);
  gtk_container_add(GTK_CONTAINER(container),label1);

  gtk_widget_show_all(deleteDialog);
  int response=gtk_dialog_run(GTK_DIALOG(deleteDialog));
  switch (response)
  {
    case 1:
    gtk_widget_destroy(GTK_WIDGET(deleteDialog));
    delete_info=gtk_button_get_label(GTK_BUTTON(data));
    hide_note();
    delete_note();
    copy();
    int a=0;
    for(int i=1;i<=num;i++)
    {
      
      note[i]=gtk_button_new_with_label("");
      gtk_button_set_label(GTK_BUTTON(note[i]),save[i]);
      gtk_fixed_put(GTK_FIXED(fixed_window), note[i], 10, 220+a);
      a+=50;
      g_signal_connect(note[i],"clicked",G_CALLBACK(delete_dialog),NULL);
      gtk_widget_set_name(note[i], "note_content");
      gtk_widget_show(note[i]);
    }
    
    break;
    case 2:
    gtk_widget_destroy(GTK_WIDGET(deleteDialog));
    break;
  }
}
void connect_note()
{
  int a=0;
  for(int i=1;i<=num;i++)
  {
    note[i]=gtk_button_new_with_label("");
    gtk_button_set_label(GTK_BUTTON(note[i]),save[i]);
    gtk_fixed_put(GTK_FIXED(fixed_window), note[i], 10, 220+a);
    a+=50;
    g_signal_connect(note[i],"clicked",G_CALLBACK(delete_dialog),NULL);
    gtk_widget_set_name(note[i], "note_content");
    gtk_widget_show(note[i]);
  }
  
}
void show_note_list()
{
  hide_note();
  copy();
  connect_note();
  /* gtk_widget_show_all(window); */
}
gboolean show_mark_day()
{
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
  time(&rawtime);
  /* tm1 = localtime(&rawtime); */
  tm1->tm_mday=1;
  tm1->tm_mon=month;
  tm1->tm_year=year-1900;
  mktime(tm1);
  int number=tm1->tm_wday;
  if(number==0)number=7;
  dayOfMonth[2]=28;
  if((year%4==0&&year%100!=0)||year%400==0)dayOfMonth[2]=29;
  char file_name[100],try[200];
  int count_day_of_month=0;
  for(int i=1;i<1+number;i++)gtk_widget_hide(mark_note_day[i]);
  for(int i=1+number;i<=42;i++)
  {
    count_day_of_month++;
    
    itoa(count_day_of_month,add_day,10);itoa(month+1,add_month,10);itoa(year,add_year,10);
    sprintf(file_name,"%s-%s-%s.txt",add_day,add_month,add_year);
    file=fopen(file_name,"r");
    if(file!=NULL)
    {
      if(fgets(try,200,file)!=NULL)gtk_widget_show(mark_note_day[i]);
      else gtk_widget_hide(mark_note_day[i]);
    }
    else gtk_widget_hide(mark_note_day[i]);
    fclose(file);
  }
  if (stop_loop_Main == 0) {
    return TRUE;
  }
  return FALSE;
  
}
void create_mark_day()
{
  int width=0,height=0;
  for(int i=1;i<=42;i++)
  {  
    mark_event_day[i]=gtk_label_new(".");
    mark_note_day[i]=gtk_label_new(".");
    gtk_fixed_put(GTK_FIXED(fixed_window), mark_note_day[i],435+width, 265+height);
    gtk_fixed_put(GTK_FIXED(fixed_window), mark_event_day[i],435+width, 280+height);
    width+=117;
    if(i%7==0)
    {
      width=0;   height+=64;
    }
  }
}
void create_image()
{
  for(int i=0;i<=11;i++)
  {
  char path[200];
  sprintf(path,"%s\\image\\%s",address_file_app,image[i]);
  zodiac[i]=gtk_image_new_from_file (path);
  gtk_fixed_put(GTK_FIXED(fixed_window), GTK_WIDGET(zodiac[i]), 1200,300);
  }
  
}
gboolean show_image()
{
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
  if(year!=year_choose)
  {
  gtk_widget_show(zodiac[abs(year+9)%12]);
  gtk_widget_hide(zodiac[abs(year_choose+9)%12]);
  year_choose=year;
  }
  if (stop_loop_Main == 0) {
    return TRUE;
  }
  return FALSE;
}
gboolean update_event(gpointer label)
{
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  tm1->tm_mday=1;
  tm1->tm_mon=month_select;
  tm1->tm_year=year_select-1900;
  mktime(tm1);
  int number=tm1->tm_wday;
  if(number==0)number=7;
  int x=-1,y=23,count_day=0;
  for(int i=1;i<1+number;i++) gtk_widget_hide(mark_event_day[i]);
  for(int i=0;i<24;i++)
  {
     if(monthOfEvent[i]==(month_select+1))
     {
        x=i; break;
     }
  }
  if(x!=-1)
    {
      for(int i=x+1;i<24;i++)
      {
      if(monthOfEvent[i]>(month+1)) 
        {
          y=i-1;break;
        }
      }
      for(int i=1+number;i<=42;i++)
      {
         count_day++;
         if(count_day==dayOfEvent[x]) 
         {
            gtk_widget_show(mark_event_day[i]);
            x++;
         }
         else
         {
            gtk_widget_hide(mark_event_day[i]);
         }
         if(x>y)
         {
            for(int j=dayOfEvent[x-1]+number+1;j<=42;j++) gtk_widget_hide(mark_event_day[j]);
            break;
         }

      }
   }
  int check = 0;
  findEventDay(year_select);
  read_solarEvent();
  printfAscending();
  countEvent = 0;

  for (int i = 0; i < 24; i++)
  {
    if ((day_select == dayOfEvent[i]) && ((month_select + 1) == monthOfEvent[i]))
    {
      gchar *display = g_strdup_printf("%s", nameOfEvent[i]);
      gtk_label_set_text(GTK_LABEL(label), display);
      check = 1;
      break;
    }
    if (check == 0)
    {
      gchar *display = g_strdup_printf("No events");
      gtk_label_set_text(GTK_LABEL(label), display);
    }
  }
  if (stop_loop_Main == 0)
  {
    return TRUE;
  }
  else
    return FALSE;
}
void addEvent_show_double_click()
{
  GtkWidget *container;
  GtkWidget *name_event_entry, *note_event_entry;
  GtkWidget *name_event_label, *note_event_label;
  GtkWidget *fixed;
  GtkWidget *show_day_label;

  gchar *display_update;

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select); // chọn ngày đã được chọn lưu vào []_select

  dialog_double_click = gtk_dialog_new();
  fixed = gtk_fixed_new();

  /* name_event_entry = gtk_entry_new(); */
  note_event_entry = gtk_entry_new();
  

  /* name_event_label = gtk_label_new("Name:"); */
  note_event_label = gtk_label_new("Note:");
  show_day_label = gtk_label_new("");

  display_update = g_strdup_printf("Date:\t%02d/%02d/%d (dd/mm/yyyy)", day_select, month_select + 1, year_select);

  gtk_label_set_text(GTK_LABEL(show_day_label), display_update);

  gtk_dialog_add_buttons(GTK_DIALOG(dialog_double_click), "Ok", 1, "Cancel", 2, NULL); // ok = 1 vs cancel = 2

   /* gtk_fixed_put(GTK_FIXED(fixed), name_event_entry, 70, 45);  */
   gtk_fixed_put(GTK_FIXED(fixed), note_event_entry, 70, 50); 
  /*  gtk_fixed_put(GTK_FIXED(fixed), name_event_label, 10, 50);  */
  gtk_fixed_put(GTK_FIXED(fixed), note_event_label, 10, 55);
  gtk_fixed_put(GTK_FIXED(fixed), show_day_label, 10, 10);

  gtk_widget_set_name(note_event_entry, "note_event_entry");

  gtk_window_set_position(GTK_WINDOW(dialog_double_click), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(dialog_double_click), "Add event");
  gtk_container_set_border_width(GTK_CONTAINER(dialog_double_click), 10);

  g_signal_connect(dialog_double_click, "delete-event", G_CALLBACK(destroy_widget), NULL);

  container = gtk_dialog_get_content_area(GTK_DIALOG(dialog_double_click));

  gtk_container_add(GTK_CONTAINER(container), fixed);

  gtk_widget_show_all(dialog_double_click);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog_double_click));// add gia tri khi bam button
  add_info=gtk_entry_get_text(GTK_ENTRY(note_event_entry)); 
  switch (response)
  { // thuc hien ham khi gia tri = nhau
  case 1:
    
    if(add_info=="")gtk_widget_destroy(GTK_WIDGET(dialog_double_click));
    else
    {
        copy();
        add_note();
        gtk_widget_destroy(GTK_WIDGET(dialog_double_click));
        show_note_list();
    }
    break;
  case 2:
    gtk_widget_destroy(GTK_WIDGET(dialog_double_click));
    break;
  }
} 

void eventList_show()
{
  GtkWidget *container_eventList_dialog;
  GtkWidget *label_nameOfEvent, *button_enter;
  GtkWidget *eventList_label;
  GtkWidget *fixed_eventList;
  GtkWidget *eventList_dialog;
  GtkWidget *show_event_label;
  GtkWidget *box_event;

  gchar *display_eventList_update;
  int space = 0;
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);

  eventList_dialog = gtk_dialog_new();
  fixed_eventList = gtk_fixed_new();

  eventList_label = gtk_label_new("");
  show_event_label = gtk_label_new("");
  display_eventList_update = g_strdup_printf("List event of %d ", year_select);

  box_event = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  gtk_label_set_text(GTK_LABEL(eventList_label), display_eventList_update);

  gtk_fixed_put(GTK_FIXED(fixed_eventList), box_event, 6, 80);
  gtk_fixed_put(GTK_FIXED(fixed_eventList), eventList_label, 200, 10);
  findEventDay(year_select);
  read_solarEvent();
  printfAscending();
  for (int i = 0; i < countEvent; i++)
  {
    event[i] = gtk_label_new("");
    gtk_fixed_put(GTK_FIXED(fixed_eventList), event[i], 10, 100 + space);
    space += 30;
    gtk_widget_set_name(event[i], "eventList_label");
    gchar *display = g_strdup_printf("%d/%d %s", dayOfEvent[i], monthOfEvent[i], nameOfEvent[i]);
    gtk_label_set_text(GTK_LABEL(event[i]), display);
  }

  countEvent = 0;

  gtk_widget_set_name(eventList_label, "show_eventList");

  gtk_widget_set_name(box_event, "box_event_show");

  gtk_window_set_position(GTK_WINDOW(eventList_dialog), GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(eventList_dialog), 400, 400);
  gtk_window_set_resizable(GTK_WINDOW(eventList_dialog), FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(eventList_dialog), 10);
  gtk_window_set_title(GTK_WINDOW(eventList_dialog), "Event list");

  container_eventList_dialog = gtk_dialog_get_content_area(GTK_DIALOG(eventList_dialog));

  gtk_container_add(GTK_CONTAINER(container_eventList_dialog), fixed_eventList);

  gtk_widget_show_all(eventList_dialog);
}

void month_show()
{
  GtkWidget *container_month;
  GtkWidget *label_month;
  GtkWidget *button_enter;
  GtkWidget *error_label;

  month_dialog = gtk_dialog_new();

  label_month = gtk_label_new("Input month:");
  error_label = gtk_label_new("Invalid number. Try from 1 to 12 !");

  entry_month = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");

  container_month = gtk_dialog_get_content_area(GTK_DIALOG(month_dialog)); //đưa dialog vào xử lí tạo vùng để hiển thị

  gtk_window_set_position(GTK_WINDOW(month_dialog), GTK_WIN_POS_CENTER);

  gtk_container_add(GTK_CONTAINER(container_month), label_month);
  gtk_container_add(GTK_CONTAINER(container_month), entry_month);
  gtk_container_add(GTK_CONTAINER(container_month), error_label);
  gtk_container_add(GTK_CONTAINER(container_month), button_enter);

  gtk_widget_set_name(error_label, "error_label");

  g_signal_connect(button_enter, "clicked", G_CALLBACK(add_month_input), error_label);

  gtk_widget_show_all(month_dialog);

  gtk_widget_hide(error_label);
}

void year_show()
{
  GtkWidget *container_year;
  GtkWidget *label_year;
  GtkWidget *button_enter;
  GtkWidget *error_label;

  year_dialog = gtk_dialog_new();

  label_year = gtk_label_new("Input year:");
  error_label = gtk_label_new("Invalid number. Try from 1 to 9999 !");

  entry_year = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");

  container_year = gtk_dialog_get_content_area(GTK_DIALOG(year_dialog));

  gtk_window_set_position(GTK_WINDOW(year_dialog), GTK_WIN_POS_CENTER);

  gtk_container_add(GTK_CONTAINER(container_year), label_year);
  gtk_container_add(GTK_CONTAINER(container_year), entry_year);
  gtk_container_add(GTK_CONTAINER(container_year), error_label);
  gtk_container_add(GTK_CONTAINER(container_year), button_enter);

  gtk_widget_set_name(error_label, "error_label");

  g_signal_connect(button_enter, "clicked", G_CALLBACK(add_year_input), error_label);

  gtk_widget_show_all(year_dialog);

  gtk_widget_hide(error_label);
}

void today_set()
{
  update_month(month_today + 1);
  update_year(year_today);
  update_day(day_today);
}

void exit_screen()
{

  GtkWidget *exit_dialog, *container_exit;
  GtkWidget *label_ask;
  GtkWidget *fixed_exit;

  exit_dialog = gtk_dialog_new();

  label_ask = gtk_label_new("Are you want to Quit?\n");

  gtk_dialog_add_buttons(GTK_DIALOG(exit_dialog), "Yes", 1, "No", 2, NULL); // yes = 1 , no = 2

  g_signal_connect(exit_dialog, "delete-event", G_CALLBACK(destroy_widget), NULL);

  gtk_window_set_position(GTK_WINDOW(exit_dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(exit_dialog), 10);

  container_exit = gtk_dialog_get_content_area(GTK_DIALOG(exit_dialog));

  gtk_container_add(GTK_CONTAINER(container_exit), label_ask);

  gtk_widget_show_all(exit_dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(exit_dialog)); // add gia tri khi bam button

  switch (response)
  { // thuc hien ham khi gia tri = nhau
  case 1:
    gtk_main_quit();
    break;
  case 2:
    gtk_widget_destroy(GTK_WIDGET(exit_dialog));
    break;
  default:
    break;
  }
}

void logout_show() {
  GtkWidget *logout_dialog, *container_logout;
  GtkWidget *label_ask;

  logout_dialog = gtk_dialog_new();

  label_ask = gtk_label_new("Are you want to Logout?\n");

  gtk_dialog_add_buttons(GTK_DIALOG(logout_dialog), "Yes", 1, "No", 2, NULL); // yes = 1 , no = 2

  g_signal_connect(logout_dialog, "delete-event", G_CALLBACK(destroy_widget), NULL);

  gtk_window_set_position(GTK_WINDOW(logout_dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(logout_dialog), 10);

  container_logout = gtk_dialog_get_content_area(GTK_DIALOG(logout_dialog));

  gtk_container_add(GTK_CONTAINER(container_logout),label_ask);

  gtk_widget_show_all(logout_dialog);

  stop_loop_Main = 1;

  gint response = gtk_dialog_run(GTK_DIALOG(logout_dialog)); //add gia tri khi bam button

  switch (response)
  { // thuc hien ham khi gia tri = nhau
  case 1:
    gtk_widget_destroy(GTK_WIDGET(window));
    gtk_widget_show(login_dialog);
    gtk_widget_destroy(GTK_WIDGET(logout_dialog));
    break;
  case 2:
    stop_loop_Main = 0;
    gtk_widget_destroy(GTK_WIDGET(logout_dialog));
    break;
  }
}

void main_calendar()
{
  // add biến
  
  GtkWidget *button_exit, *button_logout,*button_event_list;
  GtkWidget *button_previous_month, *button_next_month, *button_previous_year, *button_next_year;
  GtkWidget *button_goto_day, *button_today;
  GtkWidget *time_label;
  GtkWidget *today_label, *choose_label;
  GtkWidget *box_info, *box_event_header, *box_event_main, *box_note;
  GtkWidget *event_show;
  GtkWidget *hello_label;
  GtkWidget *name_note;
  GtkWidget *event_on_specific_date;
  // set biến
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // cho app ở quyền cao nhất
  fixed_window = gtk_fixed_new();
  calendar = gtk_calendar_new();
  gtk_window_set_title(GTK_WINDOW(window), "Lịch thiểu năng");     // title cho app
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(window), 1580, 860);      // kích thước app mặc định
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);             // false sẽ làm cho app ko zoom out or in được
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);       // tạo khoảng cách giữa độ rộng tối đa của app và các thành phần bên trong


  // tạo button
  button_exit = gtk_button_new_with_label("Exit");
  button_logout = gtk_button_new_with_label("Logout");
  button_event_list = gtk_button_new_with_label("Event List");
  button_previous_month = gtk_button_new_with_label("<<");
  button_next_month = gtk_button_new_with_label(">>");
  button_previous_year = gtk_button_new_with_label("<<");
  button_next_year = gtk_button_new_with_label(">>");
  button_goto_day = gtk_button_new_with_label("Choose");
  button_today = gtk_button_new_with_label("Today");

  show_month = gtk_button_new_with_label("");
  show_year = gtk_button_new_with_label("");
  
  //----tạo box----
  box_info = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_event_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_event_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  box_note = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  // tạo label
  time_label = gtk_label_new("");
  today_label = gtk_label_new("");
  choose_label = gtk_label_new("");
  hello_label = gtk_label_new("");
  name_note = gtk_label_new("NOTE");
  event_on_specific_date = gtk_label_new("");

  //-----set label show show today--------//
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_today, &month_today, &day_today);
  update_month(month_today + 1);
  update_year(year_today);
  
  
  
  // tạo khả năng fixed cho từng thành phần và mặc định vị trí
  gtk_fixed_put(GTK_FIXED(fixed_window), button_exit, 1300, 760);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_logout, 1300, 150);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_event_list, 1300, 200);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_previous_month, 520, 150); //50
  gtk_fixed_put(GTK_FIXED(fixed_window), button_next_month, 930, 150);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_previous_year, 520, 65);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_next_year, 930, 65);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_goto_day, 387, 175);
  gtk_fixed_put(GTK_FIXED(fixed_window), button_today, 387, 135);
  gtk_fixed_put(GTK_FIXED(fixed_window), show_month, 680, 135);
  gtk_fixed_put(GTK_FIXED(fixed_window), show_year, 680, 30);
  gtk_fixed_put(GTK_FIXED(fixed_window), calendar, 320, 210);
  gtk_fixed_put(GTK_FIXED(fixed_window), box_event_header, 350, 690);
  gtk_fixed_put(GTK_FIXED(fixed_window), box_event_main, 350, 720);
  gtk_fixed_put(GTK_FIXED(fixed_window), box_info, 10, 50);
  gtk_fixed_put(GTK_FIXED(fixed_window), time_label, 20, 60);
  gtk_fixed_put(GTK_FIXED(fixed_window), today_label, 20, 90);
  gtk_fixed_put(GTK_FIXED(fixed_window), choose_label, 380, 695);
  gtk_fixed_put(GTK_FIXED(fixed_window), hello_label, 1300, 100);
  gtk_fixed_put(GTK_FIXED(fixed_window), box_note, 10, 180);
  gtk_fixed_put(GTK_FIXED(fixed_window), name_note, 120, 190);
  gtk_fixed_put(GTK_FIXED(fixed_window), event_on_specific_date, 450, 750);
   
    
  
  create_image();
  create_mark_day();
  
  // set biến thành id name để css có thể nhận dạng
  for(int i=1;i<=42;i++)
  {
     gtk_widget_set_name(mark_note_day[i], "mark_note_day");
     gtk_widget_set_name(mark_event_day[i], "mark_event_day");
  }  
  gtk_widget_set_name(button_exit, "button_menu");
  gtk_widget_set_name(button_logout, "button_menu");
  gtk_widget_set_name(button_event_list, "button_menu");

  gtk_widget_set_name(button_previous_month, "button_main");
  gtk_widget_set_name(button_next_month, "button_main");
  gtk_widget_set_name(button_previous_year, "button_main");
  gtk_widget_set_name(button_next_year, "button_main");

  gtk_widget_set_name(button_goto_day, "goto_day");
  gtk_widget_set_name(button_today, "goto_day");

  gtk_widget_set_name(box_info, "box_info");
  gtk_widget_set_name(box_event_header, "box_event_header");
  gtk_widget_set_name(box_event_main, "box_event_main");
  gtk_widget_set_name(box_note, "box_note");
  gtk_widget_set_name(name_note, "name_note");
  gtk_widget_set_name(event_on_specific_date, "eventName_in_box");

  gtk_widget_set_name(show_month, "show_month");
  gtk_widget_set_name(show_year, "show_year");
  gtk_widget_set_name(calendar, "calendar");
  
 

  // gọi hàm khi nhấn button
  g_signal_connect(window, "delete-event", G_CALLBACK(exit_screen), NULL); // tắt app
  g_signal_connect(button_exit, "clicked", G_CALLBACK(exit_screen), NULL);

  g_signal_connect(show_month, "clicked", G_CALLBACK(month_show), NULL);
  g_signal_connect(show_year, "clicked", G_CALLBACK(year_show), NULL);

  g_signal_connect(button_next_month, "clicked", G_CALLBACK(add_one_month), NULL);
  g_signal_connect(button_previous_month, "clicked", G_CALLBACK(minus_one_month), NULL);
  g_signal_connect(button_next_year, "clicked", G_CALLBACK(add_one_year), NULL);
  g_signal_connect(button_previous_year, "clicked", G_CALLBACK(minus_one_year), NULL);
  g_signal_connect(button_goto_day, "clicked", G_CALLBACK(goto_day_show), NULL);
  g_signal_connect(button_today, "clicked", G_CALLBACK(today_set), NULL);
  g_signal_connect(button_logout, "clicked", G_CALLBACK(logout_show), NULL);
  g_signal_connect(button_event_list, "clicked", G_CALLBACK(eventList_show), NULL);

  g_signal_connect(calendar, "day_selected_double_click", G_CALLBACK(addEvent_show_double_click), NULL);// when double click on day
  g_signal_connect(calendar,"day_selected",G_CALLBACK(show_note_list),NULL);

  connect_note();

  gtk_calendar_set_display_options(GTK_CALENDAR(calendar), 2); // 2 = calendar header (in there have year and month) and this function use to hide year and month

  gtk_container_add(GTK_CONTAINER(window), fixed_window);

  gtk_widget_show_all(window);

  for(int i=1;i<=42;i++)
  {
   gtk_widget_hide(mark_note_day[i]);
   gtk_widget_hide(mark_event_day[i]);
  }

  for(int i=0;i<=11;i++)gtk_widget_hide(zodiac[i]);

  year_choose=year_today;

  gtk_widget_show(zodiac[abs(year_choose+9)%12]);

  //---------------------------load---fuction---------------------------------//

  update_time(time_label);

  update_today(today_label);

  update_choose(choose_label);

  update_hello(hello_label);

  update_event(event_on_specific_date);

  show_mark_day();
  show_image();

  //-----------loop-----------//
  g_timeout_add(100, update_time, time_label);
  g_timeout_add(100, update_choose, choose_label);
  g_timeout_add(50, update_event, event_on_specific_date);
  g_timeout_add(100,show_mark_day,NULL);
  g_timeout_add(100,show_image,NULL);
}


int checkUserName(const gchar userNameTmp[])
{
  FILE *file_username;
  char userNameFile[MAX_LETTER];
  char passwordFile[MAX_LETTER];
  char line[256];
  int len = strlen(userNameTmp);
  int count = 0;
  file_username = fopen("acc2.txt", "r");
  do
  {

    if (count % 3 == 1) // dòng 2
     {
        fscanf(file_username, "Name: %s", &userNameFile);
       //  printf("%s", userNameFile);
     }
    if (strcmp(userNameTmp, userNameFile) == 0)
      return 0;
      count++;
  } 
  while (fgets(line, sizeof(line), file_username));
  if (strcasecmp(userNameTmp, "") == 0)
    return 0;

  return 1;

  fclose(file_username);
}

int checkPassword(const gchar *passwordTmp) // mat khau phai co tu 8 ki tu tro len va co chu hoa va chu thuong
{
  int length = strlen(passwordTmp), upperCase = 0, lowerCase = 0, number = 0;
  do
  {
    if (*passwordTmp != '\0')
    {
      if (*passwordTmp >= 65 && *passwordTmp <= 90) // do chu in hoa A(65) -> Z(90)
        upperCase++;
      if (*passwordTmp >= 97 && *passwordTmp <= 122) // do chu thuong a(97) -> z(122)
        lowerCase++;
      if (*passwordTmp >= 49 && *passwordTmp <= 58)
        number++;
      passwordTmp++;
    }
  } while (*passwordTmp != '\0'); // nếu kí tự != rỗng
  if ((upperCase > 0) && (lowerCase > 0) && (number > 0) && (length >= 8))
    return 1;
  return 0;
}

void login_callback()
{
  gtk_widget_destroy(register_dialog);
  gtk_widget_show(login_dialog);
  gtk_widget_hide(login_error_label);
  gtk_widget_destroy(popup_register);
}

void register_success()
{
  GtkWidget *label, *button;
  GtkWidget *container;

  popup_register = gtk_dialog_new();

  label = gtk_label_new("Sign Up Successfully");
  button = gtk_button_new_with_label("OK");

  gtk_widget_set_name(label, "success_label");

  gtk_window_set_position(GTK_WINDOW(popup_register), GTK_WIN_POS_CENTER);

  g_signal_connect(popup_register, "delete-event", G_CALLBACK(destroy_widget), NULL);

  g_signal_connect(button, "clicked", G_CALLBACK(login_callback), NULL);

  container = gtk_dialog_get_content_area(GTK_DIALOG(popup_register));

  gtk_container_add(GTK_CONTAINER(container), label);
  gtk_container_add(GTK_CONTAINER(container), button);

  gtk_widget_show_all(popup_register);
}

int signUp(GtkButton *button, gpointer data)
{
  passwordTmp = gtk_entry_get_text(GTK_ENTRY(password_entry));
  retypePassword = gtk_entry_get_text(GTK_ENTRY(retypePassword_entry));
  userNameTmp = gtk_entry_get_text(GTK_ENTRY(username_entry));
  char address[100];
  fflush(stdin);
  // printf(" Full name : ");
  // scanf("%[^\n]", &fullNameTmp);
  const char *fullNameTmp = gtk_entry_get_text(GTK_ENTRY(fullname_entry));
  // printf(" Username : ");

  if (checkUserName(userNameTmp) == 0)
  {
    gtk_widget_hide(error_wrong_format_pass);
    gtk_widget_show(error_username_available);
    gtk_widget_hide(error_retype_incorrect);
  }
  else if (checkPassword(passwordTmp) == 0)
  {
    gtk_widget_show(error_wrong_format_pass);
    gtk_widget_hide(error_username_available);
    gtk_widget_hide(error_retype_incorrect);
  }
  else if (strcasecmp(retypePassword, passwordTmp) == 0)
  {
    FILE *file_sign;
    // printf(" Sign up successfully. \n");
    gtk_widget_show(success_signup);
    gtk_widget_hide(error_wrong_format_pass);
    gtk_widget_hide(error_username_available);
    gtk_widget_hide(error_retype_incorrect);
    char path[200];
    sprintf(path,"%s\\acc2.txt",address_file_app);
    file_sign = fopen(path, "a");
    fprintf(file_sign, "Fullname: %s\nName: %s\nPass: %s\n", fullNameTmp, userNameTmp, passwordTmp);
    fclose(file_sign);
    strcpy(listUser[countUser][0], fullNameTmp); // gan cac fullname, username, password vao mang chinh
    strcpy(listUser[countUser][1], userNameTmp);
    strcpy(listUser[countUser][2], passwordTmp);
    creatFolder();
    countUser++;
    printf("%s\n", getcwd(address, 100));
    
  }
  else
  {
    gtk_widget_hide(error_wrong_format_pass);
    gtk_widget_hide(error_username_available);
    gtk_widget_show(error_retype_incorrect);
  }
}

void register_dialog_screen()
{ // màn hình register

  GtkWidget *container_register_dialog;
  GtkWidget *username_label, *password_label, *retypePassword_label, *fullname_label;

  GtkWidget *button_submit;
  GtkWidget *fixed_register;
  GtkWidget *register_label;
  GtkWidget *login_button;
  GtkWidget *login_label;

  register_dialog = gtk_dialog_new();
  fixed_register = gtk_fixed_new();

  button_submit = gtk_button_new_with_label("Register");
  login_button = gtk_button_new_with_label("Login now");

  fullname_label = gtk_label_new("Your full name");
  username_label = gtk_label_new("Username");
  password_label = gtk_label_new("Password");
  retypePassword_label = gtk_label_new("Re-type password");
  register_label = gtk_label_new("Register");
  login_label = gtk_label_new("Already have an account ?");

  error_username_available = gtk_label_new("Username already exists, please try another name!");
  error_retype_incorrect = gtk_label_new("Passwords do not match. Please try again !");
  error_wrong_format_pass = gtk_label_new("Wrong format. At least 8 characters with uppercase letters and numbers.");
  success_signup = gtk_label_new("Sign up successfully.");
  fullname_entry = gtk_entry_new();
  username_entry = gtk_entry_new();
  password_entry = gtk_entry_new();
  retypePassword_entry = gtk_entry_new();

  gtk_widget_set_name(button_submit, "button_submit");
  gtk_widget_set_name(register_label, "login_register_label");
  gtk_widget_set_name(login_button, "login_register_button");
  gtk_widget_set_name(register_dialog, "login_register_dialog");
  gtk_widget_set_name(error_username_available, "error_label");
  gtk_widget_set_name(error_retype_incorrect, "error_label");
  gtk_widget_set_name(error_wrong_format_pass, "error_label");
  gtk_widget_set_name(success_signup, "success_label");

  gtk_entry_set_width_chars(GTK_ENTRY(username_entry), 30);
  gtk_entry_set_width_chars(GTK_ENTRY(password_entry), 30);
  gtk_entry_set_width_chars(GTK_ENTRY(retypePassword_entry), 30);
  gtk_entry_set_width_chars(GTK_ENTRY(fullname_entry), 30);

  gtk_fixed_put(GTK_FIXED(fixed_register), username_label, 160, 222);
  gtk_fixed_put(GTK_FIXED(fixed_register), password_label, 160, 302);
  gtk_fixed_put(GTK_FIXED(fixed_register), fullname_label, 160, 142);
  gtk_fixed_put(GTK_FIXED(fixed_register), fullname_entry, 160, 170);
  gtk_fixed_put(GTK_FIXED(fixed_register), username_entry, 160, 250);
  gtk_fixed_put(GTK_FIXED(fixed_register), password_entry, 160, 330);
  gtk_fixed_put(GTK_FIXED(fixed_register), button_submit, 200, 500);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_label, 160, 382);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_entry, 160, 410);
  gtk_fixed_put(GTK_FIXED(fixed_register), register_label, 200, 50);
  gtk_fixed_put(GTK_FIXED(fixed_register), error_username_available, 120, 282);
  gtk_fixed_put(GTK_FIXED(fixed_register), error_retype_incorrect, 153, 444);
  gtk_fixed_put(GTK_FIXED(fixed_register), error_wrong_format_pass, 50, 364);
  gtk_fixed_put(GTK_FIXED(fixed_register), login_button, 325, 545);
  gtk_fixed_put(GTK_FIXED(fixed_register), login_label, 160, 550);
  gtk_fixed_put(GTK_FIXED(fixed_register), success_signup, 223, 470);

  gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
  gtk_entry_set_visibility(GTK_ENTRY(retypePassword_entry), FALSE);

  gtk_window_set_title(GTK_WINDOW(register_dialog), "Register");
  gtk_window_set_position(GTK_WINDOW(register_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(register_dialog), 580, 620);
  gtk_window_set_resizable(GTK_WINDOW(register_dialog), FALSE);

  g_signal_connect(GTK_DIALOG(register_dialog), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(login_button, "clicked", G_CALLBACK(login_callback), NULL);
  g_signal_connect(button_submit, "clicked", G_CALLBACK(signUp), register_dialog);
  // g_signal_connect(login_button,"clicked",G_CALLBACK(login_dialog),NULL);

  container_register_dialog = gtk_dialog_get_content_area(GTK_DIALOG(register_dialog)); // tao phan vung chua content cho dialog

  gtk_container_add(GTK_CONTAINER(container_register_dialog), fixed_register);

  gtk_widget_show_all(register_dialog);

  gtk_widget_hide(login_dialog);

  gtk_widget_hide(error_username_available);
  gtk_widget_hide(error_retype_incorrect);
  gtk_widget_hide(error_wrong_format_pass);
  gtk_widget_hide(success_signup);
}

int login(GtkButton *button, gpointer data)
{
  userNameTmp = gtk_entry_get_text(GTK_ENTRY(username_login_entry));
  const char *passwordTmp = gtk_entry_get_text(GTK_ENTRY(password_login_entry));
  char userNameFile[MAX_LETTER];
  char passwordFile[MAX_LETTER];
  char line[256];
  int check = 0;
  countUser = 0; // reset lại bắt đầu đọc file để gán không bị lệch

  FILE *file_login;
  char path[200];
  sprintf(path,"%s\\acc2.txt",address_file_app);
  file_login = fopen(path, "r");

  int count = 0; // đếm dòng
  do
  {
    if (count % 3 == 0) // dòng 1
    {
      fscanf(file_login, "Fullname: %[^\n]", &fullNameFile);
      // printf("%s", fullNameFile); // dùng để check xem đọc file đúng chưa
    }
    if (count % 3 == 1) // dòng 2
     {
        fscanf(file_login, "Name: %s", &userNameFile);
       //  printf("%s", userNameFile);
     }
    if (count % 3 == 2) // dòng 3
    {
      fscanf(file_login, "Pass: %s", &passwordFile);
      // printf("%s", passwordFile);
      if (strcmp(userNameTmp, userNameFile) == 0 && strcmp(passwordTmp, passwordFile) == 0)
      {
        check = 1; 
      }
      strcpy(listUser[countUser][0], fullNameFile); // gan cac fullname, username, password vao mang chinh
      strcpy(listUser[countUser][1], userNameFile); // cập nhật vào mảng
      strcpy(listUser[countUser][2], passwordFile);
      countUser++; // cập nhật số user
    }
    count++;
  } while (fgets(line, sizeof(line), file_login));

  if (check == 1)
  {
    for (int i = 0; i < countUser; i++)
    {
      if (strcmp(userNameTmp, listUser[i][1]) == 0)
        indexOfUser = i;
    }
    strcpy(address_replace,address_file_Users);
    strcat(address_replace,listUser[indexOfUser][1]);
    chdir(address_replace);
    take_today();
    copy_today();
    main_calendar();
    gtk_widget_hide(login_dialog);
    stop_loop_Main = 0;
  }
  else
  {
    gtk_widget_show(data); 
  }
  fclose(file_login);
}

void forgot_pass() {
  GtkWidget *dialog;
  GtkWidget *label, *button;
  GtkWidget *container;

  dialog = gtk_dialog_new();

  label = gtk_label_new("\n Relax and try to remember ! \n");
  button = gtk_button_new_with_label("Nice :)");

  gtk_widget_set_name(label, "success_label");

  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

  g_signal_connect(dialog, "delete-event", G_CALLBACK(destroy_widget), NULL);

  g_signal_connect(button, "clicked", G_CALLBACK(destroy_widget), dialog);

  container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(container), label);
  gtk_container_add(GTK_CONTAINER(container), button);

  gtk_widget_show_all(dialog);
}

void login_dialog_screen()
{ // màn hình login
  GtkWidget *container_login_dialog;
  GtkWidget *username_label, *password_label;
  GtkWidget *button_submit;
  GtkWidget *fixed_login;
  GtkWidget *register_button, *forgot_button;
  GtkWidget *register_label;
  GtkWidget *login_label;

  login_dialog = gtk_dialog_new();

  fixed_login = gtk_fixed_new();

  button_submit = gtk_button_new_with_label("Login");

  login_label = gtk_label_new("Login");
  username_label = gtk_label_new("Username");
  password_label = gtk_label_new("Password");
  register_label = gtk_label_new("Don't have an account ?");
  // login_error_label = gtk_label_new("The user name or password that you have entered is incorrect.");
  login_error_label = gtk_label_new("Invaild Username or Password ! Try again !");

  username_login_entry = gtk_entry_new();
  password_login_entry = gtk_entry_new();

  register_button = gtk_button_new_with_label("Register now");
  forgot_button = gtk_button_new_with_label("Forgot Password ?");

  gtk_entry_set_width_chars(GTK_ENTRY(username_login_entry), 30); // chỉnh kích thước entry theo số lượng chữ
  gtk_entry_set_width_chars(GTK_ENTRY(password_login_entry), 30); // chỉnh kích thước entry theo số lượng chữ

  gtk_widget_set_name(button_submit, "button_submit");
  gtk_widget_set_name(login_dialog, "login_register_dialog");
  gtk_widget_set_name(register_button, "login_register_button");
  gtk_widget_set_name(forgot_button, "forgot_button");
  gtk_widget_set_name(login_label, "login_register_label");
  gtk_widget_set_name(login_error_label, "error_label");

  gtk_fixed_put(GTK_FIXED(fixed_login), username_label, 50, 272);
  gtk_fixed_put(GTK_FIXED(fixed_login), password_label, 50, 352);
  gtk_fixed_put(GTK_FIXED(fixed_login), username_login_entry, 50, 300);
  gtk_fixed_put(GTK_FIXED(fixed_login), password_login_entry, 50, 380);
  gtk_fixed_put(GTK_FIXED(fixed_login), button_submit, 90, 480);
  gtk_fixed_put(GTK_FIXED(fixed_login), register_button, 200, 545);
  gtk_fixed_put(GTK_FIXED(fixed_login), forgot_button, 165, 420);
  gtk_fixed_put(GTK_FIXED(fixed_login), register_label, 50, 550);
  gtk_fixed_put(GTK_FIXED(fixed_login), login_label, 110, 150);
  gtk_fixed_put(GTK_FIXED(fixed_login), login_error_label, 40, 450);

  gtk_window_set_title(GTK_WINDOW(login_dialog), "Login");
  gtk_window_set_position(GTK_WINDOW(login_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(login_dialog), 380, 620);
  gtk_window_set_resizable(GTK_WINDOW(login_dialog), FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(login_dialog), 10);

  gtk_entry_set_visibility(GTK_ENTRY(password_login_entry), FALSE); // che lại khi nhập mật khẩu

  g_signal_connect(GTK_DIALOG(login_dialog), "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(register_button, "clicked", G_CALLBACK(register_dialog_screen), NULL);
  g_signal_connect(button_submit, "clicked", G_CALLBACK(login), login_error_label);
  g_signal_connect(forgot_button, "clicked", G_CALLBACK(forgot_pass), NULL);
  
  container_login_dialog = gtk_dialog_get_content_area(GTK_DIALOG(login_dialog));

  gtk_container_add(GTK_CONTAINER(container_login_dialog), fixed_login);

  gtk_widget_show_all(login_dialog);

  gtk_widget_hide(login_error_label);
}

int main(int argc, char *argv[])
{ // mainde03x

  gtk_init(&argc, &argv);
  //---gọi hàm---//
  get_address_Users_file();
  login_dialog_screen();

  load_css();

  gtk_main();
  //----------không viết gì dưới gtk_main() vì đây là điểm kết thúc-----------------------------//
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file
// gcc `pkg-config --cflags gtk+-3.0` -o app okay.c `pkg-config --libs gtk+-3.0`
// ./app