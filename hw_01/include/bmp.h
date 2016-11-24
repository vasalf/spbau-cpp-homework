#ifndef BMP_H_
#define BMP_H_

#include <stdint.h>

enum endianness {
    ES_LITTLE_ENDIAN,
    ES_BIG_ENDIAN
};

struct bmp_file_header {
    enum endianness endianness;
    uint32_t file_size; /* to ensure that sizeof(type) == 32 */
    uint32_t reserved;
    uint32_t pixel_array_offset;
};

struct dib_header {
    uint32_t size_of_dib_header;
    int32_t  bmp_width; /* a signed integer, yup */
    int32_t  bmp_height;
    uint16_t color_planes; /* must be 1 */
    uint16_t bits_per_pixel; /* we support 24 */
    uint32_t compression_method; /* four bytes to that stuff */
    uint32_t image_size; /* might be zero */
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t color_pallete;
    uint32_t important_colors;
};

struct bmp_file {
    struct bmp_file_header bmp_file_header;
    struct dib_header dib_header;
    uint8_t *data;
};

int load_bmp(const char *filename, struct bmp_file *bmpf);
int crop_rotate(struct bmp_file *in, int x, int y, int w, int h, struct bmp_file *out);
int save_bmp(const char *filename, struct bmp_file *bmpf);
void destroy_bmp_file(struct bmp_file *bmpf);

#endif //BMP_H_
