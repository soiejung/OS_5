#include <stdio.h>
#include <stdlib.h>

int hit(int frameNum, int *currPage, int value){

	for(int i=0;i<frameNum;i++){
		if(currPage[i] == value)
			return 1;
	}
	return 0;
}

int hitIndex(int frameNum, int *currPage, int value){

	for(int i=0;i<frameNum;i++){
		if(currPage[i] == value)
			return i;
	}
	return -1;
}
int closetIndex(int time, int currentIndex, int *currPage, int value, int direction){//현재 위치의 앞과 뒤에 값이 같은 index 찾기

	int i = currentIndex;
	if(direction == -1){

		for(i--;i>=0;i--){//현재 위치의 앞에서부터 첫번째까지
			if(currPage[i] == value)
				return i;
		}
		return i;
	}
	else{

		for(i++; i<time; i++){//현재 위치의 뒤에서부터 마지막까지
			if(currPage[i] == value){
				return i;
			}
		}
		return i;
	}
}

int MaxMin(int frameNum, int *Index, int type){//index중 max와 min 찾기

	int index =0;

	if(type==1){//제일 나중에 위치한 숫자 찾기

		int i=0;
		int max=Index[0];
		for(i++; i<frameNum; i++){
			if(Index[i] > max){
				max = Index[i];
				index = i;
			}
		}return index;
	}
	else{//제일 가깝게 위치한 숫자 찾기

		int i=0;
		int min=Index[0];
		for(i++;i<frameNum; i++){
			if(Index[i] < min){
				min = Index[i];
				index = i;
			}
		}
		return index;
	}
}

void printArray(int frameNum, int *currPage){
	for(int i=0;i<frameNum;i++){
		if(currPage[i] == -1)
			printf(" 	");
		else
			printf("%d	",currPage[i]);
	}
}

void OPT(int frameNum, int time, int prs[]){

	printf("time\n");
	int currPage[frameNum];
	int pageResult[frameNum];
	int nextIndex[frameNum];
	int faults=0;
	int index = -1;
	int filledBlocks =0;
	for(int i=0;i<frameNum; i++){
		currPage[i] = -1;
	}


	for(int i=0; i<time; i++){

		printf("%d   	    	",i+1);

		if(hit(filledBlocks, currPage, prs[i])){

			printArray(frameNum, currPage);
			printf("\n");
			continue;

		}else{

			if(filledBlocks == frameNum){//frame이 꽉 차 있는 경우

				for(int j=0;j<frameNum; j++){// 현재 frame에 차있는 숫자들이 prs에 위치한 index이후의 index를 배열에 저장
					nextIndex[j] = closetIndex(time, i, prs, currPage[j], 1);
				}

				int farest = MaxMin(frameNum, nextIndex, 1);//index중 가장 오랫동안 사용이 안될 애 찾기
				currPage[farest] = prs[i];
				printArray(frameNum, currPage);

			}
			else{//frame이 꽉 차 있지 않은 경우

				currPage[(++index)%frameNum] = prs[i];
				filledBlocks++;
				printArray(frameNum, currPage);
			}
			printf("F\n");
			faults++;
		}
	}

	printf("Number of page faults : %d times\n", faults);
}




void FIFO(int frameNum, int time, int prs[]){

	printf("time\n");
	int currPage[frameNum];
	int pageResult[frameNum];
	int faults=0;
	int index = -1;

	for(int i=0;i<frameNum; i++){
		currPage[i] = -1;
	}

	for(int i=0; i<time; i++){
		printf("%d   	    	",i+1);
		if(hit(frameNum, currPage, prs[i])){//hit일 때

			printArray(frameNum,currPage);
			printf("\n");
			continue;
		}
		else{//page fault일 때
			currPage[(++index)%frameNum] = prs[i];//가장 먼저 들어온 애 쫓아냄
			faults++;
			printArray(frameNum,currPage);
			printf("F\n");
		}

	}

	printf("Number of page faults : %d times\n",faults);
}


void LRU(int frameNum, int time, int prs[]){

	printf("time\n");
	int currPage[frameNum];
	int pageResult[frameNum];
	int prevIndex[frameNum];
	int faults=0;
	int index = -1;
	int filledBlocks =0;
	for(int i=0;i<frameNum; i++){
		currPage[i] = -1;
	}

	for(int i=0; i<time; i++){

		printf("%d   	    	",i+1);
		if(hit(frameNum, currPage, prs[i])){

			printArray(frameNum, currPage);
			printf("\n");

		}else{

			if(filledBlocks==frameNum){//frame이 꽉 찬 경우

				for(int j=0; j<frameNum; j++){

					prevIndex[j] = closetIndex(time, i, prs, currPage[j], -1);
					//현재 frame에 차있는 숫자들이 prs에 위치한 index이전의 index를 배열에 저장
				}
				int farest = MaxMin(frameNum, prevIndex, -1);//가장 오래된 애 찾기
				currPage[farest] = prs[i];
				printArray(frameNum, currPage);
			}

			else{//frame이 꽉 차지 않은 경우

				currPage[(++index)%frameNum] = prs[i];
				filledBlocks++;
				printArray(frameNum, currPage);
			}
			printf("F\n");
			faults++;
		}
	}
	printf("Number of page faults: %d times\n", faults);
}

void SecondChance(int frameNum, int time, int prs[]){


	printf("time\n");
	int currPage[frameNum];
	int pageResult[frameNum];
	int reference[frameNum];
	int faults=0;
	int index = -1;
	int referIndex = 0;
	int filledBlocks =0;
	for(int i=0;i<frameNum; i++){
		currPage[i] = -1;
		reference[i] = 0;
	}

	for(int i=0; i<time; i++){

		printf("%d   	    	",i+1);

		int h=0;
		if((h=hitIndex(filledBlocks, currPage, prs[i]))!=-1){

			if(reference[h]==0){//hit되면 값 증가
				reference[h]++;
			}
			printArray(frameNum, currPage);
			printf("\n");
			continue;

		}else{

			if(filledBlocks == frameNum){


				if(reference[(referIndex)%frameNum] == 1){

					for(int j=(referIndex%frameNum); j<frameNum; j++){
						if(reference[j] == 1){
							reference[j] = 0;//1을 0으로 바꿔주기
							referIndex++;//0을 만나기 전까지 증가, 모두가 1일때는 FIFO방식으로
						}
					}
				}
				currPage[(referIndex)%frameNum] = prs[i];
				referIndex++;
				printArray(frameNum, currPage);

			}
			else{

				currPage[(++index)%frameNum] = prs[i];
				filledBlocks++;
				printArray(frameNum, currPage);
			}
			printf("F\n");
			faults++;
		}
	}
	printf("Number of page faults : %d times\n", faults);
}





int main(void){

	char file[50];
	printf("입력 파일의 이름을 입력해주세요: ");
	scanf("%s", file);

	FILE *fp;
	if((fp=fopen(file, "r"))==NULL){

		fprintf(stderr, "%s 파일을 열 수 없습니다.\n", file);
		exit(1);
	}

	char buf[60];
	char buffer[1];
	int frameNum;
	int prs[30];
	for(int i=0;i<30;i++){
		prs[i] = -1;
	}
	int time=0;
	fgets(buf,sizeof(buf),fp);
	frameNum = atoi(buf);

	fgets(buf,sizeof(buf),fp);
	for(int i=0; i<30; i++){

		buffer[0] = buf[i*2];
		if(buffer[0] == '\0')
			break;
		else{
			prs[i] = atoi(buffer);
			time++;
		}


	}


	int method;
	printf("1. OPT		2. FIFO		3. LRU		4. Second-Chance\n");
	printf(">> ");
	scanf("%d",&method);

	switch(method){

		case 1:
			printf("\nUsed method : OPT\n");
			printf("page reference string : %s\n", buf);
			printf("        frame   ");
			for(int i=0; i<frameNum; i++){
				printf("%d	",i+1);
			}printf("page fault\n");
			OPT(frameNum, time, prs);

			break;
		case 2:

			printf("\nUsed method : FIFO\n");
			printf("page reference string : %s\n", buf);
			printf("        frame   ");
			for(int i=0; i<frameNum; i++){
				printf("%d	",i+1);
			}printf("page fault\n");
			FIFO(frameNum, time, prs);
			break;
		case 3:

			printf("\nUsed method : LRU\n");
			printf("page reference string : %s\n", buf);
			printf("        frame   ");
			for(int i=0; i<frameNum; i++){
				printf("%d	",i+1);
			}printf("page fault\n");
			LRU(frameNum, time, prs);

			break;
		case 4:
			printf("\nUsed method : Second-Chance\n");
			printf("page reference string : %s\n", buf);
			printf("        frame   ");
			for(int i=0; i<frameNum; i++){
				printf("%d	",i+1);
			}printf("page fault\n");
			SecondChance(frameNum, time, prs);
			break;
	}

	fclose(fp);
	return 0;




}
