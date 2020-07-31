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
    pthread_t videothread , waitingthread , serverthread;
    std::string vid_path = "../videos/45_Trim.mp4";
    //cv::VideoCapture cap("../videos/45_Trim.mp4"); // open the default camera
    //VideoCapture cap(capDev);

    //std::cout << (cap.isOpened()? "Initialized Video Capture" : "Initialization failed") << std::endl;
    cv::Mat image;
    int port_num = 4097;
    
    Server server = Server(port_num, waitingthread, serverthread);
    
    PlayVideo video = PlayVideo(videothread,vid_path,server.image);

    //pthread_create(&videothread,NULL,video.StartVideoThread,NULL);
    //pthread_join(videothread, NULL);

     video.Launch();
     server.StartWaiting();
     while(server.connectionStatus == false){
        
        if(server.connectionStatus == true){
            std::cout << "StartSending" << std::endl;
            server.StartSending();

            server.connectionStatus == false;
            break;
        }

     }


     pthread_join(video.videoThread, NULL);
     pthread_join(server.waitingThread, NULL);
     pthread_join(server.serverThread, NULL);

    return 0;

}