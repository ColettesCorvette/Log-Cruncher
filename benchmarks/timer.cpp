#include "timer.hpp"
#include <iostream>
#include <cstdio> // snprintf

std::string formatSize(size_t bytes) {
    const char *suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double dblBytes = static_cast<double>(bytes);

    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i < 4; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    char output[50];
    snprintf(output, sizeof(output), "%.2f %s", dblBytes, suffixes[i]);

    return std::string(output);
}
