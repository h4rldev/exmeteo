// homemade
#include "gui.h"
// macros


static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Hello World\n");
}

static void exec(GtkWidget *widget, gpointer data) {
    const char *command = (const char *)data;
    system(command);
}

static void on_response(GtkDialog *dialog, gint response_id, gpointer user_data) {
    switch (response_id) {
        case GTK_RESPONSE_OK:
            g_print("OK button clicked\n");
            break;
        case GTK_RESPONSE_CANCEL:
            g_print("Cancel button clicked\n");
            break;
        default:
            g_print("Unknown button clicked\n");
            break;
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
    g_application_quit(G_APPLICATION(user_data));
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *dialog;


  window = gtk_application_window_new(app);
  dialog = gtk_dialog_new();
  
  gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", GTK_RESPONSE_OK);
  gtk_dialog_add_button(GTK_DIALOG(dialog), "Cancel", GTK_RESPONSE_CANCEL);  gtk_window_set_default_size(GTK_WINDOW(dialog), 200, 100);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER); 

    //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
  g_signal_connect(dialog, "response", G_CALLBACK(on_response), app);
  gtk_widget_show_all(dialog);
}

int test(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}


