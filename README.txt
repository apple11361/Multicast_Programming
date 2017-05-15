Multicast_Programming

Multicast_Programming homework

使用說明

編譯方法：
gcc lab2.c -o <filename>

執行方法：
./<execute filename> <server or client> <multicast or unicast> (IP) (file_name)

1.如果是 unicast 且是 client，需設定要連線的 server IP
2.如果是執行 multicast server 但沒有先執行 multicast client server會卡住沒反應


執行範例：

1.multicast
    (1)分別在資料夾A, B, C  執行：./lab2 client multicast
    (2)在要傳送檔案的資料夾 執行：./lab2 server multicast test.mp3
    就會把test.mp3傳送到三個資料夾了，不同電腦還沒嘗試過，不過理論上可以傳送
