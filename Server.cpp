#include "Server.h"

Server::Server(int port_num){
    Server::port = port_num;
}

void Server::Init(){
    //struct  sockaddr_in localAddr, remoteAddr;
    //pthread_t thread_id;

    addrLen = sizeof(struct sockaddr_in);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }
    
    //Listening
    listen(localSocket , 3);
        std::cout <<  "Waiting for connections...\n"
                  <<  "Server Port:" << port << std::endl;
}


void Server::SendData(){

    cv::resize(image,image, cv::Size(640,480), 0, 0, cv::INTER_CUBIC);
    int imgSize = image.total() * image.elemSize();
    int bytes = 0;

    //Change format for QT
    cv::cvtColor(image, image, CV_BGR2RGB); 

    cv::imshow("Sending Image ", image);
    cv::waitKey(1);
    //send(port, image.data, imgSize, 0);

    //send processed image
    while(1){
            if ((bytes = send(port, image.data, imgSize, 0)) < 0){
            std::cerr << "bytes = " << bytes << std::endl;
            //break;
            }
    }


}

void * Server::SendDataThread(void* __this){
    Server * _this =(Server *)__this;
    _this -> SendData();
}

void Server::StartSending(void){
    
    int result = pthread_create(&thread_id, NULL, SendDataThread, (void*)this);
}





int Server::Connect(){
    
      remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
      std::cout << remoteSocket << std::endl;

      return remoteSocket;

}

