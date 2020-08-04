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

cv::Mat get_image;

int main()
{   
    pthread_t videothread , waitingthread , serverthread;
    std::string vid_path = "../videos/45_Trim.mp4";
    //cv::VideoCapture cap("../videos/45_Trim.mp4"); // open the default camera
    //VideoCapture cap(capDev);

    //std::cout << (cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;
    get_image = cv::Mat::zeros(480, 640, CV_8UC3);
    cv::Mat image ;
    int port_num = 4097;
    
    Server server = Server(port_num, waitingthread, serverthread);
    server.image = cv::Mat::zeros(480, 640, CV_8UC3);
    
    PlayVideo video = PlayVideo(videothread,vid_path,get_image);

    //pthread_create(&videothread,NULL,video.StartVideoThread,NULL);
    //pthread_join(videothread, NULL);

    pthread_create(&videothread,NULL,&video.StartVideoThread,&video);
    pthread_create(&waitingthread,NULL,&server.WaitingConnectionThread,&server);

    while(true){
        //server.image = get_image;
        cv::imshow("server.image", get_image);
    }

    // while(server.connectionStatus == false){
        
    //     if(server.connectionStatus == true){
    //         std::cout << "StartSending" << std::endl;
        
    //         //server.connectionStatus == false;
    //         break;
    //     }

    //  }

    //pthread_create(&serverthread,NULL,&server.SendDataThread,&server);

    pthread_join(videothread, NULL);
    pthread_join(waitingthread, NULL);
    pthread_join(serverthread, NULL);

    return 0;

}