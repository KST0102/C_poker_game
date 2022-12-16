#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// ī�� ����ü
struct Card {
	int number;
	int shape;
};

// 52���� ī��
struct Card card_all[52];

struct Player {
	struct Card card_player[7];// �������� ���� �Ǵ� ī�� 7��
	int high_card[3];//������ ���� ���� ī��(����� �� 3��)
	int rank;// ����ī��:0, �����:1, �����:2, ���������ī�ε�:3, ��Ʈ����Ʈ:4, �÷���:5, Ǯ�Ͽ콺:6, �������ī�ε�:7, ��Ʈ����Ʈ�÷���:8
	int money; //���� �ݾ�
};

struct table {
	struct Card card_tabel[5];
};

void shuffle()
{
	// 52���� ī�带 ������ index��° ī��� �ϳ��� �ڹٲپ� ��
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
	// 4���� ���� 13���� ���ڸ� ¦��� ī�� ����
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
	// ������ ī�带 �� �÷��̾� 2���� �տ������� ��������
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			player[i].card_player[j] = card_all[i * 2 + j];
		}
	}
	for (int i = 0; i < 5; i++) //���̺� ���� ī�� 5�� ��������
	{
		table[0].card_tabel[i] = card_all[4 + i];
	}
	for (int i = 0; i < 5; i++) // ���̺� ���� ī�带 �� �÷��̾� ī�忡 �߰�
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
	//�Ǻ�
	high_card[0] = 0;//�����, Ǯ�Ͽ콺 ��� ���� ���� ���� ���
	high_card[1] = 0;//�����, Ǯ�Ͽ콺 ��� �ι�°�� ���
	high_card[2] = 0;//���� ���� ī�ε�, �� ���� ī�ε�, ��Ʈ����Ʈ�� ���� ���� ����
	int pair = 0;
	int number_kind = 0;//���� ������ ����
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
	for (int i = 1; i < 7; i++) // ��Ʈ����Ʈ ����
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
	int flush = -1; //�⺻���� -1
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
		return 0;//����ī��
	if (pair == 1 && number_kind < 3 && straight < 5 && flush == -1)
		return 1;//�����
	if (pair == 2 && number_kind < 3 && straight < 5 && flush == -1)
		return 2;//�����
	if (pair == 0 && number_kind == 3 && straight < 5 && flush == -1)
		return 3;//���������ī�ε�
	if (straight >= 5 && flush == -1)
		return 4;//��Ʈ����Ʈ
	if (straight < 5 && flush >= 0)
		return 5;//�÷���
	if (pair >= 1 && number_kind == 3)
		return 6;//Ǯ�Ͽ콺
	if (number_kind == 4)
		return 7;//�������ī�ε�
	if (straight >= 5 && flush >= 0)
		return 8;//��Ʈ����Ʈ�÷���
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
	printf("��������������������������������������������");
	gotoxy(17, 6);
	printf("��������������������������������������������");
	gotoxy(17, 7);
	printf("��������������������������������������������");
	gotoxy(17, 8);
	printf("��������������������������������������������");
	gotoxy(17, 9);
	printf("��������������������������������������������");
	gotoxy(17, 10);
	printf("��������������������������������������������");
	gotoxy(17, 11);
	printf("��������������������������������������������");
	gotoxy(17, 12);
	printf("��������������������������������������������");
	gotoxy(17, 13);
	printf("��������������������������������������������");
	gotoxy(17, 14);
	printf("��������������������������������������������");
	gotoxy(17, 16);
	printf("���� ��(1)\n");
	gotoxy(17, 17);
	printf("���� ����(2)\n");
	gotoxy(17, 18);
	printf("��ȣ�� �������ֽʽÿ�. : ");
	scanf_s("%d", &num);
	return num;
}

void Poker_Pirate(char text[])       //ĳ���� ����
{
	gotoxy(45, 1); printf("*��.*.*.......*.......*��.* ");
	gotoxy(45, 3); printf("���ѤѤѤѤѤѤѤѤѤѤѦ�");
	gotoxy(45, 4); printf("�Ӥӡ�                   ��");
	gotoxy(45, 5); printf("��  ��          ��  ��    ��");
	gotoxy(45, 6); printf("��    ��       �ӤӤӤ�    ��");
	gotoxy(45, 7); printf("����    ������   ��.*��*.��    ��");
	gotoxy(45, 8); printf("������    �����ѤѤѤ�O��O�ѤѤѤѦ�");
	gotoxy(45, 9); printf("������  �� ����   ��������       ��");
	gotoxy(45, 10); printf("    ��   ��  �� ��            ��");
	gotoxy(45, 11); printf("    ��   �� ���ѤѤѤѤѤѤѦ�  ��");
	gotoxy(45, 12); printf("��  ��   �� ��             �� ��");
	gotoxy(45, 13); printf("��  �ӡ� �� ��             �� ��");     // �߰� ��ĭ ��ǳ��
	gotoxy(45, 14); printf("��  ��   �� ��             �� ��");
	gotoxy(45, 15); printf("    �ӡ� �� ���ѤѤѤѤѤѤѦ�  ��");
	gotoxy(45, 16); printf("    ��   ��                   ��");
	gotoxy(45, 17); printf("    ��   ��                   ��");
	gotoxy(45, 18); printf("��  ��   ��                   ��");
	gotoxy(45, 19); printf("    ���ѤѦ��ѤѤѤѤѤѤѤѤѤѦ�");
	gotoxy(45, 21); printf("��.....*......*......*.......��");

	gotoxy(59, 13); printf("%s", text);
}

void Poker_Card(int x, int y, int card_num, int card_shape)       //�⺻ ī�� ���, ?��� ��ȯ �ؾ���(���� ���� ���� �ؾ���)
{
	gotoxy(x, y); printf("�������������������� ");
	switch (card_shape)
	{
	case 0:
		gotoxy(x, y+1); printf("�� ��      �� ");
		break;
	case 1:
		gotoxy(x, y+1); printf("�� ��      �� ");
		break;
	case 2:
		gotoxy(x, y+1); printf("�� ��      �� ");
		break;
	case 3:
		gotoxy(x, y+1); printf("�� ��      �� ");
		break;
	case 99:
		gotoxy(x, y + 1); printf("�� ?      �� ");
		break;
	}
	gotoxy(x, y+2); printf("��        �� ");
	switch (card_num)
	{
	case 1:
		gotoxy(x, y+3); printf("��    A   �� ");
		break;
	case 11:
		gotoxy(x, y+3); printf("��    J   �� ");
		break;
	case 12:
		gotoxy(x, y+3); printf("��    Q   �� ");
		break;
	case 13:
		gotoxy(x, y+3); printf("��    K   �� ");
		break;
	case 99://����ǥ ����� ���� ���̽�
		gotoxy(x, y + 3); printf("��    ?   �� ");
		break;
	default:
		gotoxy(x, y+3); printf("��    %d   �� ", card_num);
	}
	gotoxy(x, y+4); printf("��        �� ");
	gotoxy(x, y+5); printf("��        �� ");
	gotoxy(x, y+6); printf("�������������������� ");
}

void Edge() {
	int i;
	int h = 0xa1;
	int v = 0xd8;

	for (i = 0; i <= 108; i++) {			// ���� ���Ӻ��� ������
		gotoxy(6 + i, 22);
		printf("%c%c", h, v);

		gotoxy(6 + i, 42);
		printf("%c%c", h, v);

	}

	for (i = 0; i <= 20; i++) {				// ���� ���Ӻ��� ������
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
	int num;//Ÿ��Ʋ ȭ�� ��ȣ ����
	system("cls");
	restart://Ÿ��Ʋ ����� ����
	num = title();
	if (num == 1)
	{
		system("cls");
		printf("�� ������ ��Ŀ ������ ���� �� '�ػ罺 Ȧ��'�̶�� �����Դϴ�.\n");
		printf("������ ���۵Ǹ� �÷��̾�� ��밡 ���� money�� 100�� ������ �����մϴ�.\n");
		printf("�÷��̾�� ���� ���� ī�带 2�徿 ����, ���� ���̺� ���� ī�尡 5�� ���̰� �˴ϴ�.\n");
		printf("������ ī�� 2��� ���� ī�� 5���� �����Ͽ� �� ���� ������ ���� �ִ� ����� ���� �ݾ��� �������ϴ�.\n");
		printf("�÷��̾�� �ڽ��� ī�� ������ ���� ���� �ݾ��� ������ �� ������ ���� �÷��̾��� ���� �ݾװ� ���� �����մϴ�.\n");
		printf("���������� ����� money�� ��� �������� �¸��մϴ�.\n");
		printf("\n");
		printf("<ī�� ���� ����>\n");
		printf("1. ���� ī�� : �ƹ��� ������ ���� ��\n");
		printf("2. �� ��� : ���ڰ� ���� ī�尡 �� �� ������ ��\n");
		printf("3. �� ��� : ���ڰ� ���� ī�尡 �� �� ������ ��\n");
		printf("4. ���� ���� �� ī�ε� : ���� ���� ī�尡 3�� ������ ��\n");
		printf("5. ��Ʈ����Ʈ : ���ӵ� ���� ī�尡 5�� �̾��� ��\n");
		printf("6. �÷��� : ���� ��� ī�尡 5�� ������ ��\n");
		printf("7. Ǯ �Ͽ콺 : ���� ���� ī�尡 3��, ���ڰ� ���� ī�� �� ���� ������ ��\n");
		printf("8. �� ���� �� ī�ε� : ���� ���� ī�尡 4�� ������ ��\n");
		printf("9. ��Ʈ����Ʈ �÷���: ��Ʈ����Ʈ�� �÷����� ������ ���ÿ� ������ ��. ���� ���� ������ ī�� ����.\n");
		printf("������ �����Ϸ��� '2'�� �Է��ϼ��� : ");
		scanf_s("%d", &num);
		if (num == 2)
			goto start;
	}
	else if (num == 2)
	{
		start://���� ����
		player[0].money = 100;
		player[1].money = 100;
		int win = -1;//�⺻ ���� -1
		int betting = 0; //���� �ݾ�
		for (int i = 0; i < 7; i++)
		{
			if (i == 0)
			{
				system("cls");
				Poker_Pirate("����ض��");
				Edge();
				gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
				gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
				filldeck();
				shuffle();
				deal(player, table);
				player[0].rank = check(player[0].card_player, player[0].high_card);
				player[1].rank = check(player[1].card_player, player[1].high_card);
			}
			if (i == 1)
			{
				system("cls");
				Poker_Pirate("������ ���ٳ�");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
				gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
			}
			if (i == 2)
			{
				system("cls");
				Poker_Pirate("������!!!!!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
				gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
			}
			if (i == 3)
			{
				system("cls");
				Poker_Pirate("���ٳ�!!!!!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4��° ����ī��
				gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
				gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
			}
			if (i == 4)
			{
				input:
				system("cls");
				Poker_Pirate("�����ض��!");
				Edge();
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
				for (int i = 0; i < 2; i++)
					Poker_Card(50 + 12 * i, 23, 99, 99);
				for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
					Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
				Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4��° ����ī��
				Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5��° ����ī��
				gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
				gotoxy(6, 20); printf("���� �ݾ�: %d\t������ �ݾ�: ", player[0].money); scanf_s("%d", &betting);
				if (betting > player[0].money || betting > player[1].money)//���ñݾ��� �ڽŰ� ����� money�� �ʰ����� ����
					goto input;
			}
			if (i == 5)
			{
				//��,��,��ȿ �Ǵ�
				if (player[0].rank > player[1].rank)
					win = 0;
				else if (player[0].rank < player[1].rank)
					win = 1;
				else//player[0].rank == player[1].rank�� ���
				{
					if (player[0].rank == 0 || player[0].rank == 1 || player[0].rank == 2)//����ī��, �����, �����
					{
						if (player[0].high_card[0] > player[1].high_card[0])
							win = 0;
						else if (player[0].high_card[0] < player[1].high_card[0])
							win = 1;
						else
						{
							if (player[0].rank == 2)//������� ���� ���� ���� �� ������
							{
								if (player[0].high_card[1] > player[1].high_card[1])//�ι�° ��� ��
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
					if (player[0].rank == 3 || player[0].rank == 4 || player[0].rank == 6 || player[0].rank == 7 || player[0].rank == 8)//���������ī�ε�, ��Ʈ����Ʈ, Ǯ�Ͽ콺, �������ī�ε�, ��Ʈ����Ʈ�÷���
					{
						if (player[0].high_card[2] > player[1].high_card[2])
							win = 0;
						else if (player[0].high_card[2] < player[1].high_card[2])
							win = 1;
						else
						{
							if (player[0].rank == 6)//Ǯ�Ͽ콺�� ��������ī�ε� ���ڰ� ������
							{
								if (player[0].high_card[0] > player[1].high_card[0])//��� ��
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
					if (player[0].rank == 5)//�÷���
						win = 2;
				}
				if (win == 0)//�̰��� ���
				{
					system("cls");
					Poker_Pirate("�ȵȴٳ�!!");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4��° ����ī��
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5��° ����ī��
					player[0].money += betting;
					player[1].money -= betting;
					gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
					gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
				}
				else if (win == 1)//���� ���
				{
					system("cls");
					Poker_Pirate("�ų��ٳ�!!");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4��° ����ī��
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5��° ����ī��
					player[0].money -= betting;
					player[1].money += betting;
					gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
					gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
				}
				else//��ȿ��
				{
					system("cls");
					Poker_Pirate("��ȿ�ٳ�..");
					Edge();
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 35, player[0].card_player[i].number, player[0].card_player[i].shape);
					for (int i = 0; i < 2; i++)
						Poker_Card(50 + 12 * i, 23, player[1].card_player[i].number, player[1].card_player[i].shape);
					for (int i = 0; i < 3; i++)//ó�� ���̺� ����ī�� 3��
						Poker_Card(10 + 12 * i, 30, table[0].card_tabel[i].number, table[0].card_tabel[i].shape);
					Poker_Card(85, 30, table[0].card_tabel[3].number, table[0].card_tabel[3].shape); //4��° ����ī��
					Poker_Card(85 + 12, 30, table[0].card_tabel[4].number, table[0].card_tabel[4].shape); //5��° ����ī��
					gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
					gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
				}
			}
			if (i == 6)
			{
				if (player[0].money <= 0)
				{
					system("cls");
					Poker_Pirate("�ʹ� ���ٳ�~");
					Edge();
					gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
					gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
				}
				else if (player[1].money <= 0)
				{
					system("cls");
					Poker_Pirate("Ū ���ϴٳ�..");
					Edge();
					gotoxy(6, 19); printf("��� ���� �ݾ�: %d", player[1].money);
					gotoxy(6, 20); printf("���� �ݾ�: %d", player[0].money);
					gotoxy(1, 43);//���� ������ ȭ�� �Ʒ��ʿ��� �ߵ��� ������(�� �ǹ̴� ����)
				}
				else
					i = -1;//�ٽ� for�� ó������ �ݺ�
			}
			getch();
		}
	}
	else//�߸� �Է��ϸ�
		goto restart;
}