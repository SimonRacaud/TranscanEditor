#ifndef MODEL_UTILS_H
#define MODEL_UTILS_H

#include <stddef.h>
#include <string>

struct Rectangle {
    size_t width;
    size_t height;
    int top;
    int left;
};

struct Vector2i {
    int x;
    int y;
};

#endif
