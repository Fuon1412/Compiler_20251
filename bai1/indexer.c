#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 10000
#define MAX_STOPWORDS 200
#define MAX_LINES 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
    int lines[MAX_LINES];
    int line_count;
} WordInfo;

static char stopwords[MAX_STOPWORDS][MAX_WORD_LEN];
static int stopword_count = 0;

static WordInfo index_table[MAX_WORDS];
static int index_size = 0;

// -----------------------------------------
// Utility
// -----------------------------------------
void to_lower(char *s) {
    for (int i = 0; s[i]; i++)
        s[i] = tolower(s[i]);
}

int is_letter(char c) {
    return isalpha((unsigned char)c);
}

void trim_newline(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

// -----------------------------------------
// Stopword
// -----------------------------------------
void load_stopwords(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open stopwords file: %s\n", filename);
        return;
    }

    char buff[MAX_WORD_LEN];
    while (fgets(buff, sizeof(buff), f)) {
        trim_newline(buff);
        if (strlen(buff) == 0) continue;
        to_lower(buff);
        strcpy(stopwords[stopword_count++], buff);
    }

    fclose(f);
}

int is_stopword(const char *word) {
    char tmp[MAX_WORD_LEN];
    strcpy(tmp, word);
    to_lower(tmp);

    for (int i = 0; i < stopword_count; i++)
        if (strcmp(tmp, stopwords[i]) == 0)
            return 1;
    return 0;
}

// -----------------------------------------
// Proper noun
// -----------------------------------------
int is_proper_noun(const char *word, int after_punctuation) {
    return (isupper(word[0]) && !after_punctuation);
}

// -----------------------------------------
// Index table
// -----------------------------------------
int find_word(const char *lower_word) {
    for (int i = 0; i < index_size; i++)
        if (strcmp(lower_word, index_table[i].word) == 0)
            return i;
    return -1;
}

void insert_word(const char *word, int line_no) {
    char lower[MAX_WORD_LEN];
    strcpy(lower, word);
    to_lower(lower);

    int pos = find_word(lower);
    if (pos == -1) {
        if (index_size >= MAX_WORDS) {
            printf("Warning: index table full, word ignored: %s\n", lower);
            return;
        }

        WordInfo *w = &index_table[index_size++];
        strcpy(w->word, lower);
        w->count = 1;
        w->lines[0] = line_no;
        w->line_count = 1;
    } else {
        WordInfo *w = &index_table[pos];
        w->count++;
        if (w->line_count == 0 || w->lines[w->line_count - 1] != line_no) {
            if (w->line_count < MAX_LINES)
                w->lines[w->line_count++] = line_no;
        }
    }
}

// -----------------------------------------
// Sorting
// -----------------------------------------
int cmp_word(const void *a, const void *b) {
    const WordInfo *A = (const WordInfo *)a;
    const WordInfo *B = (const WordInfo *)b;
    return strcmp(A->word, B->word);
}

void sort_index() {
    qsort(index_table, index_size, sizeof(WordInfo), cmp_word);
}

// -----------------------------------------
// Text processing
// -----------------------------------------
void process_text(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Cannot open input file: %s\n", filename);
        return;
    }

    char line[1000];
    int line_no = 0;
    int after_punct = 1;  // câu đầu tiên xem như sau dấu chấm

    while (fgets(line, sizeof(line), f)) {
        line_no++;

        char word[MAX_WORD_LEN];
        int len = 0;

        for (int i = 0; line[i]; i++) {
            char c = line[i];

            if (is_letter(c)) {
                if (len < MAX_WORD_LEN - 1)
                    word[len++] = c;
                // Nếu quá dài → bỏ qua phần còn lại của từ
            } else {
                if (len > 0) {
                    word[len] = '\0';

                    if (!is_stopword(word) &&
                        !is_proper_noun(word, after_punct))
                    {
                        insert_word(word, line_no);
                    }
                    len = 0;
                }

                // Update punctuation state
                if (c == '.' || c == '!' || c == '?')
                    after_punct = 1;
                else if (is_letter(c) == 0 && c != ' ' && c != '\t')
                    after_punct = 0;
            }
        }

        // Từ cuối dòng
        if (len > 0) {
            word[len] = '\0';
            if (!is_stopword(word) &&
                !is_proper_noun(word, after_punct))
            {
                insert_word(word, line_no);
            }
        }
    }

    fclose(f);
}

// -----------------------------------------
// Output
// -----------------------------------------
void write_output(const char *filename) {
    sort_index();

    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Cannot write to output file: %s\n", filename);
        return;
    }

    for (int i = 0; i < index_size; i++) {
        WordInfo *w = &index_table[i];
        fprintf(f, "%s %d", w->word, w->count);
        for (int j = 0; j < w->line_count; j++)
            fprintf(f, ",%d", w->lines[j]);
        fprintf(f, "\n");
    }

    fclose(f);
}

// -----------------------------------------
// Main
// -----------------------------------------
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <textfile> [stopwords] [output]\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];
    const char *stop_file  = (argc >= 3) ? argv[2] : "stopw.txt";
    const char *output_file = (argc >= 4) ? argv[3] : "index.txt";

    printf("Input: %s\nStopwords: %s\nOutput: %s\n", 
           input_file, stop_file, output_file);

    load_stopwords(stop_file);
    process_text(input_file);
    write_output(output_file);

    printf("Done. Unique words: %d\n", index_size);
    return 0;
}
