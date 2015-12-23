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
//r,g,b�� h,s,i������ ��ȯ�մϴ�.
void setHsiValues(int h, int w);

//1-(2)
//h,s,i�� r,g,b������ ��ȯ�մϴ�.
void setOutputRgb(int h, int w);

//1-(3)
//maximum error���� ���մϴ�.
double maximumError();

//1-(3)
//mean error���� ���մϴ�.
double MeanError();

//�� �ȼ����� r,g,b�� ���� ���� �����Ŀ� 
//r,g,b���� ���� ���� �� �������� �����մϴ�.
double calculateError(int h, int w);

//ó���� �Է��� �ް� rgb���� �ȼ����� �����մϴ�.
double inputRed[INPUT_HEIGHT][INPUT_WIDTH];
double inputGreen[INPUT_HEIGHT][INPUT_WIDTH];
double inputBlue[INPUT_HEIGHT][INPUT_WIDTH];

//rgb�� ��ȯ ������ �̿��� rgb���� ���� �Ŀ�
//hue,saturation,intensity���� �Ʒ��� ������ �����մϴ�.
//hue -> [h][w][HUE], saturation -> [h][w][SATURATION], 
//intensity -> [h][w][INTENSITY]  �� ����Ǿ� �ֽ��ϴ�.
double hsi[INPUT_HEIGHT][INPUT_WIDTH][3];

//hue,saturation,intensity�� ��ȯ ������ �̿��ؼ� 
//r,g,b������ ��ȯ�� �Ʒ��� ������ �����մϴ�.
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

double maximumError() //�� �ȼ��߿� ���� error���� ū �ȼ����� return �մϴ�.
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

double MeanError() //�� �ȼ����� �������� �� ���ؼ� ����� ���ִ� �Լ� �Դϴ�.
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

double calculateError(int h, int w)  //�� �ȼ� ������ input���� output���� ���մϴ�.
{
	double differentR = fabs(inputRed[h][w] - outputRed[h][w]);
	double differentG = fabs(inputGreen[h][w] - outputGreen[h][w]);
	double differentB = fabs(inputBlue[h][w] - outputBlue[h][w]);

	return (differentR + differentG + differentB);
}