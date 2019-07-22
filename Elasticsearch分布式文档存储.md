# 分布式文档存储
## 路由
索引文档时，Elasticsearch通过如下计算确定一个文档要存储在哪个分片上：
```math
shard = hash(routing) % number_of_primary_shards
```
注：
- routing 默认是文档_id，可以自定义
- 在创建索引时确定索引主分片数量且永远不可改变
- 所有的文档API get 、 index 、 delete 、 bulk 、 update 以及 mget ）都接受routing参数以自定义文档存储的分片位置。

## 主分片与副分片交互
- 新建、索引和删除 请求都是 写 操作， 必须在主分片上面完成之后才能被复制到相关的副本分片
  * 提升性能的可选参数，一般不会改变：
      * consistency
    $$quorum =  \frac{primary + number\_of\_replicas}{2} + 1$$
   
     quorum: 活跃副本数目
      * timeout
  > 新索引默认有1个副本分片，索引默认情况下number_of_replicas 大于1时，consistency 中 quorum才会生效。

- 取回文档
可以从主分片或者从其它任意副本分片检索文档 

- 更新文档
>更新文档需要先取回文档，然后根据原文档生成一个新文档，然后重新索引新文档，所以需要在主分片操作完成后，再复制到副分片。

>update API 还接受在 新建、索引和删除文档 章节中介绍的 routing 、 replication 、 consistency 和 timeout 参数。

- 多文档请求
>mget 和 bulk API 的 模式类似于单文档模式。区别在于协调节点知道每个文档存在于哪个分片中。 它将整个多文档请求分解成 每个分片 的多文档请求，并且将这些请求并行转发到每个参与节点。协调节点一旦收到来自每个节点的应答，就将每个节点的响应收集整理成单个响应，返回给客户端。