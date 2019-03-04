#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// expected file encoding Windows-1527

char* getWord(int length, int finishIndex, char* fileInMemory){
	int startIndex = finishIndex - length;
	char *resultString = (char*) malloc(length);
	strncpy(resultString, &fileInMemory[startIndex], length);
}

int skipLetter(unsigned char c, int i, char* fileInMemoryi){
	int values[256] = {0};
	values[99] = 76378;
	values[100] = 76795;
	values[101] = 93362;
	values[102] = 132703;
	values[103] = 146007;
	values[104] = 155814;
	values[105] = 201548;
	values[106] = 226826;
	values[107] = 253820;
	values[108] = 447388;
	values[109] = 523638;
	values[110] = 608982;
	values[111] = 647357;
	values[112] = 664688;
	values[113] = 803674;
	values[114] = 803681;
	values[115] = 872794;
	values[116] = 985633;
	values[117] = 1097214;
	values[118] = 1109827;
	values[119] = 1203761;
	values[120] = 1203784;
	values[121] = 1203836;
	values[122] = 1203865;
	values[208] = 1204230;
	values[228] = 1204239;
	values[240] = 1208972;
	values[245] = 1210771;
	values[246] = 1225554;
	values[252] = 1227032;
	values[254] = 1246250;
	if(c == 122){
		c=208;
	} else if(c==208){
		c=228;
	} else if(c==228){
		c=240;
	} else if(c==240){
		c=245;
	} else if(c == 246){
		c=252;
	} else if(c==252){
		c=254;
	} else if(c >= 254){
		return -1;
	} else {
		c++;
	}
	int x = values[c];
	if(x > 0){
		return x;
	} else {
		return 0;
	}
}

int main(int argc, char* argv[]) {
	clock_t begin = clock();

	if(argc < 3){
		printf("Programmi käivitamiseks tuleb anda vähemalt 2 argumenti");
		return -1;
	}

	// That should be enough
	char resultString[2048];
	int resultPointer;

	// TODO
	int firstLetters[256] = {0};
	int inputWord[256] = {0};
	int inputWordLength = 0;
	for(int i = 0; argv[2][i] != '\0'; i++){
		inputWord[argv[2][i]]++;
		inputWordLength++;
	}
	
	char *firstParam = argv[0];

	int fd = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);
	struct stat sb;
	if (fstat(fd,&sb) == -1) {
		perror("couldn't get file size.\n");
	}

	char *fileInMemory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	bool nextWord = false;
	int currentWordLength = 0;
	int currentWord[256] = {0};
	for (int i=0; i < sb.st_size; i++){
		unsigned int charKey = (unsigned char)fileInMemory[i];
		if(charKey == 13){
			if(nextWord == false && inputWordLength == currentWordLength){
				// TODO validate if correct				
				char *word = getWord(inputWordLength, i , fileInMemory);
				strcat(resultString, ", ");
				strcat(resultString,  word);
			}
			i++;
			nextWord = false;
			if(currentWordLength>0){
				currentWordLength = 0;
			}
		} else if(nextWord){
			continue;
		} else if(inputWord[charKey] > 0){
			currentWordLength++;
		} else {
			if(currentWordLength == 0 && inputWord[charKey] == 0){
				int tmp = skipLetter(charKey, i, fileInMemory);
					//printf("try: %d %d %d\n", i, tmp, charKey);
				if(tmp > i){
					//printf("jump: %d %d\n", i, tmp);
					i = tmp;
				} else if(tmp == -1){
					break;
				}
			}
			nextWord = true;
		}
	}
	munmap(fileInMemory, sb.st_size);
	close(fd);

	clock_t end = clock();
	double timeSpent = (double)(end - begin) / CLOCKS_PER_SEC * 1000000;
	printf("%d%s", (int)timeSpent, resultString);	
	
	return 0;
}

