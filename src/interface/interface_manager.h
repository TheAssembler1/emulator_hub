#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include <stdlib.h>
#include <gtk/gtk.h>

#include "window.h"

typedef struct {
  const GtkApplication* gtk_app;  
} Interface_Manager;

Interface_Manager* interface_manager_init();
void interface_manager_start(Interface_Manager* interface_manager, int argc, char** argv);
void interface_manager_destroy(Interface_Manager* interface_manager);

#endif
