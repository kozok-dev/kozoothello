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

	/* �o�C�i���t�@�C���̓ǂݍ��� */
	f = fopen("bin.dat", "rb");
	if (f == NULL) return;

	/* �f�[�^�T�C�Y�擾 */
	if (fseek(f, 0, SEEK_END) != 0) return;
	size = ftell(f);
	if (size < 1) return;
	rewind(f);

	/* �o�C�i���f�[�^���������ɓǂݍ��� */
	data = (char *)malloc(size);
	if (data == NULL) return;
	if (fread(data, 1, size, f) != size) return;
	fclose(f);

	pixel_count = (size + (3 - size % 3) % 3) / 3;
	width = ceil(sqrt(pixel_count));	/* �����`�ɍł��߂��� */

	/* pixel_count���犄��؂�钆�łȂ�ׂ������`�ɋ߂������v�Z���� */
	tmp = pixel_count;
	for (i = pixel_count - 1; i > 0; i--) {
		if (i - width < 0) break;	/* �摜���c���ɂȂ�̂͋p�� */
		if (pixel_count % i == 0 && i - width < tmp) {
			bestWidth = i;
			tmp = i - width;
		}
	}
	if (tmp != pixel_count && ((float)pixel_count / (float)bestWidth) / (float)bestWidth >= 0.4) {
		// ����؂�钆�ŉ����߂��Ȃ���������
		width = bestWidth;
	}

	bmfh.bfType = 'B' | 'M' << 8;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV4HEADER);

	/* BMP�w�b�_�[�ݒ� */
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

	/* BMP�̏������� */
	f = fopen("bin.bmp", "wb");
	if (f == NULL) return;

	if (fwrite(&bmfh, sizeof(BITMAPFILEHEADER), 1, f) != 1) return;
	if (fwrite(&bmh, sizeof(BITMAPV4HEADER), 1, f) != 1) return;

	width = bmh.bV4Width * (bmh.bV4BitCount / 8);
	padding = (4 - width % 4) % 4;

	/* BMP�͒ʏ퍶������Ȃ̂ōŌ�̍s����o�͂��� */
	for (i = bmh.bV4Height - 1; i >= 0; i--) {
		tmp = i * width;	/* �J�n�ʒu */

		if (tmp + width <= size) {
			/* �����s�̃s�N�Z���̒[�܂ŏo�͂��ׂ��f�[�^������ */
			if (fwrite(&data[tmp], 1, width, f) != width) return;
			tmp = 0;	/* �����s�̃_�~�[�f�[�^�͕K�v�Ȃ� */

		} else if (tmp < size) {
			/* �o�͂��ׂ��f�[�^�͂��邪�A�����s�̃s�N�Z���̓r���ŏI��� */
			if (fwrite(&data[tmp], 1, size - tmp, f) != size - tmp) return;
			tmp = width - (size - tmp);	/* �c��̃s�N�Z���̓_�~�[�f�[�^ */

		} else {
			/* �o�͂��ׂ��f�[�^�͂Ȃ��̂ŁA�����s�S�ă_�~�[�f�[�^ */
		}

		/* �_�~�[�f�[�^�o�� */
		tmp += padding;	/* BMP�͕���4�̔{���ɂ���K�v���� */
		while (tmp > 0) {
			if (fputc('\0', f) == EOF) return;
			tmp--;
		}
	}

	fclose(f);
	printf("Done");
}
