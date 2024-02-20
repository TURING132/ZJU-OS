#include "stdio.h"
#include "syscall.h"


static inline int putchar(char *buffer, int c) {
  *buffer = (char)c;
  return 0;
}

static int vprintfmt(int (*putch)(char*, int), const char *fmt, va_list vl) {
  int tail = 0;

  char buffer[1000];

  int in_format = 0, longarg = 0;
  size_t pos = 0;

  for (; *fmt; fmt++) {
    if (in_format) {
      switch (*fmt) {
      case 'l': {
        longarg = 1;
        break;
      }

      case 'x': {
        long num = longarg ? va_arg(vl, long) : va_arg(vl, int);

        int hexdigits = 2 * (longarg ? sizeof(long) : sizeof(int)) - 1;
        for (int halfbyte = hexdigits; halfbyte >= 0; halfbyte--) {
          int hex = (num >> (4 * halfbyte)) & 0xF;
          char hexchar = (hex < 10 ? '0' + hex : 'a' + hex - 10);
          putch(&buffer[tail++], hexchar);
          pos++;
        }
        longarg = 0;
        in_format = 0;
        break;
      }

      case 'd': {
        long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
        if (num < 0) {
          num = -num;
          putch(&buffer[tail++], '-');
          pos++;
        }
        int bits = 0;
        char decchar[25] = {'0', 0};
        for (long tmp = num; tmp; bits++) {
          decchar[bits] = (tmp % 10) + '0';
          tmp /= 10;
        }

        if (bits == 0)
          bits++;

        for (int i = bits - 1; i >= 0; i--) {
          putch(&buffer[tail++], decchar[i]);
        }
        pos += bits + 1;
        longarg = 0;
        in_format = 0;
        break;
      }

      case 'u': {
        unsigned long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
        int bits = 0;
        char decchar[25] = {'0', 0};
        for (long tmp = num; tmp; bits++) {
          decchar[bits] = (tmp % 10) + '0';
          tmp /= 10;
        }

        if (bits == 0)
          bits++;

        for (int i = bits - 1; i >= 0; i--) {
          putch(&buffer[tail++], decchar[i]);
        }
        pos += bits - 1;
        longarg = 0;
        in_format = 0;
        break;
      }

      case 's': {
        const char *str = va_arg(vl, const char *);
        while (*str) {
          putch(&buffer[tail++], *str);
          pos++;
          str++;
        }
        longarg = 0;
        in_format = 0;
        break;
      }

      case 'c': {
        char ch = (char)va_arg(vl, int);
        putch(&buffer[tail++], ch);
        pos++;
        longarg = 0;
        in_format = 0;
        break;
      }
      default:
        break;
      }
    } else if (*fmt == '%') {
      in_format = 1;
    } else {
      putch(&buffer[tail++], *fmt);
      pos++;
    }
  }

  long syscall_ret, fd = 1;
  buffer[tail++] = '\0';
  // 完成系统调用，将 buffer 中的内容写入文件描述符 fd 中，返回输出字符串的大小

  syscall_ret = u_syscall(SYS_WRITE, fd, (uint64_t)buffer, tail, 0, 0, 0).a0;

  return syscall_ret;
}

int printf(const char *s, ...) {
  int res = 0;
  va_list vl;
  va_start(vl, s);
  res = vprintfmt(putchar, s, vl);
  va_end(vl);
  return res;
}