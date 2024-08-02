#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>

#include "lwlog/lwlog.h"

typedef struct {
  unsigned int w;
  unsigned int h;
  const char* title;

  GtkWidget* gtk_win;
} Window;

Window* window_init(const char* title, int w, int h, GtkWidget* gtk_win);
void window_destroy(Window* win);

#endif

