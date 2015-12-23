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

//filter를 적용하는 함수 입니다.
//h = height, w = width, c = color
void  filtering(int h, int w, int c);

//input data가 저장되는 배열입니다.
//마지막 차원의 index가 0일때는RED, 1일때는 BLUE, 2일때는 GREEN
//값이 저장됩니다.
double input[INPUT_HEIGHT][INPUT_WIDTH][3];

//output data가 저장되는 배열입니다.
//필터를 적용한 결과가 저장됩니다.
//마지막 차원의 index가 0일때는RED, 1일때는 BLUE, 2일때는 GREEN
//값이 저장됩니다.
double output[INPUT_HEIGHT][INPUT_WIDTH][3];

//smoothing filter kernel값입니다.
double smoothingFilter[3][3] = { { 1 , 2 , 1 },
										{ 2 , 4 , 2 },
										{ 1 , 2 , 1 }
										};
//지난번 과제에서 smoothing filter구현을 위해서 사용한 변수입니다.
//외각 픽셀의 경우에 범위에서 벗어난 값을 더하지 않기위해서 사용합니다.
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

	///filter 수행을 하는 부분
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

	for (a = -halfFilterHeight;a <= halfFilterHeight; a++)//유효한지 체크하고 더하고 어느 필터까지 유효한지
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (h + a < 0 || a + h >= INPUT_HEIGHT || w + b < 0 || w + b >= INPUT_WIDTH) {			//유효하지 않은 필터, 픽셀 값 양끝의 픽셀일 경우
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = FALSE;
			}
			else {																										//유효한 필터값
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = TRUE;
				sumOfFilterValue += smoothingFilter[a + halfFilterHeight][b + halfFilterWidth];			//유효한 필터의 값의 총합.
			}
		}
	}
	for (a = -halfFilterHeight;a <= +halfFilterHeight; a++) //필터를 적용한 값 구하기
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (isValidPixel[a + halfFilterHeight][b + halfFilterWidth] == TRUE) {
				output_result += 
					((smoothingFilter[a + halfFilterHeight][b + halfFilterWidth]) / (double)sumOfFilterValue) * (double)input[h + a][w + b][c];  
			}

		}
	}
	output[h][w][c] = output_result;  //output파일에 필터 적용한 값 쓰기
}