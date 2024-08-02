#include "window.h"

Window* window_init(const char* title, int w, int h, GtkWidget* gtk_win) {
  Window* win = (Window*)calloc(sizeof(Window), 1);

  win->w = w;
  win->h = h;
  win->title = title;
  win->gtk_win = gtk_win;

  gtk_window_set_title(win->gtk_win, win->title);
  gtk_window_set_default_size(win->gtk_win, win->w, win->h);
  gtk_window_present(win->gtk_win);

  return win;
}

void window_destroy(Window* win) {
  free(win);
}
