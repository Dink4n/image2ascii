#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#define PIXEL_PER_CHAR 9

#define max(a, b) ((a) > (b) ? (a) : (b))

const wchar_t *density = L"N@#W$9876543210?!abc;:+=-,._ ";
/* const wchar_t *density = L".:░▒▓█"; */

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path to image>\n", argv[0]);
        return 0;
    }

    int imageWidth, imageHeight, imageComp;

    float avg;
    int densityLen = wcslen(density);
    int pixelIndex;
    int charIndex, r, g, b;
    int x, y, p;

    setlocale(LC_ALL, "");
    unsigned char *data = stbi_load(argv[1], &imageWidth, &imageHeight, &imageComp, 0);

    if (data == NULL) {
        fprintf(stderr, "%s\n", stbi_failure_reason());
        return 0;
    }

    initscr();
    int scrWidth = getmaxx(stdscr);
    int scrHeight = getmaxy(stdscr);

    int widthPerChar = imageWidth / scrWidth;
    int heightPerChar = imageHeight / scrHeight;
    endwin();

    /* More means smaller ascii art */
    int pixelPerChar = max(widthPerChar, heightPerChar);

    pixelPerChar = pixelPerChar < 1 ? 1 : pixelPerChar;
    pixelPerChar *= 1.15;

    /* printf("widthPerChar: %d, heightPerChar: %d, windowWidth: %f, windowHeight: %f\n", widthPerChar, heightPerChar, (float)imageWidth / widthPerChar, (float)imageHeight / heightPerChar); */
    wchar_t row[((imageWidth / pixelPerChar) * 2)];
    for (y = 0; y < imageHeight; y += pixelPerChar) {
        for (x = 0, p = 0; x < imageWidth; x += pixelPerChar, p += 2) {
            /* Reset rgb */
            r = 0; g = 0; b = 0;

            /* Add all the colors of the surrounding pixels */
            for (int i = 0; i < pixelPerChar; i++) {
                for (int j = 0; j < pixelPerChar; j++) {
                    pixelIndex = ((x + i) + ((y + j) * imageWidth)) * imageComp;

                    /* Check if pixelIndex is out of bounds */
                    if (pixelIndex >= imageWidth * imageHeight * imageComp)
                        break;

                    r += data[pixelIndex + 0];
                    g += data[pixelIndex + 1];
                    b += data[pixelIndex + 2];
                }
            }

            /* Get the average color of the surrounding pixels */
            if (pixelPerChar > 1) {
                r /= pixelPerChar * pixelPerChar;
                g /= pixelPerChar * pixelPerChar;
                b /= pixelPerChar * pixelPerChar;
            }

            /* Convert rgb to grayscale */
            avg = (r + g + b) / 3.0f;

            /* Map avg to density length - 1 */
            charIndex = (avg / 255) * (densityLen - 1);

            /* tty's are wide. So, use the same character twice */
            /* row[p] = density[densityLen - charIndex - 1]; */
            /* row[p + 1] = density[densityLen - charIndex - 1]; */
            row[p] = density[charIndex];
            row[p + 1] = density[charIndex];

            /* printf("pixelIndex: %d, charIndex: %d, (len - charIndex): %d, r:
             * %d, g: %d, b: %d\n", pixelIndex, charIndex, len - charIndex, r,
             * g, b); */
        }
        /* Null Terminate */
        row[((imageWidth / pixelPerChar) * 2) - 1] = '\0';

        printf("%ls\n", row);
    }
    printf("width: %d, height: %d, components: %d\n", imageWidth, imageHeight, imageComp);
    printf("screen width: %d, screen height: %d\n", scrWidth,  scrHeight);

    stbi_image_free(data);
    return 0;
}
