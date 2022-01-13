


//--------------------Thành------------------------//
#include <gtk/gtk.h>
// #include <windows.h>
#include <stdio.h> 
#include <stdlib.h>

//-----------------------------------------Thọ---------Calendar----------------------------------------------//

//------------------------------------------Hân---------Authentication---------------------------------------------//

//-------------------------------------Thành-------GTK-----------------------------------------------------//

// khai biến ở đây để tất cả các function đều truy cập được
GtkWidget *entry_year, *entry_month; // in function month_show and year_show
GtkWidget *show_month, *show_year; //in function main
GtkWidget *calendar;  //in fuction main
GtkWidget *goto_day_entry, *goto_month_entry, *goto_year_entry; // in function goto_day_show

char *monthList[] = {"","January","February","March","April","May","June","July","August","September","October","November","December"}; //loại bỏ vị trí 0

guint year_today, month_today, day_today;// in function main
//----------------------//

//--------------------------------//
static void load_css() {
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "theme_new.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

void update_time(GtkLabel *label_data) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  gchar *display = g_strdup_printf("Opened at:\t%02d : %02d : %02d",tm.tm_hour, tm.tm_min, tm.tm_sec);
  gtk_label_set_text(GTK_LABEL(label_data),display);  
}

void destroy(gpointer *data, GtkWidget *widget) {
  gtk_widget_destroy(widget);
}

void update_year(int year) { //cập nhật label trong button 
  gchar *display;
  display = g_strdup_printf("%d", year); // chuyển int thành char
  gtk_button_set_label(GTK_BUTTON(show_year), display); //vì set label phải chuyển thành char mớ nhận được

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar),&year_select, &month_select, &day_select);
  gtk_calendar_select_month(GTK_CALENDAR(calendar),month_select, year);
}

void update_month(int month) { //cập nhật label trong button 
  gtk_button_set_label(GTK_BUTTON(show_month), monthList[month]); //vì set label phải chuyển thành char mớ nhận được

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar),&year_select, &month_select, &day_select);
  gtk_calendar_select_month(GTK_CALENDAR(calendar),month - 1, year_select);
}

void update_day(int day) {
  gtk_calendar_select_day(GTK_CALENDAR(calendar),day);
}

void add_year_input() { // lấy dữ liệu từ input year
  int data = atoi(gtk_entry_get_text(GTK_ENTRY(entry_year))); // atoi là biến chuỗi số thành giá trị số

  if (data < 1){ //nếu year input < 1 thì year = 1
    data = 1; 
    update_year(data);
  }
  else {
    update_year(data);
  }
}

void add_month_input() { // lấy dữ liệu từ input month
  int data = atoi(gtk_entry_get_text(GTK_ENTRY(entry_month))); 

  if (data < 1 || data > 12){ //nếu month input < 1 hoặc > 12 thì month = 1
    data = 1;
    update_month(data);
  }
  else {
    update_month(data);
  }
}

void add_one_year() {
  int add = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
  add += 1;
  update_year(add);
}

void minus_one_year() {
  int minus = atoi(gtk_button_get_label(GTK_BUTTON(show_year)));
  minus -= 1;
  if(minus < 1) {
    minus = 1;
    update_year(minus);
  }
  else {
    update_year(minus);
  }
}

void add_one_month() {
  gchar *get_charAdd1, *get_charAdd2;
  for (int i = 1; i < 13; i++) {
    get_charAdd1 = g_strdup_printf("%s",monthList[i]); // lấy chuỗi trong monthList
    get_charAdd2 = g_strdup_printf("%s", gtk_button_get_label(GTK_BUTTON(show_month))); // lấy chuỗi của label month
    if( strcmp(get_charAdd1, get_charAdd2) == 0) { // nếu hai chuỗi = nhau thì = 0 ==> chạy if \ strcmp so sánh hai chuỗi, = nhau thì = 0
      int add = i;
      add += 1;
      if (add > 12) { // nếu value month > 12 thì month = 1
        add = 1; 
        update_month(add);
        // nếu hơn tháng 12 thì year + 1
        int year = atoi(gtk_button_get_label(GTK_BUTTON(show_year))); 
        year += 1;
        update_year(year);
        break; // dừng loop để trả đúng kết quả i
      }

      else {
        update_month(add);
        break;
      }
    }
  }
}

void minus_one_month() {
  gchar *get_charMinus1, *get_charMinus2;
  for (int i = 1; i < 13; i++) {
    get_charMinus1 = g_strdup_printf("%s",monthList[i]);
    get_charMinus2 = g_strdup_printf("%s", gtk_button_get_label(GTK_BUTTON(show_month)));
    if(strcmp(get_charMinus1, get_charMinus2) == 0) {
      int minus = i;
      minus -= 1;
      if (minus < 1) { // nếu value month > 12 thì month = 1
        minus = 12; 
        update_month(minus);

        int year = atoi(gtk_button_get_label(GTK_BUTTON(show_year))); 
        if (year < 2) { // nếu year nhỏ hơn 2 thì year tối đa chỉ đến year = 1
          year = 1;
          update_year(year);
          break;
        }
        else {
          year -= 1;
          update_year(year);
          break;
        }
      }
      else {
        update_month(minus);
        break;
      }
    }
  }
}

void goto_activate() {
  int day = atoi(gtk_entry_get_text(GTK_ENTRY(goto_day_entry)));
  int month = atoi(gtk_entry_get_text(GTK_ENTRY(goto_month_entry)));
  int year = atoi(gtk_entry_get_text(GTK_ENTRY(goto_year_entry)));
  update_month(month);
  update_year(year);
  update_day(day);
}

void goto_day_show() {
  GtkWidget *dialog, *container;
  GtkWidget *day_label, *month_label, *year_label;
  GtkWidget *fixed;
  GtkWidget *button;

  dialog = gtk_dialog_new();
  fixed = gtk_fixed_new();

  goto_day_entry = gtk_entry_new();
  goto_month_entry = gtk_entry_new();
  goto_year_entry = gtk_entry_new();

  day_label = gtk_label_new("Day:");
  month_label = gtk_label_new("Month:");
  year_label = gtk_label_new("Year:");

  button = gtk_button_new_with_label("Go to");

  gtk_fixed_put(GTK_FIXED(fixed), goto_day_entry, 70, 20);
  gtk_fixed_put(GTK_FIXED(fixed), goto_month_entry, 70, 60);
  gtk_fixed_put(GTK_FIXED(fixed), goto_year_entry, 70, 100);
  gtk_fixed_put(GTK_FIXED(fixed), day_label, 10, 20);
  gtk_fixed_put(GTK_FIXED(fixed), month_label, 10, 60);
  gtk_fixed_put(GTK_FIXED(fixed), year_label, 10, 100);
  gtk_fixed_put(GTK_FIXED(fixed), button, 90, 150);

  gtk_widget_set_name(goto_day_entry,"goto_entry");
  gtk_widget_set_name(goto_month_entry,"goto_entry");
  gtk_widget_set_name(goto_year_entry,"goto_entry");

  gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);

  g_signal_connect(button,"clicked",G_CALLBACK(goto_activate),NULL);
  g_signal_connect(button,"clicked",G_CALLBACK(destroy),dialog);


  container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(container),fixed);

  gtk_widget_show_all(dialog);

}

void addEvent_show() {
  GtkWidget *addEvent_dialog, *container_addEvent;
  GtkWidget *name_label, *day_label, *month_event, *year_event, *note_event;
  GtkWidget *name_entry;
}

void addEvent_show_double_click() {
  GtkWidget *dialog, *container;
  GtkWidget *name_event_entry, *note_event_entry;
  GtkWidget *name_event_label, *note_event_label;
  GtkWidget *fixed;
  GtkWidget *show_day_label;

  gchar *display_update;

  guint year_select, month_select, day_select;
  gtk_calendar_get_date(GTK_CALENDAR(calendar),&year_select, &month_select, &day_select); //chọn ngày đã được chọn lưu vào []_select

  dialog = gtk_dialog_new();
  fixed = gtk_fixed_new();

  name_event_entry = gtk_entry_new();
  note_event_entry = gtk_entry_new();

  name_event_label = gtk_label_new("Name:");
  note_event_label = gtk_label_new("Note:");
  show_day_label = gtk_label_new("");

  display_update = g_strdup_printf("Date:\t%02d/%02d/%02d (dd/mm/yyyy)", day_select, month_select + 1 , year_select); 

  gtk_label_set_text (GTK_LABEL(show_day_label), display_update);

  gtk_dialog_add_buttons(GTK_DIALOG(dialog),"Ok",1,"Cancel",2,NULL); //ok = 1 vs cancel = 2

  gtk_fixed_put(GTK_FIXED(fixed), name_event_entry, 70, 45);
  gtk_fixed_put(GTK_FIXED(fixed), note_event_entry, 70, 105);
  gtk_fixed_put(GTK_FIXED(fixed), name_event_label, 10, 50);
  gtk_fixed_put(GTK_FIXED(fixed), note_event_label, 10, 110);
  gtk_fixed_put(GTK_FIXED(fixed), show_day_label, 10, 10);

  gtk_widget_set_name(note_event_entry,"note_event_entry");

  gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(dialog),"Add event");

  container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_container_add(GTK_CONTAINER(container),fixed);

  gtk_widget_show_all(dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(dialog)); //add gia tri khi bam button
  switch (response) { //thuc hien ham khi gia tri = nhau
    case 1:
      gtk_widget_destroy(GTK_WIDGET(dialog));
      break;
    case 2:
      gtk_widget_destroy(GTK_WIDGET(dialog));
      break;
  }
}

void month_show() {
  GtkWidget *dialog;
  GtkWidget *container_month;
  GtkWidget *label_month;
  GtkWidget *button_enter;

  dialog = gtk_dialog_new();

  label_month = gtk_label_new("Input month:");

  entry_month = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");
  
  container_month = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); //đưa dialog vào xử lí tạo vùng để hiển thị

  gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
  
  gtk_container_add(GTK_CONTAINER(container_month),label_month);
  gtk_container_add(GTK_CONTAINER(container_month),entry_month);
  gtk_container_add(GTK_CONTAINER(container_month),button_enter);

  g_signal_connect(button_enter,"clicked",G_CALLBACK(add_month_input),NULL);
  g_signal_connect(button_enter,"clicked",G_CALLBACK(destroy),dialog);

  gtk_widget_show_all(dialog);
}

void year_show() {
  GtkWidget *dialog;
  GtkWidget *container_year;
  GtkWidget *label_year;
  GtkWidget *button_enter;

  dialog = gtk_dialog_new();

  label_year = gtk_label_new("Input year:");

  entry_year = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");
  
  container_year = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
  
  gtk_container_add(GTK_CONTAINER(container_year),label_year);
  gtk_container_add(GTK_CONTAINER(container_year),entry_year);
  gtk_container_add(GTK_CONTAINER(container_year),button_enter);

  g_signal_connect(button_enter,"clicked",G_CALLBACK(add_year_input),NULL);
  g_signal_connect(button_enter,"clicked",G_CALLBACK(destroy),dialog);

  gtk_widget_show_all(dialog);
  
}

void today_set() {
  update_month(month_today + 1);
  update_year(year_today);
  update_day(day_today);
}

void exit_screen() {

  GtkWidget *exit_dialog, *container_exit;
  GtkWidget *label_ask;
  GtkWidget *fixed_exit;

  exit_dialog = gtk_dialog_new();

  label_ask = gtk_label_new("Are you want to Quit?");

  gtk_dialog_add_buttons(GTK_DIALOG(exit_dialog),"Yes",1,"No",2,NULL); // yes = 1 , no = 2

  gtk_window_set_position(GTK_WINDOW(exit_dialog),GTK_WIN_POS_CENTER);

  container_exit = gtk_dialog_get_content_area(GTK_DIALOG(exit_dialog));

  gtk_container_add(GTK_CONTAINER(container_exit),label_ask);

  gtk_widget_show_all(exit_dialog);

  gint response = gtk_dialog_run(GTK_DIALOG(exit_dialog)); //add gia tri khi bam button

  switch (response) { //thuc hien ham khi gia tri = nhau
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

void login_dialog_screen() { //màn hình login

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

  gtk_widget_set_name(button_submit,"button_submit"); 
  gtk_widget_set_name(username_label,"authentication_label"); 
  gtk_widget_set_name(password_label,"authentication_label"); 
  gtk_widget_set_name(username_entry,"authentication_entry"); 
  gtk_widget_set_name(password_entry,"authentication_entry"); 


  gtk_fixed_put(GTK_FIXED(fixed_login), username_label, 100, 100); 
  gtk_fixed_put(GTK_FIXED(fixed_login), password_label, 100, 150);
  gtk_fixed_put(GTK_FIXED(fixed_login), username_entry, 300, 100);
  gtk_fixed_put(GTK_FIXED(fixed_login), password_entry, 300, 150);
  gtk_fixed_put(GTK_FIXED(fixed_login), button_submit, 235, 250);

  gtk_window_set_title(GTK_WINDOW(login_dialog),"Login"); 
  gtk_window_set_position(GTK_WINDOW(login_dialog),GTK_WIN_POS_CENTER); 
  gtk_window_set_default_size(GTK_WINDOW(login_dialog),640,410); 
  gtk_window_set_resizable(GTK_WINDOW(login_dialog),FALSE); 
  gtk_container_set_border_width(GTK_CONTAINER(login_dialog),10);

  container_login_dialog = gtk_dialog_get_content_area(GTK_DIALOG(login_dialog));

  gtk_container_add(GTK_CONTAINER(container_login_dialog),fixed_login);

  gtk_widget_show_all(login_dialog);
}

void register_dialog_screen() { //màn hình register

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

  gtk_widget_set_name(button_submit,"button_submit"); 
  gtk_widget_set_name(username_label,"authentication_label"); 
  gtk_widget_set_name(password_label,"authentication_label"); 
  gtk_widget_set_name(username_entry,"authentication_entry"); 
  gtk_widget_set_name(password_entry,"authentication_entry"); 
  gtk_widget_set_name(retypePassword_label,"authentication_label"); 
  gtk_widget_set_name(retypePassword_entry,"authentication_entry"); 

  gtk_fixed_put(GTK_FIXED(fixed_register), username_label, 100, 100); 
  gtk_fixed_put(GTK_FIXED(fixed_register), password_label, 100, 150);
  gtk_fixed_put(GTK_FIXED(fixed_register), username_entry, 300, 100);
  gtk_fixed_put(GTK_FIXED(fixed_register), password_entry, 300, 150);
  gtk_fixed_put(GTK_FIXED(fixed_register), button_submit, 235, 300);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_label, 100, 200);
  gtk_fixed_put(GTK_FIXED(fixed_register), retypePassword_entry, 300, 200);

  gtk_window_set_title(GTK_WINDOW(register_dialog),"Register"); 
  gtk_window_set_position(GTK_WINDOW(register_dialog),GTK_WIN_POS_CENTER); 
  gtk_window_set_default_size(GTK_WINDOW(register_dialog),640,410); 
  gtk_window_set_resizable(GTK_WINDOW(register_dialog),FALSE); 
  gtk_container_set_border_width(GTK_CONTAINER(register_dialog),10);

  container_register_dialog = gtk_dialog_get_content_area(GTK_DIALOG(register_dialog)); //tao phan vung chua content cho dialog

  gtk_container_add(GTK_CONTAINER(container_register_dialog),fixed_register);
  
  gtk_widget_show_all(register_dialog);
}

int main(int argc, char *argv[]) { //main

//----------time.h--------------//

// printf("now: %02d:%02d:%02d ",tm.tm_hour, tm.tm_min, tm.tm_sec);

//----------gtk--------------//

  gtk_init(&argc,&argv);

  //add biến
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *button_exit, *button_login, *button_register, *button_logout, *button_add_event, *button_delete_event, *button_event_list;
  GtkWidget *button_previous_month, *button_next_month, *button_previous_year, *button_next_year;
  GtkWidget *button_goto_day, *button_today;
  GtkWidget *time_label;

  //set biến
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //cho app ở quyền cao nhất
  fixed = gtk_fixed_new();
  calendar = gtk_calendar_new();
  
  gtk_window_set_title(GTK_WINDOW(window),"Lịch thiểu năng"); // title cho app
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(window),1580,860); // kích thước app mặc định
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // false sẽ làm cho app ko zoom out or in được
  gtk_container_set_border_width(GTK_CONTAINER(window),10);// tạo khoảng cách giữa độ rộng tối đa của app và các thành phần bên trong

  //tạo button
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

  //tạo label
  time_label = gtk_label_new("");

  //-----set label show show today--------//

  gtk_calendar_get_date(GTK_CALENDAR(calendar),&year_today, &month_today, &day_today);
  update_month(month_today + 1);
  update_year(year_today);

  //tạo khả năng fixed cho từng thành phần và mặc định vị trí
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
  gtk_fixed_put(GTK_FIXED(fixed), time_label, 1310, 50);

  //set biến thành id name để css có thể nhận dạng
  gtk_widget_set_name(button_exit,"button_menu"); 
  gtk_widget_set_name(button_login,"button_menu"); 
  gtk_widget_set_name(button_logout,"button_menu");
  gtk_widget_set_name(button_add_event,"button_menu");
  gtk_widget_set_name(button_register,"button_menu"); 
  gtk_widget_set_name(button_delete_event,"button_menu"); 
  gtk_widget_set_name(button_event_list,"button_menu"); 
  gtk_widget_set_name(button_previous_month,"button_main");
  gtk_widget_set_name(button_next_month,"button_main");
  gtk_widget_set_name(button_previous_year,"button_main");
  gtk_widget_set_name(button_next_year,"button_main");
  gtk_widget_set_name(button_goto_day,"goto_day");
  gtk_widget_set_name(button_today,"goto_day");
  gtk_widget_set_name(show_month,"show_month");
  gtk_widget_set_name(show_year,"show_year");
  gtk_widget_set_name(calendar,"calendar");

//gọi hàm khi nhấn button
  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); // tắt app 
  g_signal_connect(button_exit,"clicked",G_CALLBACK(exit_screen),NULL);
  g_signal_connect(button_login,"clicked",G_CALLBACK(login_dialog_screen),NULL);
  g_signal_connect(button_register,"clicked",G_CALLBACK(register_dialog_screen),NULL);
  
  g_signal_connect(show_month,"clicked",G_CALLBACK(month_show),NULL);
  g_signal_connect(show_year,"clicked",G_CALLBACK(year_show),NULL);

  g_signal_connect(button_next_month,"clicked",G_CALLBACK(add_one_month),NULL);
  g_signal_connect(button_previous_month,"clicked",G_CALLBACK(minus_one_month),NULL);
  g_signal_connect(button_next_year,"clicked",G_CALLBACK(add_one_year),NULL);
  g_signal_connect(button_previous_year,"clicked",G_CALLBACK(minus_one_year),NULL);
  g_signal_connect(button_goto_day,"clicked",G_CALLBACK(goto_day_show),NULL);
  g_signal_connect(button_today,"clicked",G_CALLBACK(today_set),NULL);

  g_signal_connect (calendar,"day_selected_double_click",G_CALLBACK(addEvent_show_double_click),NULL); // when double click on day

  gtk_calendar_set_display_options(GTK_CALENDAR (calendar), 2); // 2 = calendar header (in there have year and month) and this function use to hide year and month

  gtk_container_add(GTK_CONTAINER(window),fixed);

  gtk_widget_show_all(window); 

//---------------------------load---fuction---------------------------------//
  load_css();// gọi hàm load_css
  
  update_time(GTK_LABEL(time_label));

//---------------------------C---------------------------------//
//-----------------------------test------------------//
//----------------------------------------------------/
  gtk_main(); //mainLoop
//----------không viết gì dưới gtk_main() vì đây là điểm kết thúc-----------------------------//
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file 
// gcc `pkg-config --cflags gtk+-3.0` -o app gtk_new.c `pkg-config --libs gtk+-3.0`
// ./app

