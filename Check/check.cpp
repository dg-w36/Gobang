#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#define ROW 7
#define COL 6
#define THRESHOULD 200

using namespace std;
using namespace cv;

//-------------------------------------------------------------------------------------
//			Input the changed point and set to 1, update the chess_board.txt
//-------------------------------------------------------------------------------------
void ChangedChessBoard(int row,int col){
	int chessBoard[ROW][COL];
	
	FILE *fp;
	fp = fopen("chess_board.txt","r"); //open file
	
	//read the data from the file
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			fscanf(fp,"%d",&chessBoard[i][j]);
		}
	}
	fclose(fp);
	
	//change the chessboard
	chessBoard[row][col] = 1;
	
	//update the file
	fp = fopen("chess_board.txt","w");
	for(int i = 0; i < ROW; i++){
		for(int j = 0; j < COL; j++){
			fprintf(fp,"%d ",chessBoard[i][j]);
		}
		fprintf(fp,"\n");
	}

	
}

//-------------------------------------------------------------------------
//			Get two pictures and do 'minus'(-), store in dstImage
//-------------------------------------------------------------------------

void PictureMinus(Mat &srcImage1, Mat &srcImage2, Mat &dstImage){
	//read two file
	srcImage1 = imread("Pictures/circle_1.jpg");
	srcImage2 = imread("Pictures/circle_2.jpg");
	imshow("src1",srcImage1);
	imshow("src2",srcImage2);

	//create the dstImage, make it is the same as the srcImage2
	dstImage.create(srcImage2.rows,srcImage2.cols,srcImage2.type());
	srcImage2.copyTo(dstImage);
	
	//do minus by every pixel (C style)
	int rowNumber = dstImage.rows;
	int colNumber = dstImage.cols * dstImage.channels();
	
	for(int i = 0;i < rowNumber; i++){
		uchar* data1 = dstImage.ptr<uchar>(i);
		uchar* data2 = srcImage1.ptr<uchar>(i);
		
		for(int j = 0;j < colNumber; j++){
			data1[j] -= data2[j];  
		}
	}
	
	//display the picture after minus
	imshow("result",dstImage);
}


//-------------------------------------------------------------------------
//			Read XML file after calibration, and check every point 
//-------------------------------------------------------------------------

void FindChange(Mat &dstImage){
	//read the points found by calibration
	Mat tmp;
	FileStorage fs("out_camera_data.xml", FileStorage:: READ);
	fs["image_points"] >> tmp;
	
//	cout << tmp << endl ;
	
	//check the changed point one by one
	int rowNumber = tmp.rows;
	int colNumber = tmp.cols;
	
	int x,y;
	for(int i = 0; i < rowNumber; i++){
		float* tmpData = tmp.ptr<float>(i);
		for(int j = 0; j < colNumber; j += 2){
			x = (int)tmpData[j];
			y = (int)tmpData[j+1];
			if(dstImage.ptr<uchar>(y)[x * 3] > THRESHOULD){ // probably be the write point
//				cout << "x = " << x << endl;
//				cout << "y = " << y << endl;
				cout << "Changed point: " << endl ;
				int tmpRow = j/(2 * COL) + 1 ; 
				int tmpCol = (j%(2 * COL))/2 + 1 ; 
				cout << "row = " << tmpRow << endl ;
				cout << "col = " << tmpCol << endl ;
				ChangedChessBoard(tmpRow, tmpCol); 
			}
		}
	}
	fs.release();	
}

int main(){
	double time0 = static_cast<double>(getTickCount());
	Mat srcImage1,srcImage2;
	Mat dstImage;
	
	PictureMinus(srcImage1, srcImage2, dstImage);
	FindChange(dstImage);
	
	time0 = ((double)getTickCount() - time0) / getTickFrequency();
	cout << "Running time: "<< time0 << "s" << endl;

	waitKey(0);
	return 0;
}
