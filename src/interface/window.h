#ifndef WINDOW_H
#define WINDOW_H

#include <gtk/gtk.h>

#include "lwlog/lwlog.h"

typedef struct {
  unsigned int w;
  unsigned int h;
  const char* title;
} Window;

void window_manager_init(Window* win);
void window_manager_destroy();

#endif

