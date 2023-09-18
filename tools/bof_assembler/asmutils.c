#include <asmutils.h>

#include <string.h>

cpu_register_t get_register(char* str) {
    cpu_register_t return_reg = 0;

    switch (str[0]) {
        case 'R': {
            switch (str[1]) {
                case '1': {
                    return_reg = R1;
                    break;
                }
                case '2': {
                    return_reg = R2;
                    break;
                }
                case '3': {
                    return_reg = R3;
                    break;
                }
                case '4': {
                    return_reg = R4;
                    break;
                }
                case '5': {
                    return_reg = R5;
                    break;
                }
                case '6': {
                    return_reg = R6;
                    break;
                }
                case '7': {
                    return_reg = R7;
                    break;
                }
                case '8': {
                    return_reg = R8;
                    break;
                }
            }
        }
        case 'S': {
            if (str[1] == 'P') {
                return_reg = SP;
            }
        }
        case 'P': {
            if (str[1] == 'C') {
                return_reg = PC;
            }
        }
    }

    return return_reg;
}

bool is_digits(char* str) {
    if (str[0] == '-') {
        str++;
    }
    while (*str) {
        if (*str < '0' || *str > '9') {
            return false;
        }
        str++;
    }
    return true;
}

bool is_hex(char* str) {
    while (*str) {
        if ((*str < '0' || *str > '9') && (*str < 'A' || *str > 'F')) {
            return false;
        }
        str++;
    }
    return true;
}

bool is_binary(char* str) {
    while (*str) {
        if (*str != '0' && *str != '1') {
            return false;
        }
        str++;
    }
    return true;
}

bool is_letters(char* str) {
    while (*str) {
        if ((*str < 'A' || *str > 'Z')) {
            return false;
        }
        str++;
    }
    return true;
}

bool is_alphanumeric(char* str) {
    while (*str) {
        if ((*str < 'A' || *str > 'Z') && (*str < '0' || *str > '9')) {
            return false;
        }
        str++;
    }
    return true;
}

void str_to_upper(char* str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32;
        }
        str++;
    }
}

uint64_t hex_str_to_num(char* str) {
    uint64_t result = 0;
    while (*str) {
        result *= 16;
        if (*str >= '0' && *str <= '9') {
            result += *str - '0';
        } else if (*str >= 'A' && *str <= 'F') {
            result += *str - 'A' + 10;
        }
        str++;
    }
    return result;
}

uint64_t binary_str_to_num(char* str) {
    uint64_t result = 0;
    while (*str) {
        result *= 2;
        result += *str - '0';
        str++;
    }
    return result;
}

uint64_t str_to_num(char* str) {
    uint64_t result = 0;
    while (*str) {
        result *= 10;
        result += *str - '0';
        str++;
    }
    return result;
}

char* find_string_end(char* str, char string_delimiter) {
    bool escaped = false;
    while (*str) {
        if (*str == string_delimiter && !escaped) {
            return str;
        }
        if (*str == '\\') {
            escaped = !escaped;
        } else {
            escaped = false;
        }
        str++;
    }

    return 0;
}
