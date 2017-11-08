#include <iostream>
#include "List_t.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


const unsigned int Tablesize = 1000, Unsint = 2147483648;


struct String{
    char* str;
    int length;
};

int sizeofFILE                (FILE *filein);
char* make_buffer             ();
int num_of_str                (const char* buffer);
void make_text                (String* text, char* buffer);
unsigned int hash_one         (const String string);
unsigned int hash_ded         (const String string);
unsigned int hash_len         (const String string);
unsigned int hash_sum         (const String string);
unsigned int hash_xor         (const String string);
unsigned int hash_gnu         (const String string);
void Make_hash_table          (String* text, const int number_of_strings, const char* filename,
                               unsigned int hash_func (const String));

int main()
{
    char* buffer = make_buffer ();

    const int num_of_strings = num_of_str (buffer);

    String* text = (String*) calloc(num_of_strings, sizeof (*text));
    assert(text);
    make_text (text, buffer);

    Make_hash_table (text, num_of_strings, "table_1.csv", hash_one);
    Make_hash_table (text, num_of_strings, "table_2.csv", hash_ded);
    Make_hash_table (text, num_of_strings, "table_3.csv", hash_len);
    Make_hash_table (text, num_of_strings, "table_4.csv", hash_sum);
    Make_hash_table (text, num_of_strings, "table_5.csv", hash_xor);
    Make_hash_table (text, num_of_strings, "table_6.csv", hash_gnu);

    printf ("%d", 21 >> 1);
    return 0;
}

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
        if (!isalpha(buffer[i]) && buffer[i] != '\0') num_str++;
        i++;
    }
    return num_str;
}

void make_text (String* text, char* buffer)
{
    int num_of_strings = 0;
    text[num_of_strings++].str = buffer;
    int i = 0;
    while (buffer[i] != '\0'){
        if (!isalpha(buffer[i]) && buffer[i] != '\0'){
            buffer[i] = '\0';
            text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
            text[num_of_strings++].str = buffer + i + 1;
        }
        i++;
    }

    text[num_of_strings - 1].length = buffer + i - text[num_of_strings - 1].str + 1;
}

void Make_hash_table (String* text, const int number_of_strings, const char* filename,
                      unsigned int hash_func (const String))
{
    FILE* output_file = fopen (filename, "w");
    assert (output_file);

    List_t hash_table[Tablesize] = {};

    for (int i = 0; i < number_of_strings; i++)
    {
        if (text[i].str[0] != '\0')
        {
            unsigned int hash_word = hash_func (text[i]) % Tablesize;

            bool isINlist = 0;
            int index = hash_word;

            for (Elem* current = hash_table[index].HeadEl (); current != nullptr; current = current->next)
            {
                if (!strcmp (text[i].str, current->value))
                    isINlist = 1;
            }

            if (!isINlist)
                hash_table[index].PushBack (text[i].str);
        }
    }

    for (int i = 0; i < Tablesize; i++)
    {
        fprintf (output_file, "%d; %d\n", i, hash_table[i].Size ());
    }

    fclose (output_file);
}

unsigned int hash_one (const String string)
{
    return 1;
}

unsigned int hash_ded (const String string)
{
    return tolower (string.str[0]) - 'a';
}

unsigned int hash_len (const String string)
{
    return string.length;
}

unsigned int hash_sum (const String string)
{
    unsigned int hash = 0;

    for (const char* cur = string.str; *cur != '\0'; cur++)
    {
        hash += (*cur);
    }

    return hash;
}

unsigned int hash_xor (const String string)
{
    unsigned int hash = 0;

    for (const char* cur = string.str; *cur != '\0'; cur++)
    {
        hash = ( (hash >> 1) | (hash << 31)) ^ (*cur);
    }

    return hash;
}

unsigned int hash_gnu (const String string)
{
    unsigned int hash = 0;

    for (const char* cur = string.str; *cur != '\0'; cur++)
    {
         hash = ( (hash << 5) + hash) + (*cur);
    }

    return hash;
}
