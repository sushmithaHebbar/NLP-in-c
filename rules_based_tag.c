#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[50];
    char tag[10];
} tagword;

tagword utag[100];
int count = 0;

// Check if word already has a stored tag
char *usertag(char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(utag[i].word, word) == 0)  // Corrected string comparison
            return utag[i].tag;
    }
    return NULL;
}

// Store a new word with its tag
void storestag(char *word, char *tag) {  // Changed return type to void
    if (count < 100) {
        strncpy(utag[count].word, word, sizeof(utag[count].word) - 1);
        strncpy(utag[count].tag, tag, sizeof(utag[count].tag) - 1);
        utag[count].word[sizeof(utag[count].word) - 1] = '\0';
        utag[count].tag[sizeof(utag[count].tag) - 1] = '\0';
        count++;
    }
}

// Determine the tag for a word
char *tags(char *word) {
    char *tagexist = usertag(word);
    if (tagexist)
        return tagexist;

    int len = strlen(word);
    if (isupper(word[0])) {
        int wordup = 1;
        for (int i = 0; i < len; i++) {
            if (!isupper(word[i])) {
                wordup = 0;
                break;
            }
        }
        if (wordup)
            return "NNP";
    }

    if (len >= 4 && strcmp(&word[len - 4], "tion") == 0)
        return "NN";
    if (len >= 4 && strcmp(&word[len - 4], "ment") == 0)
        return "NN";
    if (len >= 2 && strcmp(&word[len - 2], "ly") == 0)
        return "RB";
    if (len >= 3 && strcmp(&word[len - 3], "ing") == 0)
        return "VB";
    if (len >= 2 && strcmp(&word[len - 2], "ed") == 0)
        return "VB";
    if (len >= 4 && strcmp(&word[len - 4], "able") == 0)
        return "JJ";
    if (len >= 3 && strcmp(&word[len - 3], "ful") == 0)
        return "JJ";
    if (len >= 4 && strcmp(&word[len - 4], "ness") == 0)
        return "NN";

    if (strcmp(word, "is") == 0 || strcmp(word, "are") == 0 || strcmp(word, "were") == 0)
        return "VB";
    if (strcmp(word, "The") == 0 || strcmp(word, "A") == 0 || strcmp(word, "a") == 0 || strcmp(word, "the") == 0)
        return "DT";
    if (strcmp(word, "and") == 0 || strcmp(word, "or") == 0 || strcmp(word, "but") == 0)
        return "CC";
    if (strcmp(word, "very") == 0)
        return "RB";
    if (strcmp(word, "new") == 0)
        return "JJ";
    if (strcmp(word, "in") == 0 || strcmp(word, "on") == 0 || strcmp(word, "to") == 0 || strcmp(word, "of") == 0)
        return "IN";

    return "UNK";
}

// Process a sentence and tag words
void tagsent(char *sent) {
    char word[50];
    char *wordtkn = strtok(sent, " .,!?");  // Tokenizing words
    while (wordtkn != NULL) {
        strcpy(word, wordtkn);
        char *tag = tags(word);

        if (strcmp(tag, "UNK") == 0) {
            printf("\nEnter tag for %s: ", word);
            char newtag[10];
            scanf("%s", newtag);
            getchar();
            storestag(word, newtag);
            tag = newtag;  // Update with new tag
        }
        printf("%s/%s ", word, tag);
        wordtkn = strtok(NULL, " .,!?");  // Next word
    }
    printf("\n");
}

// Main function to process multiple sentences
int main() {
    int n;
    printf("Enter the number of sentences: ");
    scanf("%d", &n);
    getchar();  // Consume newline

    char sent[500];
    for (int i = 0; i < n; i++) {
        printf("Enter sentence %d: ", i + 1);
        fgets(sent, sizeof(sent), stdin);
        sent[strcspn(sent, "\n")] = 0;  // Remove newline
        printf("Tagged output: ");
        tagsent(sent);
    }
    return 0;
}

