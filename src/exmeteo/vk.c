
#include "vk.h"

void error_handler(int error_code, const char *msg) {
  fprintf(stderr, "Error %d: %s\n", error_code, msg);
}

int init_glfw(void) {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  if (!glfwInit()) {
    error_handler(1, "Failed to initialize glfw for some reason.");
    return -1;
  }
  
  if (!glfwVulkanSupported()) {
    error_handler(2, "Your system doesn't support or doesn't have the neccessary libraries to run vulkan.");
    return -1;
  }

  glfwInitVulkanLoader(vkGetInstanceProcAddr);
  
  puts("GLFW initialized with Vulkan Successfully.");
  return 0;
}

int create_image(int w, int h, VkDevice device) {
  VkImageCreateInfo imageInfo = {};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM; // Assuming the font texture is in RGBA format
  imageInfo.extent.width = w;
  imageInfo.extent.height = h;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  VkImage fontImage;
  vkCreateImage(device, &imageInfo, NULL, &fontImage);
  
  return 0;
}

int window_init(GLFWwindow *window) { 
  uint32_t count;
  const char** extensions = glfwGetRequiredInstanceExtensions(&count);

  VkInstanceCreateInfo ici;
  memset(&ici, 0, sizeof(ici));
  ici.enabledExtensionCount = count;
  ici.ppEnabledExtensionNames = extensions;

  VkInstance vkInstance;
  if (vkCreateInstance(&ici, NULL, &vkInstance) != VK_SUCCESS) {
    error_handler(-1, "Failed to create Vulkan Instance.");  
    return -1;
  } else {
    puts("Successfully created Vulkan Instance.");
  }

  VkSurfaceKHR surface;
  VkResult err = glfwCreateWindowSurface(vkInstance, window, NULL, &surface); 
  if (err) {
    error_handler(-1, "Failed to create Window.");
    return -1;
  }
  return 0;
}
