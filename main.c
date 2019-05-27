#include <stdio.h> //49
#include <stdlib.h>
#include <string.h>
#include <time.h>

int idOfProfile = 1; /*променлива, която определя id-номера на всеки профил,
						така че профилите да са с последователна номерация; съхранява броя профили + 1*/
typedef struct Date
{
	int day;
	int month;
	int year;
}Date;

typedef struct Question /*структура, съдържаща характеристиките на всеки отделен въпрос,
							съответно един профил може да има повече от един въпрос*/
{
	int questLength, firstAnswLength, secondAnswLength,
	thirdAnswLength, fourthAnswLength;
	char *questName;
	float questRating;
	char *firstAnsw;
	char *secondAnsw;
	char *thirdAnsw;
	char *fourthAnsw;
	int firstAnswRating;
	int secondAnswRating;
	int thirdAnswRating;
	int fourthAnswRating;
}Question;

typedef struct Profile //структура, съдържаща характеристиките на всеки отделен профил
{
	int id;
	int nameLength;
	char *name;
	int gender;
	struct Date date;
	char interests[250];
	int status;
	int numOfQuest;
	struct Question *question;
}Profile;

typedef struct Node
{
	struct Profile profile;
	struct Node *next;
}Node;

Node *head = NULL;
Node *curr = NULL;

void createList(Profile profile) //създава списък
{
	Node *ptrNode = (Node*) malloc(sizeof(Node));
	if(ptrNode == NULL)
	{
		perror("1: ");
		exit(1);
	}

	ptrNode->profile = profile;
	ptrNode->next = NULL;

	head = curr = ptrNode;
}

void addToList(Profile profile) /*ще приема като параметър структурата буфер;
								в началото на функцията проверява дали head == NULL
								ако е вярно създава списък с параметър структурата буфер,
								ако пък head != NULL просто добавяме нов елемент в списъка*/
{
	if (head == NULL)
		return createList(profile);

	Node *ptrNode = (Node*) malloc(sizeof(Node));
	if (ptrNode == NULL)
	{
		perror("2: ");
		exit(2);
	}

	ptrNode->profile = profile;
	ptrNode->next = NULL;

	curr->next = ptrNode;
	curr = ptrNode;
}

void menu();
char *addressOfString();
void addNewProfile();
int addQuestions(Profile *profile);
void readFileList();
void writeFileList();
void printList();
void freeList();
void outputRandonProfile();
void deleteProfile();
void changeIdNumbers();

int main()
{
	readFileList();

	menu();

	writeFileList();

	freeList();

	return 0;
}

void menu()
{
	int choise;

	do
	{
		printf("\n\n------------------------------------");
		printf("\nChoose one of the following options:\n");
		printf("\nEnter 1 to add a new person in the database\n");
		printf("Enter 2 to see all people in the database\n");
		printf("Enter 3 to select random person and answer his questions\n");
		printf("Enter 4 to delete a person by id\n");
		printf("Enter 5 for exit\n");
		scanf("%d", &choise);
		fgetc(stdin);

		switch(choise)
		{
			case 1: addNewProfile(); break;
			case 2: printList(); break;
			case 3: outputRandonProfile(); break;
			case 4: deleteProfile(); break;
			case 5: break;
		}
	}
	while(choise != 5);
}

char *addressOfString() //връща адреса на стринг; извиквам я всеки път, когато потребителя трябва да въвежда стринг
{
	int count_ch = 2, i = 0;
	char *string_ch;
	char c;

	string_ch = (char*)malloc(1*sizeof(char));
	if (string_ch == NULL)
	{
		printf("3: ");
		exit(3);
	}

	while ((c = fgetc(stdin)) != '\n')
	{
		string_ch[i] = c;
		string_ch = (char*)realloc(string_ch, count_ch * sizeof(char));
		i++;
		count_ch++;
	}
	string_ch[i] = '\0';

	return string_ch;
}

void readFileList() /*четем файла структура по структура като буфер и
					накрая извиквам функцията за добавяне в списък, в която добавяме по стойност;
					извиквам функцията винаги в началото на програмата и по този начин още преди
					самото меню вече имам създаден списък с отделните профили, ако файлът не е празен*/
{
	int i = 0;
	Profile profile;
	FILE *fp;

	if((fp = fopen("profiles.bin", "rb")) == NULL)
		fp = fopen("profiles.bin", "wb"); //създава файл в случай че файлът не съществува

	while(1)
	{
		i = 0; //понеже се запазва старият резултат след прочитането на броя въпроси на предишна структура!

		if(fread(&(profile.id), sizeof(int), 1, fp) != 1)
		{
			//printf("NO MORE PROFILES!!");
			break;
		}

		fread(&(profile.nameLength), sizeof(int), 1, fp);
		profile.name = (char*) malloc(profile.nameLength * sizeof(char));
		if (profile.name == NULL)
		{
			perror("4: ");
			exit(4);
		}
		fread(profile.name, sizeof(char), profile.nameLength, fp);

		fread(&(profile.gender), sizeof(int), 1, fp);
		fread(&(profile.date), sizeof(Date), 1, fp);
		fread(profile.interests, sizeof(char), 250, fp);
		fread(&(profile.status), sizeof(int), 1, fp);

		fread(&(profile.numOfQuest), sizeof(int), 1, fp);
		profile.question = (Question*) calloc(profile.numOfQuest, sizeof(Question));
		if (profile.question == NULL)
		{
			perror("5: ");
			exit(5);
		}

		while(i < profile.numOfQuest)
		{
			fread(&(profile.question[i].questLength), sizeof(int), 1, fp);
			profile.question[i].questName = (char*) malloc(profile.question[i].questLength * sizeof(char));
			if (profile.question[i].questName == NULL)
			{
				perror("6: ");
				exit(6);
			}
			fread(profile.question[i].questName, sizeof(char), profile.question[i].questLength, fp);

			fread(&(profile.question[i].questRating), sizeof(float), 1, fp);

			fread(&(profile.question[i].firstAnswLength), sizeof(int), 1, fp);
			profile.question[i].firstAnsw = (char*) malloc(profile.question[i].firstAnswLength * sizeof(char));
			if (profile.question[i].firstAnsw == NULL)
			{
				perror("7: ");
				exit(7);
			}
			fread(profile.question[i].firstAnsw, sizeof(char), profile.question[i].firstAnswLength, fp);

			fread(&(profile.question[i].secondAnswLength), sizeof(int), 1, fp);
			profile.question[i].secondAnsw = (char*) malloc(profile.question[i].secondAnswLength * sizeof(char));
			if (profile.question[i].secondAnsw == NULL)
			{
				perror("8: ");
				exit(8);
			}
			fread(profile.question[i].secondAnsw, sizeof(char), profile.question[i].secondAnswLength, fp);

			fread(&(profile.question[i].thirdAnswLength), sizeof(int), 1, fp);
			profile.question[i].thirdAnsw = (char*) malloc(profile.question[i].thirdAnswLength * sizeof(char));
			if (profile.question[i].thirdAnsw == NULL)
			{
				perror("9: ");
				exit(9);
			}
			fread(profile.question[i].thirdAnsw, sizeof(char), profile.question[i].thirdAnswLength, fp);

			fread(&(profile.question[i].fourthAnswLength), sizeof(int), 1, fp);
			profile.question[i].fourthAnsw = (char*) malloc(profile.question[i].fourthAnswLength * sizeof(char));
			if (profile.question[i].fourthAnsw == NULL)
			{
				perror("10: ");
				exit(10);
			}
			fread(profile.question[i].fourthAnsw, sizeof(char), profile.question[i].fourthAnswLength, fp);

			fread(&(profile.question[i].firstAnswRating), sizeof(int), 1, fp);
			fread(&(profile.question[i].secondAnswRating), sizeof(int), 1, fp);
			fread(&(profile.question[i].thirdAnswRating), sizeof(int), 1, fp);
			fread(&(profile.question[i].fourthAnswRating), sizeof(int), 1, fp);

			i++;
		}
		idOfProfile++;

		addToList(profile);
	}
	fclose(fp);
}

int addQuestions(Profile *profile)/*тук подавам по адрес, за да мога на същия адрес(profile)
									да му задам определени въпроси; добавям въпрос\и към дадения профил*/
{
	int counter = 0, size = 1;
	char choise;

	(*profile).question = (Question*) malloc(size * sizeof(Question));
	if ((*profile).question == NULL)
	{
		perror("11: ");
		exit(11);
	}

	while(1)
	{
		printf("\nEnter your question: ");
		profile->question[counter].questName = addressOfString();
		profile->question[counter].questLength = (int) strlen(profile->question[counter].questName) + 1;

		printf("\nHow much is this question important? Enter value between 0 and 1: ");
		scanf("%f", &(profile->question[counter].questRating));
		fgetc(stdin);

		printf("\nEnter first answer of the question: ");
		profile->question[counter].firstAnsw = addressOfString();
		profile->question[counter].firstAnswLength = (int) strlen(profile->question[counter].firstAnsw) + 1;

		printf("\nEnter rating of the first answer; how much is the answer important (enter integer): ");
		scanf("%d", &(profile->question[counter].firstAnswRating));
		fgetc(stdin);

		printf("\nEnter second answer of the question: ");
		profile->question[counter].secondAnsw = addressOfString();
		profile->question[counter].secondAnswLength = (int) strlen(profile->question[counter].secondAnsw) + 1;

		printf("\nEnter rating of the second answer; how much is the answer important (enter integer): ");
		scanf("%d", &(profile->question[counter].secondAnswRating));
		fgetc(stdin);

		printf("\nEnter third answer of the question: ");
		profile->question[counter].thirdAnsw = addressOfString();
		profile->question[counter].thirdAnswLength = (int) strlen(profile->question[counter].thirdAnsw) + 1;

		printf("\nEnter rating of the third answer; how much is the answer important (enter integer): ");
		scanf("%d", &(profile->question[counter].thirdAnswRating));
		fgetc(stdin);

		printf("\nEnter fourth answer of the question: ");
		profile->question[counter].fourthAnsw = addressOfString();
		profile->question[counter].fourthAnswLength = (int) strlen(profile->question[counter].fourthAnsw) + 1;

		printf("\nEnter rating of the fourth answer; how much is the answer important (enter integer): ");
		scanf("%d", &(profile->question[counter].fourthAnswRating));
		fgetc(stdin);

		counter++;

		printf("\nDo you want to add other question? Press 'y' for yes or 'n' for no: ");
		choise = fgetc(stdin);
		fgetc(stdin);

		if (choise == 'n' || choise == 'N')
			break;
		else if (choise == 'y' || choise == 'Y')
		{
			size++;
			profile->question = (Question*) realloc(profile->question, size * sizeof(Question));
			if (profile->question == NULL)
			{
				perror("12: ");
				exit(12);
			}
		}
		else //ако потребителя въведе некоректен отговор; приключва въвеждането на въпроси
		{
			printf("\nYou didn't choose anything!");
			break;
		}
	}
	return counter;
}

void addNewProfile() /*функция за добавяне на нов профил
 	 	 	 	 	 накрая на функцията всеки път извиквам и функцията за добавяне на въпрос\и*/
{

	Profile profile;

	profile.id = idOfProfile; // id == next profile number in the row

	printf("\nEnter your name: ");
	profile.name = addressOfString();
	profile.nameLength = (int) strlen(profile.name) + 1;

	printf("\nEnter your gender. Press '0' for male or '1' for female: ");
	scanf("%d", &(profile.gender));
	fgetc(stdin);

	printf("\nEnter your birth date.");
	printf(" \nDay: ");
	scanf("%d", &(profile.date.day));
	fgetc(stdin);

	printf("Month: ");
	scanf("%d", &(profile.date.month));
	fgetc(stdin);

	printf("Year: ");
	scanf("%d", &(profile.date.year));
	fgetc(stdin);

	printf("\nSay something about your interests(free text up to 250 characters):\n ");
	fgets(profile.interests, 250, stdin);
	profile.interests[strlen(profile.interests) - 1] = '\0';

	printf("\nEnter your status. Press '0' for unbounded, '1' for bound or '2' for married: ");
	scanf("%d", &(profile.status));
	fgetc(stdin);

	profile.numOfQuest = addQuestions(&profile);

	idOfProfile++;

	addToList(profile);
}

void writeFileList() //записвам целият списък във файла; презаписвам файла всеки път преди края на програмата
{
	int counter = 0;
	Node *ptrNode = head;
	FILE *fp;

	if((fp = fopen("profiles.bin", "wb")) == NULL)
	{
		perror("13: ");
		exit(13);
	}

	while (ptrNode != NULL)
	{
		counter = 0;

		fwrite(&(ptrNode->profile.id), sizeof(int), 1, fp);

		fwrite(&(ptrNode->profile.nameLength), sizeof(int), 1, fp);
		fwrite(ptrNode->profile.name, sizeof(char), ptrNode->profile.nameLength, fp);

		fwrite(&(ptrNode->profile.gender), sizeof(int), 1, fp);

		fwrite(&(ptrNode->profile.date.day), sizeof(int), 1, fp);
		fwrite(&(ptrNode->profile.date.month), sizeof(int), 1, fp);
		fwrite(&(ptrNode->profile.date.year), sizeof(int), 1, fp);

		fwrite(ptrNode->profile.interests, sizeof(char), 250, fp);

		fwrite(&(ptrNode->profile.status), sizeof(int), 1, fp);

		fwrite(&(ptrNode->profile.numOfQuest), sizeof(int), 1, fp);

		while(counter < ptrNode->profile.numOfQuest)
		{
			fwrite(&(ptrNode->profile.question[counter].questLength), sizeof(int), 1, fp);
			fwrite(ptrNode->profile.question[counter].questName, sizeof(char), ptrNode->profile.question[counter].questLength, fp);

			fwrite(&(ptrNode->profile.question[counter].questRating), sizeof(int), 1, fp);

			fwrite(&(ptrNode->profile.question[counter].firstAnswLength), sizeof(int), 1, fp);
			fwrite(ptrNode->profile.question[counter].firstAnsw, sizeof(char), ptrNode->profile.question[counter].firstAnswLength, fp);

			fwrite(&(ptrNode->profile.question[counter].secondAnswLength), sizeof(int), 1, fp);
			fwrite(ptrNode->profile.question[counter].secondAnsw, sizeof(char), ptrNode->profile.question[counter].secondAnswLength, fp);

			fwrite(&(ptrNode->profile.question[counter].thirdAnswLength), sizeof(int), 1, fp);
			fwrite(ptrNode->profile.question[counter].thirdAnsw, sizeof(char), ptrNode->profile.question[counter].thirdAnswLength, fp);

			fwrite(&(ptrNode->profile.question[counter].fourthAnswLength), sizeof(int), 1, fp);
			fwrite(ptrNode->profile.question[counter].fourthAnsw, sizeof(char), ptrNode->profile.question[counter].fourthAnswLength, fp);

			fwrite(&(ptrNode->profile.question[counter].firstAnswRating), sizeof(int), 1, fp);
			fwrite(&(ptrNode->profile.question[counter].secondAnswRating), sizeof(int), 1, fp);
			fwrite(&(ptrNode->profile.question[counter].thirdAnswRating), sizeof(int), 1, fp);
			fwrite(&(ptrNode->profile.question[counter].fourthAnswRating), sizeof(int), 1, fp);

			counter++;
		}
		ptrNode = ptrNode->next;
	}
	fclose(fp);
}

void printList() //изкарва списъка(съдържанието на списъка до момента) на екрана
{
	int i = 0;
	Node *ptrNode = head;

	if (head == NULL)
		printf("\nFile is empty!");

	while (ptrNode != NULL)
	{
		i = 0;

		printf("\n\n%d. ", ptrNode->profile.id);
		printf("%s", ptrNode->profile.name);
		printf("\nGender - %d", ptrNode->profile.gender);
		printf("\nBirth date - %d.%d.%d", ptrNode->profile.date.day, ptrNode->profile.date.month, ptrNode->profile.date.year);
		printf("\nInterests - %s", ptrNode->profile.interests);
		printf("\nStatus - %d", ptrNode->profile.status);
		printf("\n\nQuestions: ");
//		printf("\n%d", ptrNode->profile.numOfQuest); //изкарва броя въпроси на съответния профил
		while (i < ptrNode->profile.numOfQuest)
		{
			printf("\n%s - ", ptrNode->profile.question[i].questName);
			printf("%.2f", ptrNode->profile.question[i].questRating);
			printf("\na) %s - ", ptrNode->profile.question[i].firstAnsw);
			printf("%d", ptrNode->profile.question[i].firstAnswRating);
			printf("\nb) %s - ", ptrNode->profile.question[i].secondAnsw);
			printf("%d", ptrNode->profile.question[i].secondAnswRating);
			printf("\nc) %s - ", ptrNode->profile.question[i].thirdAnsw);
			printf("%d", ptrNode->profile.question[i].thirdAnswRating);
			printf("\nd) %s - ", ptrNode->profile.question[i].fourthAnsw);
			printf("%d\n", ptrNode->profile.question[i].fourthAnswRating);

			i++;
		}
		ptrNode = ptrNode->next;
	}
}

void freeList() //освобождавам заделената памет; извиквам функцията веднъж накрая на програмата
{
	int i = 0;
	Node *ptrNode;

	while(head != NULL)
	{
		i = 0;

		ptrNode = head;
		head = head->next;
		while(i < ptrNode->profile.numOfQuest)
		{
			free(ptrNode->profile.question[i].questName);
			free(ptrNode->profile.question[i].firstAnsw);
			free(ptrNode->profile.question[i].secondAnsw);
			free(ptrNode->profile.question[i].thirdAnsw);
			free(ptrNode->profile.question[i].fourthAnsw);

			i++;
		}
		free(ptrNode->profile.question);
		free(ptrNode->profile.name);
		free(ptrNode);
	}
}

void outputRandonProfile() /*избира произволен профил от наличните до момента;
						избира произволно число от всички id-та до момента, които са с последователна номерация*/
{
	int randId, i = 0, result, j, maxResult, temp;
	int ratingBuff[3]; //статичен масив, който временно ще съхранява рейтингите на всеки един от отговорите на даден въпрос
	char choise;
	time_t t;
	Node *ptrNode = head;

	srand((unsigned) time(&t));

	do {
		randId = rand() % idOfProfile; //извежда число от посочената граница, като започва от 0;
	} while(randId == 0);				/*цикълът продължава докъто не се избере число различно от 0,
											понеже не съществува профил с id == 0*/

//	printf("\n%d", randId); //изкарва избраното "произволно" id

	while (ptrNode != NULL)
	{
		if (randId == ptrNode->profile.id)
		{
			i = 0;

			printf("\n\n%d. ", ptrNode->profile.id);
			printf("%s", ptrNode->profile.name);
			printf("\nGender - %d", ptrNode->profile.gender);
			printf("\nBirth date - %d.%d.%d", ptrNode->profile.date.day, ptrNode->profile.date.month, ptrNode->profile.date.year);
			printf("\nInterests - %s", ptrNode->profile.interests);
			printf("\nStatus - %d", ptrNode->profile.status);
			printf("\n\nQuestions: ");
			printf("\nAnswer the question(s) below! \nYou should choose only one of the answers by clicking 'a', 'b', 'c', or 'd'!\n");

			while (i < ptrNode->profile.numOfQuest)
			{
				printf("\n%s", ptrNode->profile.question[i].questName);
				printf("\na) %s", ptrNode->profile.question[i].firstAnsw);
				printf("\nb) %s", ptrNode->profile.question[i].secondAnsw);
				printf("\nc) %s", ptrNode->profile.question[i].thirdAnsw);
				printf("\nd) %s\n", ptrNode->profile.question[i].fourthAnsw);

				choise = fgetc(stdin);
				fgetc(stdin);

				if (choise == 'a')
				{
					result = (ptrNode->profile.question[i].questRating) * (ptrNode->profile.question[i].firstAnswRating);
					printf("Your points from this questions are: %d\n", result);
				}
				else if (choise == 'b')
				{
					result = (ptrNode->profile.question[i].questRating) * (ptrNode->profile.question[i].secondAnswRating);
					printf("Your points from this questions are: %d\n", result);
				}
				else if (choise == 'c')
				{
					result = (ptrNode->profile.question[i].questRating) * (ptrNode->profile.question[i].thirdAnswRating);
					printf("Your points from this questions are: %d\n", result);
				}
				else if (choise == 'd')
				{
					result = (ptrNode->profile.question[i].questRating) * (ptrNode->profile.question[i].fourthAnswRating);
					printf("Your points from this questions are: %d\n", result);
				}
				else
					printf("You didn't choose anything!\n");

				ratingBuff[0] = ptrNode->profile.question[i].firstAnswRating;
				ratingBuff[1] = ptrNode->profile.question[i].secondAnswRating;
				ratingBuff[2] = ptrNode->profile.question[i].thirdAnswRating;
				ratingBuff[3] = ptrNode->profile.question[i].fourthAnswRating;
				temp = ratingBuff[0];

				for (j = 1; j < 4; j++) //намира най-високата стоойност от масива
				{
					if (temp < ratingBuff[j])
						temp = ratingBuff[j];
				}
				maxResult = temp *(ptrNode->profile.question[i].questRating);
				printf("Max points from this question are: %d\n", maxResult);

				i++;
			}
			break;
		}
		ptrNode = ptrNode->next;
	}
}

void deleteProfile() //изтрива профил от списъка
{
	int choise;
	Node *ptrNode = head;
	Node *prev = NULL;
	Node *del = NULL;

	if (head == NULL)
		printf("\nFile is empty!");
	else
	{
		printf("\nEnter id number of profile, which you want to delete");
		do
		{
			printf("\nChoose id between 1 and %d\n", (idOfProfile-1));
			scanf("%d", &choise);
			fgetc(stdin);
		} while((choise < 1) || (choise > (idOfProfile-1)));

		while (ptrNode != NULL)
		{
			if (choise == ptrNode->profile.id)
			{
				del = ptrNode;
				if (del == head) {
					head = head->next;
				}
				break;
			}
			prev = ptrNode;
			ptrNode = ptrNode->next;
		}
		if (prev != NULL)
			prev->next = del->next;

		idOfProfile--;

		free(del);
		del = NULL;

		changeIdNumbers();
	}
}

void changeIdNumbers() /*променям id-тата на всички профили, така че да са последователни;
 	 	 	 	 	 	 извиквам функцията само когато изтривам елемент от списъка,
 	 	 	 	 	 	 понеже само тогава се разбърква номерацията - id-тата вече
 	 	 	 	 	 	 не са с последователна номерация и съответно няма как да се
 	 	 	 	 	 	 извика и функцията за произволен профил*/
{
	int i = 1;
	Node *ptrNode = head;

	while (ptrNode != NULL)
	{
		for (i = 1; i < idOfProfile; i++)
		{
			ptrNode->profile.id = i;
			ptrNode = ptrNode->next;
		}
	}
}
