#include <core/core.hpp>    
#include <highgui/highgui.hpp>    
#include <imgproc/imgproc.hpp>    
#include<iostream>    
using namespace cv;
using namespace std;
Mat image;         //当前帧图像  
Mat imageCopy;     //用于拷贝的当前帧图像  
Mat rectImage;     //子图像  
bool leftButtonDownFlag = false; //左键单击后视频暂停标志位  
Point beginPoint;  //矩形框起点  
Point endPoint;    //矩形框终点  
int resultRows;    //模板匹配result的行  
int resultcols;    //模板匹配result的列  
Mat ImageResult;   //模板匹配result  
double minValue;   //模板匹配result最小值  
double maxValude;  //模板匹配result最大值  
Point minPoint;    //模板匹配result最小值位置  
Point maxPoint;    //模板匹配result最大值位置  
int frameCount = 0; //帧数统计  

void onMouse(int event, int x, int y, int flags, void* ustc); //鼠标回调函数  

int main(int argc, char*argv[])
{
	VideoCapture capture("D:\\opencv\\workplace\\up down.mp4");

	int capture_fps = capture.get(CV_CAP_PROP_FPS); //获取视频帧率 
	int capture_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
	int capture_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int capture_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "视频帧率：" << capture_fps << endl;
	cout << "视频帧数：" << capture_count << endl;
	cout << "视频宽度：" << capture_width << endl;
	cout << "视频高度：" << capture_height << endl;
	int pauseTime = 1000 / capture_fps; //两幅画面中间间隔  
	VideoWriter writer("C:\\Users\\14527\\Desktop\\Video\\out.avi", CV_FOURCC('X', 'V', 'I', 'D'), capture_fps, Size(capture_width, capture_height));
	int choice = 0;
	cin >> choice;
	namedWindow("Video");
	setMouseCallback("Video", onMouse);   //设置鼠标回调函数
	while (choice)
	{
		if (!leftButtonDownFlag) //鼠标左键按下绘制矩形时，视频暂停播放  
		{
			capture >> image;
			frameCount++;   //帧数  
		}
		if (!image.data || waitKey(pauseTime + 30) == 27)  //图像为空或Esc键按下退出播放  
		{
			break;
		}
		if (rectImage.data)
		{
			ImageResult = Mat::zeros(resultRows, resultcols, CV_32FC1);//建立结果矩阵，注意是单通道的32位浮点型
			
			matchTemplate(image, rectImage, ImageResult, TM_CCOEFF);
			
			}
			minMaxLoc(ImageResult, &minValue, &maxValude, &minPoint, &maxPoint, Mat());  //最小值最大值获取  
			Point point;

            point = maxPoint;
				
			rectangle(image, point, Point(point.x + rectImage.cols, point.y + rectImage.rows), Scalar(0, 0, 255), 2); //绘制
																													  //更新当前模板匹配的模板  
			Mat resultImage = image(Rect(point, Point(point.x + rectImage.cols, point.y + rectImage.rows)));
			rectImage = resultImage.clone();
			//当前帧数输出到视频流  
			writer << image;
		}
		imshow("Video", image);
	}
	return 0;
}

//鼠标回调函数    
void onMouse(int event, int x, int y, int flags, void *ustc)
{
	if (event == CV_EVENT_LBUTTONDOWN)   //检测到左键按下时
	{
		leftButtonDownFlag = true; //标志位为true，也就是停止读取下一帧图像  
		beginPoint = Point(x, y);  //设置左键按下点的矩形起点  
		endPoint = beginPoint;
	}
	if (event == CV_EVENT_MOUSEMOVE && leftButtonDownFlag)
	{                               //当鼠标移动且之前左键有按下的话
		imageCopy = image.clone();
		endPoint = Point(x, y);
		if (beginPoint != endPoint)
		{
			//在复制的图像上绘制矩形  
			rectangle(imageCopy, beginPoint, endPoint, Scalar(0, 0, 255), 2);
		}
		imshow("Video", imageCopy);
	}
	if (event == CV_EVENT_LBUTTONUP) //左键放开时，开始匹配
	{
		leftButtonDownFlag = false;
		Mat subImage = image(Rect(beginPoint, endPoint)); //截取图像  
		rectImage = subImage.clone();              //给全局的待匹配图像 
		resultRows = image.rows - rectImage.rows + 1;   //输出结果图像的行数及列数
		resultcols = image.cols - rectImage.rows + 1;
		//imshow("Sub Image", rectImage);
	}
}
