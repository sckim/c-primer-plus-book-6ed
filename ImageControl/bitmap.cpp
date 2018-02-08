#pragma warning (disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main(void)
{
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE *fp;
/*
	int widthStep; // ���� �� ����Ʈ (bytes per a line)
	BYTE *lpImg; // �Է� ������ ������ (pointer for input image data)
	BYTE *lpOutImg; // ��� ������ ������ (pointer for output image data)
	int x, y;
	if (argc < 3) {
		printf("Insufficient Input Arguments \n");
		printf(" invertImage input_file ouput_file \n");
		return -1;
	}
*/

	// �Է� ���� ������ ����
	//file = fopen(argv[1], "rb");
	fp = fopen("lena_gray.bmp", "rb");
	if (fp == NULL) {
		printf("�̹��� ������ �� �� �����ϴ�! \n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); // ���� ��� ����
	if (hf.bfType != 0x4D42) { // BMP ���� ('BM') ������ Ȯ��
		printf("BMP ������ �ƴմϴ�. \n");
		return -1;
	}
	
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp); // ��Ʈ�� ������� ����
	printf("Size: (%3dx%3d) \n", hInfo.biWidth, hInfo.biHeight); // ũ������ ���
															 // 8bit gray-scale ������ Ȯ��
	if (hInfo.biBitCount != 8 || hInfo.biClrUsed != 0) {
		printf("8bit gray-scale ������ �ƴմϴ�..!! \n");
		return -1;
	}
	
	// Lookup Table ����
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // Lookup Table ����
											// �Է� �����͸� ���� ���δ� ����Ʈ �� ���
	
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE *Image = (BYTE *)malloc(ImgSize);
	// ��� �����͸� �����ϱ� ���� �޸� �Ҵ�
	BYTE *Output = (BYTE *)malloc(ImgSize);

	// �Է¿��󿡼� ���� �����͸� ����
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	// ���� ���� ����
	for (int i = 0; i < ImgSize; i++) {
		Output[i] = 255 - Image[i];
	}

	//fp = fopen(argv[2], "wb");
	fp = fopen("output.bmp", "wb");

	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	// �޸� ����|
	free(Image);
	free(Output);

	return 0;
}