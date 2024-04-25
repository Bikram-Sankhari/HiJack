#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isDir(const char *fileName, int *file, int *dir);
char de_comment(char *src, char *dest);
char parse(char *source, char ****dest, int *last_line, int *last_word);
char scan(FILE *src, char *dest);
char translate(char ****source_code, FILE *dest, int last_line, int last_word, char *file_name);

int main()
{
    char *source_code_path;
    int source_code_path_count, file, dir;

    // Get The Source Code Path
    source_code_path = (char *)malloc(1000 * sizeof(char));
    printf("Enter the location of the Source Code:\n");
    gets(source_code_path);
    source_code_path = realloc(source_code_path, ((strlen(source_code_path) + 1) * sizeof(char)));

    // Check for File or Directory
    isDir(source_code_path, &file, &dir);

    // If the path points to a FILE
    if (file == 1)
    {
        char *file_type;
        int file_type_count = 0, dot;
        source_code_path_count = strlen(source_code_path);

        // Determine the File Type
        while (*(source_code_path + source_code_path_count) != '.')
        {
            source_code_path_count--;
        }
        dot = source_code_path_count;
        source_code_path_count++;

        file_type = (char *)malloc(5 * sizeof(char));
        do
        {
            *(file_type + file_type_count) = *(source_code_path + source_code_path_count);
            file_type_count++;
            source_code_path_count++;
        } while (*(source_code_path + source_code_path_count) != '\0');
        *(file_type + file_type_count) = '\0';
        file_type = realloc(file_type, ((file_type_count + 1) * sizeof(char)));

        // If Translatable file
        if (strcmp(file_type, "vm") == 0)
        {
            free(file_type);

            char *file_name;
            int file_name_count = 0;
            source_code_path_count = dot;

            file_name = (char *)malloc(256 * sizeof(char));

            while (*(source_code_path + source_code_path_count) != '\\')
            {
                source_code_path_count--;
            }
            source_code_path_count++;

            while (*(source_code_path + source_code_path_count) != '.')
            {
                *(file_name + file_name_count) = *(source_code_path + source_code_path_count);
                file_name_count++;
                source_code_path_count++;
            }
            file_name = realloc(file_name, ((strlen(file_name) + 1) * sizeof(char)));

            // Open The File
            FILE *source_code_file = fopen(source_code_path, "r");
            char *source_code_original;

            // Scan Source File
            source_code_original = (char *)malloc(100000000 * sizeof(char));
            scan(source_code_file, source_code_original);
            source_code_original = realloc(source_code_original, ((strlen(source_code_original) + 1) * sizeof(char)));
            fclose(source_code_file);

            // De_comment The original file
            char *source_code_filtered;
            source_code_filtered = (char *)malloc(1000000 * sizeof(char));

            de_comment(source_code_original, source_code_filtered);

            source_code_filtered = realloc(source_code_filtered, ((strlen(source_code_filtered) + 1) * sizeof(char)));

            free(source_code_original);

            // Parse the Filtered String
            char ****source_code;
            int last_line, last_word;
            source_code = (char ****)malloc(1 * sizeof(char));
            parse(source_code_filtered, source_code, &last_line, &last_word);
            free(source_code_filtered);

            // Generate The Destination File
            source_code_path = realloc(source_code_path, 1000 * sizeof(char));
            source_code_path_count = dot;
            source_code_path_count++;
            *(source_code_path + source_code_path_count) = 'a';
            source_code_path_count++;
            *(source_code_path + source_code_path_count) = 's';
            source_code_path_count++;
            *(source_code_path + source_code_path_count) = 'm';
            source_code_path_count++;
            *(source_code_path + source_code_path_count) = '\0';

            source_code_path = realloc(source_code_path, ((source_code_path_count + 1) * sizeof(char)));

            FILE *destination = fopen(source_code_path, "w");
            free(source_code_path);

            // Translate the Source Code
            translate(source_code, destination, last_line, last_word, file_name);

            fclose(destination);
            free(source_code);
        }

        // If Un-Translatable file
        else
        {
            printf("Cannot translate this file\n");
        }
    }

    // If the path points to a DIRECTORY
    else if (dir == 1)
    {
        // Open The Directory
        DIR *source_dir = opendir(source_code_path);

        int d_name_count, ext_count, fixed_end_of_source_dir = strlen(source_code_path), source_code_path_count, last_line, Last_word, file_name_count;
        char *ext, *source_code_original, ****source_code, *source_code_filtered, *file_name;
        FILE *source_code_file;

        // Generate The Destination File
        source_code_path_count = strlen(source_code_path);
        file_name_count = strlen(source_code_path);
        while (*(source_code_path + file_name_count) != '\\')
        {
            file_name_count--;
        }
        file_name_count++;

        source_code_path = realloc(source_code_path, 1000 * sizeof(char));
        *(source_code_path + source_code_path_count) = '\\';
        source_code_path_count++;
        while (*(source_code_path + file_name_count) != '\\')
        {
            *(source_code_path + source_code_path_count) = *(source_code_path + file_name_count);
            file_name_count++;
            source_code_path_count++;
        }
        *(source_code_path + source_code_path_count) = '.';
        source_code_path_count++;
        *(source_code_path + source_code_path_count) = 'a';
        source_code_path_count++;
        *(source_code_path + source_code_path_count) = 's';
        source_code_path_count++;
        *(source_code_path + source_code_path_count) = 'm';
        source_code_path_count++;
        *(source_code_path + source_code_path_count) = '\0';

        FILE *destination = fopen(source_code_path, "w");

        // Bootstrap Code
        fprintf(destination, "//BOOTSRAP CODE\n");

        // SP = 256
        fprintf(destination, "@256\nD = A\n@SP\nM = D\n");

        // Call Sys.init
        //  Push Return Address
        fprintf(destination, "@RET\nD = A\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

        // Push LCL
        fprintf(destination, "@LCL\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

        // Push ARG
        fprintf(destination, "@ARG\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

        // Push THIS
        fprintf(destination, "@THIS\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

        // Push THAT
        fprintf(destination, "@THAT\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

        // ARG = SP - n - 5
        fprintf(destination, "@SP\nD = M\n@5\nD = D - A\n@ARG\nM = D\n");

        // LCL = SP
        fprintf(destination, "@SP\nD = M\n@LCL\nM = D\n");

        // GOTO Function
        fprintf(destination, "@Sys.init\n0;JMP\n");

        //(Return Address)
        fprintf(destination, "(RET)\n");

        // Read the First file of the Directory
        struct dirent *entry = readdir(source_dir);

        // Continuously Check Next Files in the Directory
        while (entry != NULL)
        {
            // Check for VM file or not
            ext = (char *)malloc(5 * sizeof(char));
            ext_count = 0;
            d_name_count = strlen(entry->d_name);
            while (entry->d_name[d_name_count] != '.')
            {
                d_name_count--;
            }
            d_name_count++;
            while (d_name_count != strlen(entry->d_name))
            {
                *(ext + ext_count) = entry->d_name[d_name_count];
                ext_count++;
                d_name_count++;
            }
            *(ext + ext_count) = '\0';
            ext = realloc(ext, ((strlen(ext) + 1) * sizeof(char)));

            // If vm file
            if (strcmp(ext, "vm") == 0)
            {
                free(ext);

                // Take the File Name
                file_name = (char *)malloc(256 * sizeof(char));
                file_name_count = 0;
                d_name_count = 0;

                while (entry->d_name[d_name_count] != '.')
                {
                    *(file_name + file_name_count) = entry->d_name[d_name_count];
                    file_name_count++;
                    d_name_count++;
                }
                *(file_name + file_name_count) = '\0';
                file_name = realloc(file_name, ((strlen(file_name) + 1) * sizeof(char)));

                // Modify The Source Code Path
                source_code_path = realloc(source_code_path, 1000 * sizeof(char));
                source_code_path_count = fixed_end_of_source_dir;
                d_name_count = 0;

                *(source_code_path + source_code_path_count) = '\\';
                source_code_path_count++;
                while (entry->d_name[d_name_count] != '\0')
                {
                    *(source_code_path + source_code_path_count) = entry->d_name[d_name_count];
                    d_name_count++;
                    source_code_path_count++;
                }
                *(source_code_path + source_code_path_count) = '\0';
                source_code_path = realloc(source_code_path, ((strlen(source_code_path) + 1) * sizeof(char)));

                // Open The File
                source_code_file = fopen(source_code_path, "r");

                // Scan The File
                source_code_original = (char *)malloc(10000 * sizeof(char));
                scan(source_code_file, source_code_original);
                source_code_original = realloc(source_code_original, ((strlen(source_code_original) + 1) * sizeof(char)));
                fclose(source_code_file);

                // De-Comment The Source File
                source_code_filtered = (char *)malloc(100000 * sizeof(char));
                de_comment(source_code_original, source_code_filtered);
                source_code_filtered = realloc(source_code_filtered, ((strlen(source_code_filtered) + 1) * sizeof(char)));
                free(source_code_original);

                // Parse The Filtered Code
                source_code = (char ****)malloc(1 * sizeof(char));
                parse(source_code_filtered, source_code, &last_line, &Last_word);
                free(source_code_filtered);

                // Translate the Source Code
                translate(source_code, destination, last_line, Last_word, file_name);

                free(source_code);
                fclose(source_code_file);
                free(file_name);
            }

            entry = readdir(source_dir);
        }
        free(source_code_path);
        fclose(destination);
    }

    // If the path doesn't exist
    else
    {
        printf("Nothing Found\n");
    }

    return 0;
}

/*
____________________________________________________________
____________________________________________________________
____________________________________________________________
____________________________________________________________
*/

// File or Directory Determining Function
int isDir(const char *fileName, int *file, int *dir)
{
    struct stat path;

    stat(fileName, &path);
    *file = S_ISREG(path.st_mode);
    *dir = S_ISDIR(path.st_mode);

    return 0;
}

// De-Commenting Function
char de_comment(char *src, char *dest)
{
    int src_line_start = 0, src_word_count = 0, dest_word_count = 0;

    while (*(src + src_word_count) != '\0')
    {

        // Handle Single Line Comment
        if (*(src + src_line_start) == '/' && *(src + src_line_start + 1) == '/')
        {
            while (*(src + src_word_count) != '\n' && *(src + src_word_count) != '\0')
            {
                src_word_count++;
            }

            if (*(src + src_word_count) == '\n')
            {
                src_word_count++;
                src_line_start = src_word_count;
            }
        }

        // Handle In-line Comment
        else if (*(src + src_word_count) == '/' && *(src + src_word_count + 1) == '/')
        {
            dest_word_count--;
            if (*(dest + dest_word_count) == ' ')
            {
                *(dest + dest_word_count) = '\n';
                dest_word_count++;
            }

            else if (*(dest + dest_word_count) == '\n')
            {
                dest_word_count++;
            }

            else
            {
                dest_word_count++;
                *(dest + dest_word_count) = '\n';
                dest_word_count++;
            }

            while (*(src + src_word_count) != '\n' && *(src + src_word_count) != '\0')
            {
                src_word_count++;
            }

            if (*(src + src_word_count) == '\n')
            {
                src_word_count++;
                src_line_start = src_word_count;
            }
        }

        // Handle Line starting with Space
        else if (*(src + src_line_start) == ' ' && *(src + src_word_count) == ' ')
        {
            while (*(src + src_word_count) == ' ')
            {
                src_word_count++;
            }
            *(src + src_line_start) = '^';
        }

        // Handle Multiple Space
        else if (*(src + src_word_count) == ' ' && *(src + src_word_count + 1) == ' ')
        {
            *(dest + dest_word_count) = *(src + src_word_count);
            dest_word_count++;

            while (*(src + src_word_count) == ' ')
            {
                src_word_count++;
            }
        }

        // Handle Empty Line
        else if (*(src + src_line_start) == '\n')
        {
            src_word_count++;
            src_line_start = src_word_count;
        }

        // Handle Line Change
        else if (*(src + src_line_start) != '\n' && *(src + src_word_count) == '\n')
        {
            dest_word_count--;
            if (*(dest + dest_word_count) == ' ' || *(dest + dest_word_count) == '\n')
            {
                *(dest + dest_word_count) = *(src + src_word_count);
                src_word_count++;
                dest_word_count++;
                src_line_start = src_word_count;
            }

            else
            {
                dest_word_count++;
                *(dest + dest_word_count) = *(src + src_word_count);
                src_word_count++;
                dest_word_count++;
                src_line_start = src_word_count;
            }
        }

        // Handle Non-Comment
        else
        {
            *(dest + dest_word_count) = *(src + src_word_count);
            src_word_count++;
            dest_word_count++;
        }
    }

    if (*(dest + dest_word_count - 1) == ' ' || *(dest + dest_word_count - 1) == '\n')
    {
        dest_word_count--;
        while (*(dest + dest_word_count) == ' ' || *(dest + dest_word_count) == '\n')
        {
            dest_word_count--;
        }
        dest_word_count++;
        *(dest + dest_word_count) = '\0';
    }

    else
    {
        *(dest + dest_word_count) = '\0';
    }

    return 0;
}

// Parsing Function
char parse(char *source, char ****dest, int *last_line, int *last_word)
{
    *dest = (char ***)malloc(100 * sizeof(**dest));

    int line_count = 0, word_count, letter_count, source_count = 0;

    while (*(source + source_count) != '\0')
    {
        word_count = 0;
        *(*dest + line_count) = (char **)malloc(100 * sizeof(***dest));

        while (*(source + source_count) != '\0' && *(source + source_count) != '\n')
        {
            letter_count = 0;
            *(*(*dest + line_count) + word_count) = (char *)malloc(100 * sizeof(char));

            while (*(source + source_count) != ' ' && *(source + source_count) != '\n' && *(source + source_count) != '\0')
            {
                *(*(*(*dest + line_count) + word_count) + letter_count) = *(source + source_count);
                source_count++;
                letter_count++;
            }

            *(*(*(*dest + line_count) + word_count) + letter_count) = '\0';
            *(*(*dest + line_count) + word_count) = (char *)realloc(*(*(*dest + line_count) + word_count), (letter_count + 1) * sizeof(char));

            if (*(source + source_count) == ' ')
            {
                word_count++;
                source_count++;
            }
        }

        *last_word = word_count;
        *(*dest + line_count) = (char **)realloc(*(*dest + line_count), (word_count + 1) * sizeof(***dest));

        if (*(source + source_count) == '\n')
        {
            source_count++;
            line_count++;
        }
    }

    *last_line = line_count;
    *dest = (char ***)realloc(*dest, (line_count + 1) * sizeof(**dest));

    return 0;
}

// Scanning Function
char scan(FILE *src, char *dest)
{
    int dest_count = 0;
    *(dest + dest_count) = fgetc(src);
    while (*(dest + dest_count) != EOF)
    {
        *(dest + dest_count + 1) = fgetc(src);
        dest_count++;
    }

    *(dest + dest_count) = '\0';
}

// Translating Function
char translate(char ****source_code, FILE *dest, int last_line, int last_word, char *file_name)
{
    int line_count = -1, word_count, label_checker = 0, local_var_count, *ret_address, ret_address_count = 0;
    char *function_name;
    function_name = (char *)malloc(10000 * sizeof(char));

    function_name = (char *)malloc(1000 * sizeof(char));

    ret_address = (int *)malloc(10000 * sizeof(int));
    *(ret_address + ret_address_count) = 0;

    while (line_count != last_line || word_count != last_word)
    {
        line_count++;
        word_count = 0;

        // push command

        if (strcmp(*(*(*source_code + line_count) + word_count), "push") == 0)
        {
            fprintf(dest, "// %s ", *(*(*source_code + line_count) + word_count));
            word_count++;

            // push local

            if (strcmp(*(*(*source_code + line_count) + word_count), "local") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@LCL\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nAM = D + M\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push argument

            else if (strcmp(*(*(*source_code + line_count) + word_count), "argument") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@ARG\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nAM = D + M\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push this

            else if (strcmp(*(*(*source_code + line_count) + word_count), "this") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@THIS\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nAM = D + M\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push that

            else if (strcmp(*(*(*source_code + line_count) + word_count), "that") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@THAT\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nAM = D + M\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push constant

            else if (strcmp(*(*(*source_code + line_count) + word_count), "constant") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@%s\nD = A\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push static

            else if (strcmp(*(*(*source_code + line_count) + word_count), "static") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@%s.%s\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", file_name, *(*(*source_code + line_count) + word_count));
            }

            // push temp

            else if (strcmp(*(*(*source_code + line_count) + word_count), "temp") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                fprintf(dest, "@5\nD = A\n@14\nM = D\n@%s\nD = A\n@14\nAM = D + M\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // push pointer

            else if (strcmp(*(*(*source_code + line_count) + word_count), "pointer") == 0)
            {
                fprintf(dest, "%s ", *(*(*source_code + line_count) + word_count));
                word_count++;
                fprintf(dest, "%s\n", *(*(*source_code + line_count) + word_count));

                if (strcmp(*(*(*source_code + line_count) + word_count), "0") == 0)
                {
                    fprintf(dest, "@THIS\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");
                }

                else
                {
                    fprintf(dest, "@THAT\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");
                }
            }

            else
            {
                fprintf(dest, "Invalid Instruction\n");
            }
        }

        // pop command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "pop") == 0)
        {
            fprintf(dest, "// %s %s %s\n", *(*(*source_code + line_count) + word_count), *(*(*source_code + line_count) + (word_count + 1)), *(*(*source_code + line_count) + (word_count + 2)));
            word_count++;

            // pop local

            if (strcmp(*(*(*source_code + line_count) + word_count), "local") == 0)
            {
                word_count++;
                fprintf(dest, "@LCL\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nM = D + M\n@SP\nAM = M - 1\nD = M\n@14\nA = M\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // pop argument

            else if (strcmp(*(*(*source_code + line_count) + word_count), "argument") == 0)
            {
                word_count++;
                fprintf(dest, "@ARG\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nM = D + M\n@SP\nAM = M - 1\nD = M\n@14\nA = M\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // pop this

            else if (strcmp(*(*(*source_code + line_count) + word_count), "this") == 0)
            {
                word_count++;
                fprintf(dest, "@THIS\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nM = D + M\n@SP\nAM = M - 1\nD = M\n@14\nA = M\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // pop that

            else if (strcmp(*(*(*source_code + line_count) + word_count), "that") == 0)
            {
                word_count++;
                fprintf(dest, "@THAT\nD = M\n@14\nM = D\n@%s\nD = A\n@14\nM = D + M\n@SP\nAM = M - 1\nD = M\n@14\nA = M\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // pop static

            else if (strcmp(*(*(*source_code + line_count) + word_count), "static") == 0)
            {
                word_count++;
                fprintf(dest, "@SP\nAM = M - 1\nD = M\n@%s.%s\nM = D\n", file_name, *(*(*source_code + line_count) + word_count));
            }

            // pop temp

            else if (strcmp(*(*(*source_code + line_count) + word_count), "temp") == 0)
            {
                word_count++;
                fprintf(dest, "@5\nD = A\n@14\nM = D\n@%s\nD = A\n@14\nM = D + M\n@SP\nAM = M - 1\nD = M\n@14\nA = M\nM = D\n", *(*(*source_code + line_count) + word_count));
            }

            // pop pointer

            else if (strcmp(*(*(*source_code + line_count) + word_count), "pointer") == 0)
            {
                word_count++;

                if (strcmp(*(*(*source_code + line_count) + word_count), "0") == 0)
                {
                    fprintf(dest, "@SP\nAM = M - 1\nD = M \n@THIS\nM = D\n");
                }

                else
                {
                    fprintf(dest, "@SP\nAM = M - 1\nD = M\n@THAT\nM = D\n");
                }
            }

            else
            {
                fprintf(dest, "Invalid Insruction");
            }
        }

        // Add command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "add") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nAM = M - 1\nD = M\n@SP\nA = M - 1\nM = D + M\n");
        }

        // Subtract Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "sub") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nAM = M - 1\nD = M\n@SP\nA = M - 1\nM = M - D\n");
        }

        // Neg Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "neg") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nA = M - 1\nM = -M\n");
        }

        // Eq Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "eq") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            if (label_checker == 0)
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JEQ\n@FALSE\n0;JMP\n(TRUE)\n@SP\nA = M - 1\nM = -1\n@SP\nA = M\nA = M\n0;JMP\n(FALSE)\n@SP\nA = M - 1\nM = 0\n@SP\nA = M\nA = M\n0;JMP\n(CT_%d)\n", label_checker, label_checker);

                label_checker++;
            }

            else
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JEQ\n@FALSE\n0;JMP\n(CT_%d)\n", label_checker, label_checker);
                label_checker++;
            }
        }

        // Lt command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "lt") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            if (label_checker == 0)
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JGT\n@FALSE\n0;JMP\n(TRUE)\n@SP\nA = M - 1\nM = -1\n@SP\nA = M\nA = M\n0;JMP\n(FALSE)\n@SP\nA = M - 1\nM = 0\n@SP\nA = M\nA = M\n0;JMP\n(CT_%d)\n", label_checker, label_checker);

                label_checker++;
            }

            else
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JGT\n@FALSE\n0;JMP\n(CT_%d)\n", label_checker, label_checker);

                label_checker++;
            }
        }

        // Gt Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "gt") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            if (label_checker == 0)
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JLT\n@FALSE\n0;JMP\n(TRUE)\n@SP\nA = M - 1\nM = -1\n@SP\nA = M\nA = M\n0;JMP\n(FALSE)\n@SP\nA = M - 1\nM = 0\n@SP\nA = M\nA = M\n0;JMP\n(CT_%d)\n", label_checker, label_checker);

                label_checker++;
            }

            else
            {
                fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D - M\n@CT_%d\nD = A\n@SP\nA = M\nM = D\n@SP\nA = M - 1\nD = M\n@TRUE\nD;JLT\n@FALSE\n0;JMP\n(CT_%d)\n", label_checker, label_checker);

                label_checker++;
            }
        }

        // And Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "and") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D & M\n");
        }

        // Or Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "or") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nAM = M - 1\nD = M\nA = A - 1\nM = D | M\n");
        }

        // Not Command

        else if (strcmp(*(*(*source_code + line_count) + word_count), "not") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nA = M - 1\nD = M\nM = !M\n");
        }

        // Label Command
        else if (strcmp(*(*(*source_code + line_count) + word_count), "label") == 0)
        {
            fprintf(dest, "// %s", *(*(*source_code + line_count) + word_count));
            word_count++;
            fprintf(dest, " %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "(%s$%s)\n", function_name, *(*(*source_code + line_count) + word_count));
        }

        // Goto Command
        else if (strcmp(*(*(*source_code + line_count) + word_count), "goto") == 0)
        {
            fprintf(dest, "// %s", *(*(*source_code + line_count) + word_count));
            word_count++;
            fprintf(dest, " %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@%s$%s\n0;JMP\n", function_name, *(*(*source_code + line_count) + word_count));
        }

        // if-goto Command
        else if (strcmp(*(*(*source_code + line_count) + word_count), "if-goto") == 0)
        {
            fprintf(dest, "// %s", *(*(*source_code + line_count) + word_count));
            word_count++;
            fprintf(dest, " %s\n", *(*(*source_code + line_count) + word_count));

            fprintf(dest, "@SP\nAM = M - 1\nD = M\n@%s$%s\nD;JNE\n", function_name, *(*(*source_code + line_count) + word_count));
        }

        // Function Command
        else if (strcmp(*(*(*source_code + line_count) + word_count), "function") == 0)
        {
            fprintf(dest, "// %s %s %s\n", *(*(*source_code + line_count) + word_count), *(*(*source_code + line_count) + (word_count + 1)), *(*(*source_code + line_count) + (word_count + 2)));
            word_count++;

            fprintf(dest, "(%s)\n", *(*(*source_code + line_count) + word_count));

            function_name = realloc(function_name, 1000 * sizeof(char));
            strcpy(function_name, *(*(*source_code + line_count) + word_count));
            function_name = realloc(function_name, ((strlen(function_name) + 1) * sizeof(char)));

            word_count++;
            local_var_count = atoi(*(*(*source_code + line_count) + word_count));

            for (int i = 0; i < local_var_count; i++)
            {
                fprintf(dest, "@SP\nM = M + 1\nA = M - 1\nM = 0\n");
            }

            ret_address_count = 0;
        }

        // Return Command
        else if (strcmp(*(*(*source_code + line_count) + word_count), "return") == 0)
        {
            fprintf(dest, "// %s\n", *(*(*source_code + line_count) + word_count));

            // FRAME = LCL && RET = *(LCL - 5)
            fprintf(dest, "@LCL\nD = M\n@13\nM = D\n@LCL\nD = M\n@5\nA = D - A\nD = M\n@14\nM = D\n");

            //*ARG = POP()
            fprintf(dest, "@SP\nA = M - 1\nD = M\n@ARG\nA = M\nM = D\n");

            // SP = ARG + 1
            fprintf(dest, "@ARG\nD = M + 1\n@SP\nM = D\n");

            // THAT = *(FRAME - 1)
            fprintf(dest, "@13\nA = M - 1\nD = M\n@THAT\nM = D\n");

            // THIS = *(FRAME - 2)
            fprintf(dest, "@13\nD = M\n@2\nA = D - A\nD = M\n@THIS\nM = D\n");

            // ARG = *(FRAME - 3)
            fprintf(dest, "@13\nD = M\n@3\nA = D - A\nD = M\n@ARG\nM = D\n");

            // LCL = *(FRAME - 4)
            fprintf(dest, "@13\nD = M\n@4\nA = D - A\nD = M\n@LCL\nM = D\n");

            // GOTO RET
            fprintf(dest, "@14\nA = M\n0;JMP\n");
        }

        // Call Function
        else if (strcmp(*(*(*source_code + line_count) + word_count), "call") == 0)
        {
            fprintf(dest, "// %s %s %s\n", *(*(*source_code + line_count) + word_count), *(*(*source_code + line_count) + (word_count + 1)), *(*(*source_code + line_count) + (word_count + 2)));
            word_count++;

            // Push Return Address
            fprintf(dest, "@%s$ret.%d\nD = A\n@SP\nM = M + 1\nA = M - 1\nM = D\n", function_name, ret_address_count);

            // Push LCL
            fprintf(dest, "@LCL\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

            // Push ARG
            fprintf(dest, "@ARG\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

            // Push THIS
            fprintf(dest, "@THIS\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

            // Push THAT
            fprintf(dest, "@THAT\nD = M\n@SP\nM = M + 1\nA = M - 1\nM = D\n");

            // ARG = SP - n - 5
            word_count++;
            fprintf(dest, "@SP\nD = M\n@%s\nD = D - A\n@5\nD = D - A\n@ARG\nM = D\n", *(*(*source_code + line_count) + word_count));

            // LCL = SP
            fprintf(dest, "@SP\nD = M\n@LCL\nM = D\n");

            // GOTO Function
            fprintf(dest, "@%s\n0;JMP\n", *(*(*source_code + line_count) + (word_count - 1)));

            //(Return Address)
            fprintf(dest, "(%s$ret.%d)\n", function_name, ret_address_count);

            ret_address_count++;
        }

        else
        {
            fprintf(dest, "Invalid Instruction");
        }
    }
    return 0;
}
