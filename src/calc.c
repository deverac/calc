#include "tinyexpr.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// These defines should be longest string + 1 (for null).
#define NAME_LEN 6
#define NOTE_LEN 30

// 53 bits store the mantissa of a (64-bit) double, so 2^53-1 is the largest
// integer where it and all smaller integers can be represented exactly.
#define LIMIT pow(2, 53) - 1

void print_formatted_function_names() {
    char blank[] = "";
    char names[][NAME_LEN] = {
        "abs", "acos", "asin", "atan", "atan2", "ceil",
        "cos", "cosh", "deg", "e", "exp", "fac",
        "floor", "ln", "log", "log10", "ncr", "npr",
        "pi", "pow", "rad", "sin", "sinh", "sqrt",
        "tan", "tanh"
    };
    char notes[][NOTE_LEN] = {
        "factorial: fac(5)=120",
        "combination: ncr(6,2)=15",
        "permutation: npr(6,2)=30",
        "atan has range (-pi/2, pi/2)",
        "atan2 has range (-pi, pi]",
        "exp is e^n"
    };
    int name_cnt = sizeof(names) / NAME_LEN;
    int note_cnt = sizeof(notes) / NOTE_LEN;
    char * s1;
    char * s2;
    char * s3;
    char * s4;
    char * s5;
    int i;
    int cols = 4;
    int rows = name_cnt / cols;

    if ((name_cnt * 1.0 / cols) - (name_cnt / cols) > 0.001) {
        rows += 1;
    }
    for (i=0; i<rows; i++) {
        s1 = (i        < name_cnt ? names[i       ] : blank);
        s2 = (i+  rows < name_cnt ? names[i+  rows] : blank);
        s3 = (i+2*rows < name_cnt ? names[i+2*rows] : blank);
        s4 = (i+3*rows < name_cnt ? names[i+3*rows] : blank);
        s5 = (i        < note_cnt ? notes[i       ] : blank);
        printf("     %-*s  %-*s  %-*s  %-*s     %-*s\n", NAME_LEN, s1, NAME_LEN, s2, NAME_LEN, s3, NAME_LEN, s4, NOTE_LEN, s5);
    }
}


void help(char * progname) {
    printf("A simple command-line calculator.\n");
    printf("Usage: %s \"expression\" [/b] [/h] [/i] [/?]\n", progname);
    printf("\n");
    printf("  expression  A mathematical expression e.g. \"3^2 + sin(30 * pi / 180)\"\n");
    printf("  /b          Display result in hex, dec, oct, bin format\n");
    printf("  /h, /?      Show this help\n");
    printf("  /i          Display result as an integer\n");
    printf("\n");
    printf("  Operators: +  -  /  *  ^  %%  (  )  ,\n");
    printf("  Functions:\n");
    print_formatted_function_names();
    printf("\n");
    printf("  Environment vars can be used in expression.\n");
    printf("  Sci. notation (1e3), hex (0xNN), octal (0cNN), binary (0bNN) is accepted.\n");
    printf("  Trigonometric functions accept and return radians.\n");
    printf("  Commas separate list items; only last item is evaluated.\n");
}


void print_binary(double a) {
    long long val = (long long) a;
    unsigned long long i = 1ULL << ((8 * sizeof(long long)) - 1);

    printf("0b");
    if (val) {
        while (i > 0 && ! (val & i)) {
            i >>= 1;
        }
        while (i > 0) {
            (val & i) ? printf("1") : printf("0");
            i >>= 1;
        }
    } else {
        printf("0");
    }
    printf("\n");
}


int main(int argc, char *argv[])
{
    int error;
    double result;
    int showIntResult = 0;
    int showBases = 0;
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
        } else if (strcmp(*arg, "/b") == 0) {
            showBases = 1;
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

    if (isnan(result)) {
        // Sanity check. 'error' should be non-zero if result is NaN.
        fprintf(stderr, "Result is not a number.\n");
        exit(1);
    }

    if (isinf(result)) {
        fprintf(stderr, "Result is infinite.\n");
        exit(1);
    }

    if (fabs(result) > LIMIT) {
        if (showBases) {
            fprintf(stderr, "Result is out of range and cannot be converted.\n");
            exit(1);
        } else {
            fprintf(stderr, "Result may be rounded.\n");
        }
    }

    if (showBases) {
        printf("0x%llX\n", (long long)result);
        printf("%.0f\n", result);
        printf("0c%llo\n",(long long)result);
        print_binary(result);
    } else if (showIntResult) {
        printf("%.0f\n", result);
    } else {
        printf("%f\n", result);
    }

    return 0;
}
