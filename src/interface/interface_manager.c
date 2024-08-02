#include "interface_manager.h"

// FIXME: temp rendering test
static void activate(GtkApplication* gtk_app, gpointer user_data) {
  Window* window = window_init("Emulator Hub", 600, 600, gtk_application_window_new(gtk_app));
}

Interface_Manager* interface_manager_init() {
  Interface_Manager* interface_manager = (Interface_Manager*)calloc(1, sizeof(Interface_Manager));
  interface_manager->gtk_app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

  return interface_manager;
}

void interface_manager_start(Interface_Manager* interface_manager, int argc, char** argv) {
  g_signal_connect(interface_manager->gtk_app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(interface_manager->gtk_app), argc, argv);
}

void interface_manager_destroy(Interface_Manager* interface_manager) {
  g_object_unref(interface_manager->gtk_app);
  free(interface_manager);
}
