

//--------------------Thành------------------------//
#include <gtk/gtk.h>
// #include <windows.h>
// #include <stdio.h>
#include <stdlib.h>
// #include <time.h>

//---------------Hân--------//

//------------------------------------------Hân---------Authentication---------------------------------------------//
#include <math.h>
#include <conio.h>
// chuyen doi ngay

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

int jdToDate(int jd) // chuyen ngay julius qua ngay thuong
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

int getLeapMonthOffset(int a11) // xac dinh thang nhuan
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

void convertSolar2Lunar(int dd, int mm, int yy)
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
  int ngay = lunarDay;
  int thang = lunarMonth;
  int nam = lunarYear;
  if (ngay == 23 && thang == 12)
    printf("Kitchen guardians");
  if (ngay == 1 && thang == 1)
    printf("Vietnamese New Year");
  if (ngay == 2 && thang == 1)
    printf("Tet holiday");
  if (ngay == 3 && thang == 1)
    printf("Tet holiday");
  if (ngay == 4 && thang == 1)
    printf("Tet holiday");
  if (ngay == 5 && thang == 1)
    printf("Tet holiday");
  if (ngay == 15 && thang == 8)
    printf("Mid autumn festivals");
  if (ngay == 10 && thang == 3)
    printf("Hung Kings Festival");
}
// ngày lễ ở lịch âm
void add_lunar_event(gpointer label)
{
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  convertSolar2Lunar(day_select, month_select, year_select);
}

void mark_solar_event()
{
  FILE *f;
  f = fopen("SolarEvent.txt", "r");
  int day, month, year;
  char data[100];
  if(f != NULL)
    while (fscanf(f, "%d %d %s", &day, &month, &data))
    {
    // gọi hàm đánh dấu ngày lễ lên giao diện tham sô (day, month)
   }
  fclose(f);
}
void read_day_note()
{
  FILE *f;
  f = fopen("SolarEvent.txt", "r");
  int day, month, year;
  char data[100];
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select);
  if( f != NULL)
    while (fscanf(f, "%d %d %s", &day, &month, &data) > 0)
   {
    
     if (day == day_select && month == month_select)
     
    // in data vào khung event
   }
  fclose(f);
  f = fopen("./abc/abcData.txt", "r");
  while (f != NULL)
  {
    fscanf(f, "%d %d %d %s", &day, &month, &year, &data);
    if (day == day_select && month == month_select && year == year_select)
    // in data vào khung note
     printf("%d %d % %s", day, month, year, data);
  }
  fclose(f);
}
//-------------------------------------Thành-------GTK-----------------------------------------------------//

// khai biến ở đây để tất cả các function đều truy cập được
GtkWidget *entry_year, *entry_month;                            // in function month_show and next from year_show
GtkWidget *show_month, *show_year;                              // in function main
GtkWidget *calendar;                                            // in fuction main
GtkWidget *goto_day_entry, *goto_month_entry, *goto_year_entry; // in function goto_day_show
GtkWidget *goto_dialog, *year_dialog, *month_dialog;            // in function goto_day_show and next from year_show and next from month_show

char *monthList[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}; // loại bỏ vị trí 0

guint year_today, month_today, day_today; // in function main
//----------------------//

//--------------------------------//
static void load_css()
{
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "theme_new.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

gboolean update_time(gpointer label)
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  gchar *display = g_strdup_printf("Clock:\t\t%02d : %02d : %02d (hh/mm/ss)", tm.tm_hour, tm.tm_min, tm.tm_sec);
  gtk_label_set_text(GTK_LABEL(label), display);

  // return G_SOURCE_CONTINUE;
}

void update_today(gpointer label)
{
  gchar *display = g_strdup_printf("Today:\t\t%02d/%02d/%d (dd/mm/yyyy)", day_today, month_today + 1, year_today);
  gtk_label_set_text(GTK_LABEL(label), display);
}

gboolean update_choose(gpointer label)
{
  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select); // chọn ngày đã được chọn lưu vào []_select
  gchar *display = g_strdup_printf("You choose:\t%02d/%02d/%d (dd/mm/yyyy)", day_select, month_select + 1, year_select);
  gtk_label_set_text(GTK_LABEL(label), display);
}

void destroy(gpointer *data, GtkWidget *widget)
{
  gtk_widget_destroy(widget);
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

void addEvent_show()
{
  GtkWidget *dialog, *container, *fixed;
  GtkWidget *name_label, *day_label, *month_label, *year_label, *note_label;
  GtkWidget *name_entry, *day_entry, *month_entry, *year_entry, *note_entry;
  GtkWidget *button;

  dialog = gtk_dialog_new();
  fixed = gtk_fixed_new();

  name_label = gtk_label_new("Name:");
  day_label = gtk_label_new("Day:");
  month_label = gtk_label_new("Month:");
  year_label = gtk_label_new("Year:");
  note_label = gtk_label_new("Note:");
}

void addEvent_show_double_click()
{
  GtkWidget *dialog, *container;
  GtkWidget *name_event_entry, *note_event_entry;
  GtkWidget *name_event_label, *note_event_label;
  GtkWidget *fixed;
  GtkWidget *show_day_label;

  gchar *display_update;

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_select, &month_select, &day_select); // chọn ngày đã được chọn lưu vào []_select

  dialog = gtk_dialog_new();
  fixed = gtk_fixed_new();

  name_event_entry = gtk_entry_new();
  note_event_entry = gtk_entry_new();

  name_event_label = gtk_label_new("Name:");
  note_event_label = gtk_label_new("Note:");
  show_day_label = gtk_label_new("");

  display_update = g_strdup_printf("Date:\t%02d/%02d/%02d (dd/mm/yyyy)", day_select, month_select + 1, year_select);

  gtk_label_set_text(GTK_LABEL(show_day_label), display_update);

  gtk_dialog_add_buttons(GTK_DIALOG(dialog), "Ok", 1, "Cancel", 2, NULL); // ok = 1 vs cancel = 2

  gtk_fixed_put(GTK_FIXED(fixed), name_event_entry, 70, 45);
  gtk_fixed_put(GTK_FIXED(fixed), note_event_entry, 70, 105);
  gtk_fixed_put(GTK_FIXED(fixed), name_event_label, 10, 50);
  gtk_fixed_put(GTK_FIXED(fixed), note_event_label, 10, 110);
  gtk_fixed_put(GTK_FIXED(fixed), show_day_label, 10, 10);

  gtk_widget_set_name(note_event_entry, "note_event_entry");

  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(dialog), "Add event");
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

  container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(container), fixed);

  gtk_widget_show_all(dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog)); // add gia tri khi bam button
  switch (response)
  { // thuc hien ham khi gia tri = nhau
  case 1:
    gtk_widget_destroy(GTK_WIDGET(dialog));
    break;
  case 2:
    gtk_widget_destroy(GTK_WIDGET(dialog));
    break;
  }
}

void eventList_show()
{
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

  label_ask = gtk_label_new("Are you want to Quit?");

  gtk_dialog_add_buttons(GTK_DIALOG(exit_dialog), "Yes", 1, "No", 2, NULL); // yes = 1 , no = 2

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

void login_dialog_screen()
{ // màn hình login

  GtkWidget *login_dialog, *container_login_dialog;
  GtkWidget *username_label, *password_label;
  GtkWidget *username_entry, *password_entry;
  GtkWidget *button_submit;
  GtkWidget *fixed_login;

  login_dialog = gtk_dialog_new();

  fixed_login = gtk_fixed_new();

  button_submit = gtk_button_new_with_label("Login");

  username_label = gtk_label_new("Username");
  password_label = gtk_label_new("Password");

  username_entry = gtk_entry_new();
  password_entry = gtk_entry_new();

  gtk_widget_set_name(button_submit, "button_submit");
  gtk_widget_set_name(username_label, "authentication_label");
  gtk_widget_set_name(password_label, "authentication_label");
  gtk_widget_set_name(username_entry, "authentication_entry");
  gtk_widget_set_name(password_entry, "authentication_entry");

  gtk_fixed_put(GTK_FIXED(fixed_login), username_label, 100, 100);
  gtk_fixed_put(GTK_FIXED(fixed_login), password_label, 100, 150);
  gtk_fixed_put(GTK_FIXED(fixed_login), username_entry, 300, 100);
  gtk_fixed_put(GTK_FIXED(fixed_login), password_entry, 300, 150);
  gtk_fixed_put(GTK_FIXED(fixed_login), button_submit, 235, 250);

  gtk_window_set_title(GTK_WINDOW(login_dialog), "Login");
  gtk_window_set_position(GTK_WINDOW(login_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(login_dialog), 640, 410);
  gtk_window_set_resizable(GTK_WINDOW(login_dialog), FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(login_dialog), 10);

  container_login_dialog = gtk_dialog_get_content_area(GTK_DIALOG(login_dialog));

  gtk_container_add(GTK_CONTAINER(container_login_dialog), fixed_login);

  gtk_widget_show_all(login_dialog);
}

void register_dialog_screen()
{ // màn hình register

  GtkWidget *register_dialog, *container_register_dialog;
  GtkWidget *username_label, *password_label, *retypePassword_label;
  GtkWidget *username_entry, *password_entry, *retypePassword_entry;
  GtkWidget *button_submit;
  GtkWidget *fixed_register;

  register_dialog = gtk_dialog_new();

  fixed_register = gtk_fixed_new();

  button_submit = gtk_button_new_with_label("Register");

  username_label = gtk_label_new("Username");
  password_label = gtk_label_new("Password");
  retypePassword_label = gtk_label_new("Re-type password");

  username_entry = gtk_entry_new();
  password_entry = gtk_entry_new();
  retypePassword_entry = gtk_entry_new();

  gtk_widget_set_name(button_submit, "button_submit");
  gtk_widget_set_name(username_label, "authentication_label");
  gtk_widget_set_name(password_label, "authentication_label");
  gtk_widget_set_name(username_entry, "authentication_entry");
  gtk_widget_set_name(password_entry, "authentication_entry");
  gtk_widget_set_name(retypePassword_label, "authentication_label");
  gtk_widget_set_name(retypePassword_entry, "authentication_entry");

  gtk_fixed_put(GTK_FIXED(fixed_register), username_label, 100, 100);
  gtk_fixed_put(GTK_FIXED(fixed_register), password_label, 100, 150);
  gtk_fixed_put(GTK_FIXED(fixed_register), username_entry, 300, 100);
  gtk_fixed_put(GTK_FIXED(fixed_register), password_entry, 300, 150);
  gtk_fixed_put(GTK_FIXED(fixed_register), button_submit, 235, 300);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_label, 100, 200);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_entry, 300, 200);

  gtk_window_set_title(GTK_WINDOW(register_dialog), "Register");
  gtk_window_set_position(GTK_WINDOW(register_dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(register_dialog), 640, 410);
  gtk_window_set_resizable(GTK_WINDOW(register_dialog), FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(register_dialog), 10);

  container_register_dialog = gtk_dialog_get_content_area(GTK_DIALOG(register_dialog)); // tao phan vung chua content cho dialog

  gtk_container_add(GTK_CONTAINER(container_register_dialog), fixed_register);

  gtk_widget_show_all(register_dialog);
}

int main(int argc, char *argv[])
{ // main

  gtk_init(&argc, &argv);

  // add biến
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *button_exit, *button_login, *button_register, *button_logout, *button_add_event, *button_delete_event, *button_event_list;
  GtkWidget *button_previous_month, *button_next_month, *button_previous_year, *button_next_year;
  GtkWidget *button_goto_day, *button_today;
  GtkWidget *time_label;
  GtkWidget *today_label, *choose_label;
  GtkWidget *box_info;
  GtkWidget *event_show;

  // set biến
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // cho app ở quyền cao nhất
  fixed = gtk_fixed_new();
  calendar = gtk_calendar_new();

  gtk_window_set_title(GTK_WINDOW(window), "Lịch thiểu năng");     // title cho app
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(window), 1580, 860);      // kích thước app mặc định
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);             // false sẽ làm cho app ko zoom out or in được
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);       // tạo khoảng cách giữa độ rộng tối đa của app và các thành phần bên trong

  // tạo button
  button_exit = gtk_button_new_with_label("Exit");
  button_login = gtk_button_new_with_label("Login");
  button_register = gtk_button_new_with_label("Register");
  button_logout = gtk_button_new_with_label("Logout");
  button_add_event = gtk_button_new_with_label("Add Event");
  button_delete_event = gtk_button_new_with_label("Delete Event");
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
  box_info = gtk_button_new();

  // tạo label
  time_label = gtk_label_new("");
  today_label = gtk_label_new("");
  choose_label = gtk_label_new("");

  //-----set label show show today--------//
  gtk_calendar_get_date(GTK_CALENDAR(calendar), &year_today, &month_today, &day_today);
  update_month(month_today + 1);
  update_year(year_today);

  // tạo khả năng fixed cho từng thành phần và mặc định vị trí
  gtk_fixed_put(GTK_FIXED(fixed), button_exit, 1300, 760);
  gtk_fixed_put(GTK_FIXED(fixed), button_login, 1300, 100);
  gtk_fixed_put(GTK_FIXED(fixed), button_add_event, 1300, 250);
  gtk_fixed_put(GTK_FIXED(fixed), button_delete_event, 1300, 300);
  gtk_fixed_put(GTK_FIXED(fixed), button_register, 1300, 150);
  gtk_fixed_put(GTK_FIXED(fixed), button_event_list, 1300, 350);
  gtk_fixed_put(GTK_FIXED(fixed), button_previous_month, 440, 150);
  gtk_fixed_put(GTK_FIXED(fixed), button_next_month, 850, 150);
  gtk_fixed_put(GTK_FIXED(fixed), button_previous_year, 440, 65);
  gtk_fixed_put(GTK_FIXED(fixed), button_next_year, 850, 65);
  gtk_fixed_put(GTK_FIXED(fixed), button_goto_day, 337, 175);
  gtk_fixed_put(GTK_FIXED(fixed), button_today, 337, 135);
  gtk_fixed_put(GTK_FIXED(fixed), show_month, 600, 135);
  gtk_fixed_put(GTK_FIXED(fixed), show_year, 600, 30);
  gtk_fixed_put(GTK_FIXED(fixed), calendar, 270, 210);
  gtk_fixed_put(GTK_FIXED(fixed), box_info, 10, 50);
  gtk_fixed_put(GTK_FIXED(fixed), time_label, 20, 60);
  gtk_fixed_put(GTK_FIXED(fixed), today_label, 20, 90);
  gtk_fixed_put(GTK_FIXED(fixed), choose_label, 20, 120);

  // set biến thành id name để css có thể nhận dạng
  gtk_widget_set_name(button_exit, "button_menu");
  gtk_widget_set_name(button_login, "button_menu");
  gtk_widget_set_name(button_logout, "button_menu");
  gtk_widget_set_name(button_add_event, "button_menu");
  gtk_widget_set_name(button_register, "button_menu");
  gtk_widget_set_name(button_delete_event, "button_menu");
  gtk_widget_set_name(button_event_list, "button_menu");

  gtk_widget_set_name(button_previous_month, "button_main");
  gtk_widget_set_name(button_next_month, "button_main");
  gtk_widget_set_name(button_previous_year, "button_main");
  gtk_widget_set_name(button_next_year, "button_main");

  gtk_widget_set_name(button_goto_day, "goto_day");
  gtk_widget_set_name(button_today, "goto_day");

  gtk_widget_set_name(show_month, "show_month");
  gtk_widget_set_name(show_year, "show_year");
  gtk_widget_set_name(calendar, "calendar");
  gtk_widget_set_name(box_info, "box_info");

  // gọi hàm khi nhấn button
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // tắt app
  g_signal_connect(button_exit, "clicked", G_CALLBACK(exit_screen), NULL);
  g_signal_connect(button_login, "clicked", G_CALLBACK(login_dialog_screen), NULL);
  g_signal_connect(button_register, "clicked", G_CALLBACK(register_dialog_screen), NULL);

  g_signal_connect(show_month, "clicked", G_CALLBACK(month_show), NULL);
  g_signal_connect(show_year, "clicked", G_CALLBACK(year_show), NULL);

  g_signal_connect(button_next_month, "clicked", G_CALLBACK(add_one_month), NULL);
  g_signal_connect(button_previous_month, "clicked", G_CALLBACK(minus_one_month), NULL);
  g_signal_connect(button_next_year, "clicked", G_CALLBACK(add_one_year), NULL);
  g_signal_connect(button_previous_year, "clicked", G_CALLBACK(minus_one_year), NULL);
  g_signal_connect(button_goto_day, "clicked", G_CALLBACK(goto_day_show), NULL);
  g_signal_connect(button_today, "clicked", G_CALLBACK(today_set), NULL);

  g_signal_connect(calendar, "day_selected_double_click", G_CALLBACK(addEvent_show_double_click), NULL); // when double click on day

  gtk_calendar_set_display_options(GTK_CALENDAR(calendar), 2); // 2 = calendar header (in there have year and month) and this function use to hide year and month

  gtk_container_add(GTK_CONTAINER(window), fixed);

  gtk_widget_show_all(window);

  //---------------------------load---fuction---------------------------------//
  load_css(); // gọi hàm load_css

  update_time(time_label);

  update_today(today_label);

  //-----------loop-----------//
  g_timeout_add(1000, update_time, time_label);
  g_timeout_add(100, update_choose, choose_label);

  //---------------------------C---------------------------------//
  //-----------------------------test------------------//
  //----------------------------------------------------/
  gtk_main();
  //----------không viết gì dưới gtk_main() vì đây là điểm kết thúc-----------------------------//
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file
// gcc `pkg-config --cflags gtk+-3.0` -o app gtk_new.c `pkg-config --libs gtk+-3.0`
// ./app
