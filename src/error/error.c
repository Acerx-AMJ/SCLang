#include "error/error.h"
#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

// Globals

View programCode;

// Private functions

[[noreturn]] void Error_vraise(int line, const char *msg, va_list args) {
   #if defined (__linux__) || defined (__unix__)
   printf("\nProgram exited due to the following error:\n \e[91m");
   #else
   printf("\nProgram exited due to the following error:\n ");
   #endif

   vprintf(msg, args);

   #if defined (__linux__) || defined (__unix__)
   printf("\e[0m");
   #endif
   putchar('\n');
   
   if (line == ERROR_NLINE) {
      Error_exit(EXIT_FAILURE);
   }

   View previous = View_zeroInit();
   View current = View_zeroInit();
   View next = View_zeroInit();
   View temp = View_zeroInit();

   int previousLine = 0;
   int nextLine = 0;
   int index = 0;
   int lineIndex = 1;

   while (index < programCode.size) {
      temp.base = programCode.base + index;
      temp.size = 0;
   
      for (; index < programCode.size && programCode.base[index] != '\n'; ++index) {
         temp.size += 1;
      }
      index += 1; // To avoid the newline

      if (temp.size == 0) {
         lineIndex += 1;
         continue;
      }

      if (lineIndex < line) {
         previousLine = lineIndex;
         previous.base = temp.base;
         previous.size = temp.size;
      } else if (lineIndex == line) {
         current.base = temp.base;
         current.size = temp.size;
      } else if (lineIndex > line && View_empty(&next)) {
         nextLine = lineIndex;
         next.base = temp.base;
         next.size = temp.size;
         break; // All lines have been found
      }
      lineIndex += 1;
   }

   if (!View_empty(&previous)) {
      printf("  %-5d ", previousLine);
      
      for (int i = 0; i < previous.size; ++i) {
         putchar(previous.base[i]);
      }
      putchar('\n');

      for (int i = previousLine; i < line - 1; ++i) {
         printf("  %-5d\n", i + 1);
      }
   }

   if (!View_empty(&current)) {
      printf("  %-5d ", line);

      for (int i = 0; i < current.size; ++i) {
         putchar(current.base[i]);
      }
      putchar('\n');

      printf("  %-5d ", line);
      #if defined (__linux__) || defined (__unix__)
      printf("\e[91m");
      #endif

      for (int i = 0; i < current.size; ++i) {
         putchar('^');
      }

      #if defined (__linux__) || defined (__unix__)
      printf("\e[0m");
      #endif
      putchar('\n');
   }

   if (!View_empty(&next)) {
      for (int i = line; i < nextLine - 1; ++i) {
         printf("  %-5d\n", i + 1);
      }
      printf("  %-5d ", nextLine);
      
      for (int i = 0; i < next.size; ++i) {
         putchar(next.base[i]);
      }
      putchar('\n');
   }

   Error_exit(EXIT_FAILURE);
}

void Error_vassert(bool condition, int line, const char *msg, va_list args) {
   if (!condition) {
      Error_vraise(line, msg, args);
   }
}

// Error functions

void Error_setProgramCode(View code) {
   programCode = code;
}

void Error_assert(bool condition, int line, const char *msg, ...) {
   va_list args;
   va_start(args, msg);
   Error_vassert(condition, line, msg, args);
   va_end(args);
}

void Error_raise(int line, const char *msg, ...) {
   va_list args;
   va_start(args, msg);
   Error_vraise(line, msg, args);
   va_end(args);
}

void Error_exit(int code) {
   #if defined (__linux__) || defined (__unix__)
   printf("%sProgram exited with exit code %d.\e[0m\n", (code == EXIT_SUCCESS ? "\n" : "\n\e[91m"), code);
   #else
   printf("\nProgram exited with exit code %d.\n", code);
   #endif
   exit(code);
}
