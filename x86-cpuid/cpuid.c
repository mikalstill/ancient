#include <stdio.h>

/*
 * Generic CPUID function (stolen from Linux 2.5.59)
 */
static inline void cpuid(int op, int *eax, int *ebx, int *ecx, int *edx)
{
       __asm__("cpuid"
              : "=a" (*eax),
                "=b" (*ebx),
                "=c" (*ecx),
                "=d" (*edx)
              : "0" (op));
}

void registerToAscii(int reg)
{
  printf("%c", reg);
  reg = reg >> 8;
  printf("%c", reg);
  reg = reg >> 8;
  printf("%c", reg);
  reg = reg >> 8;
  printf("%c", reg);
}

int main(int argc, char *argv[])
{
  int a, b, c, d;

  printf("Calling cpuid with an input operand of zero\n\n");
  cpuid(0, &a, &b, &c, &d);

  printf("Got back: 0x%08x 0x%08x 0x%08x 0x%08x\n", a, b, c, d);
  printf("This decodes to: ");
  registerToAscii(b);
  registerToAscii(d);
  registerToAscii(c);
  printf("\n");
}
