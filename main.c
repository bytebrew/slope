#include <gtk/gtk.h>
#include <slope/slope.h>

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *view;

  window = gtk_application_window_new (app);
  view = slope_view_new();
  gtk_window_set_title (GTK_WINDOW (window), "Slope");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);
  gtk_container_add(GTK_CONTAINER (window), view);
  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.slope.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
