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

void *VideoThread(void *arg){

    cv::VideoCapture cap("../videos/45_Trim.mp4");
    for (;;) {
        cv::Mat frame;
        cap >> frame;
        cv::resize(frame, frame, cv::Size(640, 480));
        flip(frame, frame, 1);

        imshow("frame", frame);

        if(cv::waitKey(30) >= 0)
            break;
    }
    pthread_exit(NULL);   
}

int main()
{   
    pthread_t videothread;
    std::string vid_path = "../videos/45_Trim.mp4";
    //cv::VideoCapture cap("../videos/45_Trim.mp4"); // open the default camera
    //VideoCapture cap(capDev);

    //std::cout << (cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;
    cv::Mat image;
    int port_num = 4097;
    
    Server server = Server(port_num);
    
    PlayVideo video = PlayVideo(videothread,vid_path,server.image);

    //pthread_create(&videothread,NULL,video.StartVideoThread,NULL);
    //pthread_join(videothread, NULL);

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

    return 0;

}