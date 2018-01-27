#include <iostream>
#include <string>
#include <ostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#pragma comment(lib, "opencv_world300d.lib")

//���̂̏��
struct obj {
	//�����W
	int width[2];
	//�n�_
	int y;
};

//�V�����L�m�R�I�u�W�F�N�g����邩�`�F�b�N����֐�
void times_check(int &times, std::vector<struct obj> &width){
	/*times��2�ɂȂ�����V�������Atimes��0�ɂ���B*/
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
		//�G�b�W�̑��������o���Ȃ��悤�ɂ���
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
				//�V�����L�m�R�I�u�W�F�N�g��ǉ����邩�`�F�b�N
				times_check(times, kinoko);
				//���̂̕������
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
		//�I�[�v�j���O
		cv::erode(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::dilate(src_img, src_img, cv::Mat(), cv::Point(-1, -1), 2);
		cv::GaussianBlur(src_img, src_img, cv::Size(9, 7), 10, 10);
		//�N���[�W���O
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

	//�L�m�R���͂�
	//�L�m�R�̎P���A���W������\����
	std::vector<struct obj> kinoko;
	//�L�m�R��������֐�
	find_kinoko(canny_img, kinoko);
	cv::imshow("image", canny_img);
	cv::waitKey(0);

	cv::imwrite("C:\\file\\result\\kinoko1\\43.jpg", canny_img);
	return 0;
}