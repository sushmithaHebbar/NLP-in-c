#include <stdio.h>
#include <string.h>

char words[100][50];
int wcount = 0;
int freq[100][100] = {0};
int count[100] = {0};

// Function to find index of a word, or add it if not present
int find(const char* word) {
    for (int i = 0; i < wcount; i++) {
        if (strcmp(words[i], word) == 0)
            return i;
    }
    strcpy(words[wcount], word);
    wcount++;
    return wcount - 1;
}

// Function to compute word frequencies
void wordfreq(int n) {
    char sentence[200];
    for (int s = 0; s < n; s++) {
        printf("Enter sentence %d: ", s + 1);
        fgets(sentence, sizeof(sentence), stdin);
        sentence[strcspn(sentence, "\n")] = 0;

        char copy[200];
        strcpy(copy, sentence);

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

        int endi = find("</s>");
        freq[prev][endi]++;
        count[endi]++;
    }
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
    int n;
    printf("Enter number of sentences: ");
    scanf("%d", &n);
    getchar(); // Consume newline character

    wordfreq(n);

    char test[200];
    printf("Enter test sequence: ");
    fgets(test, sizeof(test), stdin);
    test[strcspn(test, "\n")] = 0;
    
    char* token = strtok(test, " ");
    char* testword[100];
    int len = 0;
    
    while (token != NULL) {
        testword[len++] = token;
        token = strtok(NULL, " ");
    }
    
    double total = 1.0;
    for (int i = 1; i < len; i++) {
        int p = find(testword[i - 1]);
        int c = find(testword[i]);
        double prob_val = prob(p, c);

        if (prob_val == 0.0) {
            total = 0.0;
            break;
        }

        total *= prob_val;
        printf("P(%s | %s) = %.4f\n", testword[i], testword[i - 1], prob_val);
    }

    printf("Total Probability: %.4f\n", total);
    return 0;
}

