#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <fstream>
using namespace cv;

int ID[24];


Mat whiteBalance(Mat img);

int main( int argc, char** argv )
{
	/* Color checker pixel value*/
	ID[0] = 0x735244;
	ID[1] = 0xc29682;
	ID[2] = 0x627a9d;
	ID[3] = 0x576c43;
	ID[4] = 0x8580b1;
	ID[5] = 0x67bdaa;
	ID[6] = 0xd67e2c;
	ID[7] = 0x505ba6;
	ID[8] = 0xc15a63;
	ID[9] = 0x5e3c6c;
	ID[10] = 0x9dbc40;
	ID[11] = 0xe0a32e;
	ID[12] = 0x383d96;
	ID[13] = 0x469449;
	ID[14] = 0xaf363c;
	ID[15] = 0xe7c71f;
	ID[16] = 0xbb5695;
	ID[17] = 0x0885a1;
	ID[18] = 0xf3f3f2;
	ID[19] = 0xc8c8c8;
	ID[20] = 0xa0a0a0;
	ID[21] = 0x7a7a79;
	ID[22] = 0x555555;
	ID[23] = 0x343434;

	char* imageName = argv[1];
	Mat image;
	image = imread( imageName, 1 );
	if( argc != 2 || !image.data )
	{
	  printf( " No image data \n " );
	  return -1;
	}
	Mat WB_img = whiteBalance(image);
	imwrite( "wb.bmp", WB_img );
	namedWindow( imageName, CV_WINDOW_AUTOSIZE );

	imshow( imageName, image );
	imshow( "White Balanced Image", WB_img );
	while((cv::waitKey() & 0xEFFFFF) != 27);

	/* Comparing the results with color checker*/	
	/*for(int i = 0; i < 24; i++){
		std::cout << std::hex << ((ID[i]>>16) & 0xff) << ((ID[i]>>8) & 0xff) << (ID[i] & 0xff) << std::endl;
		
		Mat tmp = WB_img.clone();
		for(int row = 0; row < WB_img.rows; row++)
			for(int col = 0; col < WB_img.cols; col++)
				if( ((uint)WB_img.at<Vec3b>(row, col).val[2] > (((ID[i]>>16) & 0xff) - 40) 
									&& (uint)WB_img.at<Vec3b>(row, col).val[2] < (((ID[i]>>16) & 0xff) + 40)) && 
					((uint)WB_img.at<Vec3b>(row, col).val[1] > (((ID[i]>>8) & 0xff) - 40) 
									&& (uint)WB_img.at<Vec3b>(row, col).val[1] < (((ID[i]>>8) & 0xff) + 40)) && 
					((uint)WB_img.at<Vec3b>(row, col).val[0] > ((ID[i] & 0xff) - 40) 
									&& (uint)WB_img.at<Vec3b>(row, col).val[0] < ((ID[i] & 0xff) + 40)) ){
					for(int j = 0; j < 3; j++){
						tmp.at<Vec3b>(row, col).val[j] = 255;
					}
				}
		imshow( "Match White Balanced Image", tmp );
		waitKey(0);
	}*/
	return 0;
}

Mat whiteBalance(Mat img){
	Mat WB_img = img.clone();
	double* histogram;
	double* equalHistorgram;
	double normalizeFactor = img.rows * img.cols;
	for(int i = 0; i < 3; i++){
		histogram = new double[256];
		// equalHistorgram = new double[256];

		for(int index = 0; index < 256; index++)
			histogram[index] = 0;

		for(int row = 0; row < img.rows; row++)
			for(int col = 0; col < img.cols; col++)
				histogram[img.at<Vec3b>(row, col).val[i]]++;

		for(int index = 1; index < 256; index++){
			histogram[index] = histogram[index] + histogram[index-1];
		}

		/* Obtain the max and min of streching boundary based on the histogram*/
		double discard_ratio = 0.05;
		int min = 0;
		int max = 255;
		while(histogram[min] < discard_ratio*normalizeFactor)
			min += 1;
		while(histogram[max] > (1.0-discard_ratio)*normalizeFactor)
			max -= 1;

		if(max < 254)
			max += 1;


		for(int row = 0; row < img.rows; row++)
			for(int col = 0; col < img.cols; col++){
				uchar val = img.at<Vec3b>(row, col).val[i];
				if(val < min)
					val = min;
				if(val > max)
					val = max;
				/* 1 to 1 Mapping of histogram */
				// WB_img.at<Vec3b>(row, col).val[i] = histogram[img.at<Vec3b>(row, col).val[i]]/normalizeFactor*255;
				
				/* Streching based on the Histogram */
				WB_img.at<Vec3b>(row, col).val[i] = (uchar)(val - min)*255 / (max - min);

			}

	}
	return WB_img;
}
