#include <gtk/gtk.h> // khai báo thứ viện gtk
#include <stdio.h> //khai báo thư viện c

static void load_css() {
  GtkCssProvider *cssProvider = gtk_css_provider_new(); //khai báo css provider mới
  gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL); // đọc file css
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),GTK_STYLE_PROVIDER(cssProvider),GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

int main(int argc, char *argv[]) {

  gtk_init(&argc, &argv); // khai báo  

  //add biến
  GtkWidget *window, *fixed;
  GtkWidget *button_exit, *button_login, *button_register;
  
  //set biến
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //cho app ở quyền cao nhất
  fixed = gtk_fixed_new(); // khai báo biến fixed mới

  //
  gtk_window_set_title(GTK_WINDOW(window),"Lịch thiểu năng"); // title cho app
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(window),1280,820); // kích thước app mặc định
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // false sẽ làm cho app ko zoom out or in được
  gtk_container_set_border_width(GTK_CONTAINER(window),10);// tạo khoảng cách giữa độ rộng tối đa của app và các thành phần bên trong

  //tạo button
  button_exit = gtk_button_new_with_label("Exit");
  button_login = gtk_button_new_with_label("Login");
  button_register = gtk_button_new_with_label("Register");

  //tạo khả năng fixed cho từng thành phần và mặc định vị trí
  gtk_fixed_put(GTK_FIXED(fixed), button_exit, 1000, 680); 
  gtk_fixed_put(GTK_FIXED(fixed), button_login, 1000, 100);
  gtk_fixed_put(GTK_FIXED(fixed), button_register, 1000, 150);

  //set biến thành id name để css có thể nhận dạng
  gtk_widget_set_name(button_exit,"button_exit"); 
  gtk_widget_set_name(button_login,"button_login"); 
  gtk_widget_set_name(button_register,"button_register"); 

  gtk_container_add(GTK_CONTAINER(window),fixed); // thêm phần window chứa fixed để hiển thị toàn bộ thành phần được put fixed

  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); // tắt app sẽ tắt mọi thứ chạy chung bao gồm console và log
  g_signal_connect(button_exit,"clicked",G_CALLBACK(gtk_main_quit),NULL);

  gtk_widget_show_all(window); // hiển thị 

  load_css();// gọi hàm load_css

  gtk_main(); //mainLoop

  return 0;
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file 
// gcc `pkg-config --cflags gtk+-3.0` -o app test.c `pkg-config --libs gtk+-3.0`
// ./app