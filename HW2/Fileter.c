#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define MAX_SIZE			65536

#define INPUT_WIDTH		256
#define INPUT_HEIGHT		256

#define OUTPUT_WIDTH	256
#define OUTPUT_HEIGHT	256

///���͸� �����Ϸ��� �̺κ��� �����ϸ� �˴ϴ�.
#define FILTER_WIDTH		3
#define FILTER_HEIGHT		3
#define FILTER_NAME		sharpningFilter
	//avgFilter3, avgFilter5, avgFilter7, smoothingFilter ,sharpningFilter

#define BOOL int
#define TRUE  1
#define FALSE 0


double input[INPUT_HEIGHT][INPUT_WIDTH];
double output[OUTPUT_HEIGHT][OUTPUT_WIDTH];

//��ܰ��� ���͵��� ó���ϱ� ���� �迭 �Դϴ�.
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


	///output file�� ����
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
	

	for (a = -halfFilterHeight;a <= halfFilterHeight; a++)//��ȿ���� üũ�ϰ� ���ϰ� ��� ���ͱ��� ��ȿ����
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (h + a < 0 || a + h >= INPUT_HEIGHT || w + b < 0 || w + b >= INPUT_WIDTH) { //��ȿ���� ���� ����, �ȼ� �� �糡�� �ȼ��� ���
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = FALSE;
			}
			else {																								//��ȿ�� ���Ͱ�
				isValidPixel[a + halfFilterHeight][b + halfFilterWidth] = TRUE;
				sumOfFilterValue += FILTER_NAME[a +halfFilterHeight][b + halfFilterWidth];																	//�����
			}
		}
	}
	
	for (a = -halfFilterHeight ;a <= +halfFilterHeight; a++) //���͸� ������ �� ���ϱ�
	{
		for (b = -halfFilterWidth;b <= halfFilterWidth;b++)
		{
			if (isValidPixel[a + halfFilterHeight][b + halfFilterWidth] == TRUE) {
				output_result += ((FILTER_NAME[a+halfFilterHeight][b+halfFilterWidth]) / (double)sumOfFilterValue) * (double)input[h + a][w + b];    //�����
			}

		}
	}

	output[h][w] = output_result;  //output���Ͽ� ���� ������ �� ����
}