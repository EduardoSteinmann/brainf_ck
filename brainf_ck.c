#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define CELLS_SIZE 30000

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        FILE* file = fopen(argv[1], "r");

        if (file == NULL)
        {
            fprintf(stderr, "File %s could not be found or the system does not have enough resources", argv[1]);
            return EXIT_FAILURE;
        }

        static uint8_t cells[CELLS_SIZE] = { 0 };

        static uint8_t* data_pointer = cells;

        int character = 0;

        size_t bracket_not_matched = 0;

        while ((character = fgetc(file)) != EOF)
        {
            switch (character)
            {
                case '>':
                {
                    if (data_pointer + 1 > cells + (CELLS_SIZE - 1))
                    {
                        fprintf(stderr, "Error: Cannot go past memory tape...");
                        return EXIT_FAILURE;
                    }
                    data_pointer++;
                    break;
                }
                case '<':
                {
                    if (data_pointer - 1 < cells)
                    {
                        fprintf(stderr, "Error: Cannot go before memory tape...");
                        return EXIT_FAILURE;
                    }
                    data_pointer--;
                    break;
                }
                case '+': (*data_pointer)++; break;
                case '-': (*data_pointer)--; break;
                case '.': printf("%c", *data_pointer); break;
                case ',': *data_pointer = fgetc(stdin); break;
                case '[':
                {
                    if (*data_pointer == 0)
                    {
                        bracket_not_matched++;
                        while (bracket_not_matched)
                        {
                            character = fgetc(file);
                            if (character == EOF)
                            {
                                fprintf(stderr, "Error: did not find matching ']'...");
                                return EXIT_FAILURE;
                            }
                            bracket_not_matched += character == '[';
                            bracket_not_matched -= character == ']';
                        }
                    }
                    break;
                }
                case ']':
                {
                    if (*data_pointer)
                    {
                        bracket_not_matched++;
                        while (bracket_not_matched)
                        {
                            if (fseek(file, -2, SEEK_CUR))
                            {
                                fprintf(stderr, "Error: did not find matching '['...");
                                return EXIT_FAILURE;
                            }
                            character = fgetc(file);
                            bracket_not_matched -= character == '[';
                            bracket_not_matched += character == ']';
                        }
                    }
                    break;
                }
                default: break;
            }
        }

        fclose(file);
    }
    else
    {
        fprintf(stderr, "Error: Expected source code file name...");
        return EXIT_FAILURE;
    }

    return 0;
}