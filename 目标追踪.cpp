#include <core/core.hpp>    
#include <highgui/highgui.hpp>    
#include <imgproc/imgproc.hpp>    
#include<iostream>    
using namespace cv;
using namespace std;
Mat image;         //��ǰ֡ͼ��  
Mat imageCopy;     //���ڿ����ĵ�ǰ֡ͼ��  
Mat rectImage;     //��ͼ��  
bool leftButtonDownFlag = false; //�����������Ƶ��ͣ��־λ  
Point beginPoint;  //���ο����  
Point endPoint;    //���ο��յ�  
int resultRows;    //ģ��ƥ��result����  
int resultcols;    //ģ��ƥ��result����  
Mat ImageResult;   //ģ��ƥ��result  
double minValue;   //ģ��ƥ��result��Сֵ  
double maxValude;  //ģ��ƥ��result���ֵ  
Point minPoint;    //ģ��ƥ��result��Сֵλ��  
Point maxPoint;    //ģ��ƥ��result���ֵλ��  
int frameCount = 0; //֡��ͳ��  

void onMouse(int event, int x, int y, int flags, void* ustc); //���ص�����  

int main(int argc, char*argv[])
{
	VideoCapture capture("D:\\opencv\\workplace\\up down.mp4");

	int capture_fps = capture.get(CV_CAP_PROP_FPS); //��ȡ��Ƶ֡�� 
	int capture_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
	int capture_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int capture_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "��Ƶ֡�ʣ�" << capture_fps << endl;
	cout << "��Ƶ֡����" << capture_count << endl;
	cout << "��Ƶ��ȣ�" << capture_width << endl;
	cout << "��Ƶ�߶ȣ�" << capture_height << endl;
	int pauseTime = 1000 / capture_fps; //���������м���  
	VideoWriter writer("C:\\Users\\14527\\Desktop\\Video\\out.avi", CV_FOURCC('X', 'V', 'I', 'D'), capture_fps, Size(capture_width, capture_height));
	int choice = 0;
	cin >> choice;
	namedWindow("Video");
	setMouseCallback("Video", onMouse);   //�������ص�����
	while (choice)
	{
		if (!leftButtonDownFlag) //���������»��ƾ���ʱ����Ƶ��ͣ����  
		{
			capture >> image;
			frameCount++;   //֡��  
		}
		if (!image.data || waitKey(pauseTime + 30) == 27)  //ͼ��Ϊ�ջ�Esc�������˳�����  
		{
			break;
		}
		if (rectImage.data)
		{
			ImageResult = Mat::zeros(resultRows, resultcols, CV_32FC1);//�����������ע���ǵ�ͨ����32λ������
			
			matchTemplate(image, rectImage, ImageResult, TM_CCOEFF);
			
			}
			minMaxLoc(ImageResult, &minValue, &maxValude, &minPoint, &maxPoint, Mat());  //��Сֵ���ֵ��ȡ  
			Point point;

            point = maxPoint;
				
			rectangle(image, point, Point(point.x + rectImage.cols, point.y + rectImage.rows), Scalar(0, 0, 255), 2); //����
																													  //���µ�ǰģ��ƥ���ģ��  
			Mat resultImage = image(Rect(point, Point(point.x + rectImage.cols, point.y + rectImage.rows)));
			rectImage = resultImage.clone();
			//��ǰ֡���������Ƶ��  
			writer << image;
		}
		imshow("Video", image);
	}
	return 0;
}

//���ص�����    
void onMouse(int event, int x, int y, int flags, void *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)   //��⵽�������ʱ
	{
		leftButtonDownFlag = true; //��־λΪtrue��Ҳ����ֹͣ��ȡ��һ֡ͼ��  
		beginPoint = Point(x, y);  //����������µ�ľ������  
		endPoint = beginPoint;
	}
	if (event == CV_EVENT_MOUSEMOVE && leftButtonDownFlag)
	{                               //������ƶ���֮ǰ����а��µĻ�
		imageCopy = image.clone();
		endPoint = Point(x, y);
		if (beginPoint != endPoint)
		{
			//�ڸ��Ƶ�ͼ���ϻ��ƾ���  
			rectangle(imageCopy, beginPoint, endPoint, Scalar(0, 0, 255), 2);
		}
		imshow("Video", imageCopy);
	}
	if (event == CV_EVENT_LBUTTONUP) //����ſ�ʱ����ʼƥ��
	{
		leftButtonDownFlag = false;
		Mat subImage = image(Rect(beginPoint, endPoint)); //��ȡͼ��  
		rectImage = subImage.clone();              //��ȫ�ֵĴ�ƥ��ͼ�� 
		resultRows = image.rows - rectImage.rows + 1;   //������ͼ�������������
		resultcols = image.cols - rectImage.rows + 1;
		//imshow("Sub Image", rectImage);
	}
}
