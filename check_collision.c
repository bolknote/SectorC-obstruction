// gcc -O3 -march=native -pthread check_collision.c -o check_collision

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define BOUND(A, B) (i == len - 1 ? (A) : (B))

#define MAXLEN 5
#define MAXCHAR '~'
#define MINCHAR ' ' + 1

short sectorc_atoi(const char *s)
{
    short n = 0;
    for (;;) {
        char c = *s++;
        if (!c) break;

        n = 10 * n + (c - '0');
    }

    return n;
}

typedef struct {
    int min, max, target, n;
} thread_args_t;

void *check_collision(void *arg)
{
    char *s = calloc(MAXLEN + 1, sizeof(char));
    thread_args_t *args = (thread_args_t*)arg;

    fprintf(stderr, "Thread #%d started ('%c' - '%c')\n", args->n, (char) args->min, (char) args->max);

    for (int len = 1; len <= MAXLEN; len++) {
        int i;

        for (i = 0; i < len - 1; i++) s[i] = MINCHAR;
        s[i] = args->min;

        for (i = 0; i < len;) {
            int hash = sectorc_atoi(s);
            if (hash == args->target) printf("%s\n", s);

            for (i = 0; ++s[i] > BOUND(args->max, MAXCHAR) && i < len; i++) {
                s[i] = BOUND(args->min, MINCHAR);
            }

        }
    }

    free(s);
    pthread_exit(NULL);
}


int main (int argc, char *argv[])
{
    if (argc < 2) return 1;
    int target = sectorc_atoi(argv[1]);
    printf("Looking for a collision for: %s\n", argv[1]);

    int N = sysconf(_SC_NPROCESSORS_ONLN);

    pthread_t threads[N];
    thread_args_t thread_arg[N];

    double chunk = MAX((double) (MAXCHAR - MINCHAR - N) / N, 1.);
    double start = MINCHAR;

    printf("Chunk: %f\n", chunk);

    for (int i = 0; i < N && start <= MAXCHAR; i++) {
        double stop = MIN(start + chunk, MAXCHAR);

        thread_arg[i] = (thread_args_t) { (int) start, (int) stop, target, i + 1 };
        pthread_create(&threads[i], NULL, check_collision, &thread_arg[i]);

        start = stop + 1.;
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
