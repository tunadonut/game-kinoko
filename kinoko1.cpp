#include <iostream>
#include <string>
#include <ostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

//物体の情報
struct obj {
	//幅座標
	int width[2];
	//始点
	int y;
};

//新しいキノコオブジェクトを作るかチェックする関数
void times_check(int &times, std::vector<struct obj> &width){
	/*timesが2になったら新しく作り、timesを0にする。*/
	if (times % 2 == 0){
		struct obj kinoko;
		width.push_back(kinoko);
		times = 0;
	}
}

void find_width(int &times, std::vector<struct obj> &kinoko, int &x, int y){
	if (times == 0){
		kinoko[kinoko.size() - 1].width[0] = x;
		kinoko[kinoko.size() - 1].y = y;
		times++;
		//エッジの太さを検出しないようにする
		x += 4;
	}
	if (times == 1){
		kinoko[kinoko.size() - 1].width[1] = x;
	}
}
void find_kinoko(cv::Mat &canny_img, std::vector<struct obj> &kinoko){
	int times = 0;
	int index = 0;
	for (int y = 0; y < canny_img.rows; y++){
		times = 0;
		for (int x = 0; x < canny_img.cols; x++){
			index = (y * canny_img.cols + x) * canny_img.channels();
			if (canny_img.data[index] == 0){
				//新しくキノコオブジェクトを追加するかチェック
				times_check(times, kinoko);
				//物体の幅を取る
				find_width(times, kinoko, x, y);
			}
		}
	}
}
int main(int argc, char *argy[]){
	//8, 43
	cv::Mat src_img = cv::imread("C:\\file\\pos\\t_kinoko\\humika.jpg");
	
	if (src_img.empty()){
		std::cout << "cannot open file" << std::endl;
		return -1;
	}

	for (int i = 0; i < 2; i++){
		//cv::GaussianBlur(src_img, src_img, cv::Size(5, 5), 10, 10);
		//オープニング
		cv::erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::GaussianBlur(src_img, src_img, cv::Size(9, 7), 10, 10);
		//クロージング
		//cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		//cv::erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		//cv::GaussianBlur(src_img, src_img, cv::Size(9, 7), 10, 10);

		//cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 1);
	}
	cv::namedWindow("image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("image", src_img);
	cv::waitKey(0);

	cv::Mat channels[3];
	cv::split(src_img, channels);
	for (int i = 0; i < 3; i++){
		cv::Canny(channels[i], channels[i], 60, 180, 3);
	}
	cv::Mat canny_img;
	cv::bitwise_or(channels[0], channels[1], canny_img);
	cv::bitwise_or(channels[2], canny_img, canny_img);
	canny_img = ~canny_img;

	//キノコを囲む
	//キノコの傘幅、座標を入れる構造体
	std::vector<struct obj> kinoko;
	//キノコを見つける関数
	find_kinoko(canny_img, kinoko);
	cv::imshow("image", canny_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\kinoko1\\43.jpg", canny_img);
	return 0;
}