#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#define INPUT_WIDTH 256
#define INPUT_HEIGHT 256
#define OUTPUT_WIDTH 256
#define OUTPUT_HEIGHT 256

int rotation(int h, int w, double radian);

int input[INPUT_HEIGHT][INPUT_WIDTH] = { 0, };
int output[INPUT_HEIGHT][INPUT_WIDTH] = { 0, };
int i = 0;
int main()
{
	FILE * in = fopen("c:\\Users\\Jang\\Desktop\\2015_2\\DigitalImageProcessing\\hw1\\lena256.raw", "rb");
	FILE * out = fopen("c:\\Users\\Jang\\Desktop\\2015_2\\DigitalImageProcessing\\hw1\\result_rotation.raw", "wb");
	int i = 0, j = 0;
	int h = 0, w = 0;
	int ch;
	int degree;
	double radian = 0.0;

	puts("input the degree : ");
	scanf("%d", &degree);
	radian = degree*3.141592 / 180;

	//input파일을 읽어서 input[y][x]에 저장합니다.
	while ((ch = fgetc(in)) != EOF) {
		input[i][j] = ch;
		if (j == INPUT_WIDTH) {
			i++;
			j = 0;
		}
		j++;
	}

	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH; w++)
		{
			rotation(h, w, radian);
		}
	}

	//output file에 쓰기
	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH; w++)
		{
			fputc(output[h][w], out);
		}
	}

	fclose(in);
	fclose(out);

}



int rotation(int h, int w, double radian)
{
	//삼각함수의 값을 구합니다.
	double sinRadian = sin(radian); 
	double cosRadian = cos(radian);

	//중점의 좌표를 구합니다.
	int xSubCenter = w - INPUT_WIDTH / 2.0;
	int ySubCenter = h - INPUT_HEIGHT / 2.0;

	double x = cosRadian*xSubCenter + sinRadian*ySubCenter + INPUT_WIDTH / 2.0;
	double y = cosRadian*ySubCenter - sinRadian*xSubCenter + INPUT_HEIGHT / 2.0;
	// x,y좌표에 내림 함수를 취해줍니다.
	int idxX = floor(x);
	int idxY = floor(y);
	
	if ((idxX >= 0 && idxX < INPUT_WIDTH) && (idxY >= 0 && idxY <= INPUT_HEIGHT))
	{
		output[h][w] = input[idxY][idxX];//input의 값을 참조해서 output픽셀에 씁니다..
	}

}