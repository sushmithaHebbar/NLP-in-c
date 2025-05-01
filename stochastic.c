#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define word_s 100
#define tag_s 10
#define tag_len 5

typedef struct 
{
    char word[20];
    char tag[tag_len];
}word_tag;
word_tag training_data[word_s];
int tsize=0;

int count_tag(char *tag) //it counts the total tags from the training data which is enterd by the user
{
    int count=0;
    for (int i=0; i<tsize; i++) 
    {
        if (strcmp(training_data[i].tag,tag)==0) 
        {
            count++;
        }
    }
    return count;	//returns the count 
}

float emissionprobability(char *word, char *tag) //calculates the emission probability by using training data i.e word and tag and calculates the words 
{
    int word_tagc=0, tag_count=count_tag(tag);
    for (int i=0;i<tsize;i++) 
    {
        if (strcmp(training_data[i].word,word)==0 && strcmp(training_data[i].tag, tag)==0) //compare both the tag and word from the trining data and give test data 		
        {
            word_tagc++;     //if the both data exist it icrements and returns the float value by calculation
        }
    }
    return (float)word_tagc/tag_count;  //returns the count eg(c(the|dt))/c(dt)=0.75)
}

 /*the below function compares the trained data tag with prev tag and trained data tag with current tag .here it compare both the tags prev and current tag 
 eg(c(dt|NN)/c(NN)).once it is equal it increments the count and return s the float value by calculating probabilty of bigramcount tag and prevcount tag*/
 
float transitionprob(char *prev, char *curr) {
    int bigramcount=0, prev_cnt=count_tag(prev);
    for (int i=0; i<tsize-1; i++) 
    {
        if (strcmp(training_data[i].tag,prev)==0&&strcmp(training_data[i+1].tag,curr)==0) 
       
        {
            bigramcount++;
        }
    }
    return (float)bigramcount/prev_cnt;
}

/* the tagsentence is divided into words and added to the sentence tags  and calculates the emission and transsion probabilities by the help of trained data and calculates the joint probabilities*/

void tagsentence(char *sentence) 
{
    char *words[word_s],*tags[word_s];
    int wc=0,i;
    char *token=strtok(sentence," ");   //
    while (token!=NULL) 
    {
        words[wc++]=token;
        token=strtok(NULL," ");
    }
    printf("\nTagged Sentence:\n");
    float finalProbability=1.0;
    for (int i=0;i<wc;i++) 
    {
        tags[i]="UNK"; 
        float emissionprobab=0.0;
        for (int j=0; j<tsize;j++) 
        {
            if (strcmp(words[i],training_data[j].word)==0) 
            {
                float emissionprob=emissionprobability(words[i],training_data[j].tag);
                if (emissionprob>emissionprobab) 
                {
                    emissionprobab=emissionprob;
                    tags[i]=training_data[j].tag;
                }
            }
        }
        printf("%s/%s ",words[i],tags[i]);
        finalProbability *=emissionprobab;
       	if(i>0)
       	{      	
            float transitionProb=transitionprob(tags[i-1],tags[i]);
            finalProbability*=transitionProb;
            
         }
    }
    printf("\nFinal Probability: %.6f\n",finalProbability);
}

int main() {
    int n;
    printf("Enter the number of training word data: ");
    scanf("%d",&n);
    getchar();
    printf("Enter word and tag :\n");
    for (int i=0;i<n;i++) {
        scanf("%s %s",training_data[i].word,training_data[i].tag);
    }
    tsize=n;
    printf("Bigram Probability:\n");
    getchar();

    char testsent[200];
    printf("Enter the test sentence: ");
    fgets(testsent,sizeof(testsent), stdin);
    testsent[strcspn(testsent, "\n")]=0; 
    tagsentence(testsent );
    return 0;
}



