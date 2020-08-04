#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>


class PlayVideo
{   
    public:
        pthread_t videoThread;
        //cv::VideoCapture cap;
        std::string video_path;
        cv::Mat server_image;
        void StartVideo();
        static void * StartVideoThread(void* );
        void Launch();
        PlayVideo(pthread_t videoTh,std::string vid_path,cv::Mat server_image);
        
};