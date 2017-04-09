// screenshot to jpeg for linux

#include "shot2jpeg.h"

#define IMG_AT(x, y, i) image->data[((y) * image->width + (x)) * 4 + (i)]
#define RGBA_AT(x, y, i) rgba[((y) * image->width + (x)) * 4 + (i)]
#define RGB_AT(x, y, i) rgb[((y) * image->width + (x)) * 3 + (i)]

xcb_image_t *take_screenshot(xcb_connection_t *conn, xcb_screen_t *screen) {
    return xcb_image_get(conn,
        screen->root,
        0, 0,
        screen->width_in_pixels, screen->height_in_pixels,
        UINT32_MAX,
        XCB_IMAGE_FORMAT_Z_PIXMAP);
}

xcb_pixmap_t image_to_pixmap(xcb_connection_t *conn, xcb_screen_t *screen, xcb_image_t *image) {
    xcb_pixmap_t pixmap = xcb_generate_id(conn);
    xcb_create_pixmap(conn, 24, pixmap, screen->root, image->width, image->height);

    xcb_gcontext_t gc = xcb_generate_id(conn);
    xcb_create_gc(conn, gc, pixmap,
        XCB_GC_FOREGROUND | XCB_GC_BACKGROUND,
        (uint32_t[]){ screen->black_pixel, 0xffffff });

    xcb_image_put(conn, pixmap, gc, image, 0, 0, 0);

    return pixmap;
}

void get_rgba_image_data(xcb_image_t *image, uint8_t *rgba) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGBA_AT(x, y, 0) = IMG_AT(x, y, 2); // r
            RGBA_AT(x, y, 1) = IMG_AT(x, y, 1); // g
            RGBA_AT(x, y, 2) = IMG_AT(x, y, 0); // b
            RGBA_AT(x, y, 3) = IMG_AT(x, y, 3); // a
        }
    }
}

void get_rgb_image_data(xcb_image_t *image, uint8_t *rgb) {
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            RGB_AT(x, y, 0) = IMG_AT(x, y, 2); // r
            RGB_AT(x, y, 1) = IMG_AT(x, y, 1); // g
            RGB_AT(x, y, 2) = IMG_AT(x, y, 0); // b
        }
    }
}

void write_to_jpeg(char *filename, int quality, xcb_image_t *image) {
    uint8_t data[image->width*image->height*3];
    get_rgb_image_data(image, data);
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;
    JSAMPROW row_pointer[1];
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = image->width * 3;

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &data[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}