#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <Windows.h>

#define MAX_CARD_SIZE 52
#define cash 500

typedef struct cards {
	char pattern[3];
	int num;
}element;

typedef struct decks {	//�� 
	element data[MAX_CARD_SIZE];
	int top;
}deck;

typedef struct DlistNode {
	element data;
	struct DlistNode* Llist;
	struct DlistNode* Rlist;
}hand_cards;

typedef struct player {
	int coin;
	int hand_count;
	int stop;
	int result;
	int character;
	int playable;
	hand_cards hand;
}player;	//player

typedef struct dealer {
	int hand_count;
	int stop;
	int result;
	int character;
	hand_cards hand;
}deal;	//dealer

//���� ����
void init_stack();
int is_empty();
int is_full();
void push(element card);
element pop();

//���߿��Ḯ��Ʈ
void init(hand_cards* head);
void insert(hand_cards* before, element data);
void delete(hand_cards* head, hand_cards* removed);
void print_hand(hand_cards* head);
element check_hand(hand_cards* head);

int start();		//���� �غ�
void makedeak();	//�� �����
void shakedeak();	//ī�� ���¼��Լ�

void make_player(int player_num);	//�÷��̾� �غ� �Լ�
int player_character();		//player ����
void getCard(int num);	//ī�带 �޴� �Լ�
void player_play(int num);	//�÷��̾� �÷��� �Լ�

void make_dealer();				//���� �غ� �Լ�
int dealer_character();		//dealer ����
void getCard_dealer();	//������ ī�带 �޴� �Լ�
void dealer_play();		//���� �÷��� �Լ�

void play(int player_num);		//���� �÷��� �Լ�
int betting(int u_coin, int player_num);	//�����ϴ� �Լ�
void print_all(int player_num);	//��� player ī�� ���
void showcard(int num);	//ī�� ��� �Լ�
void check_winner(int player_num, int bet); //����� Ȯ��

void reset_all(int player_num);	//��� ����
void empty_deck();			//�� �ʱ�ȭ
void empty_hand(int player_num);	//���� �ʱ�ȭ

void change(char* hit);	//���ڿ� �ҹ��� ��ȯ �Լ�

deck card_deck;
char shape[4][3] = { "��", "��", "��", "��" };	//ī�� ����
element cards[52];	//ī�� ��
player player_arr[4];	//�ִ� �÷��̾� 4��
deal dealer;

int main(void) {
	int player_num = start();
	char select;
	int players;

	init_stack();
	makedeak();

	while (player_arr[0].coin > 0) {
		players = 0;

		shakedeak();
		play(player_num);


		if (player_arr[0].coin == 0) {
			printf("YOU LOSE\n");
		}
		else {
			for (int i = 1; i < player_num; i++) {	//�� �� ������ player ����
				if (player_arr[i].coin == 0) {
					player_arr[i].stop = 1;
					player_arr[i].result = 999;
					player_arr[i].playable = 0;
				}
			}
			for (int i = 1; i < player_num; i++) {		//all-in�� player ���
				if (player_arr[i].playable == 0)
					players++;
			}
			if (players == (player_num - 1)) {	//���� user�� ������ ��� player�� all-in�� �Ǹ� ���� ����
				system("cls");
				printf("        you win the game!");
				exit(0);
			}

			system("cls");
			printf("     PLAYER CASH : $%d\n", player_arr[0].coin);
			printf("\n     ������ ����Ǿ����ϴ�.\n");
			printf("     ������ ����Ͻðڽ��ϱ�?    [Y/N]     \n");
			printf("\n     command : ");
			scanf("%c", &select);
			getchar();

			while (select != 'y' || select == 'Y') {
				if (select == 'n' || select == 'N') {
					exit(0);
				}
				else if (select == 'y' || select == 'Y') {}
				else {
					printf("\n");
					printf("�Է��� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ��� [Y/N].\n");
					printf("\n     command : ");
					scanf("%c", &select);
					getchar();
				}
			}
		}
	}

}

//---------------------���ð��� �Լ�----------------------------
void init_stack() {
	card_deck.top = -1;
}

int is_empty() {
	return (card_deck.top == -1);
}

int is_full() {
	return (card_deck.top == MAX_CARD_SIZE - 1);
}

void push(element card) {
	if (is_full(card_deck)) {
		fprintf(stderr, "���� ��ȭ ����");
		return;
	}
	else {
		card_deck.data[++(card_deck.top)] = card;
	}
}

element pop() {
	if (is_empty(card_deck)) {
		fprintf(stderr, "���� ���� ����");
		return;
	}
	else {
		return card_deck.data[(card_deck.top)--];
	}
}
//--------------------------------------------------

//---------------------����Ʈ���� �Լ�----------------------------
void init(hand_cards* head) {
	head->Llist = head;
	head->Rlist = head;
}

void insert(hand_cards* before, element data) {
	hand_cards* node = (hand_cards*)malloc(sizeof(hand_cards));
	node->data = data;

	node->Rlist = before->Rlist;
	before->Rlist->Llist = node;
	node->Llist = before;
	before->Rlist = node;
}

void delete(hand_cards* head) {
	if (head == NULL)	return;

	hand_cards* p;

	p = head->Llist;
	p->Rlist->Llist = p->Llist;
	p->Llist->Rlist = p->Rlist;

	free(p);
}

//---------------------------------------------------------------------------------------------------------
void print_hand(hand_cards* head) {
	hand_cards* p;
	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("��������������\t");
	}
	printf("��������������\n");

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("��%s   ��\t", p->data.pattern);
	}
	printf("��%s   ��\n", p->data.pattern);

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		showcard(p->data.num);
	}
	showcard(p->data.num);
	printf("\n");

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("��   %s��\t", p->data.pattern);
	}
	printf("��   %s��\n", p->data.pattern);

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("��������������\t");
	}
	printf("��������������\n");
}
//---------------------------------------------------------------------------------------------------------

element check_hand(hand_cards* head) {
	hand_cards* p;
	int max = 0;
	element top_card;

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		if (p->data.num == 1) {
			return p->data;
		}
		else if (p->data.num > max) {
			max = p->data.num;
			top_card = p->data;
		}
	}

	return top_card;
}
//--------------------------------------------------

// �ʱ� ���� ȭ�� �� ���� CPU�� ������ �� ������ ����
int start() {
	int player_num;
	char choice1;
	printf("     ���� ���ӿ� ���� ���� ȯ���մϴ�!     \n");
	printf("            ������ �����Ͻðڽ��ϱ�?         \n");
	printf("                    [Y/N]                   \n ");
	printf("\n     command : ");

	scanf("%c", &choice1);
	getchar();

	while ((choice1 != 'Y') && (choice1 != 'y') && (choice1 != 'N') && (choice1 != 'n'))
	{
		printf("\n");
		printf("�Է��� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ��� [Y/N].\n");
		printf("\n     command : ");
		scanf("%c", &choice1);
		getchar();
	}

	// Y�� N�� ���� ��� (�ùٸ� ���)
	if (choice1 == 'Y' || choice1 == 'y') {
		printf("\n     CPU PLAYER�� �� ������ �Ͻðڽ��ϱ�?      ");
		printf("\n              CPU PLAYER [1/2/3]          \n");
		printf("\n     command : ");
		scanf("%d", &player_num);
		getchar();
		// CPU PLAYER �Է��� (�ùٸ��� ���� ���)
		while (player_num != 3 && player_num != 2 && player_num != 1) {
			printf("\n���� 3, 2, 1 �߿��� �Է����ֽñ� �ٶ��ϴ�.\n");
			printf("\n     command : ");
			scanf("%d", &player_num);
			getchar();
		}
		if (player_num == 3 || player_num == 2 || player_num == 1) {
			printf("\n     CPU PLAYER �ο��� �� %d������ ���õǾ����ϴ�.     ", player_num);
			Sleep(1000);

			player_num++;
			make_player(player_num);
			make_dealer();
		}
	}
	else if (choice1 == 'N' || choice1 == 'n') {
		printf("             ������ �����ϰڽ��ϴ�.             \n");
	}


	return player_num;
}


void showcard(int num) {	//ī�� ���
	if (num == 1) {
		printf("��  A  ��\t");
	}
	else if (num < 10) {
		printf("��  %d  ��\t", num);
	}
	else if (num == 10) {
		printf("�� 10  ��\t");
	}
	else if (num == 11) {
		printf("��  J  ��\t");
	}
	else if (num == 12) {
		printf("��  Q  ��\t");
	}
	else if (num == 13) {
		printf("��  K  ��\t");
	}
	else {
		fprintf(stderr, "ī�� ����");
	}
}

void makedeak() {
	element card;
	for (int i = 0; i < 52; i++) {
		strcpy(card.pattern, shape[i / 13]);
		card.num = (i % 13) + 1;
		cards[i] = card;
	}	// set cards
}

void shakedeak() {
	srand((unsigned)time(NULL));

	for (int i = 0; i < 300; i++) {	//ī�� ����

		int x = rand() % 52;
		int y = rand() % 52;
		element tmp;

		if (x != y) {
			tmp.num = cards[x].num;
			strcpy(tmp.pattern, cards[x].pattern);
			cards[x].num = cards[y].num;
			strcpy(cards[x].pattern, cards[y].pattern);
			cards[y].num = tmp.num;
			strcpy(cards[y].pattern, tmp.pattern);
		}
	}

	for (int i = 0; i < 52; i++) {	//ī�带 ���� ����ֱ�
		push(cards[i]);
	}
}

void make_player(int player_num) {
	for (int i = 0; i < player_num; i++) {
		player_arr[i].coin = cash;
		player_arr[i].hand_count = 0;
		player_arr[i].stop = 0;
		player_arr[i].result = 0;
		player_arr[i].playable = 1;
		player_arr[i].character = player_character();
		init(&player_arr[i].hand);
	}
}

void make_dealer() {
	dealer.hand_count = 0;
	dealer.result = 0;
	dealer.stop = 0;
	dealer.character = dealer_character();
	init(&dealer.hand);
}

void play(int player_num) {
	int betting_cash = 0;	//�̱�� ����
	betting_cash = betting(player_arr[0].coin, player_num);
	int is_stop = 0;
	char hit[5];
	for (int i = 0; i < player_num; i++) {
		getCard(i);
	}
	getCard_dealer();



	while (1)
	{
		is_stop = 0;

		system("cls");
		printf("     PLAYER1 CASH = $%d\n", player_arr[0].coin);
		print_all(player_num);
		printf("\n");
		system("pause");

		if (player_arr[0].stop == 0) {
			printf("\n          HIT OR STAY \n\n");
			printf("     ��������������      ��������������\n");
			printf("     �� HIT ��      �� STAY��\n");
			printf("     ��������������      ��������������\n");
			printf("commend : ");
			scanf("%s", hit);
			getchar();

			change(&hit);

			if (strcmp(hit, "hit") == 0) {	//hit
				if (player_arr[0].result < 21) {
					getCard(0);
					if (player_arr[0].stop == 1)
						is_stop++;
				}
				else {
					player_arr[0].stop = 1;
					is_stop++;
				}
			}

			if (strcmp(hit, "stay") == 0) {	//stay
				player_arr[0].stop = 1;
				is_stop++;
			}
		}
		else {	//stay�� ������
			is_stop++;
		}

		for (int i = 1; i < player_num; i++) {
			if (player_arr[i].stop == 0) {	//hit
				player_play(i);
				if (player_arr[i].stop == 1) {
					is_stop++;
				}
			}
			else {		//stay�� �� ����
				is_stop++;
			}
		}

		if (dealer.stop == 0) {
			dealer_play();
			if (dealer.stop == 1)
				is_stop++;
		}
		else {		//stay�� �� ����
			is_stop++;
		}

		if (is_stop == player_num + 1) {	//���� ��� �̵��� stay�� �ߴٸ�
			check_winner(player_num, betting_cash);
			reset_all(player_num);
			break;
		}

	}
}

int betting(int u_coin, int player_num) {
	int bet;
	system("cls");
	printf("     PLAYER CASH = $%d\n", u_coin);
	printf("     �󸶳� ������ ���ΰ���?\n");
	printf("\n     command: $");
	scanf("%d", &bet);
	getchar();
	while (bet > player_arr[0].coin) {
		system("cls");
		printf("your cash = $%d\n", u_coin);
		printf("�󸶳� ������ ���ΰ���?\n");
		printf("���� �ݾ��� �ʹ� �����ϴ�. �ٽ� �Է����ּ���.\n");
		printf("$ ");
		scanf("%d", &bet);
		getchar();
	}


	printf("\n");
	printf("--------------------------------------\n");
	printf("            LEFT COIN      \n");
	printf("--------------------------------------\n");

	for (int i = 0; i < player_num; i++) {
		if (player_arr[i].coin <= bet) {
			printf("         player %d : ALL-IN\n", i + 1);
			player_arr[i].coin = 0;
		}
		else {
			player_arr[i].coin = player_arr[i].coin - bet;
			printf("         player %d : $%d\n", i + 1, player_arr[i].coin);
		}
	}
	printf("--------------------------------------\n");
	system("pause");
	return bet;
}

void getCard(int num) {		//player�� ī�带 �޴� �Լ�
	int jqk = 0;

	if (player_arr[num].stop == 1) {
		return;
	}

	element data = pop();
	insert(&player_arr[num].hand, data);
	player_arr[num].hand_count++;		//ī�带 ������ ���� 1�� �߰�

	if (player_arr[num].hand_count == 1 && data.num > 10) {  // ù ���̸鼭 �������� ���ڰ� 10�� ���� ��
		player_arr[num].result += 10;
	}
	else if (data.num > 10) {  // ù �а� �ƴѵ� ������ ���ڰ� 10�� ���� ��
		if (data.num == 11) { // J�� �� (���� ī�� -10)
			if (player_arr[num].hand.Rlist->Rlist->data.num > 10) { // ���� ī�尡 J�� ��
				jqk = 0;
				player_arr[num].result += jqk;
			}
			else {  // ������ 
				jqk = player_arr[num].hand.Rlist->Rlist->data.num - 10;
				player_arr[num].result += jqk;
			}
		}
		else if (data.num == 12) { // Q�� �� (���� ī�� * 2)
			if (player_arr[num].hand.Rlist->Rlist->data.num > 10) { // ���� ī�尡 J�� ��
				jqk = 20;
				player_arr[num].result += jqk;
			}
			else {  // ������
				jqk = player_arr[num].hand.Rlist->Rlist->data.num * 2;
				player_arr[num].result += jqk;
			}
		}
		else { // K�� �� ( 0~10 ����)
			jqk = rand() % 10;
			player_arr[num].result += jqk;
		}
	}
	else if (data.num == 1) {	//A�� ������
		if (num == 0) {	//user���
			int r = 0;
			system("cls");
			printf("     PLAYER HAND : %d\n", player_arr[0].result);
			printf("\n     A ī�尡 ���Խ��ϴ�.\n     1�� 11 �� � ���� �����Ͻðڽ��ϱ�?\n");
			printf("              [1/11]");
			printf("\n     command : ");
			scanf("%d", &r);	//1�� 11 �� �ϳ� ����
			getchar();

			while (r != 1 && r != 11) {	//�ٸ� ���� ������ �ٽ�
				system("cls");
				printf("PLAYER1 HAND : %d\n", player_arr[0].result);
				printf("     A ī�尡 ���Խ��ϴ�.\n1�� 11 �� � ���� �����Ͻðڽ��ϱ�?\n");
				printf("     �Է��� �߸��Ǿ����ϴ�. �ٽ� �Է����ּ���.\n");
				printf("\n     command : ");
				scanf("%d", &r);
				getchar();
			}
			player_arr[0].result += r;		//1�̳� 11 +
		}
		else {		//CPU�� A�� ������
			if (player_arr[num].result + 11 > 21) {	// 11�� ������ �� 22 �̻��� �Ǹ�
				player_arr[num].result += 1;	//1 ���ϱ�
			}
			else {	//�ƴϸ� 11 ���ϱ�
				player_arr[num].result += 11;
			}
		}
	}
	else {		//�Ϲ� ī�� ���ϱ�
		player_arr[num].result += data.num;
	}
}

void getCard_dealer() {		//������ ī�带 �޴� �Լ�
	int jqk = 0;
	if (dealer.stop == 1)
		return;

	element data = pop();
	insert(&dealer.hand, data);
	dealer.hand_count++;

	if (dealer.hand_count == 1 && data.num > 10) {  // ù ���̸鼭 �������� ���ڰ� 10�� ���� ��
		dealer.result += 10;
	}
	else if (data.num > 10) {  // ù �а� �ƴѵ� ������ ���ڰ� 10�� ���� ��
		if (data.num == 11) { // J�� ��
			if (dealer.hand.Rlist->Rlist->data.num > 10) { // ���� ī�尡 J�� ��
				jqk = 0;
				dealer.result += jqk;
			}
			else {  // ������ 
				jqk = dealer.hand.Rlist->Rlist->data.num - 10;
				dealer.result += jqk;
			}
		}
		else if (data.num == 12) { // Q�� ��
			if (dealer.hand.Rlist->Rlist->data.num > 10) { // ���� ī�尡 J�� ��
				jqk = 20;
				dealer.result += jqk;
			}
			else {  // ������
				jqk = (dealer.hand.Rlist->Rlist->data.num) * 2;
				dealer.result += jqk;
			}
		}
		else { // K�� �� ( 0~10 ����)
			jqk = rand() % 10;
			dealer.result += jqk;
		}
	}
	else if (data.num == 1) {		//������ A�� ������
		if (dealer.result + 11 > 21) {		//22 �̻��̸� 1 �߰�
			dealer.result += 1;
		}
		else {				//21 ���ϸ� 11 �߰�
			dealer.result += 11;
		}
	}
	else if (data.num > 10) {	//J, Q, K ������ 10 ���ϱ�
		dealer.result += 10;
	}
	else {
		dealer.result += data.num;
	}

}

void print_all(int player_num) {	//ī�� ���
	printf("--------------------------------------\n");
	printf("             PLAYER CARD\n");
	printf("--------------------------------------\n");
	print_hand(&player_arr[0].hand);
	printf("--------------------------------------\n");
	printf("         PLAYER RESULT = % d\n", player_arr[0].result);
	printf("--------------------------------------\n");

	for (int i = 1; i < player_num; i++) {
		if (player_arr[i].playable == 1) {
			printf("\n--------------------------------------\n");
			printf("           PLAYER %d CARD\n", i + 1);
			printf("--------------------------------------\n");
			print_hand(&player_arr[i].hand);
			printf("--------------------------------------\n");
			printf("        PLAYER%d RESULT = % d\n", i + 1, player_arr[i].result);
			printf("--------------------------------------\n");
		}
	}

	printf("\n--------------------------------------\n");
	printf("             DEALER CARD\n");
	printf("--------------------------------------\n");
	print_hand(&dealer.hand);
	printf("--------------------------------------\n");
	printf("         DEALER RESULT = %d\n", dealer.result);
	printf("--------------------------------------\n");
}

int player_character() {		//ĳ���� ����
	int x = rand() % 17;
	int y = rand() % 13;
	int z = rand() % 7;
	int character = (x * y * z) % 50;

	return character;
}

int dealer_character() {		//ĳ���� ����
	int x = rand() % 5;
	int y = rand() % 13;
	int z = rand() % 7;
	int character = (x * y * z) % 30;

	return character;
}

void player_play(int num) {
	int n;
	if (player_arr[num].result < 17)		//���а� 17 �̸��̸� hit
		getCard(num);
	else if (player_arr[num].result >= 17 && player_arr[num].result < 19) {	// ���� ���а� 17, 18�̸�
		n = rand() % 100;					//100 ����
		if (n < player_arr[num].character)		//���� ĳ���� �������� ���� ������
			getCard(num);					//hit
		else								//�ƴϸ�
			player_arr[num].stop = 1;		//stay
	}
	else									//���� ���а� 19 �̻��̸�
		player_arr[num].stop = 1;			//stay
}

void dealer_play() {
	int n;
	if (dealer.result < 17)		//���а� 17 �̸��̸� hit
		getCard_dealer();
	else if (dealer.result >= 17 && dealer.result < 19) {	// ���� ���а� 17, 18�̸�
		n = rand() % 100;					//100 ����
		if (n < dealer.character)		//���� ĳ���� �������� ���� ������
			getCard_dealer();					//hit
		else								//�ƴϸ�
			dealer.stop = 1;		//stay
	}
	else									//���� ���а� 19 �̻��̸�
		dealer.stop = 1;			//stay
}

void change(char* hit) {	//�빮�ڸ� �ҹ��ڷ� ����
	for (int i = 0; i < 5; i++) {
		{
			if ((hit[i] >= 'A') && (hit[i] <= 'Z'))
			{
				hit[i] = hit[i] - 'A' + 'a';
			}
		}
	}
}

void check_winner(int player_num, int bet) {
	int big = 0, num = 0;
	element top_card;	//player�� ž ī��

	system("cls");

	for (int i = 0; i < player_num; i++) {
		if (player_arr[i].result <= 21) {	//player�� ����� 21 ���϶��
			if (player_arr[i].result > big) {	//����� ���� ũ�ٸ�
				big = player_arr[i].result;
				num = i;
				top_card = check_hand(&player_arr[i].hand);	//����� ���� ū player�� top ī��
			}
			else if (player_arr[i].result == big) {	//����� ���ٸ�
				if (player_arr[i].hand_count < player_arr[num].hand_count) {	//���а� �� ������
					big = player_arr[i].result;
					num = i;
					top_card = check_hand(&player_arr[i].hand);
				}
				if (player_arr[i].hand_count == player_arr[num].hand_count) {	//���а� ������
					//top_card�� ������ ����� ������ �̱�
					element top_card1 = check_hand(&player_arr[i].hand);

					if (top_card.pattern == "��") {	//top_card�� ������ �߶��
						if (top_card1.pattern == "��") {	//�����̵� > ���̾�
							big = player_arr[i].result;
							num = i;
							top_card = check_hand(&player_arr[i].hand);
						}
					}
					else if (top_card.pattern == "��") {	//top_card�� ������ �����
						if (top_card1.pattern == "��" || top_card1.pattern == "��") {	//�����̵� > ���̾� > ��Ʈ
							big = player_arr[i].result;
							num = i;
							top_card = check_hand(&player_arr[i].hand);
						}
					}
					else if (top_card.pattern == "��") {	//top_card�� ������ ����� ��� ��
						big = player_arr[i].result;
						num = i;
						top_card = check_hand(&player_arr[i].hand);
					}
				}
			}
		}
	}

	if (dealer.result <= 21) {
		if (dealer.result > big) {
			printf("\n     DEALER WIN\n\n");
		}
		else if (dealer.result == big) {
			element dealertop;

			if (dealer.hand_count < player_arr[num].hand_count) {	//���а� �� ������
				printf("\n     DEALER WIN\n\n");
			}
			else if (dealer.hand_count == player_arr[num].hand_count) {	//���а� ������
				//top_card�� ������ ����� ������ �̱�

				if (top_card.pattern == "��") {	//top_card�� ������ �߶��
					if (dealertop.pattern == "��") {	//�����̵� > ���̾�
						printf("\n     DEALER WIN\n\n");
					}
				}
				else if (top_card.pattern == "��") {	//top_card�� ������ �����
					if (dealertop.pattern == "��" || dealertop.pattern == "��") {	//�����̵� > ���̾� > ��Ʈ
						printf("\n     DEALER WIN\n\n");
					}
				}
				else if (top_card.pattern == "��") {	//top_card�� ������ ����� ��� ��
					printf("\n     DEALER WIN\n\n");
				}
			}
			else {
				if (num == 0) {
					printf(" YOU WIN\n");
					player_arr[0].coin += (bet * player_num);
				}
				else {
					printf("PLAYER %d WIN\n", num + 1);
					player_arr[num].coin += (bet * player_num);
				}
			}
		}
		else {
			if (num == 0) {
				printf("\n     YOU WIN\n\n");
				player_arr[0].coin += (bet * player_num);
			}
			else {
				printf("\n     PLAYER %d WIN\n\n", num + 1);
				player_arr[num].coin += (bet * player_num);
			}
		}
	}
	else if (player_arr[0].result > 21 && dealer.result > 21) {	//��� bust�� ���
		printf("\n     DRAW\n\n");
	}
	else {
		if (num == 0) {
			printf("\n      YOU WIN\n\n");
			player_arr[0].coin += (bet * player_num);
		}
		else {
			printf("\n     PLAYER %d WIN\n\n", num + 1);
			player_arr[num].coin += (bet * player_num);
		}
	}
	system("pause");
}

void reset_all(int player_num) {
	empty_deck();
	empty_hand(player_num);
	for (int i = 0; i < player_num; i++) {
		player_arr[i].hand_count = 0;
		player_arr[i].result = 0;
		player_arr[i].stop = 0;
	}
	dealer.hand_count = 0;
	dealer.result = 0;
	dealer.stop = 0;
}

void empty_deck() {
	while (card_deck.top > -1) {
		pop();
	}
}

void empty_hand(int player_num) {
	for (int i = 0; i < player_num; i++) {
		for (int j = 0; j < player_arr[i].hand_count; j++) {
			delete(&player_arr[i].hand);
		}
	}
	for (int i = 0; i < dealer.hand_count; i++)
		delete(&dealer.hand);
}