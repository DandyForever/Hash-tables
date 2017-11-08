#include <iostream>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <cstdlib>

struct String{
    char* str;
    int length;
};

int sizeofFILE (FILE *filein);
char* make_buffer ();
int num_of_str (const char* buffer);
void make_text (String* text, char* buffer);


char* make_buffer ()
{
    FILE* filein = fopen ("text.txt", "rb");
    assert(filein);

    const int file_length = sizeofFILE (filein);
    assert (file_length != -1);

    char* buffer = (char*) calloc (file_length + 1, sizeof (*buffer));
    assert(buffer);

    fread (buffer, file_length, sizeof (*buffer), filein);

    return buffer;

    /*const int num_of_strings = num_of_str (buffer);
    assert (num_of_strings != -1);

    String* text = (String*) calloc(num_of_strings, sizeof (*text));
    assert(text);
    make_text (text, buffer);*/
}

int sizeofFILE (FILE *filein){
    fseek (filein, 0, SEEK_END);
    const int file_length = ftell (filein);
    rewind (filein);
    return file_length;
}

int num_of_str (const char* buffer){
    int num_str = 1, i = 0;
    while (buffer[i] != '\0'){
        if (buffer[i] == '\n') num_str++;
        i++;
    }
    return num_str;
}

void make_text (String* text, char* buffer){
    int num_of_strings = 0;
    text[num_of_strings++].str = buffer;
    int i = 0;
    while (buffer[i] != '\0'){
        if (buffer[i] == '\n'){
            buffer[i] = '\0';
            assert (buffer[i] != '\n');
            assert (buffer[i] == '\0');
            text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
            text[num_of_strings++].str = buffer + i + 1;
        }
        i++;
    }
    text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
    assert (num_of_strings != 0);
}
