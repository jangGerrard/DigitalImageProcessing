//http://www.had2know.com/technology/hsi-rgb-color-converter-equations.html

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define INPUT_WIDTH		1920
#define INPUT_HEIGHT		1080

#define HUE 0
#define SATURATION 1
#define INTENSITY 2

#define OUTPUT_WIDTH	1920
#define OUTPUT_HEIGHT	1080


double min(double a, double b, double c);

//1-(1)
//r,g,b를 h,s,i값으로 변환합니다.
void setHsiValues(int h, int w);

//1-(2)
//h,s,i를 r,g,b값으로 변환합니다.
void setOutputRgb(int h, int w);

//1-(3)
//maximum error값을 구합니다.
double maximumError();

//1-(3)
//mean error값을 구합니다.
double MeanError();

//각 픽셀별로 r,g,b의 에러 값을 구한후에 
//r,g,b에러 값을 더한 총 에러값을 리턴합니다.
double calculateError(int h, int w);

//처음에 입력을 받고 rgb별로 픽셀값을 저장합니다.
double inputRed[INPUT_HEIGHT][INPUT_WIDTH];
double inputGreen[INPUT_HEIGHT][INPUT_WIDTH];
double inputBlue[INPUT_HEIGHT][INPUT_WIDTH];

//rgb를 변환 공식을 이용해 rgb값을 구한 후에
//hue,saturation,intensity값을 아래의 변수에 저장합니다.
//hue -> [h][w][HUE], saturation -> [h][w][SATURATION], 
//intensity -> [h][w][INTENSITY]  에 저장되어 있습니다.
double hsi[INPUT_HEIGHT][INPUT_WIDTH][3];

//hue,saturation,intensity를 변환 공식을 이용해서 
//r,g,b값으로 변환해 아래의 변수에 저장합니다.
double outputRed[INPUT_HEIGHT][INPUT_WIDTH];
double outputGreen[INPUT_HEIGHT][INPUT_WIDTH];
double outputBlue[INPUT_HEIGHT][INPUT_WIDTH];



int main()
{

	char* inputPath = "BQTerrace_1920x1080.rgb";
	char * outputPath = "HSI_result.rgb";
	FILE * in = fopen(inputPath, "rb");
	FILE * out = fopen(outputPath, "wb");

	int ch;
	int w = 0, h = 0;
	//read
	for (h = 0; h < INPUT_HEIGHT; h++)
	{
		for (w = 0; w < INPUT_WIDTH * 3; w += 3)
		{
			inputRed[h][w / 3] = fgetc(in);
			inputGreen[h][w / 3] = fgetc(in);
			inputBlue[h][w / 3] = fgetc(in);
		}
	}

	//red , green. blue;
	for (int h = 0; h < INPUT_HEIGHT;h++) {
		for (int w = 0; w < INPUT_WIDTH; w++) {
			setHsiValues(h, w);
			setOutputRgb(h, w);
		}
	}


	double meanError = MeanError();
	double maxError = maximumError();

	///write
	for (h = 0; h < OUTPUT_HEIGHT; h++)
	{
		for (w = 0; w < OUTPUT_WIDTH ; w ++)
		{
			fputc(outputRed[h][w], out);
			fputc(outputGreen[h][w], out);
			fputc(outputBlue[h][w], out);
		}
	}

	printf("meanError : %lf\n", meanError);

	printf("maxError : %lf\n", maxError);

}

double min(double a, double b, double c)
{
	if (a <= b) {
		if (a <= c) {
			return a;
		}
		if (c <= a) {
			return c;
		}
	}
	else
	{
		if (b <= c) {
			return b;
		}
		if (c <= b) {
			return c;
		}
	}
}

void setHsiValues(int h, int w)
{
	double r = inputRed[h][w];
	double g = inputGreen[h][w];
	double b = inputBlue[h][w];

	double hueDegree = (0.5 * ((r - g) + (r - b))) / (sqrt(pow(r - g, 2) + (r - b)*(g - b)));
	double saturation = 1 - (3.0 / (r + g + b)) * min(r, g, b);
	double intensity = (1.0 / 3.0)*(r + g + b);
	
	double hue = (float)acos(hueDegree);

	hue *= 57.29577951f;
	
	if (b <= g) {
		
	}
	else {
		hue = 360.0 - hue;
	}

	if (intensity == 0) {
		saturation = 0;
	}

	if (r == g && r == b) {
		hue = 0;
		saturation = 0;
	}
	hsi[h][w][HUE] = hue;
	hsi[h][w][SATURATION] = saturation;
	hsi[h][w][INTENSITY] = intensity;
}

void setOutputRgb(int h, int w)
{
	double hue = hsi[h][w][HUE];
	double s = hsi[h][w][SATURATION];
	double i = hsi[h][w][INTENSITY];

	double r = -1;
	double g = -1;
	double b = -1;

	if (hue == 360) {
		hue == 0;
	}

	if (hue >= 0 && hue < 120) {
		double hRadian = hue * 3.141592 / 180.0;
		double hMin60Degree = (60 - hue) * 3.141592 / 180.0;
		b = i*(1 - s);
		r = i*(1 + s*cos(hRadian) / cos(hMin60Degree));
		g = 3 * i - (r + b);
	}
	else if (hue == 120) 
	{
		r = i - i*s;
		g = i + 2 * i*s;
		b = i - i *s;
	}
	else if (hue > 120 && hue < 240) 
	{
		hue = hue - 120;
		double hRadian = hue * 3.141592 / 180.0;
		double hMin60Degree = (60 - hue) * 3.141592 / 180.0;
		r = i * (1 - s);
		g = i*(1 + (s*cos(hRadian) / cos(hMin60Degree)));
		b = 3 * i - (r + g);
	} 
	else if (hue == 240)
	{
		r = i - i*s;
		g = i - i*s;
		b = i + 2 * i*s;
	}
	else if (hue > 240 && hue < 360) {
		hue = hue - 240;
		double hRadian = hue * 3.141592 / 180.0;
		double hMin60Degree = (60 - hue) * 3.141592 / 180.0;
		g = i * (1 - s);
		b = i*(1 + (s*cos(hRadian) / cos(hMin60Degree)));
		r = 3 * i - (g + b);
	}

	//outputRed[h][w] = r;
	//outputGreen[h][w] = g;
	//outputBlue[h][w] = b;

	outputRed[h][w] = round(r);
	outputGreen[h][w] = round(g);
	outputBlue[h][w] = round(b);
}

double maximumError() //각 픽셀중에 가장 error값이 큰 픽셀값을 return 합니다.
{
	double maxError = -1;
	for (int h = 0; h < INPUT_HEIGHT; h++)
	{
		for (int w = 0; w < INPUT_WIDTH;w++)
		{
			double temp = calculateError(h, w);
			if (temp > maxError) {
				maxError = temp;
			}
		}
	}

	return maxError;
}

double MeanError() //각 픽셀들의 에러값을 다 더해서 평균을 내주는 함수 입니다.
{
	double sumOfError = 0;
	for (int h = 0; h < INPUT_HEIGHT; h++)
	{
		for (int w = 0; w < INPUT_WIDTH;w++)
		{
			sumOfError = calculateError(h, w);
		}
	}

	return sumOfError / INPUT_HEIGHT*INPUT_WIDTH;
}

double calculateError(int h, int w)  //한 픽셀 단위의 input값과 output값을 비교합니다.
{
	double differentR = fabs(inputRed[h][w] - outputRed[h][w]);
	double differentG = fabs(inputGreen[h][w] - outputGreen[h][w]);
	double differentB = fabs(inputBlue[h][w] - outputBlue[h][w]);

	return (differentR + differentG + differentB);
}