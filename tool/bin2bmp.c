#include <stdio.h>
#include <math.h>
#pragma pack(1)

typedef struct {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
	int ciexyzX;
	int ciexyzY;
	int ciexyzZ;
} CIEXYZ;

typedef struct {
	CIEXYZ ciexyzRed;
	CIEXYZ ciexyzGreen;
	CIEXYZ ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
	unsigned int bV4Size;
	int bV4Width;
	int bV4Height;
	unsigned short bV4Planes;
	unsigned short bV4BitCount;
	unsigned int bV4V4Compression;
	unsigned int bV4SizeImage;
	int bV4XPelsPerMeter;
	int bV4YPelsPerMeter;
	unsigned int bV4ClrUsed;
	unsigned int bV4ClrImportant;
	unsigned int bV4RedMask;
	unsigned int bV4GreenMask;
	unsigned int bV4BlueMask;
	unsigned int bV4AlphaMask;
	unsigned int bV4CSType;
	CIEXYZTRIPLE bV4Endpoints;
	unsigned int bV4GammaRed;
	unsigned int bV4GammaGreen;
	unsigned int bV4GammaBlue;
} BITMAPV4HEADER;

void main()
{
	char *data;
	int size, pixel_count, width, padding;
	int bestWidth, tmp, i;
	FILE *f;
	BITMAPFILEHEADER bmfh;
	BITMAPV4HEADER bmh;

	/* バイナリファイルの読み込み */
	f = fopen("bin.dat", "rb");
	if (f == NULL) return;

	/* データサイズ取得 */
	if (fseek(f, 0, SEEK_END) != 0) return;
	size = ftell(f);
	if (size < 1) return;
	rewind(f);

	/* バイナリデータをメモリに読み込む */
	data = (char *)malloc(size);
	if (data == NULL) return;
	if (fread(data, 1, size, f) != size) return;
	fclose(f);

	pixel_count = (size + (3 - size % 3) % 3) / 3;
	width = ceil(sqrt(pixel_count));	/* 正方形に最も近い幅 */

	/* pixel_countから割り切れる中でなるべく正方形に近い幅を計算する */
	tmp = pixel_count;
	for (i = pixel_count - 1; i > 0; i--) {
		if (i - width < 0) break;	/* 画像が縦長になるのは却下 */
		if (pixel_count % i == 0 && i - width < tmp) {
			bestWidth = i;
			tmp = i - width;
		}
	}
	if (tmp != pixel_count && ((float)pixel_count / (float)bestWidth) / (float)bestWidth >= 0.4) {
		// 割り切れる中で横長過ぎない幅だった
		width = bestWidth;
	}

	bmfh.bfType = 'B' | 'M' << 8;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV4HEADER);

	/* BMPヘッダー設定 */
	bmh.bV4Size = sizeof(BITMAPV4HEADER);
	bmh.bV4Width = width;
	bmh.bV4Height = (int)ceil((float)pixel_count / (float)width);
	bmh.bV4Planes = 1;
	bmh.bV4BitCount = 24;
	bmh.bV4V4Compression = 0;
	bmh.bV4SizeImage = bmh.bV4Width * bmh.bV4Height * (bmh.bV4BitCount / 8);
	bmh.bV4XPelsPerMeter = 0;
	bmh.bV4YPelsPerMeter = 0;
	bmh.bV4ClrUsed = 0;
	bmh.bV4ClrImportant = 0;
	bmh.bV4RedMask = 0;
	bmh.bV4GreenMask = 0;
	bmh.bV4BlueMask =0;
	bmh.bV4AlphaMask = 0;
	bmh.bV4CSType = 0x73524742;
	bmh.bV4Endpoints.ciexyzRed.ciexyzX = 0;
	bmh.bV4Endpoints.ciexyzRed.ciexyzY = 0;
	bmh.bV4Endpoints.ciexyzRed.ciexyzZ = 0;
	bmh.bV4Endpoints.ciexyzGreen.ciexyzX = 0;
	bmh.bV4Endpoints.ciexyzGreen.ciexyzY = 0;
	bmh.bV4Endpoints.ciexyzGreen.ciexyzZ = 0;
	bmh.bV4Endpoints.ciexyzBlue.ciexyzX = 0;
	bmh.bV4Endpoints.ciexyzBlue.ciexyzY = 0;
	bmh.bV4Endpoints.ciexyzBlue.ciexyzZ = 0;
	bmh.bV4GammaRed = 0;
	bmh.bV4GammaGreen = 0;
	bmh.bV4GammaBlue = 0;

	bmfh.bfSize = bmfh.bfOffBits + bmh.bV4SizeImage;

	printf("Size = %d, Image size = %d, Width = %d, Height = %d\n", size, bmh.bV4SizeImage, bmh.bV4Width, bmh.bV4Height);

	/* BMPの書き込み */
	f = fopen("bin.bmp", "wb");
	if (f == NULL) return;

	if (fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, f) != 1) return;
	if (fwrite(&bmh, sizeof(BITMAPV4HEADER), 1, f) != 1) return;

	width = bmh.bV4Width * (bmh.bV4BitCount / 8);
	padding = (4 - width % 4) % 4;

	/* BMPは通常左下からなので最後の行から出力する */
	for (i = bmh.bV4Height - 1; i >= 0; i--) {
		tmp = i * width;	/* 開始位置 */

		if (tmp + width <= size) {
			/* 処理行のピクセルの端まで出力すべきデータがある */
			if (fwrite(&data[tmp], 1, width, f) != width) return;
			tmp = 0;	/* 処理行のダミーデータは必要ない */

		} else if (tmp < size) {
			/* 出力すべきデータはあるが、処理行のピクセルの途中で終わる */
			if (fwrite(&data[tmp], 1, size - tmp, f) != size - tmp) return;
			tmp = width - (size - tmp);	/* 残りのピクセルはダミーデータ */

		} else {
			/* 出力すべきデータはないので、処理行全てダミーデータ */
		}

		/* ダミーデータ出力 */
		tmp += padding;	/* BMPは幅を4の倍数にする必要あり */
		while (tmp > 0) {
			if (fputc('\0', f) == EOF) return;
			tmp--;
		}
	}

	fclose(f);
	printf("Done");
}
