#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

typedef struct {
	char name[10];
	int position;
	int score;
}Horse;

void gotoxy(int x, int y);
int isInteger(char *str, int len);
void initName(int num, Horse *pHorses);
int bettingHorse(int num, Horse *pHorses);
void showGame(int num, Horse *pHorses);
void startGame(int num, Horse *pHorses);
void bettingResult(int horse, Horse *pHorses);

int main(void)
{
	char strNum[5];
	int horseNum;
	int betHorse;
	Horse *pHorses;
	char choice;

	printf("경마게임\n\n");

	while (1) {
		printf("경주할 말의 숫자(최대 10)를 입력하고 Enter>");
		scanf("%s", strNum);
		while (getchar() != '\n');

		if (isInteger(strNum, strlen(strNum))) {
			horseNum = atoi(strNum);

			if (horseNum > 0 && horseNum <= 10)
				break;
		}
		printf("10 이하의 자연수를 입력해주세요.\n\n");
	}

	pHorses = (Horse *)malloc(sizeof(Horse) * horseNum);
	initName(horseNum, pHorses);
	betHorse = bettingHorse(horseNum, pHorses);
	Sleep(500);

	while (1) {
		showGame(horseNum, pHorses);
		bettingResult(betHorse, pHorses);
		printf("\n\n메뉴를 선택하세요.\n");
		printf("0 게임 종료\t1 재경기\n");

		while (1) {
			printf("메뉴 선택: ");
			scanf("%c", &choice);
			while (getchar() != '\n');

			if (choice == '0') {
				free(pHorses);
				return 0;
			}
			else if (choice == '1') {
				break;
			}
			else
				printf("없는 메뉴입니다. 다시 입력하세요.\n\n");
		}
	}
}

void initName(int num, Horse *pHorses)
{
	int length = 0;
	printf("\n\n%d개 말의 이름(최대 한글 3자)를 입력하고 Enter하세요.\n\n", num);

	for (int i = 0; i < num; i++) {
		while (1) {
			printf("%d번 말 이름 : ", i + 1);
			scanf("%s", pHorses[i].name);
			length = strlen(pHorses[i].name);
			while (getchar() != '\n');

			if (length > 6)
				printf("입력할 수 있는 글자 수를 초과하였습니다. 다시 입력해주세요.\n\n");
			else
				break;
		}

		for (int j = 0; j < length; j++) {
			if ((pHorses[i].name[j] & 0x80) == 0) {
				printf("한글이 아닌 문자가 있습니다. 다시 입력해주세요.\n\n");
				i--;
				break;
			}
		}

		if (i > 0) {
			if (strcmp(pHorses[i].name, pHorses[i - 1].name) == 0) {
				printf("이미 있는 이름입니다. 다른 이름을 입력하세요.\n\n");
				i--;
			}
		}
	}
}

int bettingHorse(int num, Horse *pHorses) {
	char bet[10];
	int horse;
	int op = 0;

	printf("\n\n");
	while (1) {
		printf("어떤 말에 돈을 거시겠어요?(이름을 입력하고 Enter하세요): ");
		scanf("%s", bet);
		while (getchar() != '\n');
		for (int i = 0; i < num; i++) {
			if (strcmp(pHorses[i].name, bet) == 0) {
				horse = i;
				op++;
				break;
			}
		}
		if (op != 0)
			break;
		printf("없는 말입니다. 다시 입력하세요.\n\n");
	}

	return horse;
}

void showGame(int num, Horse *pHorses)
{
	system("cls");
	gotoxy(54, 1);   printf("소요시간:\n\n");
	printf("<start>");
	gotoxy(11, 3);   printf("1");
	gotoxy(20, 3);   printf("10");
	gotoxy(30, 3);   printf("20");
	gotoxy(40, 3);   printf("30");
	gotoxy(50, 3);   printf("40<end>");
	gotoxy(58, 3);   printf("등수");
	gotoxy(63, 3);   printf("시간<초>\n");
	printf("-------------------------------------------------------------------------\n");
	for (int i = 0; i < num; i++) {
		printf("%-9s:>\n\n", pHorses[i].name);
	}

	printf("아무키나 누르면 경주를 시작합니다(단, 한글키는 누르지 마십시오.).\n");
	_getch();
	startGame(num, pHorses);
}

void startGame(int num, Horse *pHorses)
{
	double start, finish, duration;
	int movedHorse, movingDist;
	int grade = 0;
	for (int i = 0; i < num; i++) {
		pHorses[i].position = 1;
		pHorses[i].score = 0;
	}

	srand(time(NULL));
	start = clock();

	while (1) {
		movedHorse = rand() % num;

		if (pHorses[movedHorse].position == 40) {
			continue;
		}
		else if (pHorses[movedHorse].position == 39) {
			movingDist = 1;
		}
		else {
			movingDist = rand() % 2 + 1;
		}

		gotoxy(pHorses[movedHorse].position + 10, movedHorse * 2 + 5);
		printf("   ");

		pHorses[movedHorse].position += movingDist;
		gotoxy(pHorses[movedHorse].position + 10, movedHorse * 2 + 5);
		printf(">%d", pHorses[movedHorse].position);

		finish = clock();
		duration = ((double)(finish - start) / CLOCKS_PER_SEC);
		gotoxy(63, 1);
		printf("%8.3lf초", duration);
		Sleep(200);

		if (pHorses[movedHorse].position == 40) {
			grade++;
			pHorses[movedHorse].score = grade;
			gotoxy(58, movedHorse * 2 + 5);
			printf("%d등%", pHorses[movedHorse].score);
			gotoxy(63, movedHorse * 2 + 5);
			printf("%.3lf", duration);

			if (grade == num)
				break;
		}
	}

	gotoxy(1, 27);
	printf("게임 종료!\n");

}

void bettingResult(int horse, Horse *pHorses) {
	if (pHorses[horse].score == 1) {
		printf("%s이(가) 1등을 하여 돈을 얻으셨습니다!!\n", pHorses[horse].name);
	}
	else {
		printf("%s이(가) %d등을 하여 돈을 잃으셨습니다...\n", pHorses[horse].name, pHorses[horse].score);
	}
}

int isInteger(char *str, int len)
{
	for (int i = 0; i < len - 1; i++) {
		if (!isdigit(str[i]))
			return 0;
	}

	return 1;
}

void gotoxy(int x, int y)
{
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
