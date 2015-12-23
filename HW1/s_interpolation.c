#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define MAX_SIZE			65536
#define INPUT_WIDTH	256
#define INPUT_HEIGHT	256

//output파일의 크기를 정해 줍니다.
#define OUTPUT_WIDTH 512
#define OUTPUT_HEIGHT 512

void interpolation(int w, int h);
int findX(int w);
int findY(int h);
double input[INPUT_HEIGHT][INPUT_WIDTH];
double output[OUTPUT_HEIGHT][OUTPUT_WIDTH];

int main()
{
	FILE * in = fopen("c:\\Users\\Jang\\Desktop\\2015_2\\DigitalImageProcessing\\hw1\\lena256.raw", "rb");
	FILE * out = fopen("c:\\Users\\Jang\\Desktop\\2015_2\\DigitalImageProcessing\\hw1\\result_interpolation.raw", "wb");
	int ch;
	int count = 0;
	int i = 0, j = 0;

	int w = 0, h = 0;

	//input파일을 읽어서 input[h][w]에 저장한다.
	while ((ch = fgetc(in)) != EOF) {
		input[i][j] = ch;
		if (j == INPUT_WIDTH) {
			i++;
			j = 0;
		}
		j++;
	}

	//result파일의 크기를 기준으로 interpolation
	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH; w++)
		{
			interpolation(h, w);
		}
	}

	///output file에 쓰기
	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH; w++)
		{
			fputc(output[h][w], out);
		}
	}


	fclose(in);
	fclose(out);

	return 0;
}

void interpolation(int h, int w)
{
	int x = findX(w);
	int y = findY(h);
	double q1 = 0;
	double q2 = 0;
	double q = 0;

	double tempWidth = ((double)w / OUTPUT_WIDTH) - ((double)x / INPUT_WIDTH);
	double tempHeight = (double)h / OUTPUT_HEIGHT - ((double)y / INPUT_HEIGHT);

	q1 = (tempWidth)*input[y][x] + ((1 - tempWidth)*input[y][x + 1]);
	q2 = (tempWidth)*input[y + 1][x] + (1 - tempWidth)*input[y + 1][x + 1];
	q = (1 - tempHeight)*q2 + (tempHeight)*q1;
	
	//참조할 값이 없는 외곽픽셀의 경우 인접한 픽셀 값을 사용합니다.
	if (w == OUTPUT_WIDTH - 1) { q = output[h][w - 1]; }
	if (h == OUTPUT_HEIGHT - 1) { q = output[h - 1][w]; }
	if (w == OUTPUT_WIDTH - 1 && h == OUTPUT_HEIGHT - 1) { q = output[h - 1][w - 1]; }

	output[h][w] = q;////yx를 이용해서 구해야 합니다.
}

int findX(int w)
{
	int i = 0;
	int x = 0;
	for (i = 0; i < INPUT_WIDTH; i++)
	{
		if ((double)i / INPUT_WIDTH < (double)w / OUTPUT_WIDTH)
		{
			x = i;
		}
		else
		{
			break;
		}
	}
	return x;
}
int findY(int h)
{
	int i = 0;
	int y = 0;
	for (i = 0; i < INPUT_HEIGHT; i++)
	{
		if ((double)i / INPUT_HEIGHT < (double)h / OUTPUT_HEIGHT)
		{
			y = i;
		}
		else
		{
			break;
		}
	}
	return y;
}
