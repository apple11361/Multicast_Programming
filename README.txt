Multicast_Programming homework

測試環境：linux/ubuntu 14.04



/**********************************使用說明**************************************/
這是一支可以利用multicast 或是 unicast + multi-thread
來同時對多個client 傳送檔案的程式



編譯方法：
gcc lab2.c -lpthread -o <filename>

編譯範例：
gcc lab2.c -lpthread -o lab2

執行方法：
./<execute filename> <server or client> <multicast or unicast> (IP) (file_name)

1.如果是 unicast 且是 client，需設定要連線的 server IP
2.兩種方法的 server 都需要設定file_name
3.使用unicast 如果傳送檔案很大，較multicast需明顯花費較多傳送時間
4.沒有做FEC

執行範例：

1.multicast
    (1)分別在資料夾A, B, C  執行：./lab2 client multicast
    (2)在要傳送檔案的資料夾 執行：./lab2 server multicast test.mp3
       就會把test.mp3傳送到三個資料夾了
    (3)可以很多client
    (4)test.mp3 可以替換成任何類型的檔案
    (5)已試驗過不同電腦可以傳送成功

2.unicast multi-thread
    (1)在要傳送檔案的資料夾 執行：./lab2 server unicast test.mp3
    (2)在要接收檔案的資料夾 執行：./lab2 client unicast 192.168.0.5
    (3)實驗時因為在子網路下所以是192.168.0.5
       助教測試時server IP需依照正確IP輸入，同一台電腦可用127.0.0.1
    (4)可以同時執行許多client
    (5)test.mp3 可以替換成任何類型的檔案
    (6)已試驗過不同電腦可以傳送成功


/***********************************心得*******************************************/
multicast 的部份，依照助教的範例程式碼學習再搭配上第一次作業的datagram socket不算太難
。但還是有一個小問題一開始卡很久，那就是一開始不知道怎麼讓server也傳送一份datagram給自
己，看範例程式碼loopback沒關一樣不成功，最後直接把server也加入group類似同時成為client
的概念，總算是可以接收到檔案。
unicast multi-thread的部份就是把第一次作業的TCP使用pthread實作傳送檔案的部份而已。遇到
的困難點就是有時候server會傳很快，導致client來不及接收造成資料亂掉，不是很清楚為什麼用
stream socket還會這樣。所以一樣每次傳送一次，就等待client回傳check。


