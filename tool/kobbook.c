#include <stdio.h>
#define TEXT 0

/* 定石ゲーム木 */
typedef struct book_tree {
	char move, eval;
	struct book_tree *sibling, *child;
} BOOK_TREE;

/* 定石表 */
typedef struct book_table {
	/* bitboard リトルエンディアンとするのでa1に石があった場合、0x0000000000000001となり、h8だと0x8000000000000000となる */
	unsigned long long black;	/* 黒ビット */
	unsigned long long white;	/* 白ビット */
	char turn, eval;
} BOOK_TABLE;
BOOK_TABLE *book_table = NULL;
int book_table_count = 0;

/* moveを指定した方向に移動 */
unsigned long long transfer(unsigned long long move, const int direction)
{
	switch (direction) {
	case 0:	/* 上 */
		return move >> 8;
	case 1:	/* 右上 */
		return move >> 7 & 0x00fefefefefefefeULL;
	case 2:	/* 右 */
		return move << 1 & 0xfefefefefefefefeULL;
	case 3:	/* 右下 */
		return move << 9 & 0xfefefefefefefe00ULL;
	case 4:	/* 下 */
		return move << 8;
	case 5:	/* 左下 */
		return move << 7 & 0x7f7f7f7f7f7f7f00ULL;
	case 6:	/* 左 */
		return move >> 1 & 0x7f7f7f7f7f7f7f7fULL;
	default:	/* 左上 */
		return move >> 9 & 0x007f7f7f7f7f7f7fULL;
	}
}

/* 石を返す */
unsigned long long flip(unsigned long long *black, unsigned long long *white, unsigned long long move)
{
	unsigned long long rev, rev_dir, mask;
	int i;

	if (((*black | *white) & move) != 0) return 0;	/* 空所でない */

	rev = 0;
	for (i = 0; i < 8; i++) {
		rev_dir = 0;
		mask = transfer(move, i);

		/* 相手の石が続く間、反転用ビットを設定する */
		while (mask != 0 && (mask & *white) != 0) {
			rev_dir |= mask;
			mask = transfer(mask, i);
		}

		/* 相手の石が続いた後、自分の石だったので反転用ビットを適用する */
		if ((mask & *black) != 0) rev |= rev_dir;
	}

	if (rev == 0) return 0;

	/* 石が置けるのでこれまでのビットを双方のビットに適用する */
	*black ^= move | rev;
	*white ^= rev;
	return rev;
}

/* ゲーム木から定石表を生成する */
int create_book_table(unsigned long long black, unsigned long long white, char turn, BOOK_TREE *bt)
{
	unsigned long long move, rev;
	int i, j;
	BOOK_TABLE *ptmp, tmp;

	while (bt != NULL) {
		/* 着手 */
		move = 1ULL << bt->move;
		rev = flip(&black, &white, move);
		if (rev == 0) {
			/* 着手できなかった。あり得ないが、念のため */
			return 0;
		}

		/* 定石表の領域追加 */
		if (book_table_count % 10000 == 0) {
			/* 特定回数毎に領域確保 */
			book_table = (BOOK_TABLE *)realloc(book_table, (book_table_count / 10000 + 1) * 10000 * sizeof(BOOK_TABLE));
		}
		book_table_count++;
		ptmp = &book_table[book_table_count - 1];
		printf("\b\b\b\b\b\b%6d", book_table_count);

		/* 定石表に追加 */
		if (turn == 0) {
			ptmp->black = white;
			ptmp->white = black;
		} else {
			ptmp->black = black;
			ptmp->white = white;
		}
		ptmp->turn = turn;
		ptmp->eval = -bt->eval;

		/* 定石表を盤面の黒、白の配置順で並べ替え */
		for (i = book_table_count - 1; i > 0; i--) {
			if (book_table[i - 1].black < book_table[i].black) {
				break;

			} else if (book_table[i - 1].black == book_table[i].black) {
				/* 黒は同じ配置なので、白の配置で並べ替え */
				for (j = i; j > 0 && book_table[j - 1].black == book_table[j].black; j--) {
					if (book_table[j - 1].white < book_table[j].white) {
						break;
					} else if (book_table[j - 1].white == book_table[j].white) {
						/* 同じ棋譜だった */
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

		/* 石を戻す */
		black ^= move | rev;
		white ^= rev;

		bt = bt->sibling;
	}

	return 1;
}

/* ゲーム木の各ノードにミニマックス法に基づいた評価値を設定する */
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

/* 定石棋譜データを読み込み、ゲーム木を生成。評価値は末端ノードのみ設定する */
BOOK_TREE *create_book_tree()
{
	char book[200], buf[200], move;
	int eval, break_flag, i;
	FILE *f;
	BOOK_TREE *bt, *bt_start;

	f = fopen("book.txt", "r");
	if (f == NULL) return NULL;

	/* 初手はf5 */
	bt = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
	if (bt == NULL) return NULL;
	bt->move = 37;
	bt->eval = 0;
	bt->sibling = NULL;
	bt->child = NULL;
	bt_start = bt;

	while (fgets(buf, 200, f) != NULL) {
		/* 棋譜でないなら飛ばす */
		if (buf[0] != '\t' || buf[1] != '\t' || buf[2] == '/') continue;

		/* 黒にとっての評価値と棋譜を読み込む(「+2,d6,c3...,"\0",」という形式) */
		sscanf(buf, "\t\t%3d,%s", &eval, book);

		/* ゲーム木に存在しない手になるまで飛ばす */
		bt = bt_start;
		i = 0;
		while (bt->child != NULL) {
			if (book[i] == '\0' || book[i] == '"') {
				/* 棋譜の最後まで確認した */
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
						/* 構築済みのゲーム木よりさらに手がある棋譜だった */
						break_flag = 1;
						break;
					}

					/* ゲーム木に構築済みの手なので、子ノードへ */
					break;
				}

				if (bt->sibling == NULL) {
					/* ゲーム木に存在しない手なので、兄弟ノードを追加し、そこから残りの棋譜をゲーム木に構築できるようにする */
					bt->sibling = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
					if (bt->sibling == NULL) return NULL;
					bt = bt->sibling;
					bt->move = move;
					bt->sibling = NULL;

					i += 3;
					break_flag = 1;
					break;
				}

				/* 他のノードで構築済みの手かもしれないので、兄弟ノードへ */
				bt = bt->sibling;
			}
			if (break_flag) break;
		}

		if (i == -1) continue;

		/* 存在しない手順をゲーム木に構築する */
		while (book[i] != '\0' && book[i] != '"') {
			bt->child = (BOOK_TREE *)malloc(sizeof(BOOK_TREE));
			if (bt->child == NULL) return NULL;

			bt = bt->child;
			bt->move = (book[i] - 'a') + (book[i + 1] - '1') * 8;
			bt->sibling = NULL;

			i += 3;
		}

		/* 末端ノード */
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

	/* 初期盤面 */
	black = 0x0000000810000000ULL;
	white = 0x0000001008000000ULL;

	printf("Create book table       ");
	if (!create_book_table(black, white, 1, bt)) return;

	/* 定石表出力 */
	printf("\nWrite book table\n");
	f = fopen("book.dat", "wb");
	if (f == NULL) return;
	for (i = 0; i < book_table_count; i++) {
		/* f5d6c3d3c4とf5d6c4d3c3等、棋譜が違っても盤面は同じになる場合があるので、重複した場合は出力しない */
		if (i > 0 && book_table[i].black == book_table[i - 1].black && book_table[i].white == book_table[i - 1].white) continue;

#if TEXT == 0
		if (fwrite(&book_table[i].black, sizeof(unsigned long long), 1, f) != 1) return;
		if (fwrite(&book_table[i].white, sizeof(unsigned long long), 1, f) != 1) return;
		if (fwrite(&book_table[i].turn, sizeof(char), 1, f) != 1) return;
		if (fwrite(&book_table[i].eval, sizeof(char), 1, f) != 1) return;
		/* 4の倍数にするため、ダミーを出力 */
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

	/* 初手黒f5着手後 */
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

	/* 定石表は盤面の昇順に並べられているので検索例として、まず黒の盤面を二分探索後、対象が絞られているはずなので白は線形検索する */
	i = 0;
	j = book_table_count - 1;
	k = 0;
	while (i <= j) {
		int m = (i + j) / 2;

		if (black == book_table[m].black) {
			/* 検索した黒の盤面の先頭位置まで移動 */
			while (m > 0 && book_table[m].black == book_table[m - 1].black) m--;

			while (m < book_table_count && black == book_table[m].black) {
				if (white == book_table[m].white) {
					/* 見つかった */
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
