#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define INPUT_WIDTH		1920
#define INPUT_HEIGHT		1080

#define BOOL int
#define TRUE 1
#define FALSE 0

#define RED 0
#define GREEN 1
#define BLUE 2

#define HUE 0
#define SATURATION 1
#define INTENSITY 2

#define FILTER_WIDTH		3
#define FILTER_HEIGHT		3

#define OUTPUT_WIDTH	1920
#define OUTPUT_HEIGHT	1080

//filter�� �����ϴ� �Լ� �Դϴ�.
//h = height, w = width, c = color
void  filtering(int h, int w, int c);

//input data�� ����Ǵ� �迭�Դϴ�.
//������ ������ index�� 0�϶���RED, 1�϶��� BLUE, 2�϶��� GREEN
//���� ����˴ϴ�.
double input[INPUT_HEIGHT][INPUT_WIDTH][3];

//output data�� ����Ǵ� �迭�Դϴ�.
//���͸� ������ ����� ����˴ϴ�.
//������ ������ index�� 0�϶���RED, 1�϶��� BLUE, 2�϶��� GREEN
//���� ����˴ϴ�.
double output[INPUT_HEIGHT][INPUT_WIDTH][3];

//smoothing filter kernel���Դϴ�.
double smoothingFilter[3][3] = { { 1 , 2 , 1 },
										{ 2 , 4 , 2 },
										{ 1 , 2 , 1 }
										};
//������ �������� smoothing filter������ ���ؼ� ����� �����Դϴ�.
//�ܰ� �ȼ��� ��쿡 �������� ��� ���� ������ �ʱ����ؼ� ����մϴ�.
BOOL isValidPixel[7][7];

int main()
{
	char* inputPath = "BQTerrace_1920x1080.rgb";
	char * outputPath = "Smoothing_result.rgb";
	FILE * in = fopen(inputPath, "rb");
	FILE * out = fopen(outputPath, "wb");

	int ch;
	int w = 0, h = 0;
	//read
	for (h = 0; h < INPUT_HEIGHT; h++)
	{
		for (w = 0; w < INPUT_WIDTH * 3; w += 3)
		{
			input[h][w / 3][RED] = fgetc(in);
			input[h][w / 3][GREEN] = fgetc(in);
			input[h][w / 3][BLUE] = fgetc(in);
		}
	}

	///filter ������ �ϴ� �κ�
	for (int c = 0; c < 3; c++) {
		for (int h = 0; h < INPUT_HEIGHT;h++) {
			for (int w = 0; w < INPUT_WIDTH; w++) {
				filtering(h, w, c);
			}
		}
	}

	///write
	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH; w++)
		{
			fputc(output[h][w][RED], out);
			fputc(output[h][w][GREEN], out);
			fputc(output[h][w][BLUE], out);
		}
	}
}

void  filtering(int h, int w,int c)
{
	int i = 0, j = 0;
	int a = 0, b = 0;
	double output_result = 0;
	int sumOfFilterValue = 0;
	int halfFilterHeight = floor(FILTER_HEIGHT / 2);
	int halfFilterWidth = floor(FILTER_WIDTH / 2);

	for (a = -halfFilterHeight;a <= halfFilterHeight; a++)//��ȿ���� üũ�ϰ� ���ϰ� ��� ���ͱ��� ��ȿ����
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (h + a < 0 || a + h >= INPUT_HEIGHT || w + b < 0 || w + b >= INPUT_WIDTH) {			//��ȿ���� ���� ����, �ȼ� �� �糡�� �ȼ��� ���
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = FALSE;
			}
			else {																										//��ȿ�� ���Ͱ�
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = TRUE;
				sumOfFilterValue += smoothingFilter[a + halfFilterHeight][b + halfFilterWidth];			//��ȿ�� ������ ���� ����.
			}
		}
	}
	for (a = -halfFilterHeight;a <= +halfFilterHeight; a++) //���͸� ������ �� ���ϱ�
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (isValidPixel[a + halfFilterHeight][b + halfFilterWidth] == TRUE) {
				output_result += 
					((smoothingFilter[a + halfFilterHeight][b + halfFilterWidth]) / (double)sumOfFilterValue) * (double)input[h + a][w + b][c];  
			}

		}
	}
	output[h][w][c] = output_result;  //output���Ͽ� ���� ������ �� ����
}