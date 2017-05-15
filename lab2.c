#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256


int main(int argc, char *argv[])
{
    int reuse = 1;
    int temp;
    char buffer[BUFFER_SIZE];
    FILE *fp;                           //檔案指標
    int sock_fd;
    struct sockaddr_in group_addr;      //multicast 的 group 位址結構
    struct sockaddr_in my_addr;         //本地端位址結構
    struct ip_mreq group;               //設定multicast的group

    /*********程式參數*********/
    int mode;           //0:multicast, 1:unicast
    int role;           //0:server, 1:client
    char file_name[256]; 

    /******************檢查、設定參數******************/
    if(argc<3)
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<執行檔名> <server or client> <multicast or unicast> (IP) (file_name)\n");
        exit(1);
    }
    
    //設定role
    if(strcmp(argv[1], "server") && strcmp(argv[1], "client"))
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<執行檔名> <server or client> <multicast or unicast> (IP) (file_name)\n");
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
        printf("執行範例：./<執行檔名> <server or client> <multicast or unicast> (IP) (file_name)\n");
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
        printf("執行範例：./<執行檔名> <server or client> <multicast or unicast> (IP) (file_name)\n");
        exit(1);
    }
    //...

    //設定要傳送的檔案
    if(!role && argc!=4)
    {
        printf("參數輸入錯誤\n");
        printf("執行範例：./<執行檔名> <server or client> <multicast or unicast> (IP) (file_name)\n");
        exit(1);

    }
    else if(!role)
    {
        strcpy(file_name, argv[3]);

        fp = fopen(file_name, "rb");
        if(fp==NULL)
        {
            printf("fopen() failed.\n");
            exit(1);
        }
    }
    
    /*****************server multicast send file*****************/
    if(!mode && !role)
    {
        /**Create a datagram socket to send data**/
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock_fd<0)
        {
            printf("Opening datagram socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Opening datagram socket OK.\n");
        }

        /**依照group address 226.1.1.1和port 8787設定group sockaddr structure**/
        memset((char *)&group_addr, 0, sizeof(group_addr));
        group_addr.sin_family = AF_INET;
        group_addr.sin_addr.s_addr = inet_addr("226.1.1.1");
        group_addr.sin_port = htons(8787);

        /************設定本地端位址結構，以及socket***************/        
        memset((char *)&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port = htons(8787);

        //setsockopt 才可以使用重複port
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse))<0)
        {
            printf("Setting socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Setting socket OK.\n");
        }

        //bind
        if(bind(sock_fd, (struct sockaddr *)&group_addr, sizeof(group_addr))<0)
        {
            printf("Binding socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Binding socke OK.\n");
        }

        //setsockopt
        if(setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&my_addr.sin_addr, sizeof(my_addr.sin_addr))<0)
        {
            printf("Setting socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Setting socket OK.\n");
        }

        /*********開始傳送資料**********/
        printf("Sending file...\n");

        /***********先傳檔名************/
        strcpy(buffer, file_name);
        if(sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&group_addr, sizeof(group_addr))<0)
        {
            printf("Sending file_name failed.\n");
            exit(1);
        }

        /*********確認回傳訊息*********/
        if(read(sock_fd, buffer, BUFFER_SIZE)<0)
        {
            printf("Reading check failed.\n");
            exit(1);
        }
        else
        {
            printf("Sending file_name: %s OK.\n", buffer);
        }

        /*********再傳檔案內容*********/
        while(!feof(fp))
        {
            //傳送
            temp = fread(buffer, sizeof(char), BUFFER_SIZE, fp);
            if(sendto(sock_fd, buffer, temp, 0, (struct sockaddr *)&group_addr, sizeof(group_addr))<0)
            {
                printf("Sending file failed.\n");
                exit(1);
            }

            //check
            if(read(sock_fd, buffer, BUFFER_SIZE)<0)
            {
                printf("Reading check failed.\n");
                exit(1);
            }
        }

        printf("Sending file OK.\n");
        fclose(fp);
        close(sock_fd);
    
    }
    
    /****************client multicast receive file***************/
    else if(!mode && role)
    {
        /*********Create a datagram socket to receive data**********/
        sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sock_fd<0)
        {
            printf("Opening datagram socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Opening datagram socket OK.\n");
        }

        /**************setsockopt 才可以使用重複port***************/
        if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse))<0)
        {
            printf("Setting socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Setting socket OK.\n");
        }

        /************設定本地端位址結構，以及socket***************/
        memset((char *)&my_addr, 0, sizeof(my_addr));
        my_addr.sin_family = AF_INET;
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        my_addr.sin_port = htons(8787);

        //bind
        if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(my_addr))<0)
        {
            printf("Binding socket failed.\n");
            exit(1);
        }
        else
        {
            printf("Binding socket OK.\n");
        }

        /*************加入 multicast group 226.1.1.1***************/
        group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
        group.imr_interface.s_addr = htonl(INADDR_ANY);
        if(setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group))<0)
        {
            printf("Adding multicast group failed.\n");
            exit(1);
        }
        else
        {
            printf("Adding multicast group OK.\n");
        }

        /*********************準備接收資料************************/
        printf("Ready to receive...\n");

        /**********************先接收檔名*************************/
        temp = read(sock_fd, buffer, BUFFER_SIZE);
        if(temp<0)
        {
            printf("Receiving file_name failed.\n");
            exit(1);
        }
        else
        {
            printf("Receiving file_name OK.\n");
        }
        buffer[temp] = '\0';        //完全不知道為什麼不加這個會錯，好想知道..........

        //開檔
        fp = fopen(buffer, "wb");
        if(fp==NULL)
        {
            printf("fopen() failed.\n");
            exit(1);
        }
        else
        {
            printf("fopen() OK.\n");
        }

        /********************再接收檔案內容***********************/
        while(1)
        {
            //接收
            temp = read(sock_fd, buffer, BUFFER_SIZE);
            if(temp<0)
            {
                printf("Receiving file_name failed.\n");
                exit(1);
            }
            buffer[temp] = '\0';        //完全不知道為什麼不加這個會錯，好想知道..........
            fwrite(buffer, sizeof(char), temp, fp);

            //檔案結尾
            if(temp>=0 && temp<BUFFER_SIZE)
            {
                break;
            }
        }

        printf("Receiving file OK.\n");
        fclose(fp);
        close(sock_fd);


    }

    /***********server unicast multi_thread sned file************/
    else if(mode && !role)
    {
    }

    return 0;
}





