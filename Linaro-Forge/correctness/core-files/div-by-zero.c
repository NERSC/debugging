#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

void print_fraction(int numerator, int denominator)
{
    printf("%d\n", numerator / denominator); kill (getpid(), SIGFPE);
} 


int main(int argc, char* argv[])
{
    /* Take a value out of the arguments so that loading multiple */
    /* core files with different values works in ddt. */
    int numerator = 1;
    if(argc == 2) {
        numerator = atoi(argv[1]);
    }

    /* ensure a core file is generated */
    struct rlimit limit;
    limit.rlim_cur = limit.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &limit);
    /* divide by zero */
    print_fraction(numerator, 0);

    return 0;
}
