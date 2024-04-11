// builtin
#include <stdio.h>

// nuklear
#define NK_IMPLEMENTATION
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO
#include "../../include/nuklear.h"

// homemade
#include "vk.h"
#include "gui.h"

// macros
#define MAX_MEMORY 1024 * 1024

int test(void) {
  int glfw_initialization = init_glfw();
 
  if (!glfw_initialization) {
    error_handler(1, "Failed to initialize GLFW & Vulkan");
    return 1;
  }

  GLFWwindow *window = glfwCreateWindow(800, 600, "Vulkan", NULL, NULL);

  if (!window) {
    error_handler(3, "Failed to create a GLFWwindow..");
    glfwTerminate();
    return -1;
  }
  /* init gui state */
  window_init(window);
  puts("Window successfully(?) initialized");


  struct nk_font_atlas atlas;
  nk_font_atlas_init_default(&atlas);
  nk_font_atlas_begin(&atlas);

  extern const char _binary_font_ttf_start[];
  extern const char _binary_font_ttf_end[];
  size_t font_size = _binary_font_ttf_end - _binary_font_ttf_start;
  nk_font_atlas_add_from_memory(&atlas, _binary_font_ttf_start, font_size, 14, 0);

  const void* image; int w, h;
  image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);

  struct nk_context ctx;
  nk_init_default(&ctx, const struct nk_user_font *)

  return 0;
}


