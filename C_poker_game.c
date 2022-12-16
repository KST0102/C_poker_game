#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// 카드 구조체
struct Card {
	int number;
	int shape;
};

// 52장의 카드
struct Card card_all[52];

struct Player {
	struct Card card_player[7];// 선수마다 갖게 되는 카드 7장
	int high_card[3];//선수의 가장 높은 카드(경우의 수 3장)
	int rank;// 하이카드:0, 원페어:1, 투페어:2, 쓰리오브어카인드:3, 스트레이트:4, 플러시:5, 풀하우스:6, 포오브어카인드:7, 스트레이트플러시:8
	int money; //보유 금액
};

struct table {
	struct Card card_tabel[5];
};

void shuffle()
{
	// 52장의 카드를 랜덤한 index번째 카드와 하나씩 뒤바꾸어 줌
	srand(time(NULL));
	for (int i = 0; i < 52; i++)
	{
		int index = rand() % 52;
		struct Card temp = card_all[index];
		card_all[index] = card_all[i];
		card_all[i] = temp;
	}
}

void filldeck()
{
	int i = 0;
	// 4개의 모양과 13개의 숫자를 짝지어서 카드 만듦
	for (int j = 0; j < 4; j++)
	{
		for (int k = 1; k <= 13; k++)
		{
			card_all[i].number = k;
			card_all[i].shape = j;
			i++;
		}
	}
}

void deal(struct Player player[], struct table table[])
{
	// 섞어진 카드를 각 플레이어 2명에게 앞에서부터 나누어줌
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			player[i].card_player[j] = card_all[i * 2 + j];
		}
	}
	for (int i = 0; i < 5; i++) //테이블 공유 카드 5장 나누어줌
	{
		table[0].card_tabel[i] = card_all[4 + i];
	}
	for (int i = 0; i < 5; i++) // 테이블 공유 카드를 각 플레이어 카드에 추가
	{
		player[0].card_player[2 + i] = table[0].card_tabel[i];
		player[1].card_player[2 + i] = table[0].card_tabel[i];
	}
}

int check(struct Card card_player[], int high_card[])
{
	int same[7];
	for (int i = 0; i < 7; i++)
		same[i] = card_player[i].number;
	for (int i = 0; i < 6; i++)
	{
		for (int j = i; j >= 0; j--)
		{
			if (same[j] > same[j + 1])
			{
				int t = same[j];
				same[j] = same[j + 1];
				same[j + 1] = t;
			}
		}
	}
	//판별
	high_card[0] = 0;//투페어, 풀하우스 등에서 가장 높은 숫자 페어
	high_card[1] = 0;//투페어, 풀하우스 등에서 두번째로 페어
	high_card[2] = 0;//쓰리 오브 카인드, 포 오브 카인드, 스트레이트의 가장 높은 숫자
	int pair = 0;
	int number_kind = 0;//같은 숫자의 개수
	int straight = 1;
	for (int i = 0; i < 7; i++)
	{
		int cnt = 1;
		for (int j = i + 1; j < 7; j++)
		{
			if (same[i] == same[j] && same[i] != same[i - 1])
				cnt += 1;
		}
		if (cnt == 2)
		{
			pair += 1;
			if (pair >= 2)
			{
				high_card[1] = high_card[0];
				high_card[0] = same[i];
			}
			else
			{
				high_card[0] = same[i];
			}
		}
		else if (cnt >= 3)
		{
			number_kind = cnt;
			high_card[2] = same[i];
		}
		else
			high_card[0] = same[i];
	}
	for (int i = 1; i < 7; i++) // 스트레이트 판정
	{
		if (same[i] == same[i - 1] + 1)
		{
			straight += 1;
			if (straight >= 5)
				high_card[2] = same[i];
		}
		else if (same[i] == same[i - 1])
			continue;
	}
	int shape[4] = { 0, 0, 0, 0 };
	int flush = -1; //기본값이 -1
	for (int i = 0; i < 7; i++)
	{
		if (card_player[i].shape == 0)
			shape[0] += 1;
		else if (card_player[i].shape == 1)
			shape[1] += 1;
		else if (card_player[i].shape == 2)
			shape[2] += 1;
		else
			shape[3] += 1;
	}
	for (int i = 0; i < 4; i++)
	{
		if (shape[i] >= 5)
			flush = i;
	}

	if (pair == 0 && number_kind < 3 && straight < 5)
		return 0;//하이카드
	if (pair == 1 && number_kind < 3 && straight < 5 && flush == -1)
		return 1;//원페어
	if (pair == 2 && number_kind < 3 && straight < 5 && flush == -1)
		return 2;//투페어
	if (pair == 0 && number_kind == 3 && straight < 5 && flush == -1)
		return 3;//쓰리오브어카인드
	if (straight >= 5 && flush == -1)
		return 4;//스트레이트
	if (straight < 5 && flush >= 0)
		return 5;//플러시
	if (pair >= 1 && number_kind == 3)
		return 6;//풀하우스
	if (number_kind == 4)
		return 7;//포오브어카인드
	if (straight >= 5 && flush >= 0)
		return 8;//스트레이트플러시
}

void gotoxy(int x, int y) {	// gotoxy
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int title()
{
	int num;

	system("cls");
	gotoxy(17, 5);
	printf("□□■■■■■□□□□■■■■□□□■■□□□■■□□■■■■■■□□■■■■■□□□");
	gotoxy(17, 6);
	printf("□□■■■■■■□□■■■■■■□□■■□□□■■□□■■■■■■□□■■■■■■□□");
	gotoxy(17, 7);
	printf("□□■■□□■■□□■■□□■■□□■■□□□■■□□■■□□□□□□■■□□■■□□");
	gotoxy(17, 8);
	printf("□□■■□□■■□□■■□□■■□□■■□□■■□□□■■□□□□□□■■□□■■□□");
	gotoxy(17, 9);
	printf("□□■■■■■■□□■■□□■■□□■■■■■□□□□■■■■■■□□■■■■■■□□");
	gotoxy(17, 10);
	printf("□□■■■■■□□□■■□□■■□□■■■■■■□□□■■■■■■□□■■■■■□□□");
	gotoxy(17, 11);
	printf("□□■■□□□□□□■■□□■■□□■■□□□■■□□■■□□□□□□■■□■■□□□");
	gotoxy(17, 12);
	printf("□□■■□□□□□□■■□□■■□□■■□□□■■□□■■□□□□□□■■□□■■□□");
	gotoxy(17, 13);
	printf("□□■■□□□□□□■■□□■■□□■■□□□■■□□■■■■■■□□■■□□■■□□");
	gotoxy(17, 14);
	printf("□□■■□□□□□□□■■■■□□□■■□□□■■□□■■■■■■□□■■□□■■□□");
	gotoxy(17, 16);
	printf("게임 룰(1)\n");
	gotoxy(17, 17);
	printf("게임 시작(2)\n");
	gotoxy(17, 18);
	printf("번호를 선택해주십시오. : ");
	scanf_s("%d", &num);
	return num;
}

void Poker_Pirate(char text[])       //캐릭터 지정
{
	gotoxy(45, 1); printf("*☆.*.*.......*.......*☆.* ");
	gotoxy(45, 3); printf("┏ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ┓");
	gotoxy(45, 4); printf("ㅣㅣ　                   ㅣ");
	gotoxy(45, 5); printf("ㅣ  ㅣ          ∧  ∧    ㅣ");
	gotoxy(45, 6); printf("ㅣ    ㅣ       ㅣㅣㅣㅣ    ㅣ");
	gotoxy(45, 7); printf("┃　    ├┓　   （.*ω*.）    ㅣ");
	gotoxy(45, 8); printf("┗┬　    ├┳ㅡㅡㅡㅡOㅡOㅡㅡㅡㅡ┓");
	gotoxy(45, 9); printf("　┗┬  　 ┃　   　　　　       ┃");
	gotoxy(45, 10); printf("    ㅣ   ㅣ  　 　            ㅣ");
	gotoxy(45, 11); printf("    ㅣ   ㅣ ┏ㅡㅡㅡㅡㅡㅡㅡ┓  ㅣ");
	gotoxy(45, 12); printf("　  ㅣ   ㅣ ㅣ             ㅣ ㅣ");
	gotoxy(45, 13); printf("　  ㅣ　 ㅣ ㅣ             ㅣ ㅣ");     // 중간 빈칸 말풍선
	gotoxy(45, 14); printf("　  ㅣ   ㅣ ㅣ             ㅣ ㅣ");
	gotoxy(45, 15); printf("    ㅣ　 ㅣ ┗ㅡㅡㅡㅡㅡㅡㅡ┛  ㅣ");
	gotoxy(45, 16); printf("    ㅣ   ㅣ                   ㅣ");
	gotoxy(45, 17); printf("    ㅣ   ㅣ                   ㅣ");
	gotoxy(45, 18); printf("　  ㅣ   ㅣ                   ㅣ");
	gotoxy(45, 19); printf("    ┗ㅡㅡ┻ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ┛");
	gotoxy(45, 21); printf("☆.....*......*......*.......☆");

	gotoxy(59, 13); printf("%s", text);
}

void Poker_Card(int x, int y, int card_num, int card_shape)       //기본 카드 모양, ?모양 변환 해야함(색도 따로 지정 해야함)
{
	gotoxy(x, y); printf("┏━━━━━━━━┓ ");
	switch (card_shape)
	{
	case 0:
		gotoxy(x, y+1); printf("┃ ♥      ┃ ");
		break;
	case 1:
		gotoxy(x, y+1); printf("┃ ◆      ┃ ");
		break;
	case 2:
		gotoxy(x, y+1); printf("┃ ♣      ┃ ");
		break;
	case 3:
		gotoxy(x, y+1); printf("┃ ♠      ┃ ");
		break;
	case 99:
		gotoxy(x, y + 1); printf("┃ ?      ┃ ");
		break;
	}
	gotoxy(x, y+2); printf("┃        ┃ ");
	switch (card_num)
	{
	case 1:
		gotoxy(x, y+3); printf("┃    A   ┃ ");
		break;
	case 11:
		gotoxy(x, y+3); printf("┃    J   ┃ ");
		break;
	case 12:
		gotoxy(x, y+3); printf("┃    Q   ┃ ");
		break;
	case 13:
		gotoxy(x, y+3); printf("┃    K   ┃ ");
		break;
	case 99://물음표 출력을 위한 케이스
		gotoxy(x, y + 3); printf("┃    ?   ┃ ");
		break;
	default:
		gotoxy(x, y+3); printf("┃    %d   ┃ ", card_num);
	}
	gotoxy(x, y+4); printf("┃        ┃ ");
	gotoxy(x, y+5); printf("┃        ┃ ");
	gotoxy(x, y+6); printf("┗━━━━━━━━┛ ");
}

void Edge() {
	int i;
	int h = 0xa1;
	int v = 0xd8;

	for (i = 0; i <= 108; i++) {			// 메인 게임보드 가로줄
		gotoxy(6 + i, 22);
		printf("%c%c", h, v);

		gotoxy(6 + i, 42);
		printf("%c%c", h, v);

	}

	for (i = 0; i <= 20; i++) {				// 메인 게임보드 세로줄
		gotoxy(6, 22 + i);
		printf("%c%c", h, v);

		gotoxy(114, 22 + i);
		printf("%c%c", h, v);
	}
}

void main()
{
	struct Player player[2];
	struct table table[1];
	system("mode con cols=120 lines=60");
	int num;//타이틀 화면 번호 선택
	system("cls");
	restart://타이틀 재시작 구간
	num = title();
	if (num == 1)
	{
		system("cls");
		printf("이 게임은 포커 게임의 종류 중 '텍사스 홀덤'이라는 게임입니다.\n");
		printf("게임이 시작되면 플레이어와 상대가 각자 money를 100씩 가지고 시작합니다.\n");
		printf("플레이어와 상대는 각자 카드를 2장씩 갖고, 이후 테이블에 공유 카드가 5개 놓이게 됩니다.\n");
		printf("각자의 카드 2장과 공유 카드 5개를 조합하여 더 높은 조합을 갖고 있는 사람이 배팅 금액을 가져갑니다.\n");
		printf("플레이어는 자신의 카드 조합을 보고 배팅 금액을 결정할 수 있으며 상대는 플레이어의 배팅 금액과 맞춰 배팅합니다.\n");
		printf("최종적으로 상대의 money를 모두 빼앗으면 승리합니다.\n");
		printf("\n");
		printf("<카드 조합 족보>\n");
		printf("1. 하이 카드 : 아무런 조합이 없을 때\n");
		printf("2. 원 페어 : 숫자가 같은 카드가 한 쌍 존재할 때\n");
		printf("3. 투 페어 : 숫자가 같은 카드가 두 쌍 존재할 때\n");
		printf("4. 쓰리 오브 어 카인드 : 같은 숫자 카드가 3장 존재할 때\n");
		printf("5. 스트레이트 : 연속된 숫자 카드가 5장 이어질 때\n");
		printf("6. 플러시 : 같은 모양 카드가 5장 존재할 때\n");
		printf("7. 풀 하우스 : 같은 숫자 카드가 3장, 숫자가 같은 카드 한 쌍이 존재할 때\n");
		printf("8. 포 오브 어 카인드 : 같은 숫자 카드가 4장 존재할 때\n");
		printf("9. 스트레이트 플러시: 스트레이트와 플러시의 조건이 동시에 충족될 때. 가장 높은 순위의 카드 조합.\n");
		printf("게임을 시작하려면 '2'를 입력하세요 : ");
		scanf_s("%d", &num);
		if (num == 2)
			goto start;
	}
	else if (num == 2)
	{
		start://시작 구간
		player[0].money = 100;
		player[1].money = 100;
		int win = -1;//기본 상태 -1
		int betting = 0; //배팅 금액
		for (int i = 0; i < 7; i++)
		{
			if (i == 0)
			{
				system("cls");
				Poker_Pirate("기대해라냥");
				Edge();
				gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
				gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
				filldeck();
				shuffle();
				deal(player, table);
				player[0].rank = check(player[0].card_player, player[0].high_card);
				player[1].rank = check(player[1].card_player, player[1].high_card);
			}
			if (i == 1)
			{
				system("cls");
				Poker_Pirate("느낌이 좋다냥");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
				gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
			}
			if (i == 2)
			{
				system("cls");
				Poker_Pirate("오오오!!!!!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
				gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
			}
			if (i == 3)
			{
				system("cls");
				Poker_Pirate("좋다냥!!!!!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4번째 공유카드
				gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
				gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
			}
			if (i == 4)
			{
				input:
				system("cls");
				Poker_Pirate("배팅해라냥!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4번째 공유카드
				Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5번째 공유카드
				gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
				gotoxy(6, 20); printf("보유 금액: %d\t배팅할 금액: ", player[0].money); scanf_s("%d", &betting);
				if (betting > player[0].money || betting > player[1].money)//배팅금액이 자신과 상대의 money를 초과하지 못함
					goto input;
			}
			if (i == 5)
			{
				//승,패,무효 판단
				if (player[0].rank > player[1].rank)
					win = 0;
				else if (player[0].rank < player[1].rank)
					win = 1;
				else//player[0].rank == player[1].rank인 경우
				{
					if (player[0].rank == 0 || player[0].rank == 1 || player[0].rank == 2)//하이카드, 원페어, 투페어
					{
						if (player[0].high_card[0] > player[1].high_card[0])
							win = 0;
						else if (player[0].high_card[0] < player[1].high_card[0])
							win = 1;
						else
						{
							if (player[0].rank == 2)//투페어의 가장 높은 숫자 페어가 같으면
							{
								if (player[0].high_card[1] > player[1].high_card[1])//두번째 페어 비교
									win = 0;
								else if (player[0].high_card[1] < player[1].high_card[1])
									win = 1;
								else
									win = 2;
							}
							else
								win = 2;
						}
					}
					if (player[0].rank == 3 || player[0].rank == 4 || player[0].rank == 6 || player[0].rank == 7 || player[0].rank == 8)//쓰리오브어카인드, 스트레이트, 풀하우스, 포오브어카인드, 스트레이트플러시
					{
						if (player[0].high_card[2] > player[1].high_card[2])
							win = 0;
						else if (player[0].high_card[2] < player[1].high_card[2])
							win = 1;
						else
						{
							if (player[0].rank == 6)//풀하우스의 쓰리오브카인드 숫자가 같으면
							{
								if (player[0].high_card[0] > player[1].high_card[0])//페어 비교
									win = 0;
								else if (player[0].high_card[0] < player[1].high_card[0])
									win = 1;
								else
									win = 2;
							}
							else
								win = 2;
						}
					}
					if (player[0].rank == 5)//플러시
						win = 2;
				}
				if (win == 0)//이겼을 경우
				{
					system("cls");
					Poker_Pirate("안된다냥!!");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4번째 공유카드
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5번째 공유카드
					player[0].money += betting;
					player[1].money -= betting;
					gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
					gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
				}
				else if (win == 1)//졌을 경우
				{
					system("cls");
					Poker_Pirate("신난다냥!!");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4번째 공유카드
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5번째 공유카드
					player[0].money -= betting;
					player[1].money += betting;
					gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
					gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
				}
				else//무효판
				{
					system("cls");
					Poker_Pirate("무효다냥..");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//처음 테이블 공유카드 3개
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4번째 공유카드
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5번째 공유카드
					gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
					gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
				}
			}
			if (i == 6)
			{
				if (player[0].money <= 0)
				{
					system("cls");
					Poker_Pirate("너무 쉽다냥~");
					Edge();
					gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
					gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
				}
				else if (player[1].money <= 0)
				{
					system("cls");
					Poker_Pirate("큭 분하다냥..");
					Edge();
					gotoxy(6, 19); printf("상대 보유 금액: %d", player[1].money);
					gotoxy(6, 20); printf("보유 금액: %d", player[0].money);
					gotoxy(1, 43);//종료 문구가 화면 아래쪽에서 뜨도록 설정함(별 의미는 없음)
				}
				else
					i = -1;//다시 for문 처음부터 반복
			}
			getch();
		}
	}
	else//잘못 입력하면
		goto restart;
}