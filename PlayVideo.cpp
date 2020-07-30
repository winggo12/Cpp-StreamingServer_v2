#include "PlayVideo.h"

PlayVideo::PlayVideo(cv::VideoCapture c,cv::Mat server_img){
        cap = c;
        server_image = server_img;
}

void PlayVideo::StartVideo(){

        cv::Mat image;
        std::cout << "cap.isOpened() : " << cap.isOpened();
        while(cap.isOpened()){
            cap >> image;
            cv::resize(image,server_image, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
            cv::imshow("Image", server_image);
            if(image.empty()){
                break;
            }
        }
}

void * PlayVideo::StartVideoThread(void* __this){
    PlayVideo * _this =(PlayVideo *)__this;
    _this-> StartVideo();
}

void PlayVideo::Launch(){
        pthread_t videoThread;
        pthread_create(&videoThread,NULL,StartVideoThread,(void*)this);
}