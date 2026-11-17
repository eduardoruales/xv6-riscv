#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void cpu_bound(int id) {
  int start = uptime();
  volatile unsigned long x = 0;
  for (unsigned long i = 0; i < 400000000UL; i++) {
    x += i;
  }
  int end = uptime();
  printf("[CPU-bound %d] inicio=%d ticks, fin=%d ticks, duracion=%d ticks\n",
         id, start, end, end - start);
  exit(0);
}

void io_bound(int id) {
  int start = uptime();
  for (int i = 0; i < 5; i++) {
    pause (2); // Cede la CPU voluntariamente antes de agotar su quantum
  }
  int end = uptime();
  printf("[IO-bound  %d] inicio=%d ticks, fin=%d ticks, duracion=%d ticks\n",
         id, start, end, end - start);
  exit(0);
}

int main(void) {
  printf("=== Iniciando prueba MLFQ (3 colas: 1, 2, 4 ticks | boost=100) ===\n");
  for (int i = 1; i <= 3; i++) {
    if (fork() == 0) cpu_bound(i);
  }
  for (int i = 1; i <= 2; i++) {
    if (fork() == 0) io_bound(i);
  }
  for (int i = 0; i < 5; i++) {
    wait(0);
  }
  printf("=== Prueba MLFQ finalizada ===\n");
  exit(0);
}
