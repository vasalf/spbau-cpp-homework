#ifndef _BSD_SOURCE
#define _BSD_SOURCE 1
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <endian.h>
#include <bmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static uint32_t read_uint32_t(FILE *f, enum endianness endianness) {
    uint32_t ans;
    if (fread(&ans, 4, 1, f) != 1) // Weird.
        return 0;
    if (endianness == ES_BIG_ENDIAN) {
        ans = be32toh(ans);
    } else {
        ans = le32toh(ans);
    }
    return ans;
}

static uint16_t read_uint16_t(FILE *f, enum endianness endianness) {
    uint16_t ans;
    if (fread(&ans, 2, 1, f) != 1) // Weird.
        return 0;
    if (endianness == ES_BIG_ENDIAN) {
        ans = be16toh(ans);
    } else {
        ans = le16toh(ans);
    }
    return ans;
}

#define ROW_SIZE(w, bp) ((((w) * (bp) + 31) / 32) * 4)
#define IMAGE_SIZE(h, w, bp) ((h) * ROW_SIZE(w, bp))

int load_bmp(const char *filename, struct bmp_file *bmpf) {
    FILE *f = fopen(filename, "rb");
    if (!f)
        return 1;
    
    uint16_t bm;
    if (fread(&bm, 2, 1, f) != 1)
        return 1;
    if (bm == 0x4d42)
        bmpf->bmp_file_header.endianness = ES_LITTLE_ENDIAN;
    else
        bmpf->bmp_file_header.endianness = ES_BIG_ENDIAN;

    bmpf->bmp_file_header.file_size = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->bmp_file_header.reserved = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->bmp_file_header.pixel_array_offset = read_uint32_t(f, bmpf->bmp_file_header.endianness);
   
    bmpf->dib_header.size_of_dib_header = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.bmp_width = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.bmp_height = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.color_planes = read_uint16_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.bits_per_pixel = read_uint16_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.compression_method = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.image_size = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.horizontal_resolution = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.vertical_resolution = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.color_pallete = read_uint32_t(f, bmpf->bmp_file_header.endianness);
    bmpf->dib_header.important_colors = read_uint32_t(f, bmpf->bmp_file_header.endianness);
     
    uint32_t image_size = IMAGE_SIZE(bmpf->dib_header.bmp_height, bmpf->dib_header.bmp_width, bmpf->dib_header.bits_per_pixel);

    bmpf->data = malloc(image_size);
    if (!bmpf->data)
        return 1;

    if (fread(bmpf->data, image_size, 1, f) != 1) // Weird.
        return 1;

    fclose(f);
    return 0;
}

static uint32_t offset_of(struct bmp_file *in, int i, int j) {
    // 8|24
    return (in->dib_header.bmp_height - i - 1) * ROW_SIZE(in->dib_header.bmp_width, in->dib_header.bits_per_pixel) + j * (in->dib_header.bits_per_pixel) / 8;
}

static void read_a_pixel(struct bmp_file *in, int i, int j, uint8_t where[]) {
    memcpy(where, in->data + offset_of(in, i, j), in->dib_header.bits_per_pixel / 8);
}

static void write_a_pixel(struct bmp_file *in, int i, int j, uint8_t from[]) {
    memcpy(in->data + offset_of(in, i, j), from, in->dib_header.bits_per_pixel / 8);
}

int crop_rotate(struct bmp_file *in, int x, int y, int w, int h, struct bmp_file *out) {
    out->bmp_file_header = in->bmp_file_header;
    out->dib_header = in->dib_header;
    
    out->dib_header.bmp_width = h;
    out->dib_header.bmp_height = w;
    
    uint32_t image_size = IMAGE_SIZE(w, h, in->dib_header.bits_per_pixel);
    out->dib_header.image_size = image_size;

    out->data = malloc(image_size);
    if (!out->data)
        return 1;
    memset(out->data, 0, image_size); // alignment

    uint8_t pixel[3];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            read_a_pixel(in, i + y, j + x, pixel);
            write_a_pixel(out, j, h - i - 1, pixel);
        }
    }

    out->bmp_file_header.file_size = 14 + 40 + image_size;
    return 0;
}

void write_uint32_t(FILE *f, uint32_t val, enum endianness endianness) {
    if (endianness == ES_BIG_ENDIAN) {
        val = htobe32(val);
    } else {
        val = htole32(val);
    }
    fwrite(&val, 4, 1, f);
}

void write_uint16_t(FILE *f, uint16_t val, enum endianness endianness) {
    if (endianness == ES_BIG_ENDIAN) {
        val = htobe16(val);
    } else {
        val = htole16(val);
    }
    fwrite(&val, 2, 1, f);
}

int save_bmp(const char *filename, struct bmp_file *bmpf) {
    FILE *out = fopen(filename, "wb");
    if (!out)
        return 1;

    write_uint16_t(out, 0x4d42, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->bmp_file_header.file_size, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->bmp_file_header.reserved, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->bmp_file_header.pixel_array_offset, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.size_of_dib_header, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.bmp_width, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.bmp_height, bmpf->bmp_file_header.endianness);
    write_uint16_t(out, bmpf->dib_header.color_planes, bmpf->bmp_file_header.endianness);
    write_uint16_t(out, bmpf->dib_header.bits_per_pixel, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.compression_method, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.image_size, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.horizontal_resolution, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.vertical_resolution, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.color_pallete, bmpf->bmp_file_header.endianness);
    write_uint32_t(out, bmpf->dib_header.important_colors, bmpf->bmp_file_header.endianness);

    fwrite(bmpf->data, ROW_SIZE(bmpf->dib_header.bmp_width, bmpf->dib_header.bits_per_pixel), bmpf->dib_header.bmp_height, out);

    fclose(out);
    return 0;
}

void destroy_bmp_file(struct bmp_file *bmpf) {
    free(bmpf->data);
}
