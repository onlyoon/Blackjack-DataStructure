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

typedef struct decks {	//덱 
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

//스택 관련
void init_stack();
int is_empty();
int is_full();
void push(element card);
element pop();

//이중연결리스트
void init(hand_cards* head);
void insert(hand_cards* before, element data);
void delete(hand_cards* head, hand_cards* removed);
void print_hand(hand_cards* head);
element check_hand(hand_cards* head);

int start();		//시작 준비
void makedeak();	//패 만들기
void shakedeak();	//카드 섞는수함수

void make_player(int player_num);	//플레이어 준비 함수
int player_character();		//player 성격
void getCard(int num);	//카드를 받는 함수
void player_play(int num);	//플레이어 플레이 함수

void make_dealer();				//딜러 준비 함수
int dealer_character();		//dealer 성격
void getCard_dealer();	//딜러가 카드를 받는 함수
void dealer_play();		//딜러 플레이 함수

void play(int player_num);		//블랙잭 플레이 함수
int betting(int u_coin, int player_num);	//배팅하는 함수
void print_all(int player_num);	//모든 player 카드 출력
void showcard(int num);	//카드 출력 함수
void check_winner(int player_num, int bet); //우승자 확인

void reset_all(int player_num);	//모두 리셋
void empty_deck();			//덱 초기화
void empty_hand(int player_num);	//손패 초기화

void change(char* hit);	//문자열 소문자 변환 함수

deck card_deck;
char shape[4][3] = { "♠", "◆", "♥", "♣" };	//카드 문양
element cards[52];	//카드 덱
player player_arr[4];	//최대 플레이어 4명
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
			for (int i = 1; i < player_num; i++) {	//돈 다 떨어진 player 삭제
				if (player_arr[i].coin == 0) {
					player_arr[i].stop = 1;
					player_arr[i].result = 999;
					player_arr[i].playable = 0;
				}
			}
			for (int i = 1; i < player_num; i++) {		//all-in된 player 계산
				if (player_arr[i].playable == 0)
					players++;
			}
			if (players == (player_num - 1)) {	//만약 user를 제외한 모든 player가 all-in이 되면 게임 종료
				system("cls");
				printf("        you win the game!");
				exit(0);
			}

			system("cls");
			printf("     PLAYER CASH : $%d\n", player_arr[0].coin);
			printf("\n     게임이 종료되었습니다.\n");
			printf("     게임을 계속하시겠습니까?    [Y/N]     \n");
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
					printf("입력이 잘못되었습니다. 다시 입력해주세요 [Y/N].\n");
					printf("\n     command : ");
					scanf("%c", &select);
					getchar();
				}
			}
		}
	}

}

//---------------------스택관련 함수----------------------------
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
		fprintf(stderr, "스택 포화 에러");
		return;
	}
	else {
		card_deck.data[++(card_deck.top)] = card;
	}
}

element pop() {
	if (is_empty(card_deck)) {
		fprintf(stderr, "스택 공백 에러");
		return;
	}
	else {
		return card_deck.data[(card_deck.top)--];
	}
}
//--------------------------------------------------

//---------------------리스트관련 함수----------------------------
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
		printf("┌─────┐\t");
	}
	printf("┌─────┐\n");

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("│%s   │\t", p->data.pattern);
	}
	printf("│%s   │\n", p->data.pattern);

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		showcard(p->data.num);
	}
	showcard(p->data.num);
	printf("\n");

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("│   %s│\t", p->data.pattern);
	}
	printf("│   %s│\n", p->data.pattern);

	for (p = head->Rlist; p->Rlist != head; p = p->Rlist) {
		printf("└─────┘\t");
	}
	printf("└─────┘\n");
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

// 초기 시작 화면 몇 명의 CPU와 게임을 할 것인지 정함
int start() {
	int player_num;
	char choice1;
	printf("     블랙잭 게임에 오신 것을 환영합니다!     \n");
	printf("            게임을 시작하시겠습니까?         \n");
	printf("                    [Y/N]                   \n ");
	printf("\n     command : ");

	scanf("%c", &choice1);
	getchar();

	while ((choice1 != 'Y') && (choice1 != 'y') && (choice1 != 'N') && (choice1 != 'n'))
	{
		printf("\n");
		printf("입력이 잘못되었습니다. 다시 입력해주세요 [Y/N].\n");
		printf("\n     command : ");
		scanf("%c", &choice1);
		getchar();
	}

	// Y나 N을 누를 경우 (올바를 경우)
	if (choice1 == 'Y' || choice1 == 'y') {
		printf("\n     CPU PLAYER를 몇 명으로 하시겠습니까?      ");
		printf("\n              CPU PLAYER [1/2/3]          \n");
		printf("\n     command : ");
		scanf("%d", &player_num);
		getchar();
		// CPU PLAYER 입력이 (올바르지 않을 경우)
		while (player_num != 3 && player_num != 2 && player_num != 1) {
			printf("\n숫자 3, 2, 1 중에서 입력해주시기 바랍니다.\n");
			printf("\n     command : ");
			scanf("%d", &player_num);
			getchar();
		}
		if (player_num == 3 || player_num == 2 || player_num == 1) {
			printf("\n     CPU PLAYER 인원이 총 %d명으로 세팅되었습니다.     ", player_num);
			Sleep(1000);

			player_num++;
			make_player(player_num);
			make_dealer();
		}
	}
	else if (choice1 == 'N' || choice1 == 'n') {
		printf("             게임을 종료하겠습니다.             \n");
	}


	return player_num;
}


void showcard(int num) {	//카드 출력
	if (num == 1) {
		printf("│  A  │\t");
	}
	else if (num < 10) {
		printf("│  %d  │\t", num);
	}
	else if (num == 10) {
		printf("│ 10  │\t");
	}
	else if (num == 11) {
		printf("│  J  │\t");
	}
	else if (num == 12) {
		printf("│  Q  │\t");
	}
	else if (num == 13) {
		printf("│  K  │\t");
	}
	else {
		fprintf(stderr, "카드 에러");
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

	for (int i = 0; i < 300; i++) {	//카드 섞기

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

	for (int i = 0; i < 52; i++) {	//카드를 덱에 집어넣기
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
	int betting_cash = 0;	//이기면 받음
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
			printf("     ┏━━━━━┓      ┏━━━━━┓\n");
			printf("     ┃ HIT ┃      ┃ STAY┃\n");
			printf("     ┗━━━━━┛      ┗━━━━━┛\n");
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
		else {	//stay를 했으면
			is_stop++;
		}

		for (int i = 1; i < player_num; i++) {
			if (player_arr[i].stop == 0) {	//hit
				player_play(i);
				if (player_arr[i].stop == 1) {
					is_stop++;
				}
			}
			else {		//stay를 한 이후
				is_stop++;
			}
		}

		if (dealer.stop == 0) {
			dealer_play();
			if (dealer.stop == 1)
				is_stop++;
		}
		else {		//stay를 한 이후
			is_stop++;
		}

		if (is_stop == player_num + 1) {	//만약 모든 이들이 stay를 했다면
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
	printf("     얼마나 베팅할 것인가요?\n");
	printf("\n     command: $");
	scanf("%d", &bet);
	getchar();
	while (bet > player_arr[0].coin) {
		system("cls");
		printf("your cash = $%d\n", u_coin);
		printf("얼마나 베팅할 것인가요?\n");
		printf("베팅 금액이 너무 높습니다. 다시 입력해주세요.\n");
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

void getCard(int num) {		//player가 카드를 받는 함수
	int jqk = 0;

	if (player_arr[num].stop == 1) {
		return;
	}

	element data = pop();
	insert(&player_arr[num].hand, data);
	player_arr[num].hand_count++;		//카드를 받으면 손패 1장 추가

	if (player_arr[num].hand_count == 1 && data.num > 10) {  // 첫 패이면서 데이터의 숫자가 10이 넘을 때
		player_arr[num].result += 10;
	}
	else if (data.num > 10) {  // 첫 패가 아닌데 데이터 숫자가 10이 넘을 때
		if (data.num == 11) { // J일 때 (이전 카드 -10)
			if (player_arr[num].hand.Rlist->Rlist->data.num > 10) { // 이전 카드가 J일 때
				jqk = 0;
				player_arr[num].result += jqk;
			}
			else {  // 나머지 
				jqk = player_arr[num].hand.Rlist->Rlist->data.num - 10;
				player_arr[num].result += jqk;
			}
		}
		else if (data.num == 12) { // Q일 때 (이전 카드 * 2)
			if (player_arr[num].hand.Rlist->Rlist->data.num > 10) { // 이전 카드가 J일 때
				jqk = 20;
				player_arr[num].result += jqk;
			}
			else {  // 나머지
				jqk = player_arr[num].hand.Rlist->Rlist->data.num * 2;
				player_arr[num].result += jqk;
			}
		}
		else { // K일 때 ( 0~10 랜덤)
			jqk = rand() % 10;
			player_arr[num].result += jqk;
		}
	}
	else if (data.num == 1) {	//A가 나오면
		if (num == 0) {	//user라면
			int r = 0;
			system("cls");
			printf("     PLAYER HAND : %d\n", player_arr[0].result);
			printf("\n     A 카드가 나왔습니다.\n     1과 11 중 어떤 수를 선택하시겠습니까?\n");
			printf("              [1/11]");
			printf("\n     command : ");
			scanf("%d", &r);	//1과 11 중 하나 선택
			getchar();

			while (r != 1 && r != 11) {	//다른 값을 넣으면 다시
				system("cls");
				printf("PLAYER1 HAND : %d\n", player_arr[0].result);
				printf("     A 카드가 나왔습니다.\n1과 11 중 어떤 수를 선택하시겠습니까?\n");
				printf("     입력이 잘못되었습니다. 다시 입력해주세요.\n");
				printf("\n     command : ");
				scanf("%d", &r);
				getchar();
			}
			player_arr[0].result += r;		//1이나 11 +
		}
		else {		//CPU가 A가 나오면
			if (player_arr[num].result + 11 > 21) {	// 11을 더했을 때 22 이상이 되면
				player_arr[num].result += 1;	//1 더하기
			}
			else {	//아니면 11 더하기
				player_arr[num].result += 11;
			}
		}
	}
	else {		//일반 카드 더하기
		player_arr[num].result += data.num;
	}
}

void getCard_dealer() {		//딜러가 카드를 받는 함수
	int jqk = 0;
	if (dealer.stop == 1)
		return;

	element data = pop();
	insert(&dealer.hand, data);
	dealer.hand_count++;

	if (dealer.hand_count == 1 && data.num > 10) {  // 첫 패이면서 데이터의 숫자가 10이 넘을 때
		dealer.result += 10;
	}
	else if (data.num > 10) {  // 첫 패가 아닌데 데이터 숫자가 10이 넘을 때
		if (data.num == 11) { // J일 때
			if (dealer.hand.Rlist->Rlist->data.num > 10) { // 이전 카드가 J일 때
				jqk = 0;
				dealer.result += jqk;
			}
			else {  // 나머지 
				jqk = dealer.hand.Rlist->Rlist->data.num - 10;
				dealer.result += jqk;
			}
		}
		else if (data.num == 12) { // Q일 때
			if (dealer.hand.Rlist->Rlist->data.num > 10) { // 이전 카드가 J일 때
				jqk = 20;
				dealer.result += jqk;
			}
			else {  // 나머지
				jqk = (dealer.hand.Rlist->Rlist->data.num) * 2;
				dealer.result += jqk;
			}
		}
		else { // K일 때 ( 0~10 랜덤)
			jqk = rand() % 10;
			dealer.result += jqk;
		}
	}
	else if (data.num == 1) {		//딜러가 A를 받으면
		if (dealer.result + 11 > 21) {		//22 이상이면 1 추가
			dealer.result += 1;
		}
		else {				//21 이하면 11 추가
			dealer.result += 11;
		}
	}
	else if (data.num > 10) {	//J, Q, K 나오면 10 더하기
		dealer.result += 10;
	}
	else {
		dealer.result += data.num;
	}

}

void print_all(int player_num) {	//카드 출력
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

int player_character() {		//캐릭터 난수
	int x = rand() % 17;
	int y = rand() % 13;
	int z = rand() % 7;
	int character = (x * y * z) % 50;

	return character;
}

int dealer_character() {		//캐릭터 난수
	int x = rand() % 5;
	int y = rand() % 13;
	int z = rand() % 7;
	int character = (x * y * z) % 30;

	return character;
}

void player_play(int num) {
	int n;
	if (player_arr[num].result < 17)		//손패가 17 미만이면 hit
		getCard(num);
	else if (player_arr[num].result >= 17 && player_arr[num].result < 19) {	// 현재 손패가 17, 18이면
		n = rand() % 100;					//100 난수
		if (n < player_arr[num].character)		//만약 캐릭터 변수보다 낮게 나오면
			getCard(num);					//hit
		else								//아니면
			player_arr[num].stop = 1;		//stay
	}
	else									//현재 손패가 19 이상이면
		player_arr[num].stop = 1;			//stay
}

void dealer_play() {
	int n;
	if (dealer.result < 17)		//손패가 17 미만이면 hit
		getCard_dealer();
	else if (dealer.result >= 17 && dealer.result < 19) {	// 현재 손패가 17, 18이면
		n = rand() % 100;					//100 난수
		if (n < dealer.character)		//만약 캐릭터 변수보다 낮게 나오면
			getCard_dealer();					//hit
		else								//아니면
			dealer.stop = 1;		//stay
	}
	else									//현재 손패가 19 이상이면
		dealer.stop = 1;			//stay
}

void change(char* hit) {	//대문자를 소문자로 변경
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
	element top_card;	//player의 탑 카드

	system("cls");

	for (int i = 0; i < player_num; i++) {
		if (player_arr[i].result <= 21) {	//player의 결과가 21 이하라면
			if (player_arr[i].result > big) {	//결과가 가장 크다면
				big = player_arr[i].result;
				num = i;
				top_card = check_hand(&player_arr[i].hand);	//결과가 가장 큰 player의 top 카드
			}
			else if (player_arr[i].result == big) {	//결과가 같다면
				if (player_arr[i].hand_count < player_arr[num].hand_count) {	//손패가 더 적으면
					big = player_arr[i].result;
					num = i;
					top_card = check_hand(&player_arr[i].hand);
				}
				if (player_arr[i].hand_count == player_arr[num].hand_count) {	//손패가 같으면
					//top_card의 문양이 ♠라면 무조건 이김
					element top_card1 = check_hand(&player_arr[i].hand);

					if (top_card.pattern == "◆") {	//top_card의 문양이 ◆라면
						if (top_card1.pattern == "♠") {	//스페이드 > 다이아
							big = player_arr[i].result;
							num = i;
							top_card = check_hand(&player_arr[i].hand);
						}
					}
					else if (top_card.pattern == "♥") {	//top_card의 문양이 ♥라면
						if (top_card1.pattern == "♠" || top_card1.pattern == "◆") {	//스페이드 > 다이아 > 하트
							big = player_arr[i].result;
							num = i;
							top_card = check_hand(&player_arr[i].hand);
						}
					}
					else if (top_card.pattern == "♣") {	//top_card의 문양이 ♣라면 모두 짐
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

			if (dealer.hand_count < player_arr[num].hand_count) {	//손패가 더 적으면
				printf("\n     DEALER WIN\n\n");
			}
			else if (dealer.hand_count == player_arr[num].hand_count) {	//손패가 같으면
				//top_card의 문양이 ♠라면 무조건 이김

				if (top_card.pattern == "◆") {	//top_card의 문양이 ◆라면
					if (dealertop.pattern == "♠") {	//스페이드 > 다이아
						printf("\n     DEALER WIN\n\n");
					}
				}
				else if (top_card.pattern == "♥") {	//top_card의 문양이 ♥라면
					if (dealertop.pattern == "♠" || dealertop.pattern == "◆") {	//스페이드 > 다이아 > 하트
						printf("\n     DEALER WIN\n\n");
					}
				}
				else if (top_card.pattern == "♣") {	//top_card의 문양이 ♣라면 모두 짐
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
	else if (player_arr[0].result > 21 && dealer.result > 21) {	//모두 bust인 경우
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