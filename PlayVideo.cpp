#include "PlayVideo.h"

PlayVideo::PlayVideo(pthread_t videoTh,std::string vid_path,cv::Mat server_img){
        //cap = c;
        videoThread = videoTh;
        video_path = vid_path;
        server_image = server_img;
}

void PlayVideo::StartVideo(){

    cv::VideoCapture cap("../videos/45_Trim.mp4");
    std::cout << "Start Video Now : " << std::endl;
    for (;;) {
        cv::Mat frame;
        cap >> frame;
        cv::resize(frame, frame, cv::Size(640, 480));
        server_image = frame.clone();
        flip(frame, frame, 1);

        imshow("frame", frame);

        if(cv::waitKey(30) >= 0)
            break;
    }
    pthread_exit(NULL);  

}

void * PlayVideo::StartVideoThread(void* __this){
    std::cout << "StartVideoThread" << std::endl;
    PlayVideo * _this =(PlayVideo *)__this;
    _this-> StartVideo();
    std::cout << "StartVideoThread -> StartVideo() " << std::endl;
}

void PlayVideo::Launch(){
        
        int ret = pthread_create(&videoThread,NULL,&PlayVideo::StartVideoThread,this);
        //Let the main function Waiting for the serverThread end , otherwise main will break instantly 
        //pthread_join(videoThread, NULL);

        if(ret != 0){
            std::cout  << "videoThread's creation error!\n";
            exit(1);
        }
}