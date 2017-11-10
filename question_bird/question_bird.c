#include<stdio.h>
#include <stdlib.h>
//使用当前时钟做种子
#include <time.h>

// 问题结构体
typedef struct question
{
	// 第一个数
	int first_num;
	//第二个数
	int second_num;
	// 操作符
	char operation;
} Question;

// 小鸟结构体
typedef struct bird
{
	// 问题
	Question * question;
	// 鸟出现的序号
	int serial_num;
	//鸟的x坐标
	int x_coordinate;
	//鸟的y坐标
	int y_coordinate;
	// 指向后继节点的指针
	struct bird * next;
} Bird;

//创建初始链表
Bird * create_init_bird_linklist();
// 生成指定数目的[不重复]的随机数,值位于start和end之间,包含start和end
void generate_random_num(int random_num[], int amount, int start, int end);
// 计算一个问题的答案并返回
int calculate_answer_of_question(Question * question);
//生成一个Question,对于[/]必须处理为可以正好整除的情况
Question * generate_question();
//生成一只鸟
void generate_bird();
//在链表尾部插入一个小鸟节点
void insert_a_bird_end_of_linklist(Bird * head, Bird * bird);
// 遍历链表输出所有的小鸟题目
void traverse_bird_linklist(Bird * head);
// 从链表中删除指定序号的小鸟,当serial_num == 0时删除最开始的小鸟
void delete_bird_from_linklist(Bird * head, int serial_num);
// 保存游戏进度到文件(把小鸟的链表保存到文件)
void save_bird_linklist_to_file(Bird * head);
// 从游戏存档恢复链表
void create_bird_linklist_from_file(Bird * head);


void test_save();
void test_read();

int main()
{
	return 0;
}

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

/*
void test_read() {
	Bird * head;
	head = create_init_bird_linklist();
	create_bird_linklist_from_file(head);
	traverse_bird_linklist(head);
}

void test_save() {
	Bird * head;
	head = create_init_bird_linklist();
	// 生成小鸟(头节点serial_num的值保存最后一只小鸟的序号)
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	generate_bird(head);
	traverse_bird_linklist(head);
	save_bird_linklist_to_file(head);
}
*/

/**
 * [创建初始链表]
 * @return [链表头指针]
 */
Bird * create_init_bird_linklist()
{
	Bird * head = (Bird *)malloc(sizeof(Bird));
	// 头结点序号为0
	head->serial_num = 0;
	head->question = NULL;
	head->x_coordinate = -1;
	head->y_coordinate = -1;
	head->next = NULL;
	return head;
}

/**
 * [从游戏存档恢复链表]
 * @param head [链表头指针]
 */
void create_bird_linklist_from_file(Bird * head)
{
	FILE * fp;
	Bird * pbird;
	Question * question;
	// 以只读方式打开存档文件
	fp = fopen("result.txt", "rt");
	int i = 0;
	while (!feof(fp)) {
		question = (Question *)malloc(sizeof(Question));
		pbird = (Bird *)malloc(sizeof(Bird));
		pbird->question = question;
		pbird->next = NULL;
		fscanf(fp, "%4d%4c%4d%4d%4d%4d", &pbird->question->first_num, &pbird->question->operation,
		       &pbird->question->second_num, &pbird->serial_num, &pbird->x_coordinate, &pbird->y_coordinate);

		// 此处由于是写入时存在换行,所以读取时会读取最后一行空行,空行的serial_num为0,可以过滤掉
		if (pbird->serial_num != 0)
		{
			// 尾插法创建链表
			insert_a_bird_end_of_linklist(head, pbird);
		}
	}
	// 关闭文件
	fclose(fp);
}

/**
 * [保存游戏进度到文件(把小鸟的链表保存到文件)]
 * @param head [链表头指针]
 */
void save_bird_linklist_to_file(Bird * head)
{
	Bird * pbird;
	FILE * fp;
	pbird = head->next;
	if (pbird != NULL)
	{
		//开始保存到文件
		//以写的方式打开文件,文件不存在则创建
		fp = fopen("result.txt", "w+");

		while (pbird != NULL) {
			fprintf(fp, "%4d%4c%4d%4d%4d%4d\n", pbird->question->first_num, pbird->question->operation,
			        pbird->question->second_num, pbird->serial_num, pbird->x_coordinate, pbird->y_coordinate);
			pbird = pbird->next;
		}
		//关闭文件
		fclose(fp);
	}
}

/**
 * [在链表尾部插入一个小鸟节点]
 * @param head [链表头节点]
 * @param bird [要插入的小鸟节点]
 */
void insert_a_bird_end_of_linklist(Bird * head, Bird * bird)
{
	Bird * pbird;
	pbird = head;
	// 遍历找到最后一个节点
	while (pbird->next != NULL) {
		pbird = pbird->next;
	}
	// 将生成的小鸟加到链表的最后面
	pbird->next = bird;
}

/**
 * [遍历链表输出所有的小鸟题目]
 * @param head [链表头指针]
 */
void traverse_bird_linklist(Bird * head)
{
	Bird * pbird;
	// 头指针不输出
	pbird = head->next;
	while (pbird != NULL) {
		printf("序号：%d,  坐标(%d, %d),  问题： %d  %c  %d \n", pbird->serial_num, pbird->x_coordinate, pbird->y_coordinate,
		       pbird->question->first_num, pbird->question->operation, pbird->question->second_num);
		pbird = pbird->next;
	}
	printf("\n");
}

/**
 * [从链表中删除指定序号的小鸟,当serial_num == 0时删除最开始的小鸟]
 * @param head       [链表头指针]
 * @param serial_num [小鸟序号]
 */
void delete_bird_from_linklist(Bird * head, int serial_num)
{
	Bird * pbird;
	Bird * qbird;

	pbird = head;
	if (pbird->next != NULL) {
		if (serial_num == 0) {
			//删除最先产生的小鸟
			pbird = pbird->next;
			head->next = pbird->next;
			// 释放删除的节点
			free(pbird);
		} else {
			// 删除指定序号的小鸟
			for (pbird = pbird->next; pbird; pbird = pbird->next)
			{
				qbird = pbird->next;
				if (qbird->serial_num == serial_num)
				{
					// 删除节点
					pbird->next = qbird->next;
					//释放该节点
					free(qbird);
					break;
				}
			}
		}
	}
}

/**
 * [生成一个Question,对于[/]必须处理为可以正好整除的情况]
 * @return          [生成的问题]
 */
Question * generate_question()
{
	Question * question;
	question = (Question *)malloc(sizeof(Question));

	// 运算操作符
	char operation_symbol[] = {'+', '-', '*', '/'};
	//两个随机操作数
	int random_operation_num[2];
	// 获得一个随机的操作符下标()
	int random_operation_symbol_index[1];
	generate_random_num(random_operation_symbol_index, 1, 1, 3);
	// 获得随机的操作符
	char operation = operation_symbol[random_operation_symbol_index[0]];
	// 生成1-9之间的随机操作数(包括1-9)
	generate_random_num(random_operation_num, 2, 1, 9);

	// 通过比较在下标为0的位置放置较大的第一个操作数,下表为1的位置放置较小的操作数
	if (random_operation_num[0] < random_operation_num[1])
	{
		//不使用第三方变量交换两个数
		random_operation_num[0] = random_operation_num[0] + random_operation_num[1];
		random_operation_num[1] = random_operation_num[0] - random_operation_num[1];
		random_operation_num[0] = random_operation_num[0] - random_operation_num[1];
	}

	// 如果操作符是[/]必须要将其处理为可以整除
	if (operation == '/')
	{
		int res = random_operation_num[0] / random_operation_num[1];
		if (random_operation_num[0] / random_operation_num[1] != random_operation_num[0] / random_operation_num[1] * random_operation_num[1])
		{
			// 这样处理之后就可以实现整除
			random_operation_num[0] = random_operation_num[0] / random_operation_num[1] * random_operation_num[1];
		}
	}
	// 生成题目
	question->first_num = random_operation_num[0];
	question->second_num = random_operation_num[1];
	question->operation = operation;
	return question;
}

/**
 * [生成一只小鸟]
 * @return [生成的小鸟题目]
 */
void generate_bird(Bird * head)
{
	Question * question;
	Bird * bird;

	bird = (Bird *)malloc(sizeof(Bird));

	// 生成一个问题
	question = generate_question();
	bird->serial_num = head->serial_num + 1;
	bird->question = question;
	bird->x_coordinate = 0;
	bird->y_coordinate = 0;
	bird->next = NULL;

	// 头节点的序号保存的是最后一只小鸟的序号
	head->serial_num++;

	// 将生成的小鸟插入链表尾部
	insert_a_bird_end_of_linklist(head, bird);
}

/**
 * [计算一个问题的答案并返回]
 * @param  question [需要计算的问题的结构体]
 * @return          [计算结果]
 */
int calculate_answer_of_question(Question * question)
{
	//操作符(+ - * /)
	char operation_symbol = question->operation;
	// 第一个操作数
	int first_num = question->first_num;
	// 第二个操作数
	int second_num = question->second_num;
	// 计算结果
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
 * [生成指定数目的不重复的随机数,值位于start和end之间,包含start和end]
 * @param random_num [随机数答案数组]
 * @param amount        [需要生成的随机数的数目]
 * @param start         [开始范围]
 * @param end           [结束范围]
 */
void generate_random_num(int random_num[], int amount, int start, int end)
{
	//初始化随机数
	srand((unsigned)time(NULL));
	int i, j;
	for (i = 0; i < amount; i++)
	{
		random_num[i] = rand() % (end - start + 1) + start;
		// 通过内层比较处理重复值
		for (int j = 0; j < i; j++)
		{
			if (random_num[i] == random_num[j])
			{
				i--;
			}
		}
	}
}