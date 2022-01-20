# tcp_clinet_server
### 简介：服务端运行环境：linux 
###       客户端运行环境windows ，客户端源码为QT
###       实现了不同客户端之间的通讯

# 客户端使用说明
1.在tcpserver_final.c文件中填写服务端的端口
2.编译tcpserver_final.c，编译指令
```
gcc tcpserver_final.c -lpthread -o serverfinal
```
3.运行
```
./serverfinal
```
备注：后台运行指令
```
nohup ./serverfinal &
```
# 服务端使用说明
1.启动软件main_client.exe

2.设置昵称，填写端口，服务器ip或域名

3.确定，连接服务器，连接成功即可使用

注意：运行软件时可能会少dll，建议自己重新编译Qt源文件

