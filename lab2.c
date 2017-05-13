#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
    int sock_fd;


    /*********程式參數*********/
    int mode;           //0:multicast, 1:unicast
    int role;           //0:server, 1:client

    /******************檢查、設定參數******************/
    if(argc<3)
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<execute filename> <server or client> <multicast or unicast> (IP)\n");
        exit(1);
    }
    
    //設定role
    if(strcmp(argv[1], "server") && strcmp(argv[1], "client"))
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<execute filename> <server or client> <multicast or unicast> (IP)\n");
        exit(1);
    }
    if(!strcmp(argv[1], "server"))
    {
        role = 0;
    }
    else
    {
        role = 1;
    }

    //設定mode
    if(strcmp(argv[2], "multicast") && strcmp(argv[2], "unicast"))
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<execute filename> <server or client> <multicast or unicast> (IP)\n");
        exit(1);
    }
    if(!strcmp(argv[2], "multicast"))
    {
        mode = 0;
    }
    else
    {
        mode = 1;
    }
    
    //如果是unicast且是client，設定要連線的server IP
    if(role && mode && argc!=4)
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<execute filename> <server or client> <multicast or unicast> (IP)\n");
        exit(1);
    }


    /********************server send file********************/
    if(!mode)
    {
        /**Create a datagram socket to send data**/
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    }
    
    /*******************client receive file******************/
    else
    {

    }


    return 0;
}





