# redis数据类型了解
>redis支持以下数据类型：
- 二进制安全字符串
- Lists: 按照插入顺序存储的字符串元素集合，底层是链表。
- Sets: 无序且唯一的字符串元素集合。
- Sorted sets: 与Sets相似，但是每个元素都与一个浮点型评分值相关联，集合内元素按照评分值进行排序。
- Hashes: 存储字符串与字符串映射关系的集合。
- Bit Arrays(bitmaps): 可以允许像操作位数组一样操作字符串，可以对某一位设置为1或者重置等。
- HyperLogLogs:  一个用于估算集合基数的概率性数据结构。
- Streams: 提供对日志数据的抽象的数据类型，它与map类似，且只能追加写入。

## Redis keys
>Redis keys 是二进制安全的，这意味着使用任何二进制序列作为key,空字符串也是合法的key.
>以下是对key的要求：
- key不要太长。对于非常长的key,可以用hash处理后作为key,这样可以节省内存也可以节省带宽。
- key也不要太短。具有明确的意义的key更好，当然，在占用内存和意义明确之间需要权衡。
- 数据遵循一种模式。
- key最大限制为512MB.

## Redis Strings
>操作string的命令：
- SET 设置key->value 
>值可以是string或者其他任何类型(包括二进制数据)，value的大小不能超过512MB.

>SET 命令还有一些可选参数：
    - nx 当且仅当key不存在时本次SET才会执行
    - xx 当且仅当key存在时本次SET才会执行
- GET 根据key获取value
- incr 对value(如果不是整型数据，则会尝试将其转换为整型数据)进行自增操作(原子操作)，然后将自增后的值写入。
- incrby key addValue
>对value自增addValue(将value增大addValue).
- DECR
- DECRBY
- GETSET
>为key设置新value并返回原来的value.
- MSET
>批量设置多个key->value
- MGET
>批量获取多个key对应的value

## 修改或者查询key占用空间
>有一些命令适用于所有类型的key:
- EXISTS
>判断key是否存在，如果存在则返回1，否则返回2.
- DEL
- TYPE
>返回某个key所对应的value的类型

## Redis键超时：有存活时间上限的键
>Redis expires:为redis某个键设置一个过期时间，在过期时间到了之后这个键会自动被删除，这种机制就称为键过期。它可以在所有的键上工作，无论这个键对应的值类型是什么：
- 过期时间的单位精度可以是秒或者毫秒。
- 但是过期时间的精度始终都是1毫秒。
- 关于键值过期的信息会被复制且会被持久化到磁盘上，在Redis停止运行期间，流逝的时间会计入过期时间。

- EXPIRE
>用于为键设置过期时间，可以覆盖此前设置的过期时间为本次设置的过期时间或者位没有过期时间的键设置过期时间。
- PERSIST
>删除键上的过期时间，操作成功后，该键将永久存在
- SET KEY VALUE EX 10
>在SET命令中设置键过期时间
- TTL
>用于查看键剩余过期时间
- PEXPIRE/PTTL
>使用此命令以毫秒为单位设置键过期时间

## Redis Lists
>Redis lists通过类似链表的方式实现。链表方式实现保证了在lists头部和尾部添加元素的操作是O(1)。lists相关操作命令如下：
- LPUSH 可同时将多个元素添加到lists头部
- RPUSH 可同时将多个元素添加到lists尾部
- LRANGE 索引参数可正(从最左端以0开始计数)可负(从最右端以-1开始计数)
- rpop

### lists的一些通用场景
- 记录用户在社交网络中发送的最新更新
- 以生产者-消费者模型实现进程间通信

### 优选 Lists
>在很多场景中，我们会需要保存最新的前N个记录，其他旧的记录统统丢掉；在这种情况下，我们可以使用LTRIM命令将给定范围内的子序列设置为key对应的值，而范围外的所有值都会被删除。

### lists 的阻塞操作
>为了实现阻塞队列，Redis提供了命令：
- BRPOP/BLPOP
>在队列为空时，暂时阻塞。只有在达到用户指定的超时时间或者获取到元素后，才能返回。
```sh
> brpop tasks 5
1) "tasks"
2) "do_something"
```
><em>brpop tasks 5</em>从tasks中获取元素，超时时间为5s.设置超时时间为0时，如果没有元素，则此操作会永久等待；也可以指定从多个队列中获取元素。

>关于BRPOP命令，需要注意以下几点：
- 客户端的请求会按照次序依次被处理：第一个阻塞的客户端会在队列收到元素时第一个被处理。
- 返回值与RPOP不同：返回一个二元数组：key->element.
- 达到超时时间仍无元素，则返回NULL

>除了上述命令之外，还有其他命令可用于构建阻塞队列：
- RPOPLPUSH
>可用于构建安全队列或者旋转队列
- BRPOPLPUSH
>阻塞命令变体

## 自动创建和删除key
>对于一些由多个元素组成的集合元素，redis会自动负责在此元素不存在而用户试图向其中添加元素时创建集合；而在该集合为空时删除集合。这样的集合元素有：
- Streams
- Sets
- Sorted
- Sets
- Hashes
  
>以下为创建和销毁规则：
- 当我们向集合元素添加元素时，如果集合元素不存在，则在添加元素之前集合元素会被自动创建。
- 如果一个集合元素为空，除Streams外的其他集合元素都会被自动销毁
- 对一个对应的集合为空的key执行读取命令如LLEN或者写入删除命令,一些命令的结果会看起来像这个key值所对应的value是一个空集合一样(但实际上这个集合不存在)。

## Redis Hashes
>Hashes用于表示对象很方便：
```sh
> hmset user:1000 username antirez birthyear 1977 verified 1
OK
> hget user:1000 username
"antirez"
> hget user:1000 birthyear
"1977"
> hgetall user:1000
1) "username"
2) "antirez"
3) "birthyear"
4) "1977"
5) "verified"
6) "1"
```
>操作hashes的命令：
- HSET
- HGET
- HMSET
- HMGET
- HGETALL
- HINCRBY

## Redis Sets
>Sets是无需，元素唯一的集合。与Sets相关的部分命令如下：
- SADD
- SMEMBERS
>获取集合内所有元素
- SISMEMBER
>查看某个元素是否在集合中
- SINTER
>对多个结合取交集
- 除了取交集，我们还可以取并集、取差集、随机取一个元素
- SPOP
>从集合中随机删除一个元素
- SUNIONSTORE
>将多个集合元素取并集后存入指定的key中。
- SCARD
>返回集合基数。
- ARANDMEMBER
>从集合中随机获取一个元素而不会删除它

## Redis Sorted sets
>介于set与hash之间，元素唯一，且每个元素都与一个浮点型数字关联，此值为评分，集合内元素按照评分排序，排序规则如下：
- A.score > B.score => A>B
- A.score = B.score && A>B(以字符串字典序为依据比较) => A > B

>Sorted sets 通过双端数据结构实现，它包含一个跳跃表和一个hash table,所以向其中添加元素的时间复杂度是O(logN).

>操作sorted sets命令如下：
- ZADD
- ZRANGE
>列出指定范围内元素，元素排序为由小到大
- ZREVRANGE
>列出指定范围内元素，元素排序为由大到小
```sh
zrange hackers 0 -1 withscores
```
- ZRANGEBYSCORE
>返回指定分数范围内的元素
```sh
zrangebyscore hackers -inf 1950
```
- ZREMRANGEBYSCORE
>删除指定分数范围内的元素
- ZRANK
>获取指定值在集合中由小到大排序时的序号
- ZREVRANK
>获取指定值在集合中由大到小排序时的序号

>词典评分：版本2.8以后，提供了一个新功能可以允许对所有的元素给予相同的评分，然后返回指定字典序范围内的元素。相关命令如下：
- ZRANGEBYLEX
```sh
> zrangebylex hackers [B [P
1) "Claude Shannon"
2) "Hedy Lamarr"
3) "Linus Torvalds"
```
- ZREVRANGEBYLEX
- ZREMRANGEBYLEX
- ZLEXCOUT

>更新评分
>对于已经存在与sorted sets中的元素，使用ZADD命令就可以改变指定key的评分。

## BitMaps
>BitMaps 不是一种真正的数据类型，它是一组定义在字符串类型上的位操作。由于String类型二进制安全且其最大容量为512MB，也就是string类型最多可有2^32位。

>位操作分为两类：
- 第一类是操作时间复杂度为O(1)，比如将某个位设置为1或者0，获取某个位的值等。
- 第二类是对一组位进行处理的操作，比如获取某个范围内位的总数等。

>位操作命令如下：
- SETBIT
- GETBIT
