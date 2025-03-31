#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define m 100
#define sen_len 200
#define mword 100
#define mlenword 20
#define msize 500
#define sfactor 1  

int Ewords(char *sentence, char words[][mlenword]) {
    int count = 0;
    strcpy(words[count++], "<s>");
    char temp[sen_len];
    strcpy(temp, sentence);
   
    char *token = strtok(temp, " \t\n"); 
    while (token != NULL && count < m - 1) {
        strcpy(words[count++], token);
        token = strtok(NULL, " \t\n");
    }
    strcpy(words[count++], "</s>");
    return count;
}

double Probability(int bigram, int unigram, int vs) {
    return (double)(bigram + sfactor) / (unigram + sfactor * vs);
}

int wordindex(char word[], char v[][mlenword], int vs) {
    for (int i = 0; i < vs; i++) {
        if (strcmp(word, v[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    int n;
    printf("Enter the number of training sentences: ");
    scanf("%d", &n);
    getchar();

    char trainingcorpus[m][sen_len];
    for (int i = 0; i < n; i++) {
        printf("Enter sentence %d: ", i + 1);
        fgets(trainingcorpus[i], sen_len, stdin);
        trainingcorpus[i][strcspn(trainingcorpus[i], "\n")] = '\0';
    }

    char testingcorpus[sen_len];
    printf("\nEnter test sentence: ");
    fgets(testingcorpus, sen_len, stdin);
    testingcorpus[strcspn(testingcorpus, "\n")] = '\0';

    char allwords[m * mword][mlenword];
    int total = 0;
    for (int i = 0; i < n; i++) {
        char sentence[mword][mlenword];
        int wordcount = Ewords(trainingcorpus[i], sentence);
        for (int j = 0; j < wordcount; j++) {
            strcpy(allwords[total++], sentence[j]);
        }
    }

    char v[msize][mlenword];
    int vs = 0;
    for (int i = 0; i < total; i++) {
        int unique = 1;
        for (int j = 0; j < vs; j++) {
            if (strcmp(allwords[i], v[j]) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique && vs < msize) {
            strcpy(v[vs++], allwords[i]);
        }
    }

    int unigram[msize] = {0};
    for (int i = 0; i < total; i++) {
        int index = wordindex(allwords[i], v, vs);
        if (index != -1) {
            unigram[index]++;
        }
    }

    int bigram[msize][msize] = {0};
    for (int s = 0; s < n; s++) {
        char sentence[mword][mlenword];
        int wcount = Ewords(trainingcorpus[s], sentence);
        for (int i = 0; i < wcount - 1; i++) {
            int firstindex = wordindex(sentence[i], v, vs);
            int secondindex = wordindex(sentence[i + 1], v, vs);
            if (firstindex != -1 && secondindex != -1) {
                bigram[firstindex][secondindex]++;
            }
        }
    }

    char testwords[mword][mlenword];
    int testwcount = Ewords(testingcorpus, testwords);
    printf("\nBigram Probabilities\n");

    double Probab = 1.0;
    for (int i = 0; i < testwcount - 1; i++) {
        int wordindex1 = wordindex(testwords[i], v, vs);
        int wordindex2 = wordindex(testwords[i + 1], v, vs);

        if (wordindex1 == -1 || wordindex2 == -1) {
            Probab *= 1.0 / (vs + sfactor * vs); 
            continue;
        }

        int bigramcount = bigram[wordindex1][wordindex2];
        int unigramcount = unigram[wordindex1];

        
        double prob = Probability(bigramcount, unigramcount, vs);
        Probab *= prob;

        printf("P(%s | %s) = %.10f\n", testwords[i + 1], testwords[i], prob);
    }

    printf("\nTotal Probability: %.10f\n", Probab);

    return 0;
}

