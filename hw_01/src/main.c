#include <bmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define unused __attribute__((unused))

void usage(void) {
    fprintf(stderr, "usage: ./main crop-rotate <in-bmp> <out-bmp> <x> <y> <w> <h>\n");
}

int read_arg(char *s) {
    char *endptr;
    int ans = strtol(s, &endptr, 10);
    if (*endptr == '\0')
        return ans;
    else
        return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 8 || strcmp(argv[1], "crop-rotate") != 0) {
        usage();
        return 1;
    }
    int x = read_arg(argv[4]);
    int y = read_arg(argv[5]);
    int w = read_arg(argv[6]);
    int h = read_arg(argv[7]);
    if (x < 0 || y < 0 || w < 0 || h < 0) {
        usage();
        return 1;
    }
    struct bmp_file in;
    if (load_bmp(argv[2], &in) != 0) {
        fprintf(stderr, "Failed to load bmp file\n");
        return 1;
    }
    struct bmp_file out;
    if (crop_rotate(&in, x, y, w, h, &out) != 0) {
        fprintf(stderr, "Failed to crop-rotate\n");
        return 1;
    }
    if (save_bmp(argv[3], &out) != 0) {
        fprintf(stderr, "Failed to write the rotated image\n");
        return 1;
    }
    destroy_bmp_file(&in);
    destroy_bmp_file(&out);
    return 0;
}
