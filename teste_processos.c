#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#define MAX_PROC 64
void cpu_bound(long ops) {
    long i;
    double x = 1.0;
    for (i = 0; i < ops; i++) x *= 1.0000001;
}
void io_bound(long ops) {
    long i;
    FILE *f = fopen("/tmp/iotmp", "w");
    for (i = 0; i < ops; i++) fprintf(f, "x");
    fclose(f);
}
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <nproc> <io_ops> <cpu_ops>\n", argv[0]);
        return 1;
    }
    int nproc = atoi(argv[1]);
    long io_ops = atol(argv[2]);
    long cpu_ops = atol(argv[3]);
    int i;
    struct timeval start, end;
    pid_t pids[MAX_PROC];
    for (i = 0; i < nproc; i++) {
        gettimeofday(&start, NULL);
        pids[i] = fork();
        if (pids[i] == 0) {
            if (i % 2 == 0) io_bound(io_ops);
            else cpu_bound(cpu_ops);
            gettimeofday(&end, NULL);
            double elapsed = (end.tv_sec - start.tv_sec) +
                             (end.tv_usec - start.tv_usec) / 1000000.0;
            printf("%d %.6f\n", i, elapsed);
            exit(0);
        }
    }
    for (i = 0; i < nproc; i++) wait(NULL);
    return 0;
}
