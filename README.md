# brick
Make a brick game
游戏名：打砖块

打开exe方式：双击运行

server.exe为OnlineScene.cpp、OnlineScene.h和   除了OnlineScene1.cpp、OnlineScene1.h的其他文件   一同编译运行得到，为服务器端

client.exe为OnlineScene1.cpp、OnlineScene1.h和   除了OnlineScene.cpp、OnlineScene.h的其他文件   一同编译运行得到，为客户端

即OnlineScene.cpp和OnlineScene1.cpp为相互替代的程序，若玩单机模式，则没有区别

玩法介绍：通过按A/D键实现板的左右移动，保证小球不碰到下边界（否则游戏判负）；击打宝箱可获得底板加长道具、得分加倍道具；在游戏开始界面可查看历史得分排行榜前三位，可选择单机版和多人版。

（多人版）玩法介绍：首先在OnlineScene.cpp里将ip地址改为双方的ip地址，若不修改则默认为127.0.0.1本机，打开两个exe（服务器端与客户端），同时执行多人版，在游戏界面右半部分可查看对方消除砖块情况，一方消除完成、对方直接判负；或一方小球碰到下边界，则该方直接判负。