#include "log.h"

int main() {
    char* str = "some string";
    debug("Debug log!");
    info("Info log (str = '%s')", str);
    warn("Warning!");
    error("Error!");

    return 0;
}
