// OpenCVApplication.cpp : Defines the entry point for the console application.
//
//2D go/backgammon table (or other board game) decoding 
//Given an image of a board from above and the template images of the pieces determine the position of each piece and output the table configuration in a standard format.
//https://commons.wikimedia.org/wiki/Category:PNG_chess_pieces/Standard_transparent
#include "stdafx.h"
#include "common.h"


bool equals(uchar p1, uchar p2) {
	return p1==p2;
}

//matricea de piese
vector<Mat> piese() {
	vector<Mat> piese;

	Mat image1 = imread("D:/An_3_sem_2/PI/proiect/Images/cal_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image2 = imread("D:/An_3_sem_2/PI/proiect/Images/cal_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image3 = imread("D:/An_3_sem_2/PI/proiect/Images/nebun_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image4 = imread("D:/An_3_sem_2/PI/proiect/Images/nebun_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image5 = imread("D:/An_3_sem_2/PI/proiect/Images/rege_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image6 = imread("D:/An_3_sem_2/PI/proiect/Images/rege_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image7 = imread("D:/An_3_sem_2/PI/proiect/Images/regina_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image8 = imread("D:/An_3_sem_2/PI/proiect/Images/regina_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image9 = imread("D:/An_3_sem_2/PI/proiect/Images/tura_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image10 = imread("D:/An_3_sem_2/PI/proiect/Images/tura_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image11 = imread("D:/An_3_sem_2/PI/proiect/Images/pion_a.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image12 = imread("D:/An_3_sem_2/PI/proiect/Images/pion_n.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	piese.push_back(image1);
	piese.push_back(image2);
	piese.push_back(image3);
	piese.push_back(image4);
	piese.push_back(image5);
	piese.push_back(image6);
	piese.push_back(image7);
	piese.push_back(image8);
	piese.push_back(image9);
	piese.push_back(image10);
	piese.push_back(image11);
	piese.push_back(image12);
	
	return piese;
}
//primeste un punct si compara 60x60 de pixeli cu fiecare imagine din arhiva returnand
// matricea cu cel mai mare scor/ numele ei???/ codul ei 


//decupeaza de la x si y un patrat de 60 pe 60 si il returneaza
Mat croppedMat(int x, int y, Mat image) {
	int height = image.cols;
	int width = image.rows;

	Mat cropped = Mat(60, 60, CV_8UC1);
	int xx = x + 60;
	int yy = y + 60;
	
	int k = 0; 
	int l = 0;
	for (int i = x; i < xx; i++) {
		for (int j = y; j < yy; j++) {
			//printf("%d    %d\n", i, j);
			cropped.at<uchar>(k, l) = image.at<uchar>(i, j);
			l++;
		}
		k++;
		l = 0;
	}
	return cropped;
}
/*
Mat binarizare(Mat src) {
	Mat dst = src;
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
			if (src.at<uchar>(i, j) > 245)
				src.at<uchar>(i, j) = 255;
			else
				if (src.at<uchar>(i, j) < 15)
					src.at<uchar>(i, j) = 0;
				else
					src.at<uchar>(i, j) = 128;
	return dst;
}*/
int comparatie(Mat img1, Mat img2) {
	if (img1.cols != img2.cols || img1.rows != img2.rows)
		return 0;

	//img1 = binarizare(img1);
	//img2 = binarizare(img2);

	int same = 0;
	for(int i = 0; i < img1.rows ; i++)
		for (int j = 0; j < img1.cols; j++) {
			
			if (img1.at<uchar>(i, j) == img2.at<uchar>(i, j)) /*&& (img1.at<uchar>(i, j)==0 || img1.at<uchar>(i, j) == 255))*/
			{
				same++;
				//printf("%d cu %d\n", img1.at<uchar>(i, j), img2.at<uchar>(i, j));

			}
		}
			
	//printf("Scor in functie %d\n", same);
	return same;
}
bool white(Mat img) {
	int s = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {

			Vec3b pixel = img.at<uchar>(i, j);
			s += (pixel[0] - 255)* (pixel[0] - 255);
		}
	}
	
	//printf("white: %d\n", s);

	if (s < 50)
		return true;
	return false;

}
bool black(Mat img) {
	int s = 0;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			
			Vec3b pixel = img.at<uchar>(i, j);
			//valore obtinuta din mai multe rulari si printari
			s += (pixel[0] - 62) * (pixel[0] - 62);
			
		}
	}
			
	if (s < 720)
		return true;
	return false;
}
//o sa returneze indicele si atunci il identific
//primeste coordonatele pixelul din coltul din stanga sus a piesei pe care dorim sa o identificam
int getChessPieceId(int x, int y, Mat image) {
	
	int ind=0;
	int max_value = 0;
	int value = 0;
	
	vector<Mat> piese_vect = piese();

	Mat croppedImage = croppedMat(x,y,image);
	
	if (white(croppedImage))
		return 12;
	if (black(croppedImage))
		return 13;

	for (int i = 0; i < piese_vect.size(); i++)
	{
		//printf("scor: %d cu %d\n", value, i);
		
		int value = comparatie(croppedImage, piese_vect[i]);
		//printf("scor: %d cu %d\n", value, i);
		if (value > max_value) {
			max_value = value;
			ind = i;
		}		
	}
	
	return ind;
}
void all(Mat src) {
	//imshow("cropped", croppedImage);
	//waitKey(0);
	//imshow("piesa", piese_vect[ind]);
	//waitKey(0);
	//printf("Indicele este %d", ind);
	int X[8] = { 0, 60, 120, 180, 240, 300, 360, 420 };
	int Y[8] = { 0, 60, 120, 180, 240, 300, 360, 420 };

	vector<Mat> piese_vect = piese();

	FILE* fptr;
	fptr = fopen("D:/An_3_sem_2/PI/proiect/Images/result.txt", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}

	//fprintf(fptr, "Tabla de sah\n");
	

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int ind = getChessPieceId(X[i], Y[j], src);
			
			
			switch (ind) {
			case 0:
				printf("[(%d, %d) - cal alb       ]", i, j);
				//fprintf(fptr, "[(%d, %d) - cal alb       ]", i, j);
				break;
			case 1:
				printf("[(%d, %d) - cal negru     ]", i, j);
				//fprintf(fptr, "[(%d, %d) - cal negru     ]", i, j);
				break;
			case 2:
				printf("[(%d, %d) - nebun negru   ]", i, j);
				//fprintf(fptr, "[(%d, %d) - nebun negru   ]", i, j);
				break;
			case 3:
				printf("[(%d, %d) - nebun alb     ]", i, j);
				//fprintf(fptr, "[(%d, %d) - nebun alb     ]", i, j);
				break;
			case 4:
				printf("[(%d, %d) - rege alb      ]", i, j);
				//fprintf(fptr, "[(%d, %d) - rege alb      ]", i, j);
				break;
			case 5:
				printf("[(%d, %d) - rege negru    ]", i, j);
				//fprintf(fptr, "[(%d, %d) - rege negru    ]", i, j);
				break;
			case 6:
				printf("[(%d, %d) - regina alba   ]", i, j);
				//fprintf(fptr, "[(%d, %d) - regina alba   ]", i, j);
				break;
			case 7:
				printf("[(%d, %d) - regina neagra ]", i, j);
				//fprintf(fptr, "[(%d, %d) - regina neagra ]", i, j);
				break;
			case 8:
				printf("[(%d, %d) - tura alba     ]", i, j);
				//fprintf(fptr, "[(%d, %d) - tura alba     ]", i, j);
				break;
			case 9:
				printf("[(%d, %d) - tura neagra   ]", i, j);
				//fprintf(fptr, "[(%d, %d) - tura neagra   ]", i, j);
				break;
			case 10:
				printf("[(%d, %d) - pion alb      ]", i, j);
				//fprintf(fptr, "[(%d, %d) - pion alb      ]", i, j);
				break;
			case 11:
				printf("[(%d, %d) - pion negru    ]", i, j);
				//fprintf(fptr, "[(%d, %d) - pion negru    ]", i, j);
				break;
			case 12:
				printf("[(%d, %d) - spatiu alb    ]", i, j);
				//fprintf(fptr, "[(%d, %d) - spatiu alb    ]", i, j);
				break;
			case 13:
				printf("[(%d, %d) - spatiu negru  ]", i, j);
				//fprintf(fptr, "[(%d, %d) - spatiu negru  ]", i, j);
				break;
			}
			

		}
		printf("\n");
		fclose(fptr);
	}
}
void proiect()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);		
		all(src);
	}
}
int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		
		printf(" 1 - alege una din tablele de sah.\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			
			case 1:
				proiect();
				break;
			
		}
	}
	while (op!=0);
	return 0;
}