// test for shot2jpeg

#include <sys/time.h>

#include "shot2jpeg.h"

int get_int(char *str) {
    int result = 0, sign = 1;

    while (('-' == (*str)) || ((*str) == '+')) {
        if (*str == '-')
            sign = sign * -1;
        str++;
    }
    while ((*str >= '0') && (*str <= '9')) {
        result = (result * 10) + ((*str) - '0');
        str++;
    }
    return (result * sign);
}

int main(int argc, char *argv[]) {
    int quality = 100;
    int frames = 200;
    printf("test shot2jpeg start\n");

    if (argc == 2) {
        quality = get_int(argv[1]);
    } else if (argc > 2) {
        printf("Too many arguments supplied!\n");
        return 0;
    }
    printf("Quality: %d\n", quality);

    struct timeval s, ss, sss;

    xcb_connection_t *conn = xcb_connect(NULL, NULL);

    gettimeofday(&s, NULL);
    const xcb_setup_t *setup = xcb_get_setup(conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    xcb_screen_t *screen = iter.data;

    for (int i = 0; i < frames; i++) {
        xcb_image_t *screenshot;
        screenshot = take_screenshot(conn, screen);
        xcb_pixmap_t pixmap = image_to_pixmap(conn, screen, screenshot);
        printf("screenshot: width: %d, height: %d, size: %d\n", screenshot->width, screenshot->height, screenshot->size);
        printf("pixmap: %d\n", pixmap);
        uint8_t data[screenshot->width*screenshot->height*4];
        struct timeval t, tt;
        gettimeofday(&t, NULL);
        get_rgba_image_data2(screenshot, data);
        xcb_image_destroy(screenshot);
        gettimeofday(&tt, NULL);
        printf("BGRA to RGBA use: %.3fs\n",
            ((tt.tv_sec - t.tv_sec) * 1000000 + (tt.tv_usec - t.tv_usec))/1000000.0);
    }
    gettimeofday(&ss, NULL);

    xcb_image_t *screenshot;
    screenshot = take_screenshot(conn, screen);
    xcb_pixmap_t pixmap = image_to_pixmap(conn, screen, screenshot);
    write_to_jpeg("./test.jpeg", quality, screenshot);
    xcb_image_destroy(screenshot);

    gettimeofday(&sss, NULL);
    printf("shot use: %.3fs, write use: %.3fs, fps: %.3f\n",
        ((ss.tv_sec - s.tv_sec) * 1000000 + (ss.tv_usec - s.tv_usec))/1000000.0,
        ((sss.tv_sec - ss.tv_sec) * 1000000 + (sss.tv_usec - ss.tv_usec))/1000000.0,
        frames/(((ss.tv_sec - s.tv_sec) * 1000000 + (ss.tv_usec - s.tv_usec))/1000000.0));

    printf("test shot2jpeg end\n");
    return 0;
}