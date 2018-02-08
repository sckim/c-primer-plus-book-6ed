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
	int widthStep; // 라인 당 바이트 (bytes per a line)
	BYTE *lpImg; // 입력 데이터 포인터 (pointer for input image data)
	BYTE *lpOutImg; // 출력 데이터 포인터 (pointer for output image data)
	int x, y;
	if (argc < 3) {
		printf("Insufficient Input Arguments \n");
		printf(" invertImage input_file ouput_file \n");
		return -1;
	}
*/

	// 입력 영상 파일을 연다
	//file = fopen(argv[1], "rb");
	fp = fopen("lena_gray.bmp", "rb");
	if (fp == NULL) {
		printf("이미지 파일을 열 수 없습니다! \n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp); // 파일 헤더 읽음
	if (hf.bfType != 0x4D42) { // BMP 포맷 ('BM') 인지를 확인
		printf("BMP 파일이 아닙니다. \n");
		return -1;
	}
	
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp); // 비트맵 정보헤더 읽음
	printf("Size: (%3dx%3d) \n", hInfo.biWidth, hInfo.biHeight); // 크기정보 출력
															 // 8bit gray-scale 영상을 확인
	if (hInfo.biBitCount != 8 || hInfo.biClrUsed != 0) {
		printf("8bit gray-scale 영상이 아닙니다..!! \n");
		return -1;
	}
	
	// Lookup Table 읽음
	fread(hRGB, sizeof(RGBQUAD), 256, fp); // Lookup Table 읽음
											// 입력 데이터를 위한 라인당 바이트 수 계산
	
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE *Image = (BYTE *)malloc(ImgSize);
	// 결과 데이터를 저장하기 위한 메모리 할당
	BYTE *Output = (BYTE *)malloc(ImgSize);

	// 입력영상에서 영상 데이터를 읽음
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	// 영상 반전 연산
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

	// 메모리 해제|
	free(Image);
	free(Output);

	return 0;
}