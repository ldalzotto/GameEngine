#pragma once

#include "Window_def.h"

extern const uint32_t WINDOW_WIDTH;
extern const uint32_t WINDOW_HEIGHT;


void Window_init(Window* p_window);
void Window_closeWindow(Window* p_window);
char Window_askedForClose(Window* p_window);
char Window_consumeSizeChangeEvent(Window* p_window);

WindowSize Window_getSize(Window* p_window);
void Window_presentTexture(Window* p_window, Texture3c_PTR p_texture);