# Elasticsearch集群部署
## 硬件
* 内存
  > 排序和聚合很耗内存，不要64GB>=内存>不可小于8GB
* CPUs
  > 多核远胜时钟频率稍快
* 硬盘
 > SSD最好

 > 注：需要确保系统I/O调度是正确的。*nix系统下调度程序是<b>cfq(完全公平队列)</b>,对机械硬盘是高效的，但对SSD却很低效。在使用SSD时应使用deadline或者noop。不论是机械硬盘还是SSD，都应该使用RAID 0。没有必要使用镜像或其他变体。避免使用NAS。
 * 网络
  低延迟网络最好。
## 虚拟机
* 尽可能使用最新版JVM
* java客户端版本应保持与Elasticsearch服务器JVM版本一致，若不一致则可导致未知异常
 >Transport Client
   传输客户端作为一个集群和应用程序之间的通信层。它知道 API 并能自动帮你在节点之间轮询，帮你嗅探集群等等。但它是集群 外部的 ，和 REST 客户端类似。

 >Node Client
   节点客户端，实际上是一个集群中的节点（但不保存数据，不能成为主节点）。因为它是一个节点，它知道整个集群状态（所有节点驻留，分片分布在哪些节点，等等）。 这意味着它可以执行 APIs 但少了一个网络跃点。

>> 案例

>>>如果要将应用程序和 Elasticsearch 集群进行解耦，传输客户端是一个理想的选择。例如，如果您的应用程序需要快速的创建和销毁到集群的连接，传输客户端比节点客户端”轻”，因为它不是一个集群的一部分。
类似地，如果您需要创建成千上万的连接，你不想有成千上万节点加入集群。传输客户端（ TC ）将是一个更好的选择。
另一方面，如果你只需要有少数的、长期持久的对象连接到集群，客户端节点可以更高效，因为它知道集群的布局。但是它会使你的应用程序和集群耦合在一起，所以从防火墙的角度，它可能会构成问题。

## 重要配置修改
# 指定名字
> Elasticsearch默认启动集群的名字叫elasticsearch,可在elasticsearch.yml文件中修改：
```yml
cluster.name: elasticsearch_production
```
节点名称默认是一个UUID值，可在elasticsearch.yml中修改：
```yml
node.name: elasticsearch_005_data
```
# 路径
默认会将插件、日志和数据放在安装目录下，如果重装Elasticsearch,则有可能会覆盖数据。所以最好将数据放在其他目录下。
* 数据存储路径
```yml
path.data: /path/to/data1,/path/to/data2
```
* 日志路径
```yml
path.logs: /path/to/logs
```
* 插件路径
```yml
path.plugins: /path/to/plugins
```
> 注：使用多个数据路径并不能提高性能，Elasticsearch会将整个分片置于单个驱动器上尽可能减小驱动器损坏时的损失。
* 最小主节点数
>脑裂
   一种两个主节点同时存在于一个集群的现象
> 最小节点数 告诉Elasticsearch 当没有足够master候选节点时，就不要进行master节点选举，等master候选节点足够了才进行选举。该项应始终配置为master节点的法定个数(master候选节点/2 + 1)
可通过修改elasticsearch.yml配置此项：

```yml
discovery.zen.minimum_master_nodes: 2
```
> 动态修改此项
修改为永久生效，且较配置文件优先
```js
PUT /_cluster/settings
{
    "persistent" : {
        "discovery.zen.minimum_master_nodes" : 2
    }
}
```
* 集群恢复相关配置
 设置如下三个配置项以避免集群重启时过多的分片交换。
```yml
#等待集群至少存在 8 个节点
#等待 5 分钟，或者10 个节点上线后，才进行数据恢复，这取决于哪个条件先达到
gateway.recover_after_nodes: 8
gateway.expected_nodes: 10
gateway.recover_after_time: 5m
```
* 单播代替组播
默认使用单播，可以为Elasticsearch提供一些尝试连接的节点列表：
```yml
discovery.zen.ping.unicast.hosts: ["host1", "host2:port"]
```
## 避免修改的配置项
* 垃圾回收器 
 默认垃圾回收器是CMS
* 线程池
搜索线程池设置的大一点，配置为 int ((核心数 * 3 )/2)＋ 1
线程池的线程数应该根据CPU核数来定，最大设置为核数*2，再大反而会降低性能。

## 堆内存:大小和交换
Elasticsearch 默认使用堆内存1GB。修改Elasticsearch堆内存大小的方式：
```sh
export ES_HEAP_SIZE=10g
```
```sh
#堆内存最大值与最小值相同，防止堆内存大小调整过程消耗性能
./bin/elasticsearch -Xmx10g -Xms10g
```

>堆内存设置大小经验：
* JVM堆内存大小不应大于32GB
* 应将整个内存大小的50%作为Elasticsearch的堆内存，剩余50%留给Luncene
* 堆内存换出会影响Elasticsearch性能，可采用禁用/降低堆内存换出的方式优化，具体方式：
```sh
# 暂时禁用换出可用如下命令，若要永久禁用换出，则需要参考操作系统文档
sudo swapoff -a
```

```sh
# 在sysctl中做如下配置
vm.swappiness = 1
```

```yml
#elasticsearch.yml中配置项锁定堆内存
bootstrap.mlockall: true
```

## 文件描述符和MMap

Elasticsearch 需要巨量的文件描述符，而*nux系统，默认每个进程最多允许有1024个文件描述符。应该增加文件描述符数量，比如64000.
修改操作系统单进程最大文件描述符数量后，可使用如下方式进行检验：
```json
GET /_nodes/process

{
   "cluster_name": "elasticsearch__zach",
   "nodes": {
      "TGn9iO2_QQKb0kavcLbnDw": {
         "name": "Zach",
         "transport_address": "inet[/192.168.1.131:9300]",
         "host": "zacharys-air",
         "ip": "192.168.1.131",
         "version": "2.0.0-SNAPSHOT",
         "build": "612f461",
         "http_address": "inet[/192.168.1.131:9200]",
         "process": {
            "refresh_interval_in_millis": 1000,
            "id": 19808,
            "max_file_descriptors": 64000, 
            "mlockall": true
         }
      }
   }
}
# max_file_descriptors  项的值正是Elasticsearch进程可访问的可用文件描述符数量
```
Elasticsearch 对各种文件混合使用了NioFs(非阻塞文件系统),MMapFs(内存映射文件系统);需要确保配置的最大映射数量，以便有足够的虚拟内存可用于mmapped文件。
```sh
sysctl -w vm.max_map_count=262144
```
或者可以在/etc/sysctl.conf通过修改vm.max_map_count永久修改。

## linux下I/O模型
![avatar](./IO模型.png)
