// test for shot2jpeg

#include <sys/time.h>

#include "shot2jpeg.h"

int main() {
    printf("test shot2jpeg start\n");

    struct timeval s, ss, sss;

    xcb_connection_t *conn = xcb_connect(NULL, NULL);

    gettimeofday(&s, NULL);
    const xcb_setup_t *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    xcb_image_t *screenshot = take_screenshot(conn, screen);
    xcb_pixmap_t pixmap = image_to_pixmap(conn, screen, screenshot);
    printf("screenshot: width: %d, height: %d, size: %d, len: %lu\n", screenshot->width, screenshot->height, screenshot->size, sizeof(screenshot->data)/sizeof(uint8_t));
    printf("pixmap: %d\n", pixmap);
    gettimeofday(&ss, NULL);

    write_to_jpeg("./test.jpeg", 100, screenshot);

    gettimeofday(&sss, NULL);
    printf("shot use: %ld, write use: %ld\n",
        (ss.tv_sec - s.tv_sec) * 1000000 + (ss.tv_usec - s.tv_usec),
        (sss.tv_sec - ss.tv_sec) * 1000000 + (sss.tv_usec - ss.tv_usec));

    printf("test shot2jpeg end\n");
    return 0;
}