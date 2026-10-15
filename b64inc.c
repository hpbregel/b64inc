#include <stdio.h>

/* The 64 characters that base64 uses.
   The letter at position 0 is 'A', position 1 is 'B', ... position 63 is '/' */
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* Keeps track of how many characters are on the current line.
   base64 starts a new line after 76 characters. */
int column = 0;

/* Prints one character. If the line is full, it starts a new line first. */
void print_letter(char c) {
    if (column == 76) {
        printf("\n");
        column = 0;
    }
    printf("%c", c);
    column = column + 1;
}

int main(int argc, char *argv[]) {

    /* The user must type exactly one file name after the program name */
    if (argc != 2) {
        printf("Usage: ./b64inc filename\n");
        return 1;
    }

    /* Open the file for reading */
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    unsigned char bytes[3];   /* holds up to 3 bytes from the file */
    int bytes_read;           /* how many bytes we really got (1, 2, or 3) */

    while (1) {

        /* Read the next 3 bytes from the file */
        bytes_read = fread(bytes, 1, 3, file);

        /* If we got nothing, the file is finished */
        if (bytes_read == 0) {
            break;
        }

        /* Copy the bytes into easy-to-read variables.
           If the file ran out, the missing bytes are just 0. */
        int first = bytes[0];
        int second = 0;
        int third = 0;
        if (bytes_read >= 2) {
            second = bytes[1];
        }
        if (bytes_read == 3) {
            third = bytes[2];
        }

        /* Three bytes are 24 bits. Base64 cuts those 24 bits
           into four pieces of 6 bits each. Each piece is a number
           from 0 to 63 that we look up in the letters list.

           Bytes:   [ first ] [ second ] [ third ]
           Pieces:  [ 1 ][ 2 ][ 3 ][ 4 ]  (6 bits each)                  */

        /* Piece 1: the top 6 bits of the first byte */
        int piece1 = first / 4;

        /* Piece 2: the bottom 2 bits of the first byte,
                    then the top 4 bits of the second byte */
        int piece2 = (first % 4) * 16 + second / 16;

        /* Piece 3: the bottom 4 bits of the second byte,
                    then the top 2 bits of the third byte */
        int piece3 = (second % 16) * 4 + third / 64;

        /* Piece 4: the bottom 6 bits of the third byte */
        int piece4 = third % 64;

        /* Print the letters */
        if (bytes_read == 3) {
            /* A full group of 3 bytes: print all 4 letters */
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter(letters[piece3]);
            print_letter(letters[piece4]);
        }
        else if (bytes_read == 2) {
            /* Only 2 bytes left: print 3 letters and one '=' */
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter(letters[piece3]);
            print_letter('=');
        }
        else {
            /* Only 1 byte left: print 2 letters and two '=' */
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter('=');
            print_letter('=');
        }
    }

    /* End the last line with a new line (but print nothing for an empty file) */
    if (column > 0) {
        printf("\n");
    }

    fclose(file);
    return 0;
}