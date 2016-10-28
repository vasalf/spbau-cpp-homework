#include <phonebook.h>
#include <names.h>
#include <expat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define _unused __attribute__((unused))

static void start_element(void *data, const char *element, const char **attribute) {
    phonebook_t *now_parsing = (phonebook_t*) data;
    if (strcmp(element, "phonebook") == 0) {
        return;
    } else if (strcmp(element, "human") == 0) {
        human_t *human = calloc(sizeof(human_t), 1);
        
        for (int i = 0; attribute[i]; i += 2) {
            if (strcmp(attribute[i], "name") == 0) {
                char *buf = malloc(strlen(attribute[i + 1]) + 1);
                strcpy(buf, attribute[i + 1]);

                char *name_tok = strtok(buf, " ");
                char *middle_name_tok = strtok(NULL, " ");
                char *family_name_tok = strtok(NULL, " ");

                strcpy(human->name,  name_tok);
                strcpy(human->middle_name, middle_name_tok);
                strcpy(human->family_name, family_name_tok);

                free(buf);
            }
        }

        push_back_human(now_parsing, human);
        free(human);
    } else {
        assert(strcmp(element, "phone") == 0);
        return;
    }
}

static void end_element(_unused void *data, _unused const char *element) {
    return;
}

static void handle_data(void *data, const char *content, int length) {
    phonebook_t *phb = (phonebook_t*)data;
    if (content[0] < '0' || content[0] > '9')
        return;
    char *tmp = malloc(length + 1);
    strncpy(tmp, content, length);
    tmp[length] = '\0';

    int j = 0;
    for (; j < 10 && phb->humans[phb->size - 1].phones[j][0] != 0; j++);
    strcpy(phb->humans[phb->size - 1].phones[j], tmp);

    free(tmp);
}

static void init_phonebook_s(phonebook_t *phb) { 
    phb->size = 0;
    phb->capacity = 1;
    phb->humans = malloc(sizeof(human_t));
}

const int buf_sz = 256000;

int load_phonebook_xml(const char *filename, phonebook_t *book) {
    char buf[buf_sz];

    init_phonebook_s(book);

    FILE *fp;
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s for reading\n", filename);
        return 2;
    }

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, start_element, end_element);
    XML_SetCharacterDataHandler(parser, handle_data);
    XML_SetUserData(parser, book);

    memset(buf, 0, buf_sz);
    
    size_t len = 0;
    int done = 0;
    do {
        len = fread(buf, sizeof(char), buf_sz, fp);
        done = (int)len < buf_sz;

        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
            fprintf(stderr, "Error: %s\n", XML_ErrorString(XML_GetErrorCode(parser)));
            return 2;
        }
    } while (!done);

    XML_ParserFree(parser);
    fclose(fp);

    return 0;
}

void print_phonebook(phonebook_t *book) {
    printf("%lu\n", book->size);
    for (int i = book->size - 1; i >= 0; i--) {
        printf("name: %s\n", book->humans[i].name);
        printf("middle name: %s\n", book->humans[i].middle_name);
        printf("family name: %s\n", book->humans[i].family_name);
        for (int j = 0; j < 10 && book->humans[i].phones[j][0] != 0; j++) {
            printf("phone: %s\n", book->humans[i].phones[j]);
        }
        printf("\n");
    }
}

void push_back_human(phonebook_t *book, human_t *human) {
    if (book->size == book->capacity) {
        book->capacity *= 2;
        book->humans = realloc(book->humans, book->capacity * sizeof(human_t));
    }
    book->humans[book->size++] = *human;
}

int save_phonebook_xml(const char *filename, phonebook_t *book) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s on writing\n", filename);
        return 2;
    }

    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<phonebook>\n");
    for (int i = 0; i < (int)book->size; i++) {
        fprintf(fp, "    <human name=\"%s %s %s\">\n", book->humans[i].name, book->humans[i].middle_name, book->humans[i].family_name);
        for (int j = 0; j < 10 && book->humans[i].phones[j][0] != 0; j++)
            fprintf(fp, "        <phone>%s</phone>\n", book->humans[i].phones[j]);
        fprintf(fp, "    </human>\n");
    }
    fprintf(fp, "</phonebook>\n");

    fclose(fp);

    return 0;
}

human_t get_human(phonebook_t *book, char *family_name) {
    for (int i = 0; i < (int)book->size; i++)
        if (strcmp(book->humans[i].family_name, family_name) == 0)
            return book->humans[i];
    human_t blank;
    return blank;
}

void gen_phonebook(phonebook_t *book, size_t size) {
    init_phonebook_s(book);
    for (size_t i = 0; i < size; i++) {
        human_t *human = calloc(sizeof(human_t), 1);
        strcpy(human->name, kNames[rand() % NAMES_CNT]);
        strcpy(human->middle_name, kMiddleNames[rand() % MIDDLE_NAMES_CNT]);
        strcpy(human->family_name, kFamilyNames[rand() % FAMILY_NAMES_CNT]);
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 20; k++)
                human->phones[j][k] = '0' + rand() % 10;
        }
        push_back_human(book, human);
        free(human);
    }
}

void clear_phonebook(phonebook_t *book) {
    free(book->humans);
}
