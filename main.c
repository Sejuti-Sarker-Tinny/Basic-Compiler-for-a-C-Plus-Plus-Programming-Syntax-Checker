#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

const char* cpp_keywords[] = {
    "int", "float", "return", "if", "else", "for", "while", "class",
    "public", "private", "protected", "virtual", "namespace", "template",
    "using", "std", "cin", "cout", "endl", "new", "delete", "const",
    "void", "bool", "char", "double", "long", "short", "unsigned", NULL
};

int is_cpp_keyword(const char* token) {
    for (int i = 0; cpp_keywords[i] != NULL; i++) {
        if (strcmp(token, cpp_keywords[i]) == 0) return 1;
    }
    return 0;
}

int is_valid_identifier(const char* str) {
    if (!isalpha(str[0]) && str[0] != '_') return 0;  // Identifiers must start with a letter or underscore
    for (int i = 1; str[i]; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return 0;
    }
    return 1;
}

int check_parentheses(const char* line, int* balance) {
    for (int i = 0; line[i]; i++) {
        if (line[i] == '(') (*balance)++;
        else if (line[i] == ')') (*balance)--;
        if (*balance < 0) return 0;
    }
    return 1;
}

void check_syntax(char code[MAX_LINES][MAX_LINE_LENGTH], int line_count) {
    int error_count = 0;
    int paren_balance = 0;

    for (int i = 0; i < line_count; i++) {
        char line_copy[MAX_LINE_LENGTH];
        strncpy(line_copy, code[i], MAX_LINE_LENGTH);

        char* line = code[i];
        size_t len = strlen(line);

        if (len > 1 && line[len - 1] != ';' && !strchr(line, '{') && !strchr(line, '}')) {
            printf("Error on line %d: Missing semicolon.\n", i + 1);
            error_count++;
        }

        if (!check_parentheses(line, &paren_balance)) {
            printf("Error on line %d: Unbalanced parentheses.\n", i + 1);
            error_count++;
        }

        char* token = strtok(line_copy, " ");
        while (token) {
            if (is_cpp_keyword(token)) {
                // Valid C++ keyword
            } else if (!is_valid_identifier(token) && !strchr(token, '(') && !strchr(token, ')') &&
                       !strchr(token, ';') && !strchr(token, '=') && !strchr(token, '{') &&
                       !strchr(token, '}') && !strchr(token, ':') && !strchr(token, '<') &&
                       !strchr(token, '>')) {
                printf("Error on line %d: Invalid identifier '%s'.\n", i + 1, token);
                error_count++;
            }
            token = strtok(NULL, " ");
        }
    }

    if (paren_balance != 0) {
        printf("Error: Unbalanced parentheses in the code.\n");
        error_count++;
    }

    if (error_count == 0) {
        printf("No syntax errors found.\n");
    } else {
        printf("Total syntax errors: %d\n", error_count);
    }
}

int main() {
    char code[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

    printf("Enter C++ code (finish with a blank line):\n");

    while (1) {
        char line[MAX_LINE_LENGTH];
        fgets(line, sizeof(line), stdin);

        if (strcmp(line, "\n") == 0) break;

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

        strncpy(code[line_count], line, MAX_LINE_LENGTH);
        line_count++;
        if (line_count >= MAX_LINES) {
            printf("Error: Too many lines of input.\n");
            return 1;
        }
    }

    check_syntax(code, line_count);

    return 0;
}
