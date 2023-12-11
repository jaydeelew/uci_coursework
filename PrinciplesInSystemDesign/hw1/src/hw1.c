#include "hw1.h"
#include <string.h>

int main(int argc, char **argv) {

    int option;
    int Lflag = 0, Wflag = 0, Sflag = 0, Oflag = 0, Pflag = 0;
    char *Larg = NULL, *Warg = NULL, *Sarg = NULL;
    while ((option = getopt(argc, argv, "L:W:S:OP")) != -1) {
        switch (option) {
            case 'L' :
                Lflag = 1;
                if (Lflag == 1 && Wflag == 1) {
                    fprintf(stderr,"Cannot select both -L and -W\n");
                    fprintf(stderr, "exit(1)\n");
                    exit(1);
                }
                Larg = optarg;
                break;
            case 'W' :
                Wflag = 1;
                if (Lflag == 1 && Wflag == 1) {
                    fprintf(stderr,"Cannot select both -L and -W\n");
                    fprintf(stderr, "exit(1)\n");
                    exit(1);
                }
                Warg = optarg;
                break;
            case 'S' :
                Sflag = 1;
                Sarg = optarg;
                break;
            case 'O' :
                Oflag = 1;
                break;
            case 'P' :
                if (Wflag == 0) {
                    fprintf(stderr, "Option -P can only be used with option W\n");
                    fprintf(stderr, "exit(1)\n");
                    exit(1);
                }
                Pflag = 1;
                break;
            case '?' :
                if (optopt == 'L' || optopt == 'W' || optopt == 'S') {
                    fprintf(stderr, "exit(1)\n");
                    exit(1);
                }
                else {
                    fprintf(stderr, "exit(1)\n");
                    exit(1);
                }
                break;
            //default:
            //    fprintf(stderr, "getopt");
        }
    }
    printf( "Lflag: %d, Wflag: %d, Sflag: %d, Oflag: %d, Pflag: %d\n", Lflag, Wflag, Sflag, Oflag, Pflag);
    printf( "Larg: %s, Warg: %s, Sarg: %s\n\n", Larg, Warg, Sarg);

    if (Lflag == 1) {
        process_L(Larg);
    }

    return 0;
}