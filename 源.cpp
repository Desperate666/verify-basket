#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2\imgproc\types_c.h>

using namespace cv;
using namespace std;

void Blue_Bar(Mat& image, Mat& Bar0)
{
	Mat bgr;
	Mat hsv;
	//灰度值归一化
	image.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
	//颜色空间转换
	cvtColor(bgr, hsv, COLOR_BGR2HSV);
	//利用上下限提取出的目标物体，并进行二值化处理
	inRange(hsv, Scalar(210, 0.627450980392156, 0.215686274509803),Scalar(230, 0.901960784313725, 0.529411764705882), Bar0);

	//开操作,去除噪点
	Mat elem0 = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(Bar0, Bar0, MORPH_OPEN, elem0);
	//闭操作,连接连通域
	Mat elem1 = getStructuringElement(MORPH_RECT, Size(8, 8));
	morphologyEx(Bar0, Bar0, MORPH_CLOSE, elem1);
	//开操作,去除噪点
	Mat elem2 = getStructuringElement(MORPH_RECT, Size(6, 6));
	morphologyEx(Bar0, Bar0, MORPH_OPEN, elem2);
}

void Red_Bar(Mat& image, Mat& Bar0)
{
	Mat bgr;
	Mat hsv;
	//灰度值归一化
	image.convertTo(bgr, CV_32FC3, 1.0 / 255, 0);
	//颜色空间转换
	cvtColor(bgr, hsv, COLOR_BGR2HSV);
	inRange(hsv, Scalar(0, 0.549019607843137, 0.411764705882352),Scalar(19, 0.823529411764705, 0.627450980392156), Bar0);

	//开操作,去除噪点
	Mat elem0 = getStructuringElement(MORPH_RECT, Size(3, 3));
	morphologyEx(Bar0, Bar0, MORPH_OPEN, elem0);
	//闭操作 ,连接连通域
	Mat elem1 = getStructuringElement(MORPH_RECT, Size(10, 10));
	morphologyEx(Bar0, Bar0, MORPH_CLOSE, elem1);
	//开操作,去除噪点
	Mat elem2 = getStructuringElement(MORPH_RECT, Size(6, 6));
	morphologyEx(Bar0, Bar0, MORPH_OPEN, elem2);
}

int main()
{
	Mat src = imread("7_Color.png",2|4);
	Mat Blue0(src.size(), CV_32FC3);
	Mat Red0(src.size(), CV_32FC3);

	Blue_Bar(src, Blue0);
	Red_Bar(src, Red0);


	/////////////////////////////////以下两种方法可以直接替换//////////////////////////////////


	//////////////////以下是直接在加工后二值化的图上找轮廓，然后在原图上画轮廓的方法///////////
	//////////////////////////////////但是此种方法的精准度不太高////////////////////////////
	vector<vector<Point>>contours1;
	vector<Vec4i>hierarchy1;
	findContours(Blue0, contours1, hierarchy1, RETR_CCOMP, CHAIN_APPROX_NONE);
	drawContours(src, contours1, -1, Scalar(0, 0, 255), 3);

	//中心点
	for (int i = 0; i < 4; i++)
	{
		RotatedRect rects1 = minAreaRect(contours1[i]);
		rectangle(src, rects1.center, rects1.center, Scalar::all(255), 2, 8, 0);
	}

	vector<vector<Point>>contours2;
	vector<Vec4i>hierarchy2;
	findContours(Red0, contours2, hierarchy2, RETR_CCOMP, CHAIN_APPROX_NONE);
	drawContours(src, contours2, -1, Scalar(255, 0, 0), 3);

	//中心点
	for (int i = 0; i < 4; i++)
	{
		RotatedRect rects2 = minAreaRect(contours2[i]);
		rectangle(src, rects2.center, rects2.center, Scalar::all(255), 2, 8, 0);
	}
	//////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////以下是用矩形框出的方法////////////////////////////////
	////////////////////////////////此种方法较为精准//////////////////////////////////////
	//Mat blue(src.size(), CV_8UC3);
	//Mat red(src.size(), CV_8UC3);

	//mask_Blue0.convertTo(blue, CV_8UC3);
	//mask_Red0.convertTo(red, CV_8UC3);
	
    ////蓝桶轮廓提取
	//vector<vector<Point>> contours_B;
	//vector<Vec4i> hierarchy_B;
	//findContours(blue, contours_B, hierarchy_B, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//Point2f blue_pts[4];
	//for (int i = 0; i < contours_B.size(); i++)
	//{
	//	//选取倾斜包覆矩形最小集中包围盒,确定旋转矩阵的四个顶点，并画框
	//	RotatedRect rects1 = minAreaRect(contours_B[i]);
	//	rects1.points(blue_pts);
	//	line(src, blue_pts[0], blue_pts[1], Scalar(0, 0, 255), 2);
	//	line(src, blue_pts[1], blue_pts[2], Scalar(0, 0, 255), 2);
	//	line(src, blue_pts[2], blue_pts[3], Scalar(0, 0, 255), 2);
	//	line(src, blue_pts[3], blue_pts[0], Scalar(0, 0, 255), 2);

	//	//中心点
	//	rectangle(src, rects1.center, rects1.center, Scalar::all(255), 2, 8, 0);
	//}

	////红桶轮廓提取
	//vector<vector<Point>> contours_R;
	//vector<Vec4i> hierarchy_R;
	//findContours(red, contours_R, hierarchy_R, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//
	//Point2f red_pts[4];
	//for (int j = 0; j < contours_R.size(); j++)
	//{
	//	//选取倾斜包覆矩形最小集中包围盒,确定旋转矩阵的四个顶点，并画框
	//	RotatedRect rects2 = minAreaRect(contours_R[j]);
	//	rects2.points(red_pts);
	//	line(src, red_pts[0], red_pts[1], Scalar(255, 0, 0), 2);
	//	line(src, red_pts[1], red_pts[2], Scalar(255, 0, 0), 2);
	//	line(src, red_pts[2], red_pts[3], Scalar(255, 0, 0), 2);
	//	line(src, red_pts[3], red_pts[0], Scalar(255, 0, 0), 2);

	//	//中心点
	//	rectangle(src, rects2.center, rects2.center, Scalar::all(255), 2, 8, 0);
	//}
	////////////////////////////////////////////////////////////////////////////////////

	imshow("...", src);
	waitKey(0);
	return 0;
}
