/*  Gewrgios Lampris

    Leonidas Kalafatas
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define size 1000

typedef struct gList {
 char *concept;
 char *sentence;
 int timesUsed;
 char learnedFrom[5];
 struct gList *next;
 struct gList *prev;
} gtpList;

int check_cmd(char str[]);
void answers(int ans, char str[], int type);
int learn(gtpList** head, char str[]);
void forget(gtpList** head, char str[]);
void displayConcepts(gtpList *head);
void displayTalked(gtpList *head);
void insertNode(gtpList **head, gtpList *node);
gtpList *createNode(char *concept, char *sentence, int type);
int check_if_known(gtpList **head, char str[]);
void generic_ans();
void search(char str[], gtpList **head);
void idk_ans(char *concept);
void read(gtpList** head, char str[]);
void dialogue(char *text, int bot);
void botResponse(char *response);
void free_list(gtpList **head);

int known = 0;
int counter_file = 0;


int main(void){

    srand(time(NULL));

    int command,i,key,wrong;
    char str[size];
    char lastChar;
    gtpList *head = NULL;

    printf("ChatGTP$ Hi!You decided to bother me so go on but make it quick!\n");
    botResponse("ChatGTP$ Hi!You decided to bother me so go on but make it quick!");

    do{
        printf("\nme$ ");
        fgets(str , sizeof(str) , stdin);
        str[strcspn(str, "\n")] = '\0';
        dialogue(str,1);

        if(!strncmp(str, "What is the meaning of Life, The Universe, and Everything?", 58)){
            printf("ChatGTP$ 42");
            botResponse("42");
            continue;
        }
        if(!strncmp(str, "What do you know about?", 23)){
            displayConcepts(head);
            continue;
        }
        if(!strncmp(str, "What do you talk about?", 23)){
            displayTalked(head);
            continue;
        }
        if(!strncmp(str, "So Long, and Thanks for All the Fish", 36)){
            free_list(&head);
            printf("\nChatGTP$ Farewell, you will not be missed!\n");
            botResponse("Farewell, you will not be missed!\n");
            return 0;
        }
            

        int length = strlen(str);
        if (length > 0) {
            lastChar = str[length - 1];
        }

        if(lastChar == '?'){
            search(str,&head);
            continue;
        }
            

        for(i=0;i<size;i++){
            if(str[i] == '>'){
                command = check_cmd(str);
                break;
            }
        }

        switch(command){
            case 1:
                wrong = learn(&head,str);
                if(known == 1)
                    key = 1;
                else
                    key = 0;
                if(wrong == 0)
                    answers(key,str,0);
                known = 0;
            break;
            case 2:
                read(&head, str);
            break;
            case 3:
                forget(&head,str);
            break;
            default:
                generic_ans();
            break;   
        }
    }while(1);

    return 0;
}

int check_cmd(char str[]){
    char cmd1[] = "learn this >";
    char cmd2[] = "read this >";
    char cmd3[] = "forget this >";

    if(strncmp(str, cmd1, 12) == 0){
            return 1;
    } 
    else if(strncmp(str, cmd2, 11) == 0){
            return 2;
    } 
    else if(strncmp(str, cmd3, 13) == 0){
            return 3;
    }
    return 5;
}

void answers(int ans, char str[], int type){
    int key,i,j;
    char *concept;
    char log[size];
    char answers[10][size] = {"Wow!What an intresting inforation about `.",
                        "I'm sure I learned about ` much faster than you did!",
                        "What took you so long to tell me about `?",
                        "I almost died of bordom learning about `...",
                        "Was it necessary for me to learn about `?",
                        "Haha you'd hope I didn't know about ` but I am way smarter than you!",
                        "` is pretty basic knowledge...I'm surprised you know about it!",
                        "I knew about ` since the start of time",
                        "Honestly it's insulting to think that I didn't know about `",
                        "` must be impressive for you but I already know everything about it!"};
    
    if(type == 0){
        concept = strstr(str, ">") + 1;
        if (concept == NULL) {
            printf("Invalid input format. Please use 'learn this > concept: description' format.\n");
            botResponse("Invalid input format. Please use 'learn this > concept: description' format.\n");
            return;
        }
        while (*concept == ' ')
            concept++;
    }
    else
        concept = str;

    if(ans == 0) 
        key = rand()%5;
    else
        key = 5 + rand()%5;

    printf("\nChatGTP$ ");
    for(i=0;i<size;i++){
        if(answers[key][i] == '`'){
            printf("%s", concept);
        }
            
        else{
            printf("%c", answers[key][i]);
            log[i] = answers[key][i];
        }
    }
    botResponse(log);
    dialogue(concept,-1);
    dialogue("\n", -1);
    printf("\n");
}

void generic_ans(){
    int i,key;
    char log[100];
    char answers[6][50] = {"Yeah I guess you're right...",
                            "You may not be that dumb",
                            "How come and you're right?",
                            "Please open up a book once in a while...",
                            "You are so wrong, it's actually so funny!",
                            "I completely disagree and I don't even care!"};
    key = rand()%6;

    printf("ChatGTP$ ");
    for(i=0;i<50;i++){
        printf("%c", answers[key][i]);
        log[i] = answers[key][i];
    }
    botResponse(log);
}

int learn(gtpList **head, char str[]) {
    char *concept, *sentence;
    int type = 1;

    concept = strstr(str, ">") + 1;
    if (concept == NULL) {
        printf("\nInvalid input format. Please use 'learn this > concept: description' format.\n");
        botResponse("Invalid input format. Please use 'learn this > concept: description' format.\n");
        return 1;
    }

    while (*concept == ' ')
        concept++;

    sentence = strstr(concept, ":");
    if (sentence == NULL) {
        printf("\nInvalid input format. Please use 'learn this > concept: description' format.\n");
        botResponse("Invalid input format. Please use 'learn this > concept: description' format.\n");
        return 1;
    }

    *sentence = '\0';
    sentence++;
    while (*sentence == ' ')
        sentence++;

    gtpList *new = createNode(concept, sentence, type);
    insertNode(head, new);
    return 0;
}


void displayConcepts(gtpList *head) {
    gtpList *current = head;
    printf("\nChatGTP$ I certainly know more things than you, here they are:\n");
    botResponse("I certainly know more things than you, here they are:\n");
    botResponse(" ");
    while (current != NULL) {
        printf("- %s\n", current->concept);
        dialogue("-",-1);
        dialogue(current->concept,-1);
        dialogue("\n",-1);
        current = current->next;
    }
}

void displayTalked(gtpList *head){
    gtpList *current = head;
    printf("\nChatGTP$ The concepts that I talk about a lot are:\n");
    botResponse(" ");
    while (current != NULL) {
        if(current->timesUsed > 0){
            printf("- %s\n", current->concept);
            dialogue("-",-1);
            dialogue(current->concept,-1);
            dialogue("\n",-1);
        }
        current = current->next;
    }
}

void insertNode(gtpList **head, gtpList *node) {

    if (*head == NULL) {
        *head = node;
        return;
    }

    gtpList *current = *head;
    while (current != NULL) {
        if(strcmp(node->concept, current->concept) == 0){
        known = 1;
        free(node);
        return;
        }
        if (strcmp(node->concept, current->concept) < 0)
            break;
        current = current->next;
    }

    if (current == *head) {
        node->next = *head;
        (*head)->prev = node;
        *head = node;
    } else if (current == NULL) {
        gtpList *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
            temp->next = node;
            node->prev = temp;
    } else {
        node->next = current;
        node->prev = current->prev;
        current->prev->next = node;
        current->prev = node;
    }
    

}

gtpList *createNode(char *concept, char *sentence, int type) {
    gtpList *new = (gtpList *)malloc(sizeof(gtpList));
    new->concept = strdup(concept);
    new->sentence = strdup(sentence);
    new->timesUsed = 0;
    if(type == 1)
        strcpy(new->learnedFrom, "kbrd");
    else
        strcpy(new->learnedFrom, "file");
    new->next = NULL;
    new->prev = NULL;
    return new;
}

void deleteNode(gtpList** head, gtpList* node) {
    if (*head == NULL || node == NULL) {
        return;
    }

    if (*head == node) {
        *head = node->next;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
}


void forget(gtpList** head, char str[]) {
    char *delimiter = ",";
    char *token;
    char log[100];
    int deletedNodes = 0;

    char *concepts = strstr(str, ">") + 1;

    token = strtok(concepts, delimiter);
    while (token != NULL) {
        char *concept = token;
        concept = strdup(concept);
        char *conceptEnd = concept + strlen(concept) - 1;
        while (*conceptEnd == ' ') {
            *conceptEnd = '\0';
            conceptEnd--;
        }
        while (*concept == ' ')
            concept++;

        if (concept[strlen(concept) - 1] == '\n')
            concept[strlen(concept) - 1] = '\0';

        gtpList *current = *head;
        while (current != NULL) {
            if (strcmp(concept, current->concept) == 0) {
                gtpList* nextNode = current->next;
                deleteNode(head, current);
                free(current->concept);
                free(current->sentence);
                free(current);
                current = nextNode;
                deletedNodes++;
                break;
            } else {
                current = current->next;
            }
        }

        token = strtok(NULL, delimiter);
    }

    if (deletedNodes > 0) {
        printf("ChatGTP$ Deleted the concept(s) from the list.\n");
        botResponse("Deleted the concept(s) from the list.\n");
    } else {
        printf("ChatGTP$ No concepts matching the given criteria found in the list.\n");
        botResponse("ChatGTP$ No concepts matching the given criteria found in the list.\n");
    }
}

void search(char str[], gtpList **head) {
    char *concept, *conceptEnd;
    int i, counter = 0;

    for (i = 0; i < size; i++) {
        if (str[i] == '*')
            counter++;
    }


    if (counter != 2) {
        printf("ChatGTP$ You couldn't even use a second * to search for a concept...\n");
        botResponse("You couldn't even use a second * to search for a concept...\n");
        return;
    }

    if(str[0] != '*'){
        concept = strtok(str, "*");
        conceptEnd = strtok(NULL, "*");
    }
    else
        conceptEnd = strtok(str, "*");
    

    gtpList *current = *head;
    while (current != NULL) {
        if (strcmp(conceptEnd, current->concept) == 0) {
            printf("\nChatGTP$ %s\n", current->sentence);
            botResponse(current->sentence);
            current->timesUsed++;
            return;
        } else if (strcmp(conceptEnd, current->concept) < 0) {
            break;
        } else {
            current = current->next;
        }
    }

    current = *head;
    while (current != NULL) {
        if (strstr(current->sentence, conceptEnd)) {
            printf("\nChatGTP$ %s\n", current->sentence);
            botResponse(current->sentence);
            current->timesUsed++;
            return;
        } else {
            current = current->next;
        }
    }

    idk_ans(conceptEnd);
}

void idk_ans(char *concept){
    int i,key;
    char log[100];
    char answers[5][55] = {"I know nothing about `",
                            "` is probably not important for me to know",
                            "I didn't have the time to learn about `",
                            "You think ` is common knowledge?",
                            "Nothing about ` in my database and I have a huge one..."};

    key = rand()%5;
    printf("\nChatGTP$ ");
    for(i=0;i<55;i++){
        if(answers[key][i] == '`'){
            printf("%s", concept);
        }
            
        else{
            printf("%c", answers[key][i]);
            log[i] = answers[key][i];
        }
    }
    botResponse(log);
    dialogue(concept,-1);
    dialogue("\n", -1);
    printf("\n");
}

void read(gtpList **head, char str[]) {

    gtpList *new;
    char *filename;
    int key;

    filename = strrchr(str, '>') + 2;
    if (filename == NULL) {
        printf("ChatGTP$ Invalid command. Please provide a file name.\n");
        botResponse("Invalid command. Please provide a file name.\n");
        return;
    }

    while (*filename == ' ')
        filename++;

    FILE *file = fopen(filename, "r");
    if(!file){
        printf("ChatGTP$ No such file\n");
        botResponse("No such file\n");
        return;
    }

    char line[size];

    while (fgets(line, sizeof(line), file))
    {
        char *concept = strtok(line, ":");
        char *sentence = strtok(NULL, "\n");

        if(concept != NULL && sentence != NULL){
            new = createNode(concept , sentence , 0);
            insertNode(head,new);
            if(known == 1)
                    key = 1;
                else
                    key = 0;
                answers(key,concept,1);
                known = 0;
        }
    }
    fclose(file);
}

void dialogue(char *text, int bot) {

    FILE *file;
    if(counter_file == 0){
        file = fopen("dialogue.txt", "w");
        counter_file++;
    }
    else    
        file = fopen("dialogue.txt", "a");

    if (file == NULL) {
        printf("ChatGTP$ Failed to open dialogue file.\n\n");
        botResponse("Failed to open dialogue file.\n\n");
        return;
    }

    if(bot == 1)
        fprintf(file, "me$ %s\n\n", text);
    else if(bot == -1)
        fprintf(file, "%s", text);
    else if(bot == 0)
        fprintf(file, "ChatGTP$ %s\n\n", text);

    fclose(file);
}

void botResponse(char *response) {
    dialogue(response,0);
}

void free_list(gtpList **head) {
    if (*head == NULL)
        return;
    gtpList* current = *head;
    gtpList* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}
