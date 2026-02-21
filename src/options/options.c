#include "options.h"

parameters_t optionsToParameters(int argc, char* argv[]) {
    parameters_t params = getNullParameters();

    LONG_OPTIONS

    optind = 1;
    int opt;
    int long_index = 0;
    while ((opt = getopt_long(argc, argv, GETOPT_STRING, long_opts, &long_index)) != -1) {
        if (opt == 0){
            const char *name = long_opts[long_index].name;
            if (strcmp(name, "fps") == 0){
                params.fpsDefined = true;
                params.fps = atoi(optarg);
            }
        } else {
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
                    params.fullscreenDefined = true;
                    params.fullscreen = true;
                    break;
                case 'p':
                    params.partyDefined = true;
                    params.party = true;
                    break;
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
    }

    return params;

}

void usage(const char* prog) {
    printf(
"Game of Life (SDL3) — Conway cellular automaton\n\n"
"Usage:\n"
"  %s [options] [config-file]\n\n"
"Options:\n"
"  -r, --rows <n>          Grid rows\n"
"  -c, --cols <n>          Grid columns\n"
"  -b, --block-size <px>   Cell size in pixels\n"
"  -f, --fps <n>           Frames per second\n"
"  -d, --delay <ms>        Simulation delay (milliseconds)\n"
"  -h, --help              Show this help and exit\n\n"
"Config file:\n"
"  If a positional argument is provided, it is treated as a config file.\n"
"  CLI options override config values.\n\n"
"Examples:\n"
"  %s\n"
"      Run with defaults.\n\n"
"  %s config.cfg\n"
"      Load parameters from config.cfg.\n\n"
"  %s --rows 80 --cols 120 --block-size 8\n"
"      Override grid size and cell size.\n\n"
"  %s -d 50 -f 60\n"
"      Faster simulation with smoother rendering.\n\n"
"Notes:\n"
"  • Arrow keys move the grid.\n"
"  • Click to toggle cells.\n"
"  • Space pauses/resumes simulation.\n"
"  • +/- adjust simulation speed (Ctrl for larger step).\n",
        prog, prog, prog, prog, prog
    );
}



