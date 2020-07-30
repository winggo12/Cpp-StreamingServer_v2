#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include "Server.h"

using namespace cv;

void videoThread(VideoCapture cap , cv::Mat server_image){
        cv::Mat image;
        while(cap.isOpened()){
            cap >> image;
            cv::resize(image,server_image, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
            cv::imshow("Image", server_image);
            if(image.empty())
            {
                break;
            }
        }
}

int main()
{   
    pthread_t videothread;
    VideoCapture cap("../videos/45_Trim.mp4"); // open the default camera
//VideoCapture cap(capDev);

    std::cout << (cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;
    cv::Mat image;
    int port_num = 4097;
    
    Server server = Server(port_num);
    
    pthread_create(&videothread, NULL, videoThread(cap,server.image), NULL);

    server.Init();

    if (server.Connect() > 0 )
    {
        server.StartSending();
    };

    std::cout << "Server is sending data " <<  std::endl;

    // while(cap.isOpened()){
    //     cap >> image;
    //     cv::resize(image,server.image, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
    //     cv::imshow("Image", server.image);
    //     if(image.empty())
	// 	{
	// 		break;
	// 	}
		//v::imshow("Image", image);
        
        // if (server.Connect() > 0 ){
        //     server.StartSending();
        // };

        //server.SendData(image);
        //cv::waitKey(1);
        
    //}


}