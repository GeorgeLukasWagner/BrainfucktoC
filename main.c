#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MOVR,
    MOVL,
    INCR,
    DECR,
    OUTP,
    INPT,
    LOOP,
    ENDL,
    ENDC
} TokenType;

typedef struct {
    TokenType type;
    char value;
} Token;

Token *tokenize(const char *code) {
    Token *tokens = malloc(strlen(code) * sizeof(Token) + 1);
    int i = 0;

    for (; code[i] != 0; i++) {
        switch (code[i]) {
            case '>':
                tokens[i].type  = MOVR;
                tokens[i].value = '>';
                break;
            case '<':
                tokens[i].type  = MOVL;
                tokens[i].value = '<';
                break;
            case '+':
                tokens[i].type  = INCR;
                tokens[i].value = '+';
                break;
            case '-':
                tokens[i].type  = DECR;
                tokens[i].value = '-';
                break;
            case '.':
                tokens[i].type  = OUTP;
                tokens[i].value = '.';
                break;
            case ',':
                tokens[i].type  = INPT;
                tokens[i].value = ',';
                break;
            case '[':
                tokens[i].type  = LOOP;
                tokens[i].value = '[';
                break;
            case ']':
                tokens[i].type  = ENDL;
                tokens[i].value = ']';
                break;
            default:
                continue;
        }
    }
    tokens[i].type = ENDC;
    tokens[i].value = 0;

    return tokens;
}

int generateCode(Token *tokens, const char *outputName) {
    FILE *outputf = fopen(outputName, "w");
    if (outputf == NULL) {
        printf("Error: Unable to open output file.\n");
        return 1;
    }
    fprintf(outputf, "#include <stdio.h>\n");
    fprintf(outputf, "#include <stdlib.h>\n\n");
    fprintf(outputf, "int main() {\n");
    fprintf(outputf, "  char *tape = calloc(30000, sizeof(char));\n");
    fprintf(outputf, "  char *ptr = tape;\n\n");

    while (tokens->type != ENDC) {
        switch (tokens->type) {
            case MOVR:
                fprintf(outputf, "  ptr++;\n");
                break;
            case MOVL:
                fprintf(outputf, "  ptr--;\n");
                break;
            case INCR:
                fprintf(outputf, "  ++*ptr;\n");
                break;
            case DECR:
                fprintf(outputf, "  --*ptr;\n");
                break;
            case OUTP:
                fprintf(outputf, "  putchar(*ptr);\n");
                break;
            case INPT:
                fprintf(outputf, "  *ptr = getchar();\n");
                break;
            case LOOP:
                fprintf(outputf, "  while (*ptr) {\n");
                break;
            case ENDL:
                fprintf(outputf, "  };\n");
                break;
            default:
                break;
        }
        tokens++;
    }
    fprintf(outputf, "  free(tape);");
    fprintf(outputf, "\n  return 0;\n");
    fprintf(outputf, "}\n");

    fclose(outputf);
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 3) {
        FILE *brainf;
        brainf = fopen(argv[1], "r");
        if (brainf == NULL) {
            printf("Error unable to open file.");
            return 1;
        }

        char bfCode[30000];
        char *outputName = strcat(argv[2], ".c");
        
        if (fgets(bfCode, 30000, brainf) == NULL) {
            printf("fgets failed");
            return 1;
        }
        Token *tokens = tokenize(bfCode);
        generateCode(tokens, outputName);

        free(tokens);
        fclose(brainf);
        return 0;
    }
    else {
        printf("Incorrect usage of program. Do ./executable 'brainfuck-filename.bf' 'c-filename'.\n");
        return 1;
    }
}
