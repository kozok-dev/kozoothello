#include <stdio.h>
#define TEXT 0

/* ��΃Q�[���� */
typedef struct book_tree {
	char move, eval;
	struct book_tree *sibling, *child;
} BOOK_TREE;

/* ��Ε\ */
typedef struct book_table {
	/* bitboard ���g���G���f�B�A���Ƃ���̂�a1�ɐ΂��������ꍇ�A0x0000000000000001�ƂȂ�Ah8����0x8000000000000000�ƂȂ� */
	unsigned long long black;	/* ���r�b�g */
	unsigned long long white;	/* ���r�b�g */
	char turn, eval;
} BOOK_TABLE;
BOOK_TABLE *book_table = NULL;
int book_table_count = 0;

/* move���w�肵�������Ɉړ� */
unsigned long long transfer(unsigned long long move, const int direction)
{
	switch (direction) {
	case 0:	/* �� */
		return move >> 8;
	case 1:	/* �E�� */
		return move >> 7 & 0x00fefefefefefefeULL;
	case 2:	/* �E */
		return move << 1 & 0xfefefefefefefefeULL;
	case 3:	/* �E�� */
		return move << 9 & 0xfefefefefefefe00ULL;
	case 4:	/* �� */
		return move << 8;
	case 5:	/* ���� */
		return move << 7 & 0x7f7f7f7f7f7f7f00ULL;
	case 6:	/* �� */
		return move >> 1 & 0x7f7f7f7f7f7f7f7fULL;
	default:	/* ���� */
		return move >> 9 & 0x007f7f7f7f7f7f7fULL;
	}
}

/* �΂�Ԃ� */
unsigned long long flip(unsigned long long *black, unsigned long long *white, unsigned long long move)
{
	unsigned long long rev, rev_dir, mask;
	int i;

	if (((*black | *white) & move) != 0) return 0;	/* �󏊂łȂ� */

	rev = 0;
	for (i = 0; i < 8; i++) {
		rev_dir = 0;
		mask = transfer(move, i);

		/* ����̐΂������ԁA���]�p�r�b�g��ݒ肷�� */
		while (mask != 0 && (mask & *white) != 0) {
			rev_dir |= mask;
			mask = transfer(mask, i);
		}

		/* ����̐΂���������A�����̐΂������̂Ŕ��]�p�r�b�g��K�p���� */
		if ((mask & *black) != 0) rev |= rev_dir;
	}

	if (rev == 0) return 0;

	/* �΂��u����̂ł���܂ł̃r�b�g��o���̃r�b�g�ɓK�p���� */
	*black ^= move | rev;
	*white ^= rev;
	return rev;
}

/* �Q�[���؂����Ε\�𐶐����� */
int create_book_table(unsigned long long black, unsigned long long white, char turn, BOOK_TREE *bt)
{
	unsigned long long move, rev;
	int i, j;
	BOOK_TABLE *ptmp, tmp;

	while (bt != NULL) {
		/* ���� */
		move = 1ULL << bt->move;
		rev = flip(&black, &white, move);
		if (rev == 0) {
			/* ����ł��Ȃ������B���蓾�Ȃ����A�O�̂��� */
			return 0;
		}

		/* ��Ε\�̗̈�ǉ� */
		if (book_table_count % 10000 == 0) {
			/* ����񐔖��ɗ̈�m�� */
			book_table = (BOOK_TABLE *)realloc(book_table, (book_table_count / 10000 + 1) * 10000 * sizeof(BOOK_TABLE));
		}
		book_table_count++;
		ptmp = &book_table[book_table_count - 1];
		printf("\b\b\b\b\b\b%6d", book_table_count);

		/* ��Ε\�ɒǉ� */
		if (turn == 0) {
			ptmp->black = white;
			ptmp->white = black;
		} else {
			ptmp->black = black;
			ptmp->white = white;
		}
		ptmp->turn = turn;
		ptmp->eval = -bt->eval;

		/* ��Ε\��Ֆʂ̍��A���̔z�u���ŕ��בւ� */
		for (i = book_table_count - 1; i > 0; i--) {
			if (book_table[i - 1].black < book_table[i].black) {
				break;

			} else if (book_table[i - 1].black == book_table[i].black) {
				/* ���͓����z�u�Ȃ̂ŁA���̔z�u�ŕ��בւ� */
				for (j = i; j > 0 && book_table[j - 1].black == book_table[j].black; j--) {
					if (book_table[j - 1].white < book_table[j].white) {
						break;
					} else if (book_table[j - 1].white == book_table[j].white) {
						/* �������������� */
						break;
					} else {
						tmp = book_table[j - 1];
						book_table[j - 1] = book_table[j];
						book_table[j] = tmp;
					}
				}

			} else {
				tmp = book_table[i - 1];
				book_table[i - 1] = book_table[i];
				book_table[i] = tmp;
			}
		}

		if (bt->child != NULL) {
			if (!create_book_table(white, black, 1 - turn, bt->child)) {
				return 0;
			}
		}

		/* �΂�߂� */
		black ^= move | rev;
		white ^= rev;

		bt = bt->sibling;
	}

	return 1;
}

/* �Q�[���؂̊e�m�[�h�Ƀ~�j�}�b�N�X�@�Ɋ�Â����]���l��ݒ肷�� */
char eval_book_tree(BOOK_TREE *bt)
{
	char max_eval = -127;

	while (bt != NULL) {
		if (bt->child != NULL) bt->eval = -eval_book_tree(bt->child);
		if (bt->eval > max_eval) max_eval = bt->eval;

		bt = bt->sibling;
	}

	return max_eval;
}

/* ��Ί����f�[�^��ǂݍ��݁A�Q�[���؂𐶐��B�]���l�͖��[�m�[�h�̂ݐݒ肷�� */
BOOK_TREE *create_book_tree()
{
	char book[200], buf[200], move;
	int eval, break_flag, i;
	FILE *f;
	BOOK_TREE *bt, *bt_start;

	f = fopen("book.txt", "r");
	if (f == NULL) return NULL;

	/* �����f5 */
	bt = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
	if (bt == NULL) return NULL;
	bt->move = 37;
	bt->eval = 0;
	bt->sibling = NULL;
	bt->child = NULL;
	bt_start = bt;

	while (fgets(buf, 200, f) != NULL) {
		/* �����łȂ��Ȃ��΂� */
		if (buf[0] != '\t' || buf[1] != '\t' || buf[2] == '/') continue;

		/* ���ɂƂ��Ă̕]���l�Ɗ�����ǂݍ���(�u+2,d6,c3...,"\0",�v�Ƃ����`��) */
		sscanf(buf, "\t\t%3d,%s", &eval, book);

		/* �Q�[���؂ɑ��݂��Ȃ���ɂȂ�܂Ŕ�΂� */
		bt = bt_start;
		i = 0;
		while (bt->child != NULL) {
			if (book[i] == '\0' || book[i] == '"') {
				/* �����̍Ō�܂Ŋm�F���� */
				i = -1;
				break;
			}

			bt = bt->child;
			move = (book[i] - 'a') + (book[i + 1] - '1') * 8;

			break_flag = 0;
			for (;;) {
				if (bt->move == move) {
					i += 3;

					if (bt->child == NULL) {
						/* �\�z�ς݂̃Q�[���؂�肳��Ɏ肪������������� */
						break_flag = 1;
						break;
					}

					/* �Q�[���؂ɍ\�z�ς݂̎�Ȃ̂ŁA�q�m�[�h�� */
					break;
				}

				if (bt->sibling == NULL) {
					/* �Q�[���؂ɑ��݂��Ȃ���Ȃ̂ŁA�Z��m�[�h��ǉ����A��������c��̊������Q�[���؂ɍ\�z�ł���悤�ɂ��� */
					bt->sibling = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
					if (bt->sibling == NULL) return NULL;
					bt = bt->sibling;
					bt->move = move;
					bt->sibling = NULL;

					i += 3;
					break_flag = 1;
					break;
				}

				/* ���̃m�[�h�ō\�z�ς݂̎肩������Ȃ��̂ŁA�Z��m�[�h�� */
				bt = bt->sibling;
			}
			if (break_flag) break;
		}

		if (i == -1) continue;

		/* ���݂��Ȃ��菇���Q�[���؂ɍ\�z���� */
		while (book[i] != '\0' && book[i] != '"') {
			bt->child = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
			if (bt->child == NULL) return NULL;

			bt = bt->child;
			bt->move = (book[i] - 'a') + (book[i + 1] - '1') * 8;
			bt->sibling = NULL;

			i += 3;
		}

		/* ���[�m�[�h */
		bt->eval = (i / 3 % 2 > 0 ? -eval : eval);
		bt->child = NULL;
	}

	fclose(f);
	return bt_start;
}

void main()
{
	unsigned long long black, white;
	int i, j, k;
	FILE *f;
	BOOK_TREE *bt;

	printf("Create book tree\n");
	bt = create_book_tree();
	if (bt == NULL) return;

	printf("Evaluate book tree\n");
	eval_book_tree(bt);

	/* �����Ֆ� */
	black = 0x0000000810000000ULL;
	white = 0x0000001008000000ULL;

	printf("Create book table       ");
	if (!create_book_table(black, white, 1, bt)) return;

	/* ��Ε\�o�� */
	printf("\nWrite book table\n");
	f = fopen("book.dat", "wb");
	if (f == NULL) return;
	for (i = 0; i < book_table_count; i++) {
		/* f5d6c3d3c4��f5d6c4d3c3���A����������Ă��Ֆʂ͓����ɂȂ�ꍇ������̂ŁA�d�������ꍇ�͏o�͂��Ȃ� */
		if (i > 0 && book_table[i].black == book_table[i - 1].black && book_table[i].white == book_table[i - 1].white) continue;

#if TEXT == 0
		if (fwrite(&book_table[i].black, sizeof(unsigned long long), 1, f) != 1) return;
		if (fwrite(&book_table[i].white, sizeof(unsigned long long), 1, f) != 1) return;
		if (fwrite(&book_table[i].turn, sizeof(char), 1, f) != 1) return;
		if (fwrite(&book_table[i].eval, sizeof(char), 1, f) != 1) return;
		/* 4�̔{���ɂ��邽�߁A�_�~�[���o�� */
		if (fputc('\0', f) == EOF) return;
		if (fputc('\0', f) == EOF) return;
#else
		if (fprintf(f, "0x%08x%08x 0x%08x%08x %d %+d\n",
			(unsigned int)(book_table[i].black >> 32), (unsigned int)(book_table[i].black & 0xffffffffU),
			(unsigned int)(book_table[i].white >> 32), (unsigned int)(book_table[i].white & 0xffffffffU),
			book_table[i].turn, book_table[i].eval
		) < 0) return;
#endif
	}
	fclose(f);

	/* ���荕f5����� */
	black = 0x0000003810000000ULL;
	white = 0x0000000008000000ULL;
	for (i = 0; i < 64; i++) {
		if (i > 0 && i % 8 == 0) printf("\n");
		if ((black >> i & 1ULL) != 0) {
			printf("B ");
		} else if ((white >> i & 1ULL) != 0) {
			printf("W ");
		} else {
			printf("- ");
		}
	}

	/* ��Ε\�͔Ֆʂ̏����ɕ��ׂ��Ă���̂Ō�����Ƃ��āA�܂����̔Ֆʂ�񕪒T����A�Ώۂ��i���Ă���͂��Ȃ̂Ŕ��͐��`�������� */
	i = 0;
	j = book_table_count - 1;
	k = 0;
	while (i <= j) {
		int m = (i + j) / 2;

		if (black == book_table[m].black) {
			/* �����������̔Ֆʂ̐擪�ʒu�܂ňړ� */
			while (m > 0 && book_table[m].black == book_table[m - 1].black) m--;

			while (m < book_table_count && black == book_table[m].black) {
				if (white == book_table[m].white) {
					/* �������� */
					printf("%+d\n", book_table[m].eval);
					k = 1;
				}
				m++;
			}
			break;

		} else if (black < book_table[m].black) {
			j = m - 1;

		} else {
			i = m + 1;
		}
	}
	if (!k) printf("Not found\n");
}
