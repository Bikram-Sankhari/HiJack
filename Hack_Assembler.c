#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct label_table
{
    char *name;
    char *num;
} label;

int main()
{
    // Opening Source Code
    char *source_location;
    source_location = (char *)malloc(1000000 * sizeof(char));
    printf("Enter the location of the Source Code File:\n");
    gets(source_location);

    FILE *source_code_file = fopen(source_location, "r");

    if (source_code_file == 0)
    {
        printf("File Not Found\n");
    }

    else
    {
        // Declaring Variables
        int word_count, a_address_str_count, a_address_num, equal, colon, line_start,
            c_instruction[16] = {1, 1, 1, 0}, c[6], d[3], j[3], a = 0, d_str_count, c_str_count,
            j_str_count, a_address[16] = {0}, end, line_count, label_name_count, label_count, label_search_count,
            label_num_count, var_count = 0;
        char *a_address_str;
        a_address_str = (char *)malloc(6 * sizeof(char));
        char d_str[5], c_str[4], j_str[5];
        char *label_name;
        char *source_code;
        label *label;

        // Scanning the Source Code
        char *source_code_original;
        int source_code_original_count = 0;
        source_code_original = (char *)malloc(100000000 * sizeof(char));
        *(source_code_original + source_code_original_count) = fgetc(source_code_file);
        while (*(source_code_original + source_code_original_count) != EOF)
        {
            *(source_code_original + source_code_original_count + 1) = fgetc(source_code_file);
            source_code_original_count++;
        }

        *(source_code_original + source_code_original_count) = '\0';
        source_code_original = (char *)realloc(source_code_original, (source_code_original_count * sizeof(char) + 1));
        fclose(source_code_file);

        // Generating Destination Location
        char *dest_location;
        dest_location = (char *)malloc(1000000 * sizeof(char));
        int source_location_count = 0, dest_location_count = 0;
        while (*(source_location + source_location_count) != '.')
        {
            *(dest_location + dest_location_count) = *(source_location + source_location_count);
            dest_location_count++;
            source_location_count++;
        }
        *(dest_location + dest_location_count) = *(source_location + source_location_count);
        dest_location_count++;
        *(dest_location + dest_location_count) = 'h';
        dest_location_count++;
        *(dest_location + dest_location_count) = 'a';
        dest_location_count++;
        *(dest_location + dest_location_count) = 'c';
        dest_location_count++;
        *(dest_location + dest_location_count) = 'k';
        dest_location_count++;
        *(dest_location + dest_location_count) = '\0';
        dest_location = (char *)realloc(dest_location, (dest_location_count * sizeof(char) + 1));

        free(source_location);

        // Ignoring Comments
        char *source_code_lve;
        source_code_lve = (char *)malloc(100000000 * sizeof(char));

        int source_code_lve_count = 0;
        source_code_original_count = 0;
        while (*(source_code_original + source_code_original_count) != '\0')
        {
            if (*(source_code_original + source_code_original_count) == '/')
            {
                source_code_original_count++;
                if (*(source_code_original + source_code_original_count) == '/')
                {
                    while (*(source_code_original + source_code_original_count) != '\n')
                    {
                        source_code_original_count++;
                    }
                }

                else
                {
                    printf("INVALID COMMENT\n");
                }
            }

            else if (*(source_code_original + source_code_original_count) == ' ')
            {
                source_code_original_count++;
            }

            else
            {
                *(source_code_lve + source_code_lve_count) = *(source_code_original + source_code_original_count);
                source_code_original_count++;
                source_code_lve_count++;
            }
        }

        *(source_code_lve + source_code_lve_count) = '\0';
        source_code_lve = (char *)realloc(source_code_lve, (source_code_lve_count * sizeof(char) + 1));
        free(source_code_original);

        char *source_code_lv = (char *)malloc(100000000 * sizeof(char));
        int source_code_lv_count = 0;
        line_start = 0;
        source_code_lve_count = 0;
        while (*(source_code_lve + source_code_lve_count) != '\0')
        {
            if (*(source_code_lve + line_start) == '\n')
            {
                source_code_lve_count++;
                line_start = source_code_lve_count;
            }

            else
            {
                if (*(source_code_lve + source_code_lve_count) == '\n')
                {
                    *(source_code_lv + source_code_lv_count) = *(source_code_lve + source_code_lve_count);
                    source_code_lv_count++;
                    source_code_lve_count++;
                    line_start = source_code_lve_count;
                }

                else
                {
                    *(source_code_lv + source_code_lv_count) = *(source_code_lve + source_code_lve_count);
                    source_code_lv_count++;
                    source_code_lve_count++;
                }
            }
        }
        *(source_code_lv + source_code_lv_count) = '\0';
        source_code_lv = (char *)realloc(source_code_lv, (source_code_lv_count * sizeof(char) + 1));
        free(source_code_lve);

        // Generating Label Table
        label = malloc(10000 * sizeof(label));
        source_code_lv_count = 0;
        line_count = 0;
        label_count = 0;
        // Built-in Labels
        // R0
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R0");
        strcpy((label + label_count)->num, "0");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R1
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R1");
        strcpy((label + label_count)->num, "1");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R2
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R2");
        strcpy((label + label_count)->num, "2");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R3
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R3");
        strcpy((label + label_count)->num, "3");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R4
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R4");
        strcpy((label + label_count)->num, "4");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R5
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R5");
        strcpy((label + label_count)->num, "5");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R6
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R6");
        strcpy((label + label_count)->num, "6");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R7
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R7");
        strcpy((label + label_count)->num, "7");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R8
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R8");
        strcpy((label + label_count)->num, "8");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R9
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R9");
        strcpy((label + label_count)->num, "9");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R10
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R10");
        strcpy((label + label_count)->num, "10");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R11
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R11");
        strcpy((label + label_count)->num, "11");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R12
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R12");
        strcpy((label + label_count)->num, "12");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R13
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R13");
        strcpy((label + label_count)->num, "13");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R14
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R14");
        strcpy((label + label_count)->num, "14");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // R15
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "R15");
        strcpy((label + label_count)->num, "15");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // SCREEN
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "SCREEN");
        strcpy((label + label_count)->num, "16384");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // KBD
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "KBD");
        strcpy((label + label_count)->num, "24576");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // SP
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "SP");
        strcpy((label + label_count)->num, "0");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // LCL
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "LCL");
        strcpy((label + label_count)->num, "1");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // ARG
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "ARG");
        strcpy((label + label_count)->num, "2");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // THIS
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "THIS");
        strcpy((label + label_count)->num, "3");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // THAT
        (label + label_count)->name = (char *)malloc(100 * sizeof(char));
        (label + label_count)->num = (char *)malloc(100 * sizeof(char));
        strcpy((label + label_count)->name, "THAT");
        strcpy((label + label_count)->num, "4");
        (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
        (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
        label_count++;
        // User Defined Labels
        while (*(source_code_lv + source_code_lv_count) != '\0')
        {
            if (*(source_code_lv + source_code_lv_count) == '\n')
            {
                source_code_lv_count++;
                line_count++;
            }

            else if (*(source_code_lv + source_code_lv_count) == '(')
            {
                source_code_lv_count++;
                label_name_count = 0;
                (label + label_count)->name = (char *)malloc(100 * sizeof(char));
                while (*(source_code_lv + source_code_lv_count) != ')')
                {
                    *((label + label_count)->name + label_name_count) = *(source_code_lv + source_code_lv_count);
                    label_name_count++;
                    source_code_lv_count++;
                }
                *((label + label_count)->name + label_name_count) = '\0';
                (label + label_count)->name = (char *)realloc((label + label_count)->name, (label_name_count * sizeof(char) + 1));
                (label + label_count)->num = (char *)malloc(100 * sizeof(char));
                sprintf((label + label_count)->num, "%d", line_count);
                (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
                label_count++;
                source_code_lv_count++;
                source_code_lv_count++;
            }

            else
            {
                source_code_lv_count++;
            }
        }
        // Replacing Labels
        source_code = (char *)malloc(100000000 * sizeof(char));
        label_name = (char *)malloc(100 * sizeof(char));
        source_code_lv_count = 0;
        word_count = 0;
        while (*(source_code_lv + source_code_lv_count) != '\0')
        {
            if (*(source_code_lv + source_code_lv_count) == '@' && *(source_code_lv + source_code_lv_count + 1) != '0' && *(source_code_lv + source_code_lv_count + 1) != '1' && *(source_code_lv + source_code_lv_count + 1) != '2' && *(source_code_lv + source_code_lv_count + 1) != '3' && *(source_code_lv + source_code_lv_count + 1) != '4' && *(source_code_lv + source_code_lv_count + 1) != '5' && *(source_code_lv + source_code_lv_count + 1) != '6' && *(source_code_lv + source_code_lv_count + 1) != '7' && *(source_code_lv + source_code_lv_count + 1) != '8' && *(source_code_lv + source_code_lv_count + 1) != '9')
            {
                line_start = source_code_lv_count;
                source_code_lv_count++;
                label_name_count = 0;
                while (*(source_code_lv + source_code_lv_count) != '\n')
                {
                    *(label_name + label_name_count) = *(source_code_lv + source_code_lv_count);
                    label_name_count++;
                    source_code_lv_count++;
                }
                *(label_name + label_name_count) = '\0';
                label_name = (char *)realloc(label_name, (label_name_count * sizeof(char) + 1));

                label_search_count = 0;
                label_num_count = 0;
                while (label_search_count < label_count)
                {
                    if (strcmp((label + label_search_count)->name, label_name) == 0)
                    {
                        *(source_code + word_count) = *(source_code_lv + line_start);
                        word_count++;
                        while (label_num_count < strlen((label + label_search_count)->num))
                        {
                            *(source_code + word_count) = *((label + label_search_count)->num + label_num_count);
                            label_num_count++;
                            word_count++;
                        }
                        break;
                    }

                    else
                    {
                        label_search_count++;
                    }
                }

                if (label_search_count == label_count)
                {
                    (label + label_count)->name = (char *)malloc(100 * sizeof(char));
                    (label + label_count)->num = (char *)malloc(100 * sizeof(char));
                    strcpy((label + label_count)->name, label_name);
                    sprintf((label + label_count)->num, "%d", (var_count + 16));
                    var_count++;
                    (label + label_count)->name = (char *)realloc((label + label_count)->name, (strlen((label + label_count)->name) * sizeof(char) + 1));
                    (label + label_count)->num = (char *)realloc((label + label_count)->num, (strlen((label + label_count)->num) * sizeof(char) + 1));
                    label_count++;
                    source_code_lv_count = line_start;
                }

                else
                {
                    label_name = (char *)realloc(label_name, 100 * sizeof(char));
                    *(source_code + word_count) = *(source_code_lv + source_code_lv_count);
                    word_count++;
                    source_code_lv_count++;
                }
            }

            else if (*(source_code_lv + source_code_lv_count) == '(')
            {
                while (*(source_code_lv + source_code_lv_count) != '\n')
                {
                    source_code_lv_count++;
                }
                source_code_lv_count++;
            }

            else
            {
                *(source_code + word_count) = *(source_code_lv + source_code_lv_count);
                word_count++;
                source_code_lv_count++;
            }
        }
        *(source_code + word_count) = '\0';
        source_code = (char *)realloc(source_code, (word_count * sizeof(char) + 1));
        free(label_name);

        // Generating Assembled File
        line_start = 0;
        word_count = 0;

        FILE *assembled_file = fopen(dest_location, "w");
        while (*(source_code + word_count) != '\0')
        {
            while (*(source_code + word_count) != '\n' && *(source_code + word_count) != '\0')
            {
                // A-Instruction
                if (*(source_code + line_start) == '@' && *(source_code + word_count + 1) != '\n' && *(source_code + word_count + 1) != '\0')
                {
                    a_address_str_count = 0;
                    while (*(source_code + word_count + 1) != '\n' && *(source_code + word_count + 1) != '\0')
                    {
                        *(a_address_str + a_address_str_count) = *(source_code + word_count + 1);
                        a_address_str_count++;
                        word_count++;
                    }
                    *(a_address_str + a_address_str_count) = '\0';
                    a_address_str = (char *)realloc(a_address_str, (a_address_str_count * sizeof(char) + 1));
                    a_address_num = atoi(a_address_str);

                    // Converting Decimal into Binary
                    for (int p = 15; p >= 1; p--)
                    {
                        a_address[p] = a_address_num % 2;
                        a_address_num = a_address_num / 2;
                    }

                    for (int p = 0; p < 16; p++)
                    {
                        fprintf(assembled_file, "%d", a_address[p]);
                    }

                    word_count++;
                    a_address_str = (char *)realloc(a_address_str, (6 * sizeof(char)));
                }
                // A-Instruction Error Handling
                else if (*(source_code + line_start) == '@' && (*(source_code + word_count + 1) == '\n' ||
                                                                *(source_code + word_count + 1) == '\0'))
                {
                    fprintf(assembled_file, "There should be some number after @");
                    word_count++;
                }

                // C-Instruction
                else if (*(source_code + line_start) != '@')
                {
                    equal = -1;
                    colon = -1;
                    strcpy(d_str, "null");
                    strcpy(j_str, "null");
                    c_str_count = 0;
                    j_str_count = 0;
                    d_str_count = 0;
                    // Breaking C_Instruction Source Code
                    while (*(source_code + word_count) != '\n' && *(source_code + word_count) != '\0')
                    {
                        if (*(source_code + word_count) == '=')
                        {
                            equal = word_count;
                        }

                        else if (*(source_code + word_count) == ';')
                        {
                            colon = word_count;
                        }
                        word_count++;
                    }

                    end = word_count;

                    if (equal != -1)
                    {
                        for (int x = line_start; x < equal; x++)
                        {
                            d_str[d_str_count] = *(source_code + x);
                            d_str_count++;
                        }
                        d_str[d_str_count] = '\0';
                    }

                    if (colon != -1)
                    {
                        for (int x = colon + 1; x < end; x++)
                        {
                            j_str[j_str_count] = *(source_code + x);
                            j_str_count++;
                        }
                        j_str[j_str_count] = '\0';
                    }

                    if (equal != -1 && colon != -1)
                    {
                        for (int x = equal + 1; x < colon; x++)
                        {
                            c_str[c_str_count] = *(source_code + x);
                            c_str_count++;
                        }
                        c_str[c_str_count] = '\0';
                    }

                    else if (equal == -1 && colon != -1)
                    {
                        for (int x = line_start; x < colon; x++)
                        {
                            c_str[c_str_count] = *(source_code + x);
                            c_str_count++;
                        }
                        c_str[c_str_count] = '\0';
                    }

                    else if (equal != -1 && colon == -1)
                    {
                        for (int x = equal + 1; x < end; x++)
                        {
                            c_str[c_str_count] = *(source_code + x);
                            c_str_count++;
                        }
                        c_str[c_str_count] = '\0';
                    }

                    else
                    {
                        for (int x = line_start; x < end; x++)
                        {
                            c_str[c_str_count] = *(source_code + x);
                            c_str_count++;
                        }
                        c_str[c_str_count] = '\0';
                    }

                    // Generating C , D and J instruction Parts
                    if (strcmp("null", d_str) == 0)
                    {
                        d[0] = 0;
                        d[1] = 0;
                        d[2] = 0;
                    }
                    // For D Part
                    else if (strcmp("M", d_str) == 0)
                    {
                        d[0] = 0;
                        d[1] = 0;
                        d[2] = 1;
                    }

                    else if (strcmp("D", d_str) == 0)
                    {
                        d[0] = 0;
                        d[1] = 1;
                        d[2] = 0;
                    }

                    else if (strcmp("MD", d_str) == 0)
                    {
                        d[0] = 0;
                        d[1] = 1;
                        d[2] = 1;
                    }

                    else if (strcmp("A", d_str) == 0)
                    {
                        d[0] = 1;
                        d[1] = 0;
                        d[2] = 0;
                    }

                    else if (strcmp("AM", d_str) == 0)
                    {
                        d[0] = 1;
                        d[1] = 0;
                        d[2] = 1;
                    }

                    else if (strcmp("AD", d_str) == 0)
                    {
                        d[0] = 1;
                        d[1] = 1;
                        d[2] = 0;
                    }

                    else if (strcmp("AMD", d_str) == 0)
                    {
                        d[0] = 1;
                        d[1] = 1;
                        d[2] = 1;
                    }

                    else
                    {
                        fprintf(assembled_file, "Unknown D-Instruction\n");
                    }

                    // For J Part
                    if (strcmp("null", j_str) == 0)
                    {
                        j[0] = 0;
                        j[1] = 0;
                        j[2] = 0;
                    }

                    else if (strcmp("JGT", j_str) == 0)
                    {
                        j[0] = 0;
                        j[1] = 0;
                        j[2] = 1;
                    }

                    else if (strcmp("JEQ", j_str) == 0)
                    {
                        j[0] = 0;
                        j[1] = 1;
                        j[2] = 0;
                    }

                    else if (strcmp("JGE", j_str) == 0)
                    {
                        j[0] = 0;
                        j[1] = 1;
                        j[2] = 1;
                    }

                    else if (strcmp("JLT", j_str) == 0)
                    {
                        j[0] = 1;
                        j[1] = 0;
                        j[2] = 0;
                    }

                    else if (strcmp("JNE", j_str) == 0)
                    {
                        j[0] = 1;
                        j[1] = 0;
                        j[2] = 1;
                    }

                    else if (strcmp("JLE", j_str) == 0)
                    {
                        j[0] = 1;
                        j[1] = 1;
                        j[2] = 0;
                    }

                    else if (strcmp("JMP", j_str) == 0)
                    {
                        j[0] = 1;
                        j[1] = 1;
                        j[2] = 1;
                    }

                    else
                    {
                        fprintf(assembled_file, "Unknown J-Instruction\n");
                    }

                    // For C Part
                    if (strcmp("0", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 0;
                        c[2] = 1;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("-1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 1;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("D", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 0;
                        c[5] = 0;
                    }

                    else if (strcmp("A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 0;
                    }

                    else if (strcmp("M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 0;
                    }

                    else if (strcmp("!D", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 0;
                        c[5] = 1;
                    }

                    else if (strcmp("!A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 1;
                    }

                    else if (strcmp("!M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 1;
                    }

                    else if (strcmp("-D", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("-A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("-M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("D+1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 1;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("A+1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("M+1", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("D-1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 1;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("A-1", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("M-1", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 1;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("D+A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("D+M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 0;
                    }

                    else if (strcmp("D-A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("D-M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 0;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("A-D", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("M-D", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 1;
                        c[5] = 1;
                    }

                    else if (strcmp("D&A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 0;
                    }

                    else if (strcmp("D&M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 0;
                        c[1] = 0;
                        c[2] = 0;
                        c[3] = 0;
                        c[4] = 0;
                        c[5] = 0;
                    }

                    else if (strcmp("D|A", c_str) == 0)
                    {
                        a = 0;
                        c[0] = 0;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 0;
                        c[5] = 1;
                    }

                    else if (strcmp("D|M", c_str) == 0)
                    {
                        a = 1;
                        c[0] = 0;
                        c[1] = 1;
                        c[2] = 0;
                        c[3] = 1;
                        c[4] = 0;
                        c[5] = 1;
                    }

                    else
                    {
                        fprintf(assembled_file, "Unknown C-Instruction\n");
                    }

                    // Generating C_Instruction
                    c_instruction[3] = a;
                    c_instruction[4] = c[0];
                    c_instruction[5] = c[1];
                    c_instruction[6] = c[2];
                    c_instruction[7] = c[3];
                    c_instruction[8] = c[4];
                    c_instruction[9] = c[5];
                    c_instruction[10] = d[0];
                    c_instruction[11] = d[1];
                    c_instruction[12] = d[2];
                    c_instruction[13] = j[0];
                    c_instruction[14] = j[1];
                    c_instruction[15] = j[2];

                    for (int x = 0; x < 16; x++)
                    {
                        fprintf(assembled_file, "%d", c_instruction[x]);
                    }
                }
            }

            // Handling '\n' and '\0'
            if (*(source_code + word_count) == '\n')
            {
                fprintf(assembled_file, "%c", *(source_code + word_count));
                word_count++;
            }

            line_start = word_count;
        }
        fclose(assembled_file);
        free(a_address_str);
        free(dest_location);
    }
    return 0;
}
