#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PATH 1024
#define MAX_LINE 1024
#define MAX_ALGOS 100
#define MAX_CATEGORIES 20


typedef struct {
    char name[20];
    char time_complexity[256];
    char code[8192];
    char filepath[MAX_PATH];
} Algorithm;

typedef struct {
    char name[256];
    char path[MAX_PATH];
    Algorithm algos[MAX_ALGOS];
    int count;
} Category;

Category categories[MAX_CATEGORIES];
int category_count = 0;

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
        system("clear");
#endif
}

int is_directory(const char* path)
{
    struct stat sb;
    return (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
}

int is_algo_file(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    if (!ext) return 0;
    return (strcmp(ext, ".algo") == 0 || strcmp(ext, ".c") == 0);
}

void parse_algo_files(const char* filepath, Algorithm* algo)
{
    FILE* f = open(filepath, "r");
    if (!f) return;

    strcpy(algo->name, "Без названия");
    strcpy(algo->time_complexity, "Не указана");
    algo->code[0] = '\0';

    char line[MAX_LINE];
    int in_metadata = 1;
    char code_buffer[8192] = {0};

    while (fgets(line, sizeof(line), f)) {
        if (in_metadata && strncmp(line, "// Name:", 8) == 0) {
            char* start = line + 8;
            while (*start == ' ' || *start == '\t') start++;
            char* end = start + strlen(start) - 1;
            while (end > start && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
            strncpy(algo->name, start, sizeof(algo->name) - 1);
        } else if (in_metadata && strncmp(line, "// Time:", 8) == 0) {
            char* start = line + 8;
            while (*start == ' ' || *start == '\t') start++;
            char* end = start + strlen(start) - 1;
            while (end > start && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
            strncpy(algo->time_complexity, start, sizeof(algo->time_complexity) - 1);
        } else if (in_metadata && (strncmp(line, "//", 2) != 0 || strlen(line) <= 3)) {
            in_metadata = 0;
            strcat(code_buffer, line);
        } else if (!in_metadata) {
            strcat(code_buffer, line);
        }
    }

    size_t len = strlen(code_buffer);
    while (len > 0 && (code_buffer[len - 1] == '\n' || code_buffer[len - 1] == '\r')) {
        code_buffer[--len] = '\0';
    }
    strcpy(algo->code, code_buffer);
    strcpy(algo->filepath, filepath);

    fclose(f);
}

void load_algorithms(const char* base_dir) {
    DIR* base = opendir(base_dir);
    if (!base) {
        fprintf(stderr, "Ошибка: не найдена папка '%s'\n", base_dir);
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(base)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        char cat_path[MAX_PATH];
        snprintf(cat_path, sizeof(cat_path), "%s/%s", base_dir, entry->d_name);

        if (is_directory(cat_path)) {
            if (category_count >= MAX_CATEGORIES) {
                fprintf(stderr, "Слишком много категорий!\n");
                break;
            }

            Category* cat = &categories[category_count];
            strncpy(cat->name, entry->d_name, sizeof(cat->name) - 1);
            strncpy(cat->path, cat_path, sizeof(cat->path) - 1);
            cat->count = 0;

            DIR* cat_dir = opendir(cat_path);
            if (!cat_dir) continue;

            struct dirent* file_entry;
            while ((file_entry = readdir(cat_dir)) != NULL) {
                if (file_entry->d_name[0] == '.') continue;
                if (!is_algo_file(file_entry->d_name)) continue;

                if (cat->count >= MAX_ALGOS) {
                    fprintf(stderr, "Слишком много алгоритмов в категории %s!\n", cat->name);
                    break;
                }

                char file_path[MAX_PATH];
                snprintf(file_path, sizeof(file_path), "%s/%s", cat_path, file_entry->d_name);
                parse_algo_file(file_path, &cat->algos[cat->count]);
                cat->count++;
            }
            closedir(cat_dir);
            category_count++;
        }
    }
    closedir(base);
}

void trim_trailing(char* str) {
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

char* read_user_code() {
    char* buffer = NULL;
    size_t bufsize = 0;
    size_t len = 0;
    char line[1024];

    printf("Введите код (введите '--done' в отдельной строке для завершения):\n");

    while (fgets(line, sizeof(line), stdin)) {
        if (strncmp(line, "--done", 6) == 0) break;

        size_t line_len = strlen(line);
        if (len + line_len + 1 > bufsize) {
            bufsize = bufsize ? bufsize * 2 : 1024;
            buffer = realloc(buffer, bufsize);
            if (!buffer) { perror("realloc"); exit(1); }
        }
        memcpy(buffer + len, line, line_len);
        len += line_len;
    }

    if (!buffer) {
        buffer = calloc(1, 1);
    } else {
        buffer[len] = '\0';
        trim_trailing(buffer);
    }
    return buffer;
}

int codes_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

int main() {
    const char* ALGO_DIR = "algorithms";
    load_algorithms(ALGO_DIR);

    if (category_count == 0) {
        fprintf(stderr, "Нет категорий в папке '%s'\n", ALGO_DIR);
        return 1;
    }

    printf("=== Алгоритм-меморайзер ===\n\n");
    printf("Выберите категорию:\n");
    for (int i = 0; i < category_count; i++) {
        printf("%d. %s (%d алгоритмов)\n", i + 1, categories[i].name, categories[i].count);
    }

    int cat_choice;
    printf("\nВаш выбор (1-%d): ", category_count);
    if (scanf("%d", &cat_choice) != 1 || cat_choice < 1 || cat_choice > category_count) {
        fprintf(stderr, "Неверный ввод!\n");
        return 1;
    }
    getchar(); 

    Category* cat = &categories[cat_choice - 1];
    if (cat->count == 0) {
        printf("В категории нет алгоритмов.\n");
        return 1;
    }

    printf("\nАлгоритмы в категории '%s':\n", cat->name);
    for (int i = 0; i < cat->count; i++) {
        printf("%d. %s\n", i + 1, cat->algos[i].name);
    }

    int algo_choice;
    printf("\nВыберите алгоритм (1-%d): ", cat->count);
    if (scanf("%d", &algo_choice) != 1 || algo_choice < 1 || algo_choice > cat->count) {
        fprintf(stderr, "Неверный ввод!\n");
        return 1;
    }
    getchar();

    Algorithm* algo = &cat->algos[algo_choice - 1];

    printf("\n=== %s ===\n", algo->name);
    printf("Сложность: %s\n\n", algo->time_complexity);
    printf("%s\n", algo->code);
    printf("\nЗапомните! Через 6 секунд код исчезнет...\n");
    fflush(stdout);
    sleep(6);

    clear_screen();

    char* user_code = read_user_code();

    if (codes_equal(algo->code, user_code)) {
        printf("\nОтлично! Вы ввели всё верно!\n");
    } else {
        printf("\nПочти! Правильный код:\n");
        printf("%s\n", algo->code);
    }

    free(user_code);
    return 0;
}
