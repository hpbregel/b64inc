#include <stdio.h>


char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int column = 0;

void print_letter(char c) {
    if (column == 76) {
        printf("\n");
        column = 0;
    }
    printf("%c", c);
    column = column + 1;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./b64inc filename\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Could not open the file.\n");
        return 1;
    }

    unsigned char bytes[3];
    int bytes_read;

    while (1) {

        bytes_read = fread(bytes, 1, 3, file);

        if (bytes_read == 0) {
            break;
        }

        int first = bytes[0];
        int second = 0;
        int third = 0;
        if (bytes_read >= 2) {
            second = bytes[1];
        }
        if (bytes_read == 3) {
            third = bytes[2];
        }

        int piece1 = first / 4;

        int piece2 = (first % 4) * 16 + second / 16;


        int piece3 = (second % 16) * 4 + third / 64;

        int piece4 = third % 64;


        if (bytes_read == 3) {
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter(letters[piece3]);
            print_letter(letters[piece4]);
        }
        else if (bytes_read == 2) {
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter(letters[piece3]);
            print_letter('=');
        }
        else {
            print_letter(letters[piece1]);
            print_letter(letters[piece2]);
            print_letter('=');
            print_letter('=');
        }
    }

    if (column > 0) {
        printf("\n");
    }

    fclose(file);
    return 0;
}
