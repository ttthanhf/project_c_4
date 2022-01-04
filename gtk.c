#include <gtk/gtk.h> // khai báo thứ viện gtk
#include <stdio.h> //khai báo thư viện c

static void load_css() {
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

void login_dialog_screen(GtkWidget *widget, gpointer data) {

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

void register_dialog_screen(GtkWidget *widget, gpointer data) {

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


int main(int argc, char *argv[]) {

  gtk_init(&argc, &argv);

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


  //tạo khả năng fixed cho từng thành phần và mặc định vị trí
  gtk_fixed_put(GTK_FIXED(fixed), button_exit, 1300, 760); 
  gtk_fixed_put(GTK_FIXED(fixed), button_login, 1300, 100);
  gtk_fixed_put(GTK_FIXED(fixed), button_add_event, 1300, 250);
  gtk_fixed_put(GTK_FIXED(fixed), button_delete_event, 1300, 300);
  gtk_fixed_put(GTK_FIXED(fixed), button_register, 1300, 150);

  //set biến thành id name để css có thể nhận dạng
  gtk_widget_set_name(button_exit,"button_menu"); 
  gtk_widget_set_name(button_login,"button_menu"); 
  gtk_widget_set_name(button_logout,"button_menu");
  gtk_widget_set_name(button_add_event,"button_menu");
  gtk_widget_set_name(button_register,"button_menu"); 
  gtk_widget_set_name(button_delete_event,"button_menu"); 

  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); // tắt app 
  g_signal_connect(button_exit,"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(button_login,"clicked",G_CALLBACK(login_dialog_screen),NULL);
  g_signal_connect(button_register,"clicked",G_CALLBACK(register_dialog_screen),NULL);

  gtk_container_add(GTK_CONTAINER(window),fixed);

  gtk_widget_show_all(window); 

  load_css();// gọi hàm load_css

  gtk_main(); //mainLoop

  return 0;
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file 
// gcc `pkg-config --cflags gtk+-3.0` -o app test.c `pkg-config --libs gtk+-3.0`
// ./app