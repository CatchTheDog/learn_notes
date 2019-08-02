# redis 内存优化了解
## 对小型聚集数据的特殊编码
>在2.2版本以后，很多数据类型都进行了优化，它们占用的空间大小会在某一上限值范围内。

>像Hashes、Lists、元素是整数类型的Sets、Sorted Sets等，当它们包含的元素个数尚未达到上限但元素大小达到了元素大小上限时，它们将会被使用一种内存使用率非常高的编码方式来存储，这种方式最多可以节省10倍空间（平均可以节省5倍空间）。

>编码方式自动改变的操作对用户来说是透明的，由于这是一个CPU消耗换内存消耗的方式，所以需要对元素个数上限和元素大小上限做出调整（在redis.conf文件中配置）：
```
hash-max-zipmap-entries 512 (hash-max-ziplist-entries for Redis >= 2.6)
hash-max-zipmap-value 64  (hash-max-ziplist-value for Redis >= 2.6)
list-max-ziplist-entries 512
list-max-ziplist-value 64
zset-max-ziplist-entries 128
zset-max-ziplist-value 64
set-max-intset-entries 512
```
>如果某个元素采用特殊编码方式以后，占用的空间会比配置的最大大小要大，redis将会自动将其转换为采用一般编码。编码转换操作对比较小的值来说非常快，但是如果需要对大集合对象使用特殊编码，需要首先进行基准测试已确定这样做是否有收益。

## 使用32位实例
>在32位系统上编译的Redis，每个key占用内存更小，因为这样指针更小，但是它能够使用的最大内存为4GB。如果需要编译32位的redis,可以使用命令<em>make 32bit</em>.RDB和AOF文件在32位和64位实例上是兼容的(同时在大端机器和小端机器之间也是兼容的)，所以在32位和64位之间切换是没有问题的。

## 位和字节级别的操作
>Redis 2.2版本提供了新的位和字节级别的操作：GETRANGE,SETRANGE,GETBIT和SETBIT.使用这些操作时，我们可以把redis字符类型的值看做可随机访问的数组。比如可以使用bitmap存储标识型字段，这样就可以节省大量内存；例：如果有1亿用户数据，用户性别使用<em>bitmap</em>存储，则最大使用12M(10000000/(8*1024*1024)M)空间。

>注：优化数据模型，使用适合的数据类型对提高空间使用效率至关重要。

## 尽量使用hashes
>小的hashes占用很小的空间，所以建议尽可能使用hashes。hashes对存储对象非常适用。

## hashes内存使用率超高
>为了保证数据查找时间复杂度是O(1),redis使用了hash table.如果集合元素较少，即元素个数N很小，数据被存储在数组中，查找的摊销后时间复杂度仍然是O(1)。当元素个数N变得很大时(具体上限可以在redis.conf中配置)，数据将会自动被转存到hash table 中。

>虽然从时间复杂度的角度来看，线性存储(数组)结构对CPU 缓存的使用比hash table 更好。

>但是由于redis中 hash 字段和值 不是全功能的redis对象的表现形式，hash 字段不能像一般的key一样具有失效时间，而且它只能包含一个string值。所以hashes内存使用率很高，hashes对存储各个字段之间有关联关系的对象非常有用。

>对普通文本，可以将键值按照规则进行分割，然后存入hashes中：

>eg:
```sh
object:102393
object:1234
object:5

# 分割后存入
HSET object:12 34 somevalue
```
>实践证明将普通key->value映射进行处理后存入hash可以节省大量空间。这也是redis(使用hash)存储普通key->value优势所在。

>为了能够在hash中存储数据节省空间，需要在redis.conf中配置如下项：
```sh
hash-max-zipmap-entries 256
hash-max-zipmap-value 1024
```
>每次如果一个'hash table'的元素数或者元素大小超过了配置文件中指定的上限值，它就会转换为一个真正的hash table,内存节省优势就没有了。

## 内存分配
>为了保存键值，Redis将会按照maxmemory配置项的值尽可能地分配内存。精确的内存分配值可以通过在配置文件中设置或者使用命令CONFIG SET 设置。对于redis内存管理，需要注意以下事项：
- 当key被删除时，redis不会总是将内存返还给操作系统，这也是大多数malloc()的实现方式。
- 上述事项说明用户需要根据redis使用内存峰值来确定要分配给redis的内存大小。
- 虽然redis不会在删除键时将内存返回操作系统，但是在删除键以后再向redis中写入数据时，redis内存分配器会尝试重用删除键以后空出来的内存，所以redis占用内存并不会增长。
- 由于以上事项，当内存占用量峰值比一般情况下内存占用量大的多的情况下，内存碎片率并不是很可靠。

>如果<em>maxmemory</em>没有设置，redis将会逐渐地占用所有的可用物理内存。所以建议设置最大内存限制。可能也需要设置<em>maxmemory-policy</em>为<em>noeviction</em>(在老版本redis中默认值不是它)。

>设置此参数后，在redis占用内存达到上限时，写入操作会触发异常。但是不会导致整个机器崩溃。


