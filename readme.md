# C++聊天器

使用了**socket**和**select epoll**  

**好消息**是：
- 服务器端可以接受来自多台主机的消息

**坏消息**是：~~坏消息有点多~~
- 目前**仅**支持服务器向某一**固定**用户发送信息
- （~~something worse~~）服务器发送消息用户接受不到(所以我也不知道目前写出来的这个东西的存在意义在哪)
>目前发现是用户端防火墙的问题，但是我还是不知道怎么办（摊手
- 由于编码问题，目前仅支持**英文聊天**

**server.cpp** 放在Linux服务器上，**client.cpp**放在本地

---
后续是否会继续修正全看心情，目前搞了一个周搞成这个b样:shit:我属实是没啥心情了，等啥时候空了再来弄弄吧。第一次接触C++网络编程，体会到了C++的强大。

**<p align="right">tbk</p>**
**<p align="right">2022/7/17</p>**