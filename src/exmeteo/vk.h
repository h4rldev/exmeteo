#ifndef vk_H_INCLUDED
#define vk_H_INCLUDED

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void error_handler(int error_code, const char *msg);
int init_glfw(void);
int window_init(GLFWwindow *window);

#endif
