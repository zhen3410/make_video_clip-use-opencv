#include"stdafx.h"
#include<string>
#include<iostream>
#include<iomanip>
#include<sstream>

#include<highgui.hpp>
#include<imgproc.hpp>
#include<core.hpp>

using namespace std;
using namespace cv;

//实现随机颜色
static Scalar random_color(RNG& rng) {
	int color = (unsigned)rng;
	return Scalar(color & 255, (color >> 8) & 255, (color >> 16) & 255);
}

const int DELAY = 5;
//在视频底部打上字幕
void put_info(InputOutputArray img, Point info_num, Point info_name) {
	putText(img, "21721207", info_num, CV_FONT_HERSHEY_TRIPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
	putText(img, "Zhang Zhen", info_name, CV_FONT_HERSHEY_TRIPLEX, 1, Scalar(255, 255, 255), 2, LINE_AA);
}

//符号函数
int sgn(int a) {
	if (a > 0) {
		return 1;
	}
	else if (a < 0) {
		return -1;
	}
	else {
		return 0;
	}
}

VideoWriter outputVideo;

//淡入淡出效果
void FI_FO(Point info_num, Point info_name, Mat temp1, Mat temp2) {

	int row = temp1.rows;
	int col = temp1.cols*temp1.channels();
	put_info(temp1, info_num, info_name);
	put_info(temp2, info_num, info_name);

	while (true) {
		bool flag = true;
		for (int k = 0; k < row; k++) {
			uchar* p = temp1.ptr<uchar>(k);
			uchar* q = temp2.ptr<uchar>(k);
			for (int j = 0; j < col; j++) {
				int a = q[j] - p[j];
				if (a != 0) {
					p[j] += sgn(a);
					flag = false;
				}
			}
		}
		imshow("CV_PROJ1", temp1);
		outputVideo << temp1;
		if (flag == true)break;
		if (waitKey(DELAY) >= 0)exit(0);
	}
}



//百叶窗效果
void blind(Point info_num, Point info_name, Mat temp1, Mat temp2) {

	int row = temp1.rows;
	int col = temp1.cols*temp1.channels();

	int num_row = row / 30;
	for (int k = 0; k < 30; k++) {
		for (int j = 0; j < num_row; j++) {
			uchar* p = temp1.ptr<uchar>(j * 30 + k);
			uchar* q = temp2.ptr<uchar>(j * 30 + k);
			for (int a = 0; a < col; a++) {
				q[a] = p[a];
			}
		}
		put_info(temp2, info_num, info_name);
		imshow("CV_PROJ1", temp2);
		outputVideo << temp2;
		if (waitKey(DELAY) >= 0)exit(0);
	}
}

//推拉效果
void push_drag(Point info_num, Point info_name, Mat temp1, Mat temp2) {
	int col = temp1.cols*temp1.channels();
	int row = temp1.rows;
	for (int k = 0; k < row; k++) {
		for (int i = 1; i <row; i++) {
			uchar* p = temp1.ptr<uchar>(i);
			for (int j = 0; j < col; j++) {
				int x = j;
				int y = i + 1;
				if (y >= 0 && y < row) {
					p[j] = temp1.ptr<uchar>(y)[x];
				}
			}
		}
		uchar* q = temp2.ptr<uchar>(k);
		uchar* p = temp1.ptr<uchar>(row - 1);
		for (int j = 0; j < col; j++) {
			p[j] = q[j];
		}
		if (k % 3 == 2) {
			Mat temp = temp1.clone();
			put_info(temp, info_num, info_name);
			imshow("CV_PROJ1", temp);
			outputVideo << temp;
		}

		if (waitKey(DELAY) >= 0)exit(0);
	}
}
/*
void rec_exc(Point info_num, Point info_name, Mat temp1, Mat temp2) {
	int row = temp1.rows;
	int col = temp1.cols*temp1.channels();
	int num_col = col / 10;
	int ind_col = 0;
	put_info(temp1, info_num, info_name);
	put_info(temp2, info_num, info_name);

	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			uchar* p = temp1.ptr<uchar>(j);
			uchar* q = temp2.ptr<uchar>(j);
			q[i] = p[i];
		}
		ind_col++;
		if (ind_col >= num_col) {
			ind_col = 0;
			imshow("CV_PROJ1", temp2);
			outputVideo << temp2;
		}
		if (waitKey(DELAY) >= 0)exit(0);
	}
}
*/
//绘制片头
void start(Point info_num, Point info_name, int fps,Mat* res) {

	RNG rng(0xffffffff);
	Mat image = Mat::zeros(720, 1080, CV_8UC3);
	Point pt1, pt2, pt3, pt4;

	for (int i = 0; i < 50; i++) {

		pt1.x = 0 + i * 20;
		pt1.y = 0;
		pt2.x = 0;
		pt2.y = 720 - i * 14;

		pt3.y = 0 + i * 14;
		pt3.x = 1080;
		pt4.y = 720;
		pt4.x = 1080 - i * 20;

		line(image, pt1, pt2, random_color(rng), rng.uniform(1, 10), LINE_AA);
		line(image, pt3, pt4, random_color(rng), rng.uniform(1, 10), LINE_AA);

		put_info(image, info_num, info_name);

		imshow("CV_PROJ1", image);
		outputVideo << image;

		if (waitKey(DELAY) >= 0)exit(0);
	}

	pt1.x = 540;
	pt1.y = 360;

	pt2.x = 505;
	pt2.y = 400;

	const char* num[] = { "5","4","3","2","1" };
	Mat temp_image = Mat::zeros(720, 1080, CV_8UC3);
	temp_image = image.clone();

	for (int i = 0; i < fps * 5; i++) {

		circle(image, pt1, 70, random_color(rng), 5, LINE_AA);

		putText(image, num[i / fps], pt2, CV_FONT_HERSHEY_COMPLEX, 4, random_color(rng), LINE_AA);

		put_info(image, info_num, info_name);

		imshow("CV_PROJ1",image);
		outputVideo << image;

		if (i%fps == 0) {
			image = temp_image.clone();
		}

		if (waitKey(DELAY) >= 0)exit(0);
	}
	*res = image.clone();
}

int main(int argc, char** argv) {
	namedWindow("CV_PROJ1", CV_WINDOW_AUTOSIZE);


	const string source = "./a.avi";
	VideoCapture video(source);
	if (!video.isOpened()) {
		cout << "could not open refrence " << source << endl;
		return -1;
	}

	//设置输出视频信息
	Size outSize = { 1080,720 };
	string name = "./a_new.avi";

	//使输出视频速度为原视频速度的两倍
	int fps = video.get(CV_CAP_PROP_FPS) * 2;

	int codec = CV_FOURCC('D', 'I', 'V', 'X');
	//int codec = -1;

	outputVideo.open(name, codec, fps, outSize, true);
	if (!outputVideo.isOpened()) {
		cout << "could not open the output video for write" << endl;
		return -1;
	}

	Point pt1, pt2, pt3, pt4;
	Point info_num, info_name;
	info_num.x = 200;
	info_num.y = 700;
	info_name.x = 600;
	info_name.y = 700;

	Mat res = Mat::zeros(720, 1080, CV_8UC3);

	start(info_num, info_name, fps,&res);

	RNG rng(0xffffffff);

	int r = 0;
	int img_frame = fps * 10;
	for (int i = 0; i <= img_frame; i++) {
		int ind = 0;
		if (i / fps + 1 <= 10) {
			ind = i / fps + 1;
		}
		else {
			ind = 10;
		}
		string n = to_string(ind) + ".jpg";
		Mat temp = imread("./" + n);
		resize(temp, temp, outSize, 0, 0, 1);
		if (i % fps != 0) {
			//读入图片
			put_info(temp, info_num, info_name);
			outputVideo << temp;
		}
		else {
			//设置切换效果

			if (i / fps == 0) {
				Mat temp1 = res.clone();
				FI_FO(info_num, info_name, temp1, temp);
			}
			else {
				if (i / fps != 10) {
					string img_n = "./" + to_string(i / fps) + ".jpg";
					Mat temp1 = imread(img_n);
					resize(temp1, temp1, outSize, 0, 0, 1);
					int x = r++ % 3;
					switch (x)
					{
					default:
						break;
					case 0:
						FI_FO(info_num, info_name, temp1, temp); break;
					case 1:
						blind(info_num, info_name, temp, temp1); break;
					case 2:
						push_drag(info_num, info_name, temp1, temp); break;
					}
				}
				else {
					Mat temp1 = Mat::zeros(720, 1080, CV_8UC3);
					video >> temp1;
					resize(temp1, temp1, outSize, 0, 0, 1);
					FI_FO(info_num, info_name, temp, temp1);
				}
			}
		}

	}
	//将视频存入输出视频
	while (1) {
		video >> res;

		if (res.empty())break;

		resize(res, res, outSize, 0, 0, 1);

		put_info(res, info_num, info_name);
		imshow("CV_PROJ1", res);
		outputVideo << res;
		if (waitKey(DELAY) >= 0)return 0;
	}
	//waitKey(0);
	return 0;
}