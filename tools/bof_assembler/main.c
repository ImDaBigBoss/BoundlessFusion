#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assembler.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <source> <output>\n", argv[0]);
        return 1;
    }

    char* source_file = argv[1];
    char* output_file = argv[2];

    FILE* source = fopen(source_file, "r");
    if (!source) {
        printf("Error: Could not open source file '%s'\n", source_file);
        return 1;
    }
    
    FILE* output = fopen(output_file, "w");
    if (!output) {
        printf("Error: Could not create output file '%s'\n", output_file);
        return 1;
    }

    printf("Starting assembly...\n");
    if (assemble(source, output, 4096)) {
        printf("Done! Binary written to %s.\n", output_file);
    } else {
        printf("Assembly failed.\n");
        exit(1);
    }

    fclose(source);
    fclose(output);

    return 0;
}
