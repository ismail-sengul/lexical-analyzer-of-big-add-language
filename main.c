#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <conio.h>

int countOfChar = 0; //This variable holds the number of characters in the .ba file.
int readingChar = 0; /*This variable helps to control chars*/

bool isInteger(char* intWord){ /*This method checks if the word is a integer.*/
	int lenInt = strlen(intWord);
	int i;
	
	if(lenInt > 100){  /*If length of word is above 100 digit, method return false*/
		return false;
	}
	
	if(lenInt == 1 && !strcmp(intWord,"-")){   /*If length of word is one and it is '-', method return false*/
		return false;
	}
	for (i = 0;i < lenInt ;i++){ /*Checking every digit*/
		if(i == 0){  /*If first char is not '-' or a digit, return false*/
			if(intWord[i] != '-' && !isdigit(intWord[i])){
				return false;
			}
		}
		else if(i > 0){	
			if(!isdigit(intWord[i])){
				return false;
			}
		}
	}
	
	return true;	
}

bool isVariable(char *var){/*This method checks if the word is a variable*/
	int lenVar = strlen(var);
	int i;
	
	if(lenVar>20){ /*If length of word is above 20 char, method return false*/
		return false;
	}
	for(i=0;i < lenVar;i++){ /*Checking every digit*/
		if(i==0 && !isalpha(var[i])){ /*If first char is not alphabethic,method return false*/
			return false;
		}
		if(i>0 && (!isalpha(var[i]) && !isdigit(var[i]) && var[i] != '_')){/*If other digit is not alphabethic or underline,method return false*/
			return false;
		}
	}
	return true;	
}

bool isKeyword(char* word) { //This method checks if the word is a keyword

	if (!strcmp(word, "int") || !strcmp(word, "move")  || !strcmp(word, "add") || !strcmp(word, "sub") || !strcmp(word, "to") || !strcmp(word, "from") ||
		!strcmp(word, "loop") || !strcmp(word, "times") || !strcmp(word, "out") || !strcmp(word, "newline")) {
		return true;
	}
	return false;
}
void writeLxFile(char *word,FILE *lxFile){//This method checks the words or signs
	int lenWord = strlen(word);//length of word holds in a variable
			if(lenWord == 0){//if length of word is zero, moves to next char and it is not write any word in .lx file
				readingChar++;
			}else if(isKeyword(word)){//if word is a keyword
				fprintf(lxFile,"Keyword %s\n",word);//writing this keyword in .lx file
				readingChar++;
			}else if(isVariable(word)){
				fprintf(lxFile,"Identifier %s\n",word);//if word is a variable
				readingChar++;//writing this identifier in .lx file
			}else if(isInteger(word)){//if word is a int constant
				fprintf(lxFile,"IntConstant %s\n",word);//writing this integer in .lx file
				readingChar++;//moves next word
			}else if(!strcmp(word,".")){//if word is '.'
				fprintf(lxFile,"%s\n","EndOfLine");//writing EndOfLine in .lx file
				readingChar++;
			}else if(!strcmp(word,",")){//if word is ','
				fprintf(lxFile,"%s\n","Seperator");//writing Seperator in .lx file
				readingChar++;
			}else if(!strcmp(word,"[")){//if word is '['
				fprintf(lxFile,"%s\n","OpenBlock");//writing OpenBlock in .lx file
				readingChar++;
			}else if(!strcmp(word,"]")){//if word is ']'
				fprintf(lxFile,"%s\n","CloseBlock");//writing CloseBlock in .lx file
				readingChar++;
			}else{//If the word is not recognize in ba language, error prints on the screen(Console)
				printf("Error: %s is unrecognized word...\n",word);
				readingChar++;
			}
}
void lxAnalyze(char *allLines , FILE *lxFile){
	
	char *word = calloc(countOfChar,sizeof(char)); //A dynamic memory is created that holds the word value.
	int wordIndex = 0;//Word index variable is creating for assing characters to string name of word respectively.
	do{
		if(allLines[readingChar] == ' ' || allLines[readingChar] == '\n'){//If program saw a blank (' ') or nextline char ('\n)
			writeLxFile(word,lxFile);//Calling analyzer method and starts the writing process in .lx file
			free(word);//removing a dynamic memory address(Word removes after the analysis of the word is finished)
			char *word = calloc(countOfChar,sizeof(char));//New word variable is creating
			wordIndex = 0;//Word index is set to zero because
		}else if(allLines[readingChar] == '.'){//If program saw a blank ('.')    /*!!THE FOLLOWING OPERATIONS ARE ALL THE SAME AS ABOVE OPERATIONS EXCEPT A FEW OPERATION!!*/
			writeLxFile(word,lxFile);
			fprintf(lxFile,"%s\n","EndOfLine");//Writing EndOfLine in .lx file because the last char is '.'
			free(word);
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 0;
		}else if(allLines[readingChar] == ','){//If program saw a seperator (',')
			writeLxFile(word,lxFile);
			fprintf(lxFile,"%s\n","Seperator");//Writing Seperator in .lx file because the last char is ','
			free(word);
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 0;
		}else if(allLines[readingChar] == '{'){//If program saw starting char of comment ('{')
			writeLxFile(word,lxFile);
			free(word);//removing a dynamic memory address(Word removes after the analysis of the word is finished)
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 0;
			do{
				if(allLines[readingChar] == '}'){//If found '}'
					readingChar++;//next char
					break;//break the loop
				}else if(allLines[readingChar] == '\0'){//If chars of .ba file is over, an error prints on the screen and break the loop
					printf("%s\n","Error: An opened comment line was not closed... (Expected '}') ");
					break;
				}
				readingChar++; //Move next word
			}while(true);//If chars is not over or not found '{', this loop continue to process
		}else if(allLines[readingChar] == '"'){
			writeLxFile(word,lxFile);
			free(word);//removing a dynamic memory address(Word removes after the analysis of the word is finished)
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 1;
			word[0] = '"';//first char of word is should be '"'
			do{
				if(allLines[readingChar] == '"'){//If string constant was closed
					word[wordIndex] = allLines[readingChar];//last digit of word is should be '"'
					fprintf(lxFile,"StringConstant %s\n",word);//Writing in .lx file
					free(word);
					char *word = calloc(countOfChar,sizeof(char));
					wordIndex = 0;
					readingChar++;
					break;//break loop
				}else if(allLines[readingChar] == '\0'){//If string constant was not closed, error prints on the screen
					printf("%s\n","Error: An opened string constant value was not closed... ");
					break;
				}
				word[wordIndex] = allLines[readingChar];//If string constant was not closed, char of string put in word variable
				wordIndex++;//Word index variable increase 1 for creating string correctly
				readingChar++;
			}while(true);//If chars is not over or not found '"', this loop continue to process
		}else if(allLines[readingChar] == '['){//If program saw a block opener ('[')
			writeLxFile(word,lxFile);
			fprintf(lxFile,"%s\n","OpenBlock");//Writing OpenBlock in .lx file because the last char is '['
			free(word);
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 0;
		}else if(allLines[readingChar] == ']'){//If program saw a block closer (']')
			writeLxFile(word,lxFile);
			fprintf(lxFile,"%s\n","CloseBlock");//Writing CloseBlock in .lx file because the last char is ']'
			free(word);
			char *word = calloc(countOfChar,sizeof(char));
			wordIndex = 0;
		}else if(allLines[readingChar] == '\0'){//If chars of .ba file is over, break this loop and cut lexical analyze
			break;
		}else{//If there is not a recognized word or character, reading continues
			word[wordIndex] = allLines[readingChar];//word is creating
			readingChar++;//next word
			wordIndex++;//next index
		}
	}while(allLines[readingChar] != '\0');//If chars of .ba file are not finished, loop continues 
}

int main(int argc, char *argv[]) {
	
	FILE *baFile = fopen("myscript.ba","r");// .ba file called for read
	if(baFile == NULL){//If file did not found, Error message prints on screen
		printf("The file could not opened...");
	}else{
		FILE *lxFile = fopen("myscript.lx","w");// creating .lx file for write words analyze
		
		while(!feof(baFile)){ //characters of .ba file are counted.				
			fgetc(baFile);
			countOfChar++;	
		}
		
		fseek(baFile,0,SEEK_SET);//Returning to head in .ba file
		char *allLines = calloc(countOfChar,sizeof(char));//A dynamic memory adjusts for holding characters of .ba
		countOfChar = 0;//character counter sets to zero.
		
		while(!feof(baFile)){//These characters are thrown into alllines variable.
			allLines[countOfChar] = fgetc(baFile);
			countOfChar++;
		}
		allLines[countOfChar-1] = '\0'; //calloc and malloc dynamic memories has created weird characters in last of the words. That is why last char of the word must be null.
		
		lxAnalyze(allLines,lxFile);//Method called
		printf("\n-----%s-----","Lexical analyzer created...");//The end of the project and creating the .lx file is printed to console.
	}
	
	
	getch();//For console not closed.	
}

