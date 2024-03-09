#include <Arduino.h>

void
print_logo()
{
  printf("\033[1;31m");
  printf("   🌟        ██████ ███████ ███    ██ ██ ██    ██ ███████\n");
  printf("\033[1;32m");
  printf("   🚀       ██      ██      ████   ██ ██ ██    ██ ██     \n");
  printf("\033[1;33m");
  printf("   💡       ██   ██ █████   ██ ██  ██ ██ ██    ██ ███████\n");
  printf("\033[1;34m");
  printf("   🔧       ██    █ ██      ██  ██ ██ ██ ██    ██      ██\n");
  printf("\033[1;35m");
  printf("   ✨        █████  ███████ ██   ████ ██  ██████  ███████\n");
  printf("\033[0m"); // Reset text color to default
  printf("\n");
  printf("\n");
}

void
color_printf(const char* format, ...)
{
  va_list args;
  va_start(args, format);

  const char* colors[] = {
    "\033[1;31m", // Red
    "\033[1;32m", // Green
    "\033[1;33m", // Yellow
    "\033[1;34m", // Blue
    "\033[1;35m", // Magenta
    "\033[1;36m"  // Cyan
  };
  const int num_colors = sizeof(colors) / sizeof(colors[0]);

  int color_index = 0;
  bool in_word = false;

  while (*format != '\0') {
    if (*format == ' ' || *format == '\n' || *format == '\t') {
      if (in_word) {
        printf("\033[0m"); // Reset color
        in_word = false;
      }
      printf("%c", *format);
    } else {
      if (!in_word) {
        printf("%s", colors[color_index]);
        color_index = (color_index + 1) % num_colors;
        in_word = true;
      }
      printf("%c", *format);
    }
    format++;
  }

  printf("\033[0m"); // Reset color at the end
  va_end(args);
}
