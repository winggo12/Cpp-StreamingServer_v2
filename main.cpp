#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>

#include <pthread.h>
#include <semaphore.h>

cv::Mat server_image;
bool connectionStatus = false;
int remoteSocket;
pthread_mutex_t mutex;
//pthread_t videothread, displaythread, waitingthread, serverthread;

void * StartVideoThread(void* server_img){
    server_image = *(cv::Mat *)server_img;
    cv::VideoCapture cap("../videos/45_Trim.mp4");
    std::cout << "Start Video Now : " << std::endl;
    for (;;) {
        cv::Mat frame;
        cap >> frame;
        cv::resize(frame, frame, cv::Size(640, 480));
        pthread_mutex_lock(&mutex);
        server_image = frame.clone(); 
        pthread_mutex_unlock(&mutex);
        //flip(frame, frame, 1);

        cv::imshow("frame", frame);
        //cv::imshow("server_image", server_image);

        if(cv::waitKey(30) >= 0)
            break;
    }
    pthread_exit(NULL);  

}

void * Display(void* param){
        for (;;) {
        
        cv::imshow("server_image", server_image);

        if(cv::waitKey(30) >= 0)
            break;
    }

    pthread_exit(NULL);
}

void * ServerSendData(void* socket){

    int remoteSocket = *(int *)socket;
   
    while(1){
    int imgSize = server_image.total() * server_image.elemSize();
    int bytes = 0;
    //send(remoteSocket, server_image.data, imgSize, 0);
    //Change format for QT
    //cv::cvtColor(image, image, CV_BGR2RGB); 

        if ((bytes = send(remoteSocket, server_image.data, imgSize, 0)) < 0){
            std::cerr << "bytes = " << bytes << std::endl;
            break;
        }

        std::cerr << "bytes = " << bytes << std::endl;

    }

    pthread_exit(NULL);
    
}

void * ServerWaitForConnection(void* port_num){
    int port_number = *(int *)port_num;
    //struct  sockaddr_in localAddr, remoteAddr;
    //pthread_t thread_id;

    int addrLen = sizeof(struct sockaddr_in);

    int localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    struct  sockaddr_in localAddr, remoteAddr;
    int port = port_number;
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }
    
    //Listening
    int max_client = 3;
    //remoteSocket;
    listen(localSocket , max_client);
        std::cout <<  "Waiting for connections...\n"
                  <<  "Server Port:" << port << std::endl;
    while(1){
       
      remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
      std::cout << remoteSocket << std::endl;
        if (remoteSocket < 0) {
            perror("accept failed!");
            exit(1);
        } 
    std::cout << "Connection accepted" << std::endl;
    connectionStatus = true;
    
    }

    //pthread_create(&th, NULL, &ServerSendData, &remoteSocket);
    

}


int main()
{   
    pthread_mutex_init(&mutex, NULL);
    pthread_t videothread, displaythread, waitingthread, serverthread;
    std::string vid_path = "../videos/45_Trim.mp4";
    server_image = cv::Mat::zeros(480, 640, CV_8UC3);
    cv::Mat image ;
    int port_num = 4097;

    pthread_create(&videothread, NULL, &StartVideoThread, &server_image);
    //pthread_create(&displaythread, NULL, &Display, NULL);
    pthread_create(&waitingthread, NULL, &ServerWaitForConnection, &port_num);
    while(true){
        if(connectionStatus == true){
            pthread_create(&serverthread, NULL, &ServerSendData, &remoteSocket);
        }
    }
    pthread_join(videothread, NULL); 
    //pthread_join(displaythread, NULL); 
    pthread_join(waitingthread, NULL);
    pthread_join(serverthread, NULL);
    pthread_mutex_destroy(&mutex);


    return 0;

}