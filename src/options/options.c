#include "options.h"

parameters_t optionsToParameters(int argc, char* argv[]) {
    parameters_t params = getNullParameters();

    struct option long_opts[] = {
        {"rows",        required_argument, 0, 'r'},
        {"cols",        required_argument, 0, 'c'},
        {"block-size",  required_argument, 0, 'b'},
        {"fps",         required_argument, 0, 'f'},
        {"delay",       required_argument, 0, 'd'},
        {"help",        no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };


    int opt;
    while ((opt = getopt_long(argc, argv, "r:c:b:f:d:h", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'r':
                params.rowsDefined = true;
                params.rows = atoi(optarg);
                break;
            case 'c':
                params.colsDefined = true;
                params.cols = atoi(optarg);
                break;
            case 'b':
                params.blockSizeDefined = true;
                params.blockSize = atoi(optarg);
                break;
            case 'f':
                params.fpsDefined = true;
                params.fps = atoi(optarg);
                break;
            case 'd':
                params.delayDefined = true;
                params.delay = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                exit(0);
                break;
            case '?':
                critical("Unknown option. Use %s --help", argv[0]);
                break;
        }
    }

    return params;

}

void usage(const char* prog) {
    printf("\nhallo\n");
    return;
}



