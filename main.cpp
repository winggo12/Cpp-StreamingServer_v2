#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include "Server.h"
#include "PlayVideo.h"

int main()
{   
    pthread_t videothread;
    cv::VideoCapture cap("../videos/45_Trim.mp4"); // open the default camera
//VideoCapture cap(capDev);

    //std::cout << (cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;
    cv::Mat image;
    int port_num = 4097;
    
    Server server = Server(port_num);
    
    PlayVideo video = PlayVideo(cap,server.image);

    std::cout << (video.cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;

    video.Launch();

    // server.Init();

    // if (server.Connect() > 0 )
    // {
    //     server.StartSending();
    // };

    //std::cout << "Server is sending data " <<  std::endl;

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