


//--------------------Thành------------------------//
#include <gtk/gtk.h>
//------------------Thọ--------------------//
#include <time.h>
#include <stdbool.h>
//--------------------Hân----------------------//
#include <string.h>
//------------------ALL------------------//
#include <stdio.h> 
#include <stdlib.h>

//-----------------------------------------Thọ---------Calendar----------------------------------------------//

//------------------------------------------Hân---------Authentication---------------------------------------------//

//-------------------------------------Thành-------GTK-----------------------------------------------------//

// khai biến ở đây để tất cả các function đều truy cập được
GtkWidget *entry_year, *entry_month; // in function month_show and year_show
GtkWidget *month_dialog, *year_dialog;  // in function month_show and year_show

//--------------------------------//
static void load_css() {
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

// lỗi  invalid cast from 'GtkButton' to 'GtkLabel' từ đây
void update_label_in_button_show(GtkLabel *button, int value) { //cập nhật label trong button đươc3 trỏ tới
  gchar *display;
  display = g_strdup_printf("%d", value); // chuyển int thành char
  gtk_button_set_label(GTK_BUTTON(button), display);
  // g_free(display); //
}

void add_year_input(GtkButton *button, GtkButton *button_input) { // lấy dữ liệu từ input year
  int data = atoi(gtk_entry_get_text(GTK_ENTRY(entry_year))); 

  if (data < 1){ //nếu year input < 1 thì year = 1
    data = 1;
    update_label_in_button_show(GTK_LABEL(button_input),data);
  }
  else {
    update_label_in_button_show(GTK_LABEL(button_input),data);
  }

  gtk_widget_destroy(year_dialog);
}

void add_month_input(GtkButton *button, GtkButton *button_input) { // lấy dữ liệu từ input month
  int data = atoi(gtk_entry_get_text(GTK_ENTRY(entry_month))); 

  if (data < 1 || data > 12){ //nếu month input < 1 hoặc > 12 thì month = 1
    data = 1;
    update_label_in_button_show(GTK_LABEL(button_input),data);
  }
  else {
    update_label_in_button_show(GTK_LABEL(button_input),data);
  }

  gtk_widget_destroy(month_dialog);
}

void add_one_year(GtkButton *button, GtkLabel *value) {
  int add = atoi(gtk_button_get_label(GTK_BUTTON(value))); //atoi() biến char thành int
  add += 1;
  update_label_in_button_show(GTK_LABEL(value),add);
}

void minus_one_year(GtkButton *button, GtkLabel *value) {
  int minus = atoi(gtk_button_get_label(GTK_BUTTON(value)));
  minus -= 1;
  if (minus < 1){ // nếu value year < 1 thì year = 1
    minus = 1; 
    minus_one_year(button,value);
    update_label_in_button_show(GTK_LABEL(value),minus);
  }
  else {
    update_label_in_button_show(GTK_LABEL(value),minus);
  }
}

void add_one_month(GtkButton *button, GtkLabel *value) {
  int add = atoi(gtk_button_get_label(GTK_BUTTON(value))); //atoi() biến char thành int
  add += 1;
  if (add > 12){ // nếu value month > 12 thì month = 1
    add = 1; 
    update_label_in_button_show(GTK_LABEL(value),add);
  }
  else {
    update_label_in_button_show(GTK_LABEL(value),add);
  }
}

void minus_one_month(GtkButton *button, GtkLabel *value) {
  int minus = atoi(gtk_button_get_label(GTK_BUTTON(value)));
  minus -= 1;
  if (minus < 1){ // nếu value month < 1 thì month = 1, year - 1
    minus = 12; 
    update_label_in_button_show(GTK_LABEL(value),minus);
  }
  else {
    update_label_in_button_show(GTK_LABEL(value),minus);
  }
}

//đến đây

void month_show(GtkButton *button, GtkButton *button_main) {
  GtkWidget *container_month;
  GtkWidget *label_month;
  GtkWidget *button_enter;

  month_dialog = gtk_dialog_new();

  label_month = gtk_label_new("Input month:");

  entry_month = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");
  
  container_month = gtk_dialog_get_content_area(GTK_DIALOG(month_dialog)); //đưa dialog vào xử lí tạo vùng để hiển thị

  gtk_window_set_position(GTK_WINDOW(month_dialog),GTK_WIN_POS_CENTER);
  
  gtk_container_add(GTK_CONTAINER(container_month),label_month);
  gtk_container_add(GTK_CONTAINER(container_month),entry_month);
  gtk_container_add(GTK_CONTAINER(container_month),button_enter);

  g_signal_connect(button_enter,"clicked",G_CALLBACK(add_month_input),button_main);

  gtk_widget_show_all(month_dialog);
}

void year_show(GtkButton *button, GtkButton *button_main) {
  GtkWidget *container_year;
  GtkWidget *label_year;
  GtkWidget *button_enter;

  year_dialog = gtk_dialog_new();

  label_year = gtk_label_new("Input year:");

  entry_year = gtk_entry_new();

  button_enter = gtk_button_new_with_label("Enter");
  
  container_year = gtk_dialog_get_content_area(GTK_DIALOG(year_dialog));

  gtk_window_set_position(GTK_WINDOW(year_dialog),GTK_WIN_POS_CENTER);
  
  gtk_container_add(GTK_CONTAINER(container_year),label_year);
  gtk_container_add(GTK_CONTAINER(container_year),entry_year);
  gtk_container_add(GTK_CONTAINER(container_year),button_enter);

  g_signal_connect(button_enter,"clicked",G_CALLBACK(add_year_input),button_main);

  gtk_widget_show_all(year_dialog);
  
}
//void exit_screen(GtkWidget *widget, gpointer data)
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

//void login_dialog_screen(GtkWidget *widget, gpointer data)
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

//void register_dialog_screen(GtkWidget *widget, gpointer data)
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

  gtk_init(&argc,&argv);

  //add biến
  GtkWidget *window, *fixed;
  GtkWidget *button_exit, *button_login, *button_register, *button_logout, *button_add_event, *button_delete_event;
  GtkWidget *button_previous_month, *button_next_month, *button_previous_year,*button_next_year;
  GtkWidget *show_month, *show_year;

  //set biến
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //cho app ở quyền cao nhất
  fixed = gtk_fixed_new();
  
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
  button_previous_month = gtk_button_new_with_label("Previous month");
  button_next_month = gtk_button_new_with_label("Next month");
  button_previous_year = gtk_button_new_with_label("Previous year");
  button_next_year = gtk_button_new_with_label("Next year");
  show_month = gtk_button_new_with_label("1");
  show_year = gtk_button_new_with_label("2021");

  //tạo khả năng fixed cho từng thành phần và mặc định vị trí
  gtk_fixed_put(GTK_FIXED(fixed), button_exit, 1300, 760); 
  gtk_fixed_put(GTK_FIXED(fixed), button_login, 1300, 100);
  gtk_fixed_put(GTK_FIXED(fixed), button_add_event, 1300, 250);
  gtk_fixed_put(GTK_FIXED(fixed), button_delete_event, 1300, 300);
  gtk_fixed_put(GTK_FIXED(fixed), button_register, 1300, 150);
  gtk_fixed_put(GTK_FIXED(fixed), button_previous_month, 400, 250);
  gtk_fixed_put(GTK_FIXED(fixed), button_next_month, 900, 250);
  gtk_fixed_put(GTK_FIXED(fixed), button_previous_year, 400, 150);
  gtk_fixed_put(GTK_FIXED(fixed), button_next_year, 900, 150);
  gtk_fixed_put(GTK_FIXED(fixed), show_month, 680, 250);
  gtk_fixed_put(GTK_FIXED(fixed), show_year, 680, 150);

  //set biến thành id name để css có thể nhận dạng
  gtk_widget_set_name(button_exit,"button_menu"); 
  gtk_widget_set_name(button_login,"button_menu"); 
  gtk_widget_set_name(button_logout,"button_menu");
  gtk_widget_set_name(button_add_event,"button_menu");
  gtk_widget_set_name(button_register,"button_menu"); 
  gtk_widget_set_name(button_delete_event,"button_menu"); 
  gtk_widget_set_name(button_previous_month,"button_main_left");
  gtk_widget_set_name(button_next_month,"button_main_right");
  gtk_widget_set_name(button_previous_year,"button_main_left");
  gtk_widget_set_name(button_next_year,"button_main_right");
  gtk_widget_set_name(show_month,"show_month");
  gtk_widget_set_name(show_year,"show_year");

//gọi hàm khi nhấn button
  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); // tắt app 
  g_signal_connect(button_exit,"clicked",G_CALLBACK(exit_screen),NULL);
  g_signal_connect(button_login,"clicked",G_CALLBACK(login_dialog_screen),NULL);
  g_signal_connect(button_register,"clicked",G_CALLBACK(register_dialog_screen),NULL);
  
  g_signal_connect(show_month,"clicked",G_CALLBACK(month_show),show_month);
  g_signal_connect(show_year,"clicked",G_CALLBACK(year_show),show_year);

  g_signal_connect(button_next_year,"clicked",G_CALLBACK(add_one_year),show_year);
  g_signal_connect(button_previous_year,"clicked",G_CALLBACK(minus_one_year),show_year);
  g_signal_connect(button_next_month,"clicked",G_CALLBACK(add_one_month),show_month);
  g_signal_connect(button_previous_month,"clicked",G_CALLBACK(minus_one_month),show_month);


  gtk_container_add(GTK_CONTAINER(window),fixed);

  gtk_widget_show_all(window); 

//---------------------------load---fuction---------------------------------//
  load_css();// gọi hàm load_css

//---------------------------C---------------------------------//

//----------------------------------------------------/
  gtk_main(); //mainLoop
//----------không viết gì dưới gtk_main() vì đây là điểm kết thúc-----------------------------//
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file 
// gcc `pkg-config --cflags gtk+-3.0` -o app gtk.c `pkg-config --libs gtk+-3.0`
// ./app

