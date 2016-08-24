// 提供异步接口
// 线程安全问题
// 完全无锁化

// 无锁化思路 每个线程分配一块自己的buffer，而后台线程主动去每个thread buffer中取数据，再写入磁盘 (观察者模式?)
// lockfree队列

// 多个线程push数据的时候，采用何种方式通知写线程。尽量公平的取走每个buffer的数据
// linux的是eventfd ，为每个用户thread创建一个eventfd，pushlog的时候对绑定的fd的value加1即可。使用epoll和select监控变化。
// 从指定的buffer中读取相应的value条消息