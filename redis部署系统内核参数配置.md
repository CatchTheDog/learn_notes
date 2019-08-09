# redis部署注意事项
- 最好使用Linux操作系统
- 确保 Linux内核参数<em> vm.overcommit_memory = 1</em>
- 确保关闭了Linux内核：<em>echo never > /sys/kernel/mm/transparent_hugepage/enabled</em>
- 设置一些换出内存量，避免redis内存占满整个物理内存时进程被kill
- 设置 maxmemory 限制redis内存
- 对于写操作非常多的应用，redis在进行RDB保存或者重写AOF时可能会占用2倍内存。
- 在 deamontools下运行时设置 deamonise no
- 确保设置了backlog