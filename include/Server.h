#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>


class Server {
    public:
        cv::Mat image = cv::Mat::zeros(640, 480, CV_8UC3);
        struct  sockaddr_in localAddr, remoteAddr;
        int max_client = 3;
        pthread_t waitingThread , serverThread;
        int addrLen;
        int localSocket,remoteSocket;
        bool connectionStatus = false;
        //int port = 4097;   
        int port;
        //int socket;
        void Init();
        
        void WaitingConnection();
        static void * WaitingConnectionThread(void* );
        void StartWaiting();

        void SendData();
        static void * SendDataThread(void* );
        void StartSending();
        
        static void * SDT(void* );

        //static void * threadFunc(void *);
        int Connect();
        //void CreateThread();
        Server(int port_num, pthread_t waitingThread, pthread_t serverThread);

};