#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUESTIONS 100
#define MAX_OPTIONS 4
#define MAX_TEXT_LEN 256

typedef struct {
    int questionNumber;
    char questionText[MAX_TEXT_LEN];
    char optionA[MAX_TEXT_LEN];
    char optionB[MAX_TEXT_LEN];
    char optionC[MAX_TEXT_LEN];
    char optionD[MAX_TEXT_LEN];
    char correctAnswer;
} Question;

typedef struct {
    Question questions[MAX_QUESTIONS];
    int size;
} Question_Bank;

void ReadQuestionsFromFile(Question_Bank* Question_Bank, const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    char line[MAX_TEXT_LEN];
    Question question;
    Question_Bank->size = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'Q') {
            question.questionNumber = atoi(line + 1);
            fgets(question.questionText, sizeof(question.questionText), file);
            fgets(question.optionA, sizeof(question.optionA), file);
            fgets(question.optionB, sizeof(question.optionB), file);
            fgets(question.optionC, sizeof(question.optionC), file);
            fgets(question.optionD, sizeof(question.optionD), file);
            fgets(line, sizeof(line), file);
            question.correctAnswer = line[strlen(line) - 2];

            Question_Bank->questions[Question_Bank->size++] = question;
        }
    }
    fclose(file);
}
void ShuffleOptions(Question* question) {
    char* options[MAX_OPTIONS] = {question->optionA, question->optionB, question->optionC, question->optionD};
    for (int i = MAX_OPTIONS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_TEXT_LEN];
        strcpy(temp, options[i]);
        strcpy(options[i], options[j]);
        strcpy(options[j], temp);
    }
    strcpy(question->optionA, options[0]);
    strcpy(question->optionB, options[1]);
    strcpy(question->optionC, options[2]);
    strcpy(question->optionD, options[3]);
}
void SelectRandomQuestions(Question_Bank* Question_Bank, Question* randomQuestions, int numQuestions) {
    int index[MAX_QUESTIONS];
    for (int i = 0; i < Question_Bank->size; i++) {
        index[i] = i;
    }
    for (int i = Question_Bank->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = index[i];
        index[i] = index[j];
        index[j] = temp;
    }
    for (int i = 0; i < numQuestions; i++) {
        randomQuestions[i] = Question_Bank->questions[index[i]];
    }
}
void CreateBooklet(Question_Bank* Question_Bank, int numBooklets, int numQuestions) {
    for (int i = 0; i < numBooklets; i++) {
        Question bookletQuestions[numQuestions];
        SelectRandomQuestions(Question_Bank, bookletQuestions, numQuestions);
        for (int j = 0; j < numQuestions; j++) {
            ShuffleOptions(&bookletQuestions[j]);
        }
        char questionsFileName[64];
        char answersFileName[64];
        sprintf(questionsFileName, "Booklet_%d_Questions.txt", i + 1);
        sprintf(answersFileName, "Booklet_%d_Answers.txt", i + 1);
        FILE* questionsFile = fopen(questionsFileName, "w");
        FILE* answersFile = fopen(answersFileName, "w");

        for (int j = 0; j < numQuestions; j++) {
            fprintf(questionsFile, "Q%d: %s", j + 1, bookletQuestions[j].questionText);
            fprintf(questionsFile, "%s", bookletQuestions[j].optionA);
            fprintf(questionsFile, "%s", bookletQuestions[j].optionB);
            fprintf(questionsFile, "%s", bookletQuestions[j].optionC);
            fprintf(questionsFile, "%s\n", bookletQuestions[j].optionD);
            fprintf(answersFile, "Q%d: ANS: %c.\n", j + 1, bookletQuestions[j].correctAnswer);
        }
        fclose(questionsFile);
        fclose(answersFile);
    }
}

int main() {
    srand((unsigned int)time(NULL));
    Question_Bank Question_Bank;

    char fileName[256];
    printf("Enter the name of the file to read questions from: ");
    fgets(fileName, sizeof(fileName), stdin);

    fileName[strcspn(fileName, "\n")] = 0;

    ReadQuestionsFromFile(&Question_Bank, fileName);

    int numBooklets, numQuestions;
    printf("Enter the number of booklets to create: ");
    while (scanf("%d", &numBooklets) != 1) {
        printf("Invalid input. Please enter an integer: ");
        while (getchar() != '\n');
    }

    printf("Enter the number of questions per booklet: ");
    while (scanf("%d", &numQuestions) != 1) {
        printf("Invalid input. Please enter an integer: ");
        while (getchar() != '\n');
    }

    CreateBooklet(&Question_Bank, numBooklets, numQuestions);
    printf("Booklets successfully created.\n");
    return 0;
}

