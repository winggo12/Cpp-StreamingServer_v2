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

#define ConnectionMaxNum 3

cv::Mat server_image;
bool connectionStatus = false;
int remoteSocket;

pthread_mutex_t mutex, connectionstatus_mutex;
pthread_cond_t cond , cstatus , rewait;


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
        pthread_cond_signal(&cond);
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

    int thisRemoteSocket = *(int *)socket;
    
    while(1){
    pthread_cond_wait(&cond, &mutex);
    int imgSize = server_image.total() * server_image.elemSize();
    int bytes = 0;
    //send(remoteSocket, server_image.data, imgSize, 0);
    //Change format for QT
    //cv::cvtColor(image, image, CV_BGR2RGB); 
        
        if ((bytes = send(thisRemoteSocket, server_image.data, imgSize, 0)) < 0){
            std::cerr << "bytes(-ve) = " << bytes << std::endl;
            //pthread_cond_signal(&rewait);
            //exit(1);
            break;
        }

        std::cerr << "bytes = " << bytes << std::endl;
        std::cout << "thisRemoteSocket: " << thisRemoteSocket << std::endl;
    }

    //close(thisRemoteSocket);
    std::cout << "Closing Socket and Exiting Pthread" << std::endl;
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
      std::cout << "Waiting for Acception:" << std::endl;
      remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
      std::cout << remoteSocket << std::endl;
        if (remoteSocket < 0) {
            std::cout << "accept failed!" << std::endl ;
            perror("accept failed!");
            //exit(1);
        }
        else{
            pthread_mutex_lock(&connectionstatus_mutex);
            connectionStatus = true;
            pthread_cond_signal(&cstatus);
            pthread_mutex_unlock(&connectionstatus_mutex);
        } 
    std::cout << "Connection accepted" << std::endl;
    //connectionStatus = true;
    
    }

    //pthread_create(&th, NULL, &ServerSendData, &remoteSocket);
    

}


int main()
{   
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&connectionstatus_mutex, NULL);
    pthread_t videothread, displaythread, waitingthread, serverthread;
    server_image = cv::Mat::zeros(640, 480, CV_8UC3);
    cv::Mat image ;
    int port_num = 4096;

    pthread_create(&videothread, NULL, &StartVideoThread, &server_image);
    //pthread_create(&displaythread, NULL, &Display, NULL);
    pthread_create(&waitingthread, NULL, &ServerWaitForConnection, &port_num);
    while(true){

        pthread_mutex_lock(&connectionstatus_mutex);
        pthread_cond_wait(&cstatus, &connectionstatus_mutex);
        if(connectionStatus == true){
            pthread_create(&serverthread, NULL, &ServerSendData, &remoteSocket);
            connectionStatus == false;
        }
        std::cout << "connectionStatus : " <<  connectionStatus << " remotesocket: "<< remoteSocket <<std::endl;
        pthread_mutex_unlock(&connectionstatus_mutex);
    }
    pthread_join(videothread, NULL); 
    //pthread_join(displaythread, NULL); 
    pthread_join(waitingthread, NULL);
    pthread_join(serverthread, NULL);
    pthread_mutex_destroy(&mutex);


    return 0;

}