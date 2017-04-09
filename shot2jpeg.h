#ifndef _SHOT2JPEG
#define _SHOT2JPEG

#include <stdio.h>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <jpeglib.h>

xcb_image_t *take_screenshot(xcb_connection_t *conn, xcb_screen_t *screen);
xcb_pixmap_t image_to_pixmap(xcb_connection_t *conn, xcb_screen_t *screen, xcb_image_t *image);
void get_rgba_image_data(xcb_image_t *image, uint8_t *rgba);
void get_rgb_image_data(xcb_image_t *image, uint8_t *rgb);
void write_to_jpeg(char *filename, int quality, xcb_image_t *image);
void write_to_jpeg_buffer(FILE *stream, int quality, xcb_image_t *image);

#endif