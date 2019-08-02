# APPEND 
## 对简单字符串操作
### SET 
### GET
### SETNX
>如果key不存在则新增
### INCR 
>对指定key对应的value进行自增原子操作
### DEL
## 只能存在指定时间长度的key
>可以使用EXPIRE和TTL命令完成
- EXPIRE 用于指定键存活时间
- TTL 用于查询键剩余存活时间
>返回结果： -1：当前key永远都不会失效 -2：当前key已经失效 其他值：当前key剩余存活时间
- 执行SET命令后，当前key TTL会被重置——该key永远都不会失效
  
## 对list操作
>list 存储有序数据，其操作命令如下：
- RPUSH
>在list末尾加入一个值
- LPUSH
>在list头部加入一个值
- LLEN
>用于获取list当前长度
- LRANGE
>获取list的子序列，第一个参数为子序列第一个元组在list中的索引位置，第二个参数为子序列在list中的索引位置(-1表示一直到末尾);可用于获取list元素。
- LPOP
>删除list第一个元素并返回此元素。
- RPOP
>删除list最后一个元素并返回此元素。

## 对Set操作
>set 与list相似，只是它存储数据无需，且每个元素都是唯一的。
- SADD
>向set中添加给定元素
- SREM
>从set中删除指定元素
- SISMEMBER
>如果给定元素在set中，则返回1，否则返回0.
- SMEMBERS
>返回set中所有元素。
- SUNION
>合并两个或多个set并返回合并后set中的元素。

## Sorted set
>虽然set很好用，但是它无序，所以在1.2版本后redis提供了 <em>sorted set</em>.在<em>sorted set</em>中每个元素都与一个评分(score)关联，评分会被用于对所有元素评分。

- ZADD
```sh
ZADD hackers 1940 "Alan Kay"
```
- ZRANGE
```sh
ZRANGE hackers 2 4
```
## Hashes
><em>Hashes</em>是字符串字段到字符串值之间的映射。所以它是存储对象的最好的数据结构。
- HSET
>向Hash中添加数据
- HGETALL
>获取Hash中的所有数据
- HMSET
>批量向Hash中添加数据
- HGET
>获取Hash中指定字段的值
- HDEL
>删除Hash中的某个字段
- HINCRBY
>对Hash中的数字类型的值做累加