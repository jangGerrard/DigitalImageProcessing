#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define MAX_SIZE			65536

#define INPUT_WIDTH		256
#define INPUT_HEIGHT		256

#define OUTPUT_WIDTH	256
#define OUTPUT_HEIGHT	256

///필터를 변경하려면 이부분을 변경하면 됩니다.
#define FILTER_WIDTH		3
#define FILTER_HEIGHT		3
#define FILTER_NAME		sharpningFilter
	//avgFilter3, avgFilter5, avgFilter7, smoothingFilter ,sharpningFilter

#define BOOL int
#define TRUE  1
#define FALSE 0


double input[INPUT_HEIGHT][INPUT_WIDTH];
double output[OUTPUT_HEIGHT][OUTPUT_WIDTH];

//양외곽의 필터들을 처리하기 위한 배열 입니다.
BOOL isValidPixel[7][7];
int filterHeight = 0;
int filterWidth = 0;

double avgFilter3[3][3] = {	{ 1 , 1 , 1 },
								{ 1 , 1 , 1 },
								{ 1 , 1 , 1 }
							};

double avgFilter5[5][5] = {		{ 1 , 1 , 1 , 1 , 1 },
									{ 1 , 1 , 1 , 1 , 1 },
									{ 1 , 1 , 1 , 1 , 1 },
									{ 1 , 1 , 1 , 1 , 1 },
									{ 1 , 1 , 1 , 1 , 1 }
								};
double avgFilter7[7][7] = { { 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								{ 1 , 1 , 1 , 1 , 1 , 1 , 1 },
								};

double smoothingFilter[3][3] = {	{ 1 , 2 , 1 },
										{ 2 , 4 , 2 },
										{ 1 , 2 , 1 }
									};

double sharpningFilter[3][3] = {		{ 0, -1, 0 },
											{ -1, 5, -1 },
											{ 0, -1, 0 }
										};

void  filtering(int h, int w);


int main()
{
	//input File
	FILE * in = fopen("lena256.raw", "rb");
	//output File
	FILE * out = fopen("sharpning3x3.raw", "wb");
	
	int ch;
	int w = 0, h = 0;

	//input file read, store data at array
	while ((ch = fgetc(in)) != EOF) {
		input[h][w] = ch;
		if (w == INPUT_WIDTH) {
			h++;
			w = 0;
		}
		w++;
	}
	filterHeight = FILTER_HEIGHT;
	filterWidth = FILTER_WIDTH;
	
	for (h = 0; h < OUTPUT_HEIGHT; h++) {
		for (w = 0; w < OUTPUT_WIDTH; w++) {
			filtering(h, w);								//filter calculation
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


//when calcuate (filter mask value *init image value) edge value, only valid value is calculated. 
void  filtering(int h, int w)
{
	int i = 0, j = 0;
	int a = 0, b = 0;
	double output_result = 0;
	int sumOfFilterValue=0;
	int halfFilterHeight = floor(filterHeight / 2);
	int halfFilterWidth = floor(filterWidth / 2);
	

	for (a = -halfFilterHeight;a <= halfFilterHeight; a++)//유효한지 체크하고 더하고 어느 필터까지 유효한지
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (h + a < 0 || a + h >= INPUT_HEIGHT || w + b < 0 || w + b >= INPUT_WIDTH) { //유효하지 않은 필터, 픽셀 값 양끝의 픽셀일 경우
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = FALSE;
			}
			else {																								//유효한 필터값
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = TRUE;
				sumOfFilterValue += FILTER_NAME[a +halfFilterHeight][b + halfFilterWidth];																	//여기랑
			}
		}
	}
	
	for (a = -halfFilterHeight ;a <= +halfFilterHeight; a++) //필터를 적용한 값 구하기
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (isValidPixel[a + halfFilterHeight][b + halfFilterWidth] == TRUE) {
				output_result += ((FILTER_NAME[a+halfFilterHeight][b+halfFilterWidth]) / (double)sumOfFilterValue) * (double)input[h + a][w + b];    //여기랑
			}

		}
	}

	output[h][w] = output_result;  //output파일에 필터 적용한 값 쓰기
}