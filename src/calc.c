#include "tinyexpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void help(char * progname) {
    printf("A simple command-line calculator.\n");
    printf("Usage: %s \"expression\" [/i] [/h] [/?]\n", progname);
    printf("\n");
    printf("  expression  A mathematical expression e.g. \"3^2 + sin(30 * pi / 180)\"\n");
    printf("  /h, /?      Show help\n");
    printf("  /i          Display result as an integer\n");
    printf("\n");
    printf("  Operators: +  -  /  *  ^  %%  (  )  ,\n");
    printf("  Functions:\n");
    printf("      abs    cos    ln     pow          factorial: fac 5=120\n");
    printf("      acos   cosh   log    sin          combination: ncr(6,2)=15\n");
    printf("      asin   e      log10  sinh         permutation: npr(6,2)=30\n");
    printf("      atan   exp    ncr    sqrt         atan has range (-pi/2, pi/2)\n");
    printf("      atan2  fac    npr    tan          atan2 has range (-pi, pi]\n");
    printf("      ceil   floor  pi     tanh         exp is e^n\n");
    printf("  Expression must be quoted if spaces are used.\n");
    printf("  Environment vars can be used in expression.\n");
    printf("  Trigonometric functions accept and return radians.\n");
    printf("  Scientific notation may be used. e.g. 1e3=1000.\n");
    printf("  Commas separate list items; only last item is evaluated.\n");
    printf("  Calculation engine by https://github.com/codeplea/tinyexpr\n");
}

int main(int argc, char *argv[])
{
    int error;
    double result;
    int showIntResult = 0;
    char ** arg;
    char * expression = NULL;

    arg = argv;
    *arg++;  // Skip progname.
    while (*arg) {
        if (strcmp(*arg, "/h") == 0 || strcmp(*arg, "/?") == 0) {
            help(argv[0]);
            exit(0);
        } else if (strcmp(*arg, "/i") == 0) {
            showIntResult = 1;
        } else if (expression == NULL) {
            expression = *arg;
        } else {
            fprintf(stderr, "Invalid parameter: %s\n", *arg);
            exit(1);
        }
        *arg++;
    }

    if (argc < 2) {
        help(argv[0]);
        exit(1);
    }

    result = te_interp(expression, &error);

    if (error) {
        fprintf(stderr, "     Expression: %s\n", expression);
        fprintf(stderr, "Error near here: %*s^\n", error-1, "");
        exit(1);
    }

    if (showIntResult) {
        printf("%ld\n", (long)result);
    } else {
        printf("%f\n", result);
    }

    return 0;
}
