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

    xcb_image_t *screenshot;
    for (int i = 0; i < 100; i++) {
        screenshot = take_screenshot(conn, screen);
        xcb_pixmap_t pixmap = image_to_pixmap(conn, screen, screenshot);
        printf("screenshot: width: %d, height: %d, size: %d\n", screenshot->width, screenshot->height, screenshot->size);
        printf("pixmap: %d\n", pixmap);

        char *bp;
        size_t size;
        FILE *stream;
        stream = open_memstream(&bp, &size);
        write_to_jpeg_buffer(stream, quality, screenshot);
        free(bp);
        printf("buffer: size: %d\n", size);
    }
    gettimeofday(&ss, NULL);

    char *bp;
    size_t size;
    FILE *stream;
    stream = open_memstream(&bp, &size);
    write_to_jpeg_buffer(stream, quality, screenshot);
    printf("buffer: size: %d\n", size);

    char *filename = "./test_3.jpeg";
    FILE *outfile;
    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }

    fwrite(bp, sizeof(char), size, outfile);
    fclose(outfile);
    free(bp);

    gettimeofday(&sss, NULL);
    printf("shot use: %.3fs, write use: %.3fs, fps: %.3f\n",
        ((ss.tv_sec - s.tv_sec) * 1000000 + (ss.tv_usec - s.tv_usec))/1000000.0,
        ((sss.tv_sec - ss.tv_sec) * 1000000 + (sss.tv_usec - ss.tv_usec))/1000000.0,
        100.0/(((ss.tv_sec - s.tv_sec) * 1000000 + (ss.tv_usec - s.tv_usec))/1000000.0));

    printf("test shot2jpeg end\n");
    return 0;
}