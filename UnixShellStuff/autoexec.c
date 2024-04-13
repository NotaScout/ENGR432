#include <unistd.h>

int main() {
    char *argv[] = {"ls", "-l", "-h", "-a", NULL}; // << execs this, need the NULL

	execvp(argv[0], argv);

    return 0;
}