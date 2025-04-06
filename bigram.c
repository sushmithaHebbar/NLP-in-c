#include <stdio.h>
#include <string.h>

#define MAX 100
#define MAX_LEN 50

char words[MAX][MAX_LEN];
int wordCount = 0;
int freq[MAX][MAX] = {0};
int count[MAX] = {0};

// Function to find index of a word, or add it if not present
int find(const char* word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(words[i], word) == 0)
            return i;
    }
    strcpy(words[wordCount], word);
    wordCount++;
    return wordCount - 1;
}

// Function to compute word frequencies
void computation(const char* set[], int n) {
    for (int s = 0; s < n; s++) {
        char copy[200];
        strcpy(copy, set[s]);

        char* token = strtok(copy, " ");
        int prev = find("<s>");
        count[prev]++;

        while (token != NULL) {
            int curr = find(token);
            count[curr]++;
            if (prev != -1) {
                freq[prev][curr]++;
            }
            prev = curr;
            token = strtok(NULL, " ");
        }

        int endix = find("</s>");
        freq[prev][endix]++;
        count[endix]++;
    }
}

// Adding Stemming 
void stem(char* word) {
    int len = strlen(word);
    if (len > 4 && strcmp(&word[len - 3], "ing") == 0)
        word[len - 3] = '\0';
    else if (len > 3 && strcmp(&word[len - 2], "ed") == 0)
        word[len - 2] = '\0';
    else if (len > 2 && word[len - 1] == 's')
        word[len - 1] = '\0';
}

// Function to calculate probability of word transition
double prob(int prev, int curr) {
    if (count[prev] == 0) return 0.0;
    
    // Ensure that the last word always transitions to </s> with probability 1
    if (strcmp(words[curr], "</s>") == 0 && freq[prev][curr] > 0) {
        return 1.0;
    }
    
    return (double)freq[prev][curr] / count[prev];
}

// Main function
int main() {
    const char* corpus[] = {
        "there is a big garden",
        "children play in a garden",
        "they play inside a garden beautiful garden"
    };

    computation(corpus, 3);

//Stop word Flter
    const char* stopwords[] = {
    "the", "a", "an", "in", "on", "at", "is", "are", "was", "were", "of",
    "and", "or", "but", "to", "with", "for", "from", "by", "about", "<s>", "</s>"
};
int stopwords_count = sizeof(stopwords) / sizeof(stopwords[0]);

int is_stopword(const char* word) {
    for (int i = 0; i < stopwords_count; i++) {
        if (strcmp(word, stopwords[i]) == 0)
            return 1;
    }
    return 0;
}


    
    const char* test[] = { "<s>", "they", "play", "in", "a", "big", "garden", "</s>" };
    int len = 8;

    double total = 1.0;
    for (int i = 1; i < len; i++) {
        int p = find(test[i - 1]);
        int c = find(test[i]);
        double prob_val = prob(p, c);

        if (prob_val == 0.0) {
            total = 0.0;
            break;
        }

        total *= prob_val;
        printf("P(%s | %s) = %.4f\n", test[i], test[i - 1], prob_val);
    }

    printf("Total Probability: %.4f\n", total);
    return 0;
}

