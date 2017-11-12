#include<stdio.h>
#include <stdlib.h>
//ʹ�õ�ǰʱ��������
#include <time.h>
//ʹ��easyx
#include<graphics.h>
//ʹ������Sleep()����
#include<windows.h>
#include <conio.h>

//������Ļ��߽��ж���
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

//��ť��Ⱥ͸߶�
#define BUTTON_WIDTH SCREEN_WIDTH / 10
#define BUTTON_HEIGHT SCREEN_HEIGHT / 15
//��ť֮��ļ�϶
#define BUTTON_SPACE SCREEN_HEIGHT / 25


// ����ṹ��
typedef struct question
{
	// ��һ����
	int first_num;
	//�ڶ�����
	int second_num;
	// ������
	char operation;
} Question;

// С��ṹ��
typedef struct bird
{
	// ����
	Question * question;
	// ����ֵ����
	int serial_num;
	//���x����
	int x_coordinate;
	//���y����
	int y_coordinate;
	// ָ���̽ڵ��ָ��
	struct bird * next;
} Bird;

//������ʼ����
Bird * create_init_bird_linklist();
// ����ָ����Ŀ��[���ظ�]�������,ֵλ��start��end֮��,����start��end
void generate_random_num(int random_num[], int amount, int start, int end);
// ����һ������Ĵ𰸲�����
int calculate_answer_of_question(Question * question);
//����һ��Question,����[/]���봦��Ϊ�����������������
Question * generate_question();
//����һֻ��
void generate_bird();
//������β������һ��С��ڵ�
void insert_a_bird_end_of_linklist(Bird * head, Bird * bird);
// ��������������е�С����Ŀ
void traverse_bird_linklist(Bird * head);
// ��������ɾ��ָ����ŵ�С��,��serial_num == 0ʱɾ���ʼ��С��
void delete_bird_from_linklist(Bird * head, int serial_num);
// ������Ϸ���ȵ��ļ�(��С��������浽�ļ�)
void save_bird_linklist_to_file(Bird * head);
// ����Ϸ�浵�ָ�����
void create_bird_linklist_from_file(Bird * head);

void cat_question_str(Question * question, char * ques);

void show_interface();
void change_bird_coordinate(Bird * head);

//���ư�ť����
void draw_button(int left, int top, int right, int bottom, char text[]);
//���ƿ��ư�ť
void draw_control_button();
//�����û���Ϣ
void draw_user_info();
//���Ʊ�ѡ��
void draw_alternative_answer(char alternative_answer[][4]);

void test();
void test_save();
void test_read();

int main()
{    
	show_interface();
	//test_save();
	return 0;
}

//չʾ����
void show_interface()
{
	//����ͼƬ
	IMAGE background_img;
	//С��ͼƬ
	IMAGE bird_img;

	loadimage(&background_img,_T(".\\img\\bg_image.jpg"), SCREEN_WIDTH, SCREEN_HEIGHT);
    loadimage(&bird_img,_T(".\\img\\bird_img.gif"), 80, 30);
    
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);//���ݱ���ͼƬ�ĳ��Ϳ��ʼ�����ڣ�ʹ֮������
	setbkcolor(BLUE);
	putimage(0,0,&background_img);//�ڴ����ϻ��Ʊ���ͼƬ
	
	Bird * head;
	head = create_init_bird_linklist();
	create_bird_linklist_from_file(head);
	while(1){
		Sleep(500);
		putimage(0,0,&background_img);
		change_bird_coordinate(head);
		Bird * pbird;
		// ͷָ�벻���
		pbird = head->next;
		while (pbird != NULL) {
			//printf("��ţ�%d,  ����(%d, %d),  ���⣺ %d  %c  %d \n", pbird->serial_num, pbird->x_coordinate, pbird->y_coordinate,
			//       pbird->question->first_num, pbird->question->operation, pbird->question->second_num);
			char ques[10];
			//ƴ������
			cat_question_str(pbird->question, ques);
			putimage(pbird->x_coordinate,pbird->y_coordinate,&bird_img);
			// ����Ļ�����С��
			RECT r = {pbird->x_coordinate, pbird->y_coordinate, pbird->x_coordinate + 80, pbird->y_coordinate + 30};
			drawtext(ques, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			
			char * text = "��ť";
			//���ư�ť
			draw_control_button();
			draw_user_info();
			char alternative_answer[4][4] = {"1", "8", "12", "32"};
			draw_alternative_answer(alternative_answer);
			pbird = pbird->next;
		}
	}
	// ��������˳�
	_getch();
	closegraph();

}

//�ı�С���x�����y����
void change_bird_coordinate(Bird * head)
{
	//���������������
	int random_distance[100];
	//�����������±�
	int index = 0;
	generate_random_num(random_distance, head->serial_num * 2, -10, 10);

	Bird * pbird;
	// ͷָ�벻����
	pbird = head->next;
	while (pbird != NULL) {
		pbird->x_coordinate += abs(random_distance[index++]);
		pbird->y_coordinate += random_distance[index++];
		
		//���ڳ����С����д���
		if(pbird->x_coordinate >= SCREEN_WIDTH){
			pbird->x_coordinate = 0;
		}
		if(pbird->y_coordinate < 0){
			pbird->y_coordinate += SCREEN_HEIGHT / 6;
		}
		if(pbird->y_coordinate > SCREEN_HEIGHT / 3 * 2){
			pbird->y_coordinate -= SCREEN_HEIGHT / 6;
		}

		pbird = pbird->next;
	}
}

/**
 * [������ʼ����]
 * @return [����ͷָ��]
 */
Bird * create_init_bird_linklist()
{
	Bird * head = (Bird *)malloc(sizeof(Bird));
	// ͷ������Ϊ0
	head->serial_num = 0;
	head->question = NULL;
	head->x_coordinate = -1;
	head->y_coordinate = -1;
	head->next = NULL;
	return head;
}

/**
 * [����Ϸ�浵�ָ�����]
 * @param head [����ͷָ��]
 */
void create_bird_linklist_from_file(Bird * head)
{
	FILE * fp;
	Bird * pbird;
	Question * question;
	//ͷ������
	int serial_num = 0;
	// ��ֻ����ʽ�򿪴浵�ļ�
	fp = fopen("result.txt", "rt");
	int i = 0;
	while (!feof(fp)) {
		question = (Question *)malloc(sizeof(Question));
		pbird = (Bird *)malloc(sizeof(Bird));
		pbird->question = question;
		pbird->next = NULL;
		fscanf(fp, "%4d%4c%4d%4d%4d%4d", &pbird->question->first_num, &pbird->question->operation,
		       &pbird->question->second_num, &pbird->serial_num, &pbird->x_coordinate, &pbird->y_coordinate);

		// �˴�������д��ʱ���ڻ���,���Զ�ȡʱ���ȡ���һ�п���,���е�serial_numΪ0,���Թ��˵�
		if (pbird->serial_num != 0)
		{
			// β�巨��������
			insert_a_bird_end_of_linklist(head, pbird);
			serial_num++;
		}
	}
	//ͷָ����ű�ʾС�������
	head->serial_num = serial_num;
	// �ر��ļ�
	fclose(fp);
}

/**
 * [������Ϸ���ȵ��ļ�(��С��������浽�ļ�)]
 * @param head [����ͷָ��]
 */
void save_bird_linklist_to_file(Bird * head)
{
	Bird * pbird;
	FILE * fp;
	pbird = head->next;
	if (pbird != NULL)
	{
		//��ʼ���浽�ļ�
		//��д�ķ�ʽ���ļ�,�ļ��������򴴽�
		fp = fopen("result.txt", "w+");

		while (pbird != NULL) {
			fprintf(fp, "%4d%4c%4d%4d%4d%4d\n", pbird->question->first_num, pbird->question->operation,
			        pbird->question->second_num, pbird->serial_num, pbird->x_coordinate, pbird->y_coordinate);
			pbird = pbird->next;
		}
		//�ر��ļ�
		fclose(fp);
	}
}

/**
 * [������β������һ��С��ڵ�]
 * @param head [����ͷ�ڵ�]
 * @param bird [Ҫ�����С��ڵ�]
 */
void insert_a_bird_end_of_linklist(Bird * head, Bird * bird)
{
	Bird * pbird;
	pbird = head;
	// �����ҵ����һ���ڵ�
	while (pbird->next != NULL) {
		pbird = pbird->next;
	}
	// �����ɵ�С��ӵ�����������
	pbird->next = bird;
}

/**
 * [��������������е�С����Ŀ]
 * @param head [����ͷָ��]
 */
void traverse_bird_linklist(Bird * head)
{
	Bird * pbird;
	// ͷָ�벻���
	pbird = head->next;
	while (pbird != NULL) {
		printf("��ţ�%d,  ����(%d, %d),  ���⣺ %d  %c  %d \n", pbird->serial_num, pbird->x_coordinate, pbird->y_coordinate,
		       pbird->question->first_num, pbird->question->operation, pbird->question->second_num);
		pbird = pbird->next;
	}
	printf("\n");
}

/**
 * [��������ɾ��ָ����ŵ�С��,��serial_num == 0ʱɾ���ʼ��С��]
 * @param head       [����ͷָ��]
 * @param serial_num [С�����]
 */
void delete_bird_from_linklist(Bird * head, int serial_num)
{
	Bird * pbird;
	Bird * qbird;

	pbird = head;
	if (pbird->next != NULL) {
		if (serial_num == 0) {
			//ɾ�����Ȳ�����С��
			pbird = pbird->next;
			head->next = pbird->next;
			// �ͷ�ɾ���Ľڵ�
			free(pbird);
		} else {
			// ɾ��ָ����ŵ�С��
			for (pbird = pbird->next; pbird; pbird = pbird->next)
			{
				qbird = pbird->next;
				if (qbird->serial_num == serial_num)
				{
					// ɾ���ڵ�
					pbird->next = qbird->next;
					//�ͷŸýڵ�
					free(qbird);
					break;
				}
			}
		}
	}
}

/**
 * [����һ��Question,����[/]���봦��Ϊ�����������������]
 * @return          [���ɵ�����]
 */
Question * generate_question()
{
	Question * question;
	question = (Question *)malloc(sizeof(Question));

	// ���������
	char operation_symbol[] = {'+', '-', '*', '/'};
	//�������������
	int random_operation_num[2];
	// ���һ������Ĳ������±�()
	int random_operation_symbol_index[1];
	generate_random_num(random_operation_symbol_index, 1, 1, 3);
	// �������Ĳ�����
	char operation = operation_symbol[random_operation_symbol_index[0]];
	// ����1-9֮������������(����1-9)
	generate_random_num(random_operation_num, 2, 1, 9);

	// ͨ���Ƚ����±�Ϊ0��λ�÷��ýϴ�ĵ�һ��������,�±�Ϊ1��λ�÷��ý�С�Ĳ�����
	if (random_operation_num[0] < random_operation_num[1])
	{
		//��ʹ�õ�������������������
		random_operation_num[0] = random_operation_num[0] + random_operation_num[1];
		random_operation_num[1] = random_operation_num[0] - random_operation_num[1];
		random_operation_num[0] = random_operation_num[0] - random_operation_num[1];
	}

	// �����������[/]����Ҫ���䴦��Ϊ��������
	if (operation == '/')
	{
		int res = random_operation_num[0] / random_operation_num[1];
		if (random_operation_num[0] / random_operation_num[1] != random_operation_num[0] / random_operation_num[1] * random_operation_num[1])
		{
			// ��������֮��Ϳ���ʵ������
			random_operation_num[0] = random_operation_num[0] / random_operation_num[1] * random_operation_num[1];
		}
	}
	// ������Ŀ
	question->first_num = random_operation_num[0];
	question->second_num = random_operation_num[1];
	question->operation = operation;
	return question;
}

/**
 * [����һֻС��]
 * @return [���ɵ�С����Ŀ]
 */
void generate_bird(Bird * head)
{
	Question * question;
	Bird * bird;

	bird = (Bird *)malloc(sizeof(Bird));

	// ����һ������
	question = generate_question();
	bird->serial_num = head->serial_num + 1;
	bird->question = question;
	bird->x_coordinate = 0;
	bird->y_coordinate = 0;
	bird->next = NULL;

	// ͷ�ڵ����ű���������һֻС������
	head->serial_num++;

	// �����ɵ�С���������β��
	insert_a_bird_end_of_linklist(head, bird);
}

/**
 * [����һ������Ĵ𰸲�����]
 * @param  question [��Ҫ���������Ľṹ��]
 * @return          [������]
 */
int calculate_answer_of_question(Question * question)
{
	//������(+ - * /)
	char operation_symbol = question->operation;
	// ��һ��������
	int first_num = question->first_num;
	// �ڶ���������
	int second_num = question->second_num;
	// ������
	int result;
	if (operation_symbol == '+')
	{
		result = first_num + second_num;
	} else if (operation_symbol == '-') {
		result = first_num - second_num;
	} else if (operation_symbol == '*') {
		result = first_num * second_num;
	} else if (operation_symbol == '/') {
		result = first_num / second_num;
	} else {
		result = -1;
	}
	return result;
}

/**
 * [����ָ����Ŀ�Ĳ��ظ��������,ֵλ��start��end֮��,����start��end]
 * @param random_num [�����������]
 * @param amount        [��Ҫ���ɵ����������Ŀ]
 * @param start         [��ʼ��Χ]
 * @param end           [������Χ]
 */
void generate_random_num(int random_num[], int amount, int start, int end)
{
	//��ʼ�������
	srand((unsigned)time(NULL));
	int i, j;
	for (i = 0; i < amount; i++)
	{
		random_num[i] = rand() % (end - start + 1) + start;
		// ͨ���ڲ�Ƚϴ����ظ�ֵ
		for (j = 0; j < i; j++)
		{
			if (random_num[i] == random_num[j])
			{
				i--;
			}
		}
	}
}

//������ƴ��Ϊ�ַ�����
void cat_question_str(Question * question, char ques[])
{
	ques[0] = question->first_num + 48;
	ques[1] = ' ';
	ques[2] = '/';
	ques[3] = ' ';
	ques[4] = question->second_num + 48;
	ques[5] = '\0';

}

//���ư�ť����
void draw_button(int left, int top, int right, int bottom, char text[])
{
	setfillcolor(BLUE);
	fillrectangle(left, top, right, bottom);
	RECT r = {left, top, right, bottom};
	drawtext(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


//���ƿ��ư�ť
void draw_control_button()
{
	int i;
	//���ư�ť��������(0-4���������ӡ���״̬��ɾ�����˳������밴ť������)
	int control_button_array[5][4] = {
		{BUTTON_SPACE, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE, BUTTON_SPACE + BUTTON_WIDTH, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE + BUTTON_HEIGHT},
		{BUTTON_SPACE * 3 + BUTTON_WIDTH * 2, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE, BUTTON_SPACE * 3 + BUTTON_WIDTH * 2 + BUTTON_WIDTH, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE + BUTTON_HEIGHT},
		{BUTTON_SPACE, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT, BUTTON_SPACE + BUTTON_WIDTH, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT * 2},
		{BUTTON_SPACE * 2 + BUTTON_WIDTH, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT, BUTTON_SPACE * 2 + BUTTON_WIDTH * 2, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT * 2},
		{BUTTON_SPACE * 3 + BUTTON_WIDTH * 2, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT, BUTTON_SPACE * 3 + BUTTON_WIDTH * 3, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT * 2}
	};
	char control_button_text[5][8] = {"����", "��״̬", "ɾ��", "�˳�", "����"};
	//�������Ӱ�ť
	for(i = 0; i < 5; i++){
		draw_button(control_button_array[i][0],control_button_array[i][1],control_button_array[i][2],control_button_array[i][3], control_button_text[i]);	
	}
}

//�����û���Ϣ
void draw_user_info()
{
	setlinecolor(BLUE);
	line(BUTTON_SPACE * 4 + BUTTON_WIDTH * 3, SCREEN_HEIGHT / 4 * 3, BUTTON_SPACE * 4 + BUTTON_WIDTH * 3, SCREEN_HEIGHT);
	line(BUTTON_SPACE * 5 + BUTTON_WIDTH * 4, SCREEN_HEIGHT / 4 * 3, BUTTON_SPACE * 5 + BUTTON_WIDTH * 4, SCREEN_HEIGHT);
	

	RECT r_user = {BUTTON_SPACE * 4 + BUTTON_WIDTH * 3, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE, BUTTON_SPACE * 5 + BUTTON_WIDTH * 4, SCREEN_HEIGHT / 4 * 3 + BUTTON_HEIGHT};
	char * text_user = "�û�:abc";
	drawtext(text_user, &r_user, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	RECT r_score = {BUTTON_SPACE * 4 + BUTTON_WIDTH * 3, SCREEN_HEIGHT / 4 * 3 + BUTTON_SPACE * 2 + BUTTON_HEIGHT, BUTTON_SPACE * 5 + BUTTON_WIDTH * 4, SCREEN_HEIGHT / 4 * 3 + BUTTON_HEIGHT * 2 + BUTTON_SPACE * 2};
	char * text_score = "�÷�:5";
	drawtext(text_score, &r_score, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

//���Ʊ�ѡ��
void draw_alternative_answer(char alternative_answer[][4])
{

	int i = 0;
	for(i = 0; i < 4; i++){
		draw_button(BUTTON_SPACE * (6+i) + BUTTON_WIDTH * (4+i), SCREEN_HEIGHT / 4 * 3 + (BUTTON_SPACE * 3 + BUTTON_HEIGHT) / 2,
			BUTTON_SPACE * (6+i) + BUTTON_WIDTH * (4+i) + BUTTON_WIDTH * 2 / 3, SCREEN_HEIGHT / 4 * 3 + (BUTTON_SPACE * 3 + BUTTON_HEIGHT) / 2 + BUTTON_HEIGHT, alternative_answer[i]);
	}
	
}

void test()
{
	/*
	int random_num[10];
	int i;
	generate_random_num(random_num, 10, 1, 20);
	for (i = 0; i < 10; i++)
	{
		printf("%d\n", random_num[i]);
	}
	*/

	/*
	Question * question;
	question = (Question *)malloc(sizeof(Question));
	question->first_num = 8;
	question->second_num = 2;
	question->operation = '/';
	int res = calculate_answer_of_question(question);
	printf("res = %d\n", res);
	*/

	/*
	Question * question2;
	question2 = generate_question();
	printf("%d  %c  %d\n", question2->first_num, question2->operation, question2->second_num);
	*/

	// test_save();
	// test_read();
	

}

/*
void test_read() {
	Bird * head;
	head = create_init_bird_linklist();
	create_bird_linklist_from_file(head);
	traverse_bird_linklist(head);
}
*/

void test_save() {
	Bird * head;
	head = create_init_bird_linklist();
	// ����С��(ͷ�ڵ�serial_num��ֵ�������һֻС������)
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	traverse_bird_linklist(head);
	save_bird_linklist_to_file(head);
}
