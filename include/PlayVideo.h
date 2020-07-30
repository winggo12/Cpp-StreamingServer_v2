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
        cv::VideoCapture cap;
        cv::Mat server_image;
        void StartVideo();
        static void * StartVideoThread(void* );
        void Launch();
        PlayVideo(cv::VideoCapture cap,cv::Mat server_image);
        
};