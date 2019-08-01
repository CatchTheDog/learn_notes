# Elasticsearch集群部署重要配置
>Elasticsearch的默认配置已经最优，在使用时仅需要调整极少部分配置就可以了。大多数配置在集群运行期间可以通过接口<em>Cluster Update Settings API</em>进行动态调整。

>配置文件必须包含一些节点标识信息(node.name,paths)和节点加入集群的信息(cluster.name,network.host)。

## 配置文件位置
>Elasticsearch有三个配置文件
- <em>elasticsearch.yml</em> 用于配置Elasticsearch
- <em>jvm.options</em> 用于配置Elasticsearch JVM
- <em>log4j2.properties</em> 用于配置Elasticsearch 日志

>注：上述配置文件的位置与Elasticsearch的安装方式有关。
- 归档文件分发方式安装(tar.gz/zip)
>配置文件目录默认是 <em>$ES_HOME/config</em>。当然，可以通过配置环境变量 <em> ES_PATH_CONF</em>来指定配置文件存放目录：
```sh
ES_PATH_CONF=/path/to/my/config ./bin/elasticsearch
```
>除了使用环境变量的方式，也可以使用命令行或者shell profile 来export <em>ES_PATH_CONF</em>，同样也可以改变配置文件目录。
- 包分发(Debian/RPM包)
>默认情况下配置文件位置是 /etc/elasticsearch，也可以通过配置环境变量<em>ES_PATH_CONF</em>来改变，需要注意的是仅仅配置此项还不够；此变量值同样也会从/etc/default/elasticsearch(Debian 包)和 /etc/sysconfig/elasticsearch(RPM 包)获取。如果需要改变变量值，你可能需要编辑这两个文件中的<em>ES_PATH_CONF=/etc/elasticsearch</em>键值对来改变配置文件的位置。
## 配置文件格式
>配置文件格式是<em>YAML</em>.

## 环境变量替换
>可以在配置文件中使用<em>${...}</em>来引用系统环境变量。
```yml
node.name:    ${HOSTNAME}
network.host: ${ES_NETWORK_HOST}
```

## JVM配置
>谨慎改动JVM配置。如果要改动，最多也就改动一下堆内存大小。在改动虚拟机配置时，我们应该优先使用改动jvm.options配置文件的方式。该文件默认位置是config/jvm.options(以tar或者zip包安装)和/etc/elasticsearch/jvm.options(以Debian和RPM包的方式安装)。

### jvm.options配置解析
- 空格组成的行无效
- 以 # 开头的行无效
- 以 - 开头的配置项可应用于各个版本的JVM
- 以 数字:- 开头的配置项仅用于版本号与数字相同的JVM
- 以 数字-:- 开头的配置项仅用于版本号大于等于数字的JVM
- 以 数字-数字:- 开头的配置项用于版本号在两数字范围内的JVM
- 其他形式的项均非法

>另一种设置JVM配置的方式是通过ES_JAVA_OPTS环境变量,在使用RPM和Debian包安装时，ES_JAVA_OPTS可通过<em>system configuration file</em> ：
```sh
export ES_JAVA_OPTS="$ES_JAVA_OPTS -Djava.io.tmpdir=/path/to/temp/dir"
```
## Secure 配置

## Logging 配置
>Elasticsearch使用Log4j 2 作为日志系统。Log4j 2可以通过log4j2.properties配置。Elasticsearch提供了三个属性：<em>${sys:es.logs.base_path},${sys:es.logs.cluster_name},${sys:es.logs.node_name}</em>,这三个属性可以在配置文件中引用来指定日志文件的位置。

>eg:如果日志目录(path.logs)是/var/log/elasticsearch，集群名称是production。在日志文件中{sys:es.logs.base_path} 将会被解析为 /var/log/elasticsearch,${sys:es.logs.base_path}${sys:file.separator}${sys:es.logs.cluster_name}.log 将会被解析为 /var/log/elasticsearch/production.log。

>具体配置可以参考 log4j文档

### 日志级别配置
有以下四种方式：
- 通过命令行 在临时调试某个问题(比如某个启动时发生的问题或者在开发期间发生的问题)时可以使用此方式
```sh
-E logger.org.elasticsearch.transport=trace
```
- 通过elasticsearch.yml配置文件 如果你在调试问题，但是你是通过服务启动Elasticsearch而不是通过命令行的方式，或者说你想要日志级别修改永久生效。
```yml
logger.org.elasticsearch.transport: trace
```
- 通过集群配置动态修改 适用于对正在运行中的集群动态修改日志级别
```sh
curl -X PUT "localhost:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
  "transient": {
    "logger.org.elasticsearch.transport": "trace"
  }
}
'
```
- 通过修改log4j2.properties 这种方式适用于你需要对日志级别有精细的控制
```yml
logger.transport.name = org.elasticsearch.transport
logger.transport.level = trace
```

### Deprecation logging


## 重要的Elasticsearch配置
### path.data 和 path.logs
>如果使用.zip和.tar.gz归档文件安装，则data和logs文件夹默认是$ES_HOME的子文件夹，这样会在做Elasticsearch升级时使数据面临被覆盖的风险。
```yml
path:
  logs: /var/log/elasticsearch
  data: /var/data/elasticsearch
```

>path.data 可以指定多个目录：
```yml
path:
  data:
    - /mnt/elasticsearch_1
    - /mnt/elasticsearch_2
    - /mnt/elasticsearch_3
```
### cluster.name
>Elasticsearch默认集群名称为elasticsearch.只有各节点集群名称一致时，才会组成一个集群。
```yml
cluster.name: logging-prod
```
>确保不同环境不要使用重复的集群名称，否则有可能使得节点加入错误的集群。

### node.name
>Elasticsearch默认使用随机生成的一个UUID的前7位字符作为node id. node name 和 node id 是永久的，在重启之后才会生效。
```yml
node.name: prod-data-2
node.name: ${HOSTNAME}
```
### nodework.host
>默认情况下Elasticsearch绑定在回环地址上——127.0.0.1 和 [::1].这种配置对单机单节点模式非常高效。但是如果需要使用多节点集群模型运行，则需要修改此配置项为非回环地址（当然还有其他的网络配置：<em>network settings</em>）。
```yml
network.host: 192.168.1.10
```
>此配置项也支持 一些特殊值：_local_,_site_,_global_和其他修饰符：:ipv4 和 :ipv6.更多细节参见<em>Special values for network.host</em>

### 恢复配置
>Elasticsearch 使用“Zen Discovery”实现集群恢复。有以下两个重要的集群恢复配置：
- discovery.zen.ping.unicast.hosts
>默认情况下,Elasticsearch会扫描9300-9500端口去连接本机的其他节点组建集群。如果需要与其他机器上的节点组建集群，就需要配置其他节点的地址列表：
```yml
discovery.zen.ping.unicast.hosts:
   - 192.168.1.10:9300
   - 192.168.1.11 
   - seeds.mydomain.com 
```
>如果不指定端口，则会使用<em> transport.profiles.default.port </em>或者 <em>transport.tcp.port</em>
- discovery.zen.minimum_master_nodes
>为了避免数据丢失，需要配置此配置项以限制组建集群的最小节点数。如果不配置此项，某个网络异常的节点会单独构成集群，而其他的节点会组成一个集群，这也称为脑裂，这会导致数据丢失。

>为了避免脑裂导致数据丢失，我们需要配置 <em>discovery.zen.minimum_master_nodes</em>为 <em>(master_eligible_nodes / 2) + 1</em>

### 设置堆大小
>Elasticsearch JVM默认堆大小是1GB.可以在jvm.options中设置Xms与Xmx来设置堆大小。关于堆内存大小设置的经验法则：
- 设置Xms和Xmx大小一致
- 堆内存越大，Elasticsearch可用内存越大，但同时GC时间也越长
- 最大对内存大小不要超过可用物理内存50%，保证有足够的内存给内核文件系统缓存使用
- 最大堆内存大小不要超过32GB.设置为31GB最好。

### 设置JVM 堆dump路径
>默认情况，堆内存溢出后dump路径：/var/lib/elasticsearch(RPM和Debian安装包)，data文件夹下(tar/zip包安装)。可以通过在jvm.options文件中提供 -XX:HeapDumpPath=... 来设置此路径。

### GC日志
>默认情况下，Elasticsearch开启GC日志。可在jvm.options中配置。

### JVM严重异常日志
>默认情况下，JVM严重异常日志会写入：/var/lib/elasticsearch(RPM和Debian安装包)，logs文件夹下(tar/zip包安装)。可在jvm.options表中加入配置项： -XX:ErrorFile=... 来修改此路径。

## 重要的系统设置
### 操作系统配置
>系统配置的方式因安装方式不同而不同。以.zip或者.tar.gz包安装，系统配置可以使用如下方式配置：
- 临时修改可用 uliimit
- 永久修改可以改动 /etc/security/limits.conf
  
以RPM或者Debian安装包方式安装，大多数系统配置在<em>system configuration file</em>中。

#### ulimit 
>在linux系统中，ulimit 可以用来临时性地改变资源限制。比如，使用如下方式可以将打开的文件句柄数设置为65536：
```sh
sudo su  
ulimit -n 65536 
su elasticsearch 
```
#### /etc/security/limits.conf
>在linux系统中，通过改变/etc/security/limits.conf 文件内容，可以永久性地改变资源限制。比如：
```sh
# 修改用于elasticsearch的最大打开句柄数为 65536
elasticsearch  -  nofile  65536
```
#### Sysconfig file

|安装方式|配置文件路径|
|--|--|
|RPM|/etc/sysconfig/elasticsearch|
|Debian|/etc/default/elasticsearch|

>使用systemd的系统，系统限制都可以通过使用systemd来指定。

#### Systemd 配置
>当在使用systemd的系统上以RPM或者Debian安装包的方式安装Elasticsearch后，系统限制都必须使用systemd指定。

>默认情况下，systemd会使用 /usr/lib/systemd/system/elasticsearch.service 作为系统限制文件。可以添加一个文件 /etc/systemd/system/elasticsearch.service.d/override.conf 来覆盖默认文件。或者执行如下命令：
```sh
sudo systemctl edit elasticsearch
```
改变此文件内容：
```sh
[Service]
LimitMEMLOCK=infinity
```
在修改结束后，需要重新加载才会生效：
```sh
sudo systemctl daemon-reload
```
### 关闭换出
>换出操作性能很差，为使节点稳定，需要避免换出。有三种方式可以关闭换出。建议完全关闭换出。如果不能完全关闭换出，最小化换出还是内存锁定需要依据具体情况作出选择。

- 完全关闭换出
使用如下执行临时性关闭换出：
```sh
sudo swapoff -a
```
>如果要永久性关闭换出，就需要编辑 /etc/fstab文件，注释所有包含 swap的行。
>在windows操作系统上：<em>System Properties → Advanced → Performance → Advanced → Virtual memory</em>
- 配置最小化换出
>使用sysctl 配置 vm.swappiness=1 可以保证系统在紧急情况下可以换出，正常情况下不会换出。
- 打开 bootstrap.memory_lock
>另一种方式就是使用linux/unix 系统中的mlockall,或者windows系统中的VirtualLock，将进程地址空间锁定在RAM中，这样就可以避免Elasticsearch内存被换出。可以在config/elasticsearch.yml中加入如下配置实现：
```yml
bootstrap.memory_lock: true
```
>如果设置此项，在无法分配内存时，JVM或者Shell将会退出。

在Elasticsearch启动后，可以使用如下请求验证mlockall是否设置生效：
```sh
curl -X GET "localhost:9200/_nodes?filter_path=**.mlockall"
```
>设置失败的原因及解决方法省略。遇到问题时参考官方文档。

### 文件描述符
>此项仅对linux/maxos有效。Elasticsearch使用大量文件描述符和文件句柄，如果在运行期间文件描述符达到上限则可能会导致数据丢失。所以需要确保系统文件描述符数量设置为65536或者更大。可以使用如下方式验证设置是否生效：
```sh
curl -X GET "localhost:9200/_nodes/stats/process?filter_path=**.max_file_descriptors"
```
### 虚拟内存
>Elasticsearch默认使用mmapfs文件夹来存储索引。操作系统默认的mmmp 数量限制太低，可能会导致内存溢出。linux系统可以使用如下方式修改：
```sh
sysctl -w vm.max_map_count=262144
```
可以通过修改/etc/sysctl.conf 中的 vm.max_map_count 配置项值永久修改。可以执行如下命令查看修改是否生效：
```sh
sysctl vm.max_map_count
```
### 线程数
>Elasticsearch使用了大量的线程池来执行各种操作，所以在Elasticsearch需要创建线程时它可以随时创建对集群性能至关重要。确保Elasticsearch最少能够创建的线程数为4096。

### DNS 缓存
>JVM会无限期缓存DNS.如果集群依赖于DNS且各节点的DNS会随时变动，就需要改动默认的JVM行为(缓存DNS的行为)。