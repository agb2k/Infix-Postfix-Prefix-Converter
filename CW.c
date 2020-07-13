#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#define MAX 30

typedef struct Operators{ //operators stack
    char data;
    struct Operators *nextNode;
}Operators;

typedef struct Operands{ //operands stack
    char data[MAX];
    struct Operands *nextNode;
}Operands;

typedef struct Evaluation{ //operand evaluation stack
    int value;
    struct Evaluation *nextNode;
}Evaluation;

Operators *operators_top = NULL; 
Operands *operands_top = NULL;
Evaluation *evaluation_top = NULL;
FILE *defaultPtr, *inPtr, *outPtr; 

// function prototypes
void push(char in[], int code);
void pop(int code);
int evaluate(char in[]);
int priority(char in);
char *infixTOpostfix(char input[], const int last_element);
char *infixTOprefix(char input[], const int last_element);
char *postfixTOinfix(char input[], const int last_element);
char *prefixTOinfix(char input[], const int last_element);

int main(){
    char input[MAX]={'\0'}, temp[MAX]={'\0'}, input_new[5]={'\0'}, input_final[MAX], *out=NULL, typeExp, output[MAX]={'\0'}, sel;
    int type=0, i, opening=0, closing=0, j, counter;
    printf("Enter an expression: ");
    fgets(input, MAX, stdin); //Gets input
    outPtr = fopen("output.txt", "w"); //Opens text file
    for (i=0, j=0; i<=strlen(input); i++, j++){ //Goes through entire input and copies every character that's not a bracket or a null character into input_new
        if (input[i]!='(' && input[i]!=')' && input[i]!='\0'){
            input_new[j]=input[i];
        }
    }
    for (counter=0, j=0, i=0; i<=strlen(input_new); i++, j++){ //Change all operators to 'o' and operands to 'n'
        if (input_new[i]=='/'||input_new[i]=='*'||input_new[i]=='^') //Looks for operators in strings
            input_final[j] = 'o'; //replaces operators with 'o'
        else if (input_new[i]=='-'){ //Looks for '-' character
            if (isalnum(input_new[i+1])){//Checks if next character is a number or alphabet
                if (isdigit(input_new[i+2])){//Checks if next character is a digit
                    i+=2; //Adds 2 to i since we already went through 2 digits
                }
                else{
                    i++; //Adds 1 to i
                }
                input_final[j] = 'n'; //if there's no space between '+' and the number, it's a negative number so it's replaced with an 'n'
            }
            else{
                input_final[j] = 'o';//if first character is '-' and 2nd isn't a digit or number, the '-' symbol is an operator so it's replaced with an 'o'
            }
        }
        else if (input_new[i]=='+'){ //Looks for '+' character
            if (isalnum(input_new[i+1])){//Checks if next character is a number or alphabet
                if (isdigit(input_new[i+2])){//Checks if next character is a digit
                    i+=2; //Adds 2 to i since we already went through 2 characters
                }
                else{
                    i++; //Adds 1 to i
                }
                input_final[j] = 'n'; //if there's no space between '+' and the number, it's a positive number so it's replaced with an 'n'
            }
            else{
                input_final[j] = 'o';//if first character is '+' and 2nd isn't a digit or number, the '+' symbol is an operator so it's replaced with an 'o'
            }
        }
        else {//if it's not one of the operator symbols, it's either a operand or space
            if (isalnum(input_new[i])){//if it is an alphabet or number, it's a operand so it's replaced with an n
                input_final[j] = 'n';
            }
            else if (isspace(input_new[i])){//if isspace(input_new[i]) is true, it's a space so it's replaced with a ' '
                input_final[j] = ' ';
            }
        }
        counter++; // increment character counter by 1
    }

    if (input_final[0]=='n'){ //Detects the type of expression. 1, 2 and 3 are infix prefix and postfix respectively
        if (input_final[1]=='n'){
            if (input_final[2]==' '){
                if(input_final[3]=='n'){
                    type = 3;
                }
                else{
                    type = 1;
                }
            }
            else{
                type = 1;
            }
        }
        else if (input_final[1]==' '){
            if(input_final[2]=='n'){
                type = 3;
            }
            else{
                type = 1;
            }
        }
        else{
            type = 1;
        }
    }
    else if (input_final[0]=='o'){
        if (input_final[1]=='n'){
            type = 1;
        }
        else{
            type = 2;
        }
    }

    for (i=0; input[i]!='\0'; i++){ //counts both opening and closing parenthesis
        if(input[i] == '('){
            opening++;
        }
        if(input[i] == ')'){
            closing++;
        }
    }
    if (opening!=closing){//Sends error message due to imbalanced parentheses
        printf("Imbalanced parentheses\n");
        return 0;
    }

    printf("A. Convert\nB. Evaluate\nC. Exit\nInput: ");
    scanf(" %c", &sel);
    switch (sel){
        case 'A' :// conversion
            switch (type){
                case 1 : //infix
                    printf("\nThis is an infix expression.\nDo you want to convert it to A. Postfix or B. Prefix:\n");
                    scanf(" %c", &typeExp);
                    if (typeExp == 'A'){ // infix expression to postfix
                        strcpy(output, infixTOpostfix(input, strlen(input))); // convert and put result in output[]
                        printf("\nPostfix expression: %s\n", output); // output postfix expression
                    }
                    else if (typeExp == 'B'){ // infix expression to prefix
                        strcpy(output, infixTOprefix(input, strlen(input))); // convert and put result in output[]
                        printf("\nPrefix expression: %s\n", output); // output prefix expression
                    }
                    fprintf(outPtr, "%s\n", output); // save output[] into output.txt
                    fclose(outPtr); // close file using its pointer
                    break;
                case 2 : //prefix
                    printf("\nThis is a prefix expression.\nDo you want to convert it to A. Infix or B. Postfix:\n");
                    scanf(" %c", &typeExp);
                    if (typeExp == 'A'){ // prefix expression to infix
                        strcpy(output, prefixTOinfix(input, strlen(input)));
                        printf("\nInfix expression: %s\n", output);
                    }
                    else if (typeExp == 'B'){ // prefix to postfix
                        strcpy(temp, prefixTOinfix(input, strlen(input)));
                        strcpy(output, infixTOpostfix(temp, strlen(temp)));
                        printf("\nPostfix expression: %s\n", output);
                    }
                    fprintf(outPtr, "%s\n", output); // save output to text file
                    fclose(outPtr);
                    break;
                case 3 : // if expression is postfix
                    printf("\nThis is a postfix expression.\nDo you want to convert it to A. Infix or B. Prefix:\n\n");
                    scanf(" %c", &typeExp);
                    if (typeExp == 'A'){ // postfix to infix
                        strcpy(output, postfixTOinfix(input, strlen(input)));
                        printf("\nInfix expression: %s\n", output);
                    }
                    else if (typeExp == 'B'){ // postfix to prefix
                        strcpy(temp, postfixTOinfix(input, strlen(input))); // converts postfix to infix and copies to temp
                        strcpy(output, infixTOprefix(temp, strlen(temp)));// converts infix to prefix and copies to output
                        printf("\nPrefix expression: %s\n", output);
                    }
                    fprintf(outPtr, "%s\n", output); // save output to text file
                    fclose(outPtr);
                    break;
            }
            break;
        case 'B': //Evaluate
            switch (type){ // inner switch statement
                case 1 : //infix
                    out = infixTOpostfix(input, strlen(input)); // converts infix to postfix
                    strcpy(temp, out);
                    sprintf(output, "Answer: %d", evaluate(temp));
                    printf("\n%s\n", output); // output the result
                    break; 
                case 2 : //prefix
                    out = prefixTOinfix(input, strlen(input)); // prefix expression to infix
                    strcpy(temp, out); 
                    out = infixTOpostfix(temp, strlen(temp)); // infix expression to postfix
                    sprintf(output, "Answer: %d", evaluate(out)); 
                    printf("\n%s\n", output); // output the result
                    break; 
                case 3 : // postfix
                    sprintf(output, "Answer: %d", evaluate(input));
                    printf("\n%s\n", output); // output the result
                    break;
            }
            fprintf(outPtr, "%s\n", output); //save to text file
            fclose(outPtr);
        case 'C' : //exit
            printf("Goodbye!...");
            return 0; //terminates porgramme
    }
    return 0;
}

char *infixTOpostfix(char input[], const int last_element){
    static char outArray[MAX]={'\0'};
    char temp[2];
    int i, j;

    for (i=0, j=0; i<=last_element; i++){
        if (isalpha(input[i])){ // if character is an alphabet push into outArray
            outArray[j++]=input[i];
        }
        else if (isdigit(input[i])){ // if character is an integer
            if (isdigit(input[i+1])){ // and if 2nd character is an integer
                sprintf(outArray + j, "%c%c ", input[i], input[i+1]);
                j+=3;
                i++;
            }
            else {// 1 digit number
                sprintf(outArray + j, "%c ", input[i]);
                j+=2;
            }
        }
        else if (input[i]=='('){ // if character is '('
            strcpy(temp, "(\0");
            push(temp, 1); // push into stack
        }
        else if (input[i]==')'){ // if character is ')'
            while (operators_top!=NULL && (operators_top->data)!='('){ // pop from the top until it's no longer null and not a '('
                sprintf(outArray + j, "%c ", operators_top->data);
                pop(1); //pop top of the stack
                j+=2;
            }
            pop(1); // pop top of stack
        }
        else if (input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'||input[i]=='^'){ // if character is an operator
            if (input[i]=='-'){ // if character is a minus sign
                if (isdigit(input[i+1])){ // check if the number is a negative number
                    if (isdigit(input[i+2])){ //Check if 1st and 2nd characters are digits
                        sprintf(outArray + j, "%c%c%c ", input[i], input[i+1], input[i+2]);
                        j+=4;
                        i+=2;
                    }
                    else { // 1 digit
                        sprintf(outArray + j, "%c%c ", input[i], input[i+1]);
                        j+=3;
                        i++;
                    }
                    continue;
                }
            }
            while (operators_top!=NULL && priority(operators_top->data) >= priority(input[i]) && (operators_top->data)!='('){
                sprintf(outArray + j, "%c ", operators_top->data);
                pop(1); // pop the top of the stack
                j+=2;
            }
            temp[0] = input[i];
            temp[1] = '\0';
            push(temp, 1); // operator pushed into its stack
        }
    }

    while (operators_top!=NULL){  // while operators_top isn't null pop the top of the stack
        sprintf(outArray + j, "%c ", operators_top->data);
        pop(1);
    }

    return outArray;// returns the postfix expression
}

char *infixTOprefix(char input[], const int last_element){
    static char outArray[MAX]={'\0'};
    char temp[2];
    int i, j;

    strrev(input);// reverses input

    for (i=0, j=0; i<=last_element; i++){ // looks for alphabets
        if (isalpha(input[i])){
            outArray[j++]=input[i];
        }
        else if (isdigit(input[i])){ //looks for -ve numbers
            if (isdigit(input[i+1])){
                if (input[i+2]=='-'){
                    sprintf(outArray + j, "%c%c%c ", input[i], input[i+1], input[i+2]);
                    j+=4;
                    i+=2;
                    continue
                }
                //for + ve double digits
                sprintf(outArray + j, "%c%c ", input[i], input[i+1]);
                j+=3;
                i++;
            }
            else { // positive single-digit
                sprintf(outArray + j, "%c ", input[i]);
                j+=2; 
            } 
        }
        else if (input[i] == ')'){ 
            strcpy(temp, ")\0"); 
            push(temp, 1);
        }
        else if (input[i]=='('){
            while (operators_top!=NULL && (operators_top->data)!=')'){ 
                sprintf(outArray + j, "%c ", operators_top->data);
                pop(1); 
                j+=2; 
            }
            pop(1); // pop the closing parenthesis
        }
        else if (input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'||input[i]=='^'){ // if character is an operator
            while (operators_top!=NULL && priority(operators_top->data) >= priority(input[i]) && (operators_top->data)!=')'){
                sprintf(outArray + j, "%c ", operators_top->data);
                pop(1); 
                j+=2; 
            }
            temp[0]=input[i]; // convert operator into a string
            temp[1]='\0';
            push(temp, 1); // push operator into operators stack
        } 
    } 

    while (operators_top!=NULL){ 
        sprintf(outArray + j, "%c ", operators_top->data);
        pop(1);
        j+=2; 
    }

    return strrev(outArray);
}

char *postfixTOinfix(char input[], const int last_element){
    static char outArray[MAX]={'\0'};
    char operand1[MAX]={'\0'}, operand2[MAX]={'\0'}, temp[MAX]={'\0'};
    int i, operand_count=0;

    for (i=0; i<=last_element; i++){ // checks if operand is a letter
        if (isalpha(input[i])){
            sprintf(temp, "%c \0", input[i]);
            push(temp, 2);
            operand_count++;
        }
        else if (isdigit(input[i])){
            if (isdigit(input[i+1])){
                sprintf(temp, "%c%c \0", input[i], input[i+1]); // converts 2 digits to act as 1 numbber
                i++;
            }
            else { // single-digit
                sprintf(temp, "%c \0", input[i]);
            }
            push(temp, 2);
            operand_count++;
        }
        else if (input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'||input[i]=='^'){ // if character is an operator
            if (input[i]=='-'){ // checks for -ve
                if (isdigit(input[i+1])){
                    if (isdigit(input[i+2])){ // negative double digits
                        sprintf(temp, "%c%c%c \0", input[i], input[i+1], input[i+2]);
                        i+=2;
                    }
                    else { // negative single-digit
                        sprintf(temp, "%c%c \0", input[i], input[i+1]);
                        i++
                    }
                    push(temp, 2);
                    operand_count++;
                    continue;
                }
            }
            if (operand_count>=2){ // if there are at least 2 operands next to each other
                strcpy(operand2, operands_top->data);
                pop(2);
                strcpy(operand1, operands_top->data);
                pop(2);
                sprintf(temp, "( %s%c %s) ", operand1, input[i], operand2);
                push(temp, 2);
                operand_count--;
            }
        }
    }
    while (operands_top!=NULL){ // pop all the operands left in the stack
        strcat(outArray, operands_top->data);
        pop(2);
    }

    return outArray;
}

char *prefixTOinfix(char input[], const int last_element){
    // declare variables
    static char outArray[MAX]={'\0'};
    char operand1[MAX]={'\0'}, operand2[MAX]={'\0'}, temp[MAX]={'\0'};
    int i, operand_count=0;

    strrev(input); // reverse input string

    for (i=0; i<=last_element; i++){ /go through reversed input
        if (isalpha(input[i])){
            sprintf(temp, "%c \0", input[i]);
            push(temp, 2);
            operand_count++;
        }
        else if (isdigit(input[i])){ //character is a digit
            if (isdigit(input[i+1])){
                if (input[i+2]=='-'){ 
                    sprintf(temp, "%c%c%c \0", input[i+2], input[i+1], input[i]);
                    i+=2; 
                }
                else {
                    sprintf(temp, "%c%c \0", input[i+1], input[i]);
                    i++; 
                }
            }
            else {
                sprintf(temp, "%c \0", input[i]); 
            } 
            push(temp, 2); 
            operand_count++;
        }
        else if (input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'||input[i]=='^'){
            if (operand_count>=2){
                strcpy(operand2, operands_top->data);
                pop(2);
                strcpy(operand1, operands_top->data);
                pop(2);
                sprintf(temp, "( %s%c %s) ", operand2, input[i], operand1);
                push(temp, 2);
                operand_count--;
            }
        }
    }
    while (operands_top!=NULL){ 
        strcat(outArray, operands_top->data); 
        pop(2);
    } 

    return outArray;

int evaluate(char in[]){
    char temp[5];
    int op1=0, op2=0, i;

    for (i=0; i<=strlen(in); i++){
        if (isdigit(in[i])){
            if (isdigit(in[i+1])){
                sprintf(temp, "%c%c\0", in[i], in[i+1]);
                push(temp, 3);
                i++;
            }
            else {
                sprintf(temp, "%c\0", in[i]);
                push(temp, 3);
            } 
        }
        else if (in[i]=='+' || in[i]=='-' || in[i]=='*' || in[i]=='/' || in[i]=='^'){
            if (in[i]=='-'){ 
                if (isdigit(in[i+1])){
                    if (isdigit(in[i+2])){ 
                        sprintf(temp, "%c%c%c\0", in[i], in[i+1], in[i+2]); 
                        i+=2;
                    }
                    else{
                        sprintf(temp, "%c%c\0", in[i], in[i+1]); 
                        i+=1; 
                    } 
                    push(temp, 3); 
                    continue;
                } 
            } 
            op1 = evaluation_top->value; 
            pop(3); 
            op2 = evaluation_top->value;
            pop(3);
            switch (in[i]){
                case '+': 
                    sprintf(temp, "%d\0", (op1 + op2));
                    push(temp, 3);
                    break;
                case '-': 
                    sprintf(temp, "%d\0", (op2 - op1));
                    push(temp, 3);
                    break;
                case '*': 
                    sprintf(temp, "%d\0", (op2 * op1));
                    push(temp, 3);
                    break;
                case '/':
                    sprintf(temp, "%d\0", (op2 / op1));
                    push(temp, 3);
                    break;
                case '^': 
                    sprintf(temp, "%d\0", pow(op2,op1));
                    push(temp, 3);
                    break;
            } 
        } 
    } 

    return evaluation_top->value;
} 

void push(char in[], int type){
    Operators *tempOperatorPtr;
    Operands *tempOperandPtr;
    Evaluation *tempEvalPtr;
    int int_val, i, j;
    char input_new[5]={'\0'};

    switch (type){
        case 1: //operators stack
            tempOperatorPtr=(Operators *)malloc(sizeof(Operators));
            tempOperatorPtr->data = in[0];
            if (operators_top==NULL)
                tempOperatorPtr->nextNode = NULL;
            else
                tempOperatorPtr->nextNode = operators_top;
            operators_top = tempOperatorPtr;
            break;
        case 2: //operand stack
            tempOperandPtr=(Operands *)malloc(sizeof(Operands));
            strcpy(tempOperandPtr->data, in);
            if (operands_top==NULL)
                tempOperandPtr->nextNode = NULL;
            else
                tempOperandPtr->nextNode = operands_top;
            operands_top = tempOperandPtr;
            break;
        case 3: //evaluation stack
            for (i=0, j=0; i<=strlen(in); i++){
                if (isdigit(in[i]) || in[i]=='-')
                    input_new[j++] = in[i];
            }
            int_val = atoi(input_new);
            if (evaluation_top==NULL){
                evaluation_top=(Evaluation *)malloc(sizeof(Evaluation));
                evaluation_top->value = int_val;
                evaluation_top->nextNode = NULL;
            }
            else{
                tempEvalPtr = evaluation_top;
                evaluation_top = (Evaluation *)malloc(sizeof(Evaluation));
                evaluation_top->value = int_val;
                evaluation_top->nextNode = tempEvalPtr;
            }
            break;
    }
}

void pop(int type){
    Operators *tempOperatorPtr;
    Operands *tempOperandPtr;
    Evaluation *tempEvalPtr;

    switch (type){
        case 1: //operators stack
            if (operators_top->nextNode == NULL)
                operators_top = NULL;
            else {
                tempOperatorPtr = operators_top;
                operators_top = operators_top->nextNode;
                free(tempOperatorPtr);
            }
            break;
        case 2: //operands stack
            if (operands_top != NULL){
                tempOperandPtr = operands_top;
                operands_top = tempOperandPtr->nextNode;
                free(tempOperandPtr);
            }
            break;
        case 3: // evaluation stack
            if (evaluation_top->nextNode == NULL){
                evaluation_top = NULL;
            }
            else {
                tempEvalPtr = evaluation_top;
                evaluation_top = tempEvalPtr->nextNode;
                free(tempEvalPtr);
            }
            break;
    }
}

int priority(char in){
    switch(in){ // returns values corresponding to the priority i.e higher number for higher priority
    case '(':
        return 1;
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '^':
        return 4;
    }
}
