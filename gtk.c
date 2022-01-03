#include <gtk/gtk.h> // khai báo thứ viện gtk
#include <stdio.h> //khai báo thư viện c

static void on_click1(GtkWidget *widget, gpointer data) {
  g_print("Bạn đã bị thiểu năng\n");
}

int main(int argc, char *argv[]) {

  gtk_init(&argc, &argv); // khai báo  

  GtkWidget *window, *fixed, *notebook;
  GtkWidget *button1, *button_exit;
  

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //cho app ở quyền cao nhất
  fixed = gtk_fixed_new();

  gtk_window_set_title(GTK_WINDOW(window),"Lịch thiểu năng"); // title cho app
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER); // canh app khi mở sẽ ở giữa màn hình
  gtk_window_set_default_size(GTK_WINDOW(window),1280,820); // kích thước app
  gtk_window_set_resizable(GTK_WINDOW(window),FALSE); // false sẽ làm cho app ko zoom out or in được

  button1 = gtk_button_new_with_label("Nút thiểu năng");
  button_exit = gtk_button_new_with_label("Thoát");

  gtk_fixed_put(GTK_FIXED(fixed), button1, 50, 50);
  gtk_fixed_put(GTK_FIXED(fixed), button_exit, 50, 720);

  gtk_container_add(GTK_CONTAINER(window),fixed);

  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL); // tắt app sẽ tắt mọi thứ chạy chung bao gồm console và log
  g_signal_connect(button_exit,"clicked",G_CALLBACK(gtk_main_quit),NULL);
  g_signal_connect(button1,"clicked",G_CALLBACK(on_click1),NULL);

  gtk_widget_show_all(window);
  gtk_main(); //mainLoop

  return 0;
}

// mở app msys2 64 bit
// nhớ cd /c/thư mục chứ file 
// gcc `pkg-config --cflags gtk+-3.0` -o app test.c `pkg-config --libs gtk+-3.0`
// ./app