# Redis 集群简介
>此文档基于Redis版本3.0及以上。

## Redis集群 101
>可以通过部署Redis集群获得以下优势：
- 将数据自动分配存储在不同节点上
- 当一部分节点异常或者与集群失去网络连接时数据仍然可用

## Redis 集群 TCP端口
>每个Redis集群节点需要开放两个TCP连接端口。一个向客户端提供服务的端口比如6379，还需要一个数据接口，一般情况是客户端服务接口加10000，比如16379.确保防火墙开放了这两个端口，否则集群各节点将无法通信。

>第二个端口会用于集群内各节点之间使用二进制协议通信。这个端口会用于集群节点之间进行异常检测、配置更新、故障转移授权等。

>目前redis集群不支持‘NATted’环境和IP或者端口被映射过的环境。所以在Docker中部署Redis 集群需要使用Docker <em>host networking mode(Docker配置文件中 --net=host选项)</em>.

## Redis集群数据分区
>Redis集群分区不使用一致性hash算法，但是使用一种我们乘坐<em>hash slot</em>的分区方式。

>在Redis集群中一共有16384个hash slot.我们使用指定key的CRC16值对16384求模，所得值即为该key的hash slot值。

>Redis集群中的每个节点都负责集群整个hash slot中的一部分。比如我们有一个三节点的集群：
- 节点A 包含的 hash slot 范围是 0 - 5500
- 节点B 包含的 hash slot 范围是 5501 - 11000
- 节点C 包含的 hash slot 范围是 11001 - 16383

>Redis在对集群节点操作过程中，需要首先将操作节点的hash slot移动到其他节点，而从一个节点移动hash slot 到另一个节点的过程，不需要停止操作，也不需要添加或者删除节点或者改变各个节点负责的hash slot 百分比，不需要其他任何停机操作。

>Redis集群支持在一个命令中对存在于同一slot中的多个key进行操作。用户也可以强制将多个key组成一个hash slot,这种概念性地称为 hash tags.

>hash tags使用：在多个key中使用{}将需要用于计算hash slot位置的串包起来，这样多个key就会被强制性地组成一个hash slot.比如this{foo}key 和 another{foo}key  都会使用foo计算hash slot，所以能够保证这两个key都在同一个hash slot中。

## Redis 集群主从模型
>Redis集群使用主从模型来保证数据高可用性，每个hash slot都会有从1到N的副本(1 为主节点，N-1 为其他从节点).

>Redis集群不保证强一致性(有可能会丢失写操作)，比如我们将写操作请求发送到主节点A，A节点收到请求执行命令成功后向客户端返回操作成功的响应，然后A节点才会将该写操作向其他节点扩散；如果在发出扩散请求之前，A节点宕机，则集群内其他节点都不会收到该写操作，该操作就丢失了。

>当然，异步写操作会有丢失数据的风险，redis通过<em>WAIT</em>命令提供了同步写操作，同步写操作将丢失数据的风险降到了最低，需要注意的是，即使是同步写操作，当一个redis从节点被选举为主节点时还是有一定的可能性不会收到此前的主节点发送的写操作命令，还有一些复杂的场景也可能导致数据丢失。

><em>maximum window</em>

><em>node timeout</em>

## Redis 集群配置参数
>在redis.conf文件中涉及到以下与集群相关的配置，现做以下解释：
- cluster-enabled<yes/no>:是否以集群模式运行
- cluster-config-file<filename>:用户不可编辑，集群用于保存集群状态等信息，可在集群重启时读入使用。
- cluster-node-timeout<milliseconds>:集群节点可以不做响应的最大时间间；超过此时间，集群会认为此节点异常，集群就不会再向此节点发出信息。
- cluster-slave-validity-factor<factor>:如果设置为0，则从节点会一直尝试替换主节点而不论<em>cluster-node-timeout</em>指定的集群节点超时时间。如果此项是正值，那么集群节点最大断开连接时间是 <em>cluster-slave-validity-factor * cluster-node-timeout</em>；如果一个从节点超过最大断开连接时间没有同主节点通信，它将不会再作为集群内一个节点。例：如果<em>cluster-node-timeout</em> 设置为5s,而<em>cluster-slave-validity-factor</em>设置为10时，如果一个从节点与集群超过50s没有任何连接时，该节点就不会再被作为集群内一个节点。
>需要注意的是：任何一个不是0的值，都可能会引起集群主节点异常后如果没有从节点来替换它时整个集群不可用，除非原来的主节点重新加入到集群中。

- cluster-migration-barrier<count>:一个主节点连接的从节点的数目的最小值。防止脑裂。
- cluster-require-full-coverage<yes/no>:默认值yes.如果设置为yes,如果集群内有一些key值无法操作时集群会停止接受写操作请求；如果设置为no,即使只有部分key可正常操作，集群仍然会提供查询。

## 创建并使用集群
>最简集群配置：
>集群最少需要三个节点，
```sh
port 7000
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```
>使用以下命令启动集群：
```sh
cd 7000
../redis-server ./redis.conf
```
><em>Node ID</em>永远都不会变更，即使机器ip与port变更，节点ID也不会改变。

### 创建集群
>Redis 5 可以使用redis-cli完成集群创建、分片检测等操作。Redis3或者4，需要使用redis gem安装redis-trib,然后使用redis-trib来完成集群创建。我们以redis5为例。

>使用如下命令创建集群：
```sh
redis-cli --cluster create 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005 --cluster-replicas 1
```

### 使用create-cluster 脚本创建redis 集群
>如果不想通过以上所述的配置和执行命令的方式来创建集群，有一种更简单的创建集群的方式：在redis安装包的utils/create-cluster 目录下有一个 crate-cluster 脚本，可以使用如下命令启动一个6节点集群(3主节点，每个主节点一个从节点)：
```sh
# 1.
create-cluster start
# 2.
create-cluster create
# 3.
create-cluster stop
```
>脚本创建的集群默认监听30001端口。
### 手动故障转移
>使用CLUSTER FAILOVER 来触发手动故障转移，用于将主节点转换为从节点以降低操作对集群的影响。此命令必须在待转换的master的某一个从节点上运行。手动故障转移比真实的故障安全，它不会导致数据丢失。
### 使用redis集群
>使用redis-cli连接redis并使用

>发送到集群的请求会自动重定向到正确的节点执行。一个有效的方法时在客户端缓存节点和hash slot的映射以直接将请求发送到正确的节点。节点地址与hash slot映射关闭不会发生变更，除非集群内节点发生增加或者减少。

## redis 重新分区
>redis集群中重新分区意味着将hash slot从一个节点转移到另外一个节点。使用redis-cli工具完成：
```sh
redis-cli --cluster reshard 127.0.0.1:7000
# 获取节点ID命令
redis-cli -p 7000 cluster nodes | grep myself
# 在重新分区结束后检查集群状态
redis-cli --cluster check 127.0.0.1:7000
```

### 使用脚本自动完成重新分区
```sh
redis-cli reshard <host>:<port> --cluster-from <node-id> --cluster-to <node-id> --cluster-slots <number of slots> --cluster-yes
```
>目前redis-cli还无法自动在集群内节点之间平衡一个key 的分布，后续会加入此功能。

### 向集群中添加节点
```sh
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000
```
><em>CLUSTER MEET</em>

### 向集群中添加从节点
- 第一种方式：
```sh
# 没有指定为哪个主节点添加从节点，所以会为集群中随机的一个主节点添加从节点
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000 --cluster-slave
# 指定主节点ID:3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000 --cluster-slave --cluster-master-id 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e
```
- 第二种方式
    - 将一个新节点作为空的主节点加入集群
    - 连接此新节点执行如下命令：
        ```sh
        # 将当前新节点设置为 节点 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e 的从节点
        cluster replicate 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e
        ```

## 删除节点
```sh
# node-id指定了要删除的节点；可以用同样的方式删除一个空的主节点(有数据的主节点无法直接删除)，所以在删除一个主节点之前需要将此节点上的数据重新分配至到其他节点，然后才能删除此节点。
redis-cli --cluster del-node 127.0.0.1:7000 `<node-id>`
```

## 副本迁移
>在任何时刻都可以将一个节点重新设置为另一个节点的从节点：
```sh
CLUSTER REPLICATE <master-node-id>
```
>关于副本迁移：
- 集群会在某个时刻从拥有最多副本的主节点上迁移副本。
- 为了能够从副本迁移中受益，你需要为一个主节点配置一些副本
- 有一个用于控制副本迁移的功能的参数：<em>cluster-migration-barrier</em>。

## 升级集群节点
>对于从节点，只需要停止该节点，然后以新版本重启节点即可。

>对于主节点，建议做如下操作：
- 使用 CLUSTER FAILOVER 手动触发一次该主节点的故障转移，将主节点转为其他从节点
- 等待该主节点变为从节点
- 然后按照从节点的升级步骤升级此节点即可
- 然后可以再次使用CLUSTER FAILOVER 将此节点转为主节点

## 迁移到Redis 集群
在迁移到redis集群时，有以下三种情况：
- 不对多个key操作。redis-cluster支持。
- 对多个key操作，但是这些key都在同一个hash tag中。redis-cluster支持。
- 对多个key操作，这些key不在同一个hash tag中。这种情况redis-cluster 不支持。
  
>对于第一种情况和第二种情况：
- 停止客户端。目前redis-cluster还不支持在线迁移。
- 为所有的节点使用 BGREWRITEAOF 生成AOF文件，并等待AOF文件生成完成。
- 将各个节点的AOF文件备份到一个位置。这时可以停止redis实例。
- 创建一个由这N个节点组成的集群，所有节点均为主节点，0个从节点。确保所有节点都使用AOF作为持久化方式。
- 停止所有节点，然后将AOF换为原来的AOF文件。
- 重启所有节点。
- 使用redis-cli --cluster fix 命令修复集群，key将会被重新按照集群内hash slot 进行分配。
- 使用redis-cli --cluster check 确保集群状态正常
- 重启客户端恢复应用。
  
>可以使用命令： redis-cli --cluster import 将集群外节点内的所有key导入已经存在的集群。但是此操作2.8版本会比较慢。

# Redis集群详解

# Redis 哨兵(Sentinel)
>哨兵是redis提供的集群高可用的解决方案，它实现以下功能：
- 监控 哨兵会持续监控主从节点运行是否正常
- 通知 当监控的节点有异常时，哨兵会自动通过API等通知管理员
- 自动故障迁移 如果一个主节点发生异常，哨兵会自动启动故障转移过程：该主节点的某个从节点会被选举为主节点，使用此redis服务的应用将会使用新的地址连接集群。
- 配置提供 为客户端提供redis服务发现，客户端可以连接到哨兵获取服务对应的redis主节点地址。如果服务发生故障转移，哨兵会报告新的服务地址。

>哨兵的分布式特性：哨兵本身就被设计为多个哨兵同时协作，多个哨兵协作的优点：
- 只有在多个哨兵同时认为一个节点异常时，才会宣布此节点异常。这在一定程度上降低了假阴性。
- 即使有部分哨兵异常，哨兵仍然能正常工作，使得系统更健壮。对单节点使用故障转移系统根本毫无意义。

## 哨兵尝试
### 哨兵版本
>redis目前最新版本使用Sentinel 2.自2.8以后redis使用了稳定版本的哨兵。
Sentinel 1 不要再用了。

### 启动哨兵
>有以下两种方式启动哨兵：
```sh
# 1.存在sentinel.exe 
redsi-sentinel /path/to/sentinel.conf
# 2.不存在sentinel.exe
redis-server /path/to/sentinel.conf --sentinel
```
>配置文件必须有，且该目录必须有写入权限，该文件会用于保存哨兵的运行状态等信息以便在重启时加载。

>哨兵默认监听 TCP端口26379，所以必须确保此端口可用。

>在部署哨兵之前需要知道的一些基本知识：
- 至少需要三个哨兵。
- 三个哨兵异常独立——不会因为某一因素同时异常，比如在同一数据中心、同一机房、同一台机器等。
- 由于使用异步写副本的机制，哨兵+redis集群也不能保证在集群节点异常时客户端已经得到服务端确认的写入命令不会被丢失。所以有几种不同的哨兵部署方式来将丢失写操作的时间限制在一定范围内。
- 客户端也需要使用哨兵客户端。
- 在开发环境多测试，在生产环境才能低风险。
- 慎将Docker(等地址和端口映射应用)和Sentinel搭配。
