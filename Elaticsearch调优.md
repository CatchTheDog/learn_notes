# Elasticsearch调优
>大部分情况下，不需要调整任何参数。如果在使用时了解调整结果的情况下，可以对部分参数进行调整，以期获取性能提高。

## 一般性的建议
- 不要返回大结果集
>Elasticsearch被设计用作搜索引擎，在返回与查询结果最为相关的结果时性能超棒。但是如果它被用作数据库而需要返回所有与查询结果相关的记录，那么性能可能会稍微差点，如果非得这么做，一定要使用scroll.
- 避免大文档
>默认情况下<em>http.max_context_length</em>设置为100Mb.Elasticsearch会拒绝索引比此还大的文档。可以酌情考虑增大此值，但是Luncene有最大限制——2GB。大文档会增加网络压力、内存压力、CPU消耗等等，可以通过优化数据模型的方式避免对大文档的操作。

## 一些策略
### 精确查找与词干提取混合
><em>query_string 和 simple_query_string</em>提供 <em>quote_field_suffix</em>，可以同时将一个查询字符用于精确查询和文本搜索。
```sh
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "settings": {
    "analysis": {
      "analyzer": {
        "english_exact": {
          "tokenizer": "standard",
          "filter": [
            "lowercase"
          ]
        }
      }
    }
  },
  "mappings": {
    "_doc": {
      "properties": {
        "body": {
          "type": "text",
          "analyzer": "english",
          "fields": {
            "exact": {
              "type": "text",
              "analyzer": "english_exact"
            }
          }
        }
      }
    }
  }
}
'
curl -X PUT "localhost:9200/index/_doc/1" -H 'Content-Type: application/json' -d'
{
  "body": "Ski resort"
}
'
curl -X PUT "localhost:9200/index/_doc/2" -H 'Content-Type: application/json' -d'
{
  "body": "A pair of skis"
}
'
curl -X POST "localhost:9200/index/_refresh"

curl -X GET "localhost:9200/index/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "simple_query_string": {
      "fields": [ "body" ],
      "quote_field_suffix": ".exact",
      "query": "\"ski\""
    }
  }
}
'
```

### 获取一致评分
>分片和副本对获取好的评分来说是一个障碍。

#### 评分不可重现
>如果索引存在副本，同一请求，执行多次获取到的文档评分和排序可能不同。原因在于：Elasticsearch将请求基于主分片和副本进行轮询负载，所以相同请求在执行时可能是在不同的副本上执行的；而各个副本对于索引文档的统计数据又会因为文档的删除和更新的刷新延迟而会有所不同（被删除和更新的文档并不会立即删除，只有在包含此文档的段合并时才会被删除，而索引文档统计时会将这些文档考虑在内）。

>解决此问题的方式就是使用<em>preference</em>，对同一用户，始终使用同一参数(如用户id等)则可以保证用户的查询会在同一分片上执行，多次查询返回的结果会保持一致。

#### 相关性异常
>内容完全相同的两个文档，在查询时，评分可能不同；且精确匹配的文档有可能没有排在前面。

>默认情况下，Elasticsearch通过每个分片返回的评分计算文档评分作为最终响应中的评分，这种模式只有在各个分片的索引统计信息非常相似的情况下才能表现非常好，所以此种模式的假设就是在默认情况下文档被均匀地分配给各个分片，每个分片上的索引统计信息非常相似，则生成的评分就会非常接近。

>但是在如下情况下：
- 在索引文档时使用 routing
- 做多索引查询
- 数据量非常小
评分就会发生异常，因为各个分片文档差异可能会很大。

>如果数据量比较小，将数据只索引在一个分片上就可以避免此问题。<em>index.number_of_shards: 1</em>

>否则，我们的建议就是使用<em>dfs_query_then_fetch</em> search type.

>注意在查询中使用了大量的fields/terms或者fuzzy查询时，使用dfs_query_then_fetch可能会比较昂贵。正常情况下，消耗不高。

### 索引速度调优
- 使用bulk  
>批量提交时的请求数需要使用标准测试进行校准，请求过大会增加集群内存压力，建议最大请求为几十兆为宜。
- 使用多线程索引数据
>在Elasticsearch不能够应对索引速度时，会返回 TOO_MANY_REQUESTS (429)，EsRejectedExecutionException.
- 增加索引刷新间隔
>默认的索引刷新间隔是(index.refresh_interval)1s。增大此值，可以增加段的大小，降低合并段的压力。
- 在索引数据前关闭索引刷新和分片复制
>在索引大量数据前，可以关闭索引刷新(设置index.refresh_interval 为 -1)和关闭分片复制(设置index.number_of_replicas 为 0),这将会导致如果分片丢失则数据全部丢失的风险，但同时也能够提高数据索引的速度。在数据索引完毕之后恢复上述两个配置即可。
- 关闭换出
>关闭操作系统对JVM堆内存的换出
- 留出至少一半内存给文件系统缓存
>确保操作系统一半的内存留给文件系统作为缓存。
- 使用自动生成id
>在索引文档时如果指定id,Elasticsearch需要检查此id的文档是否存在，这个过程很消耗资源。使用自动生成id,则可以避免此操作。
- 使用更快的硬件
    - 使用SSD 使用RAID 0 阵列。数据冗余由集群内副本保证。
    - 一定要使用本地存储，不要使用NFS和SMB
    - 避免使用虚拟存储如Amazon’s Elastic Block Storage
- 索引缓冲大小
>如果索引需要有大量文档索引操作，则需要确保 <em>indices.memory.index_buffer_size</em>足够大，至多要能够为每个分片提供大于512MB(大于512MB时，性能并不会得到提升)的缓冲区用于索引文档时使用。

>通常认为<em>indices.memory.index_buffer_size</em>的值应为堆内存大小的10%。比如JVM 堆内存大小为10GB，那么该项就应配置为1GB。

- 关闭 _field_names
>_field_names 字段会导致索引文档开销。如果永远都不会使用exists查询，则可以关闭此字段。

- 其他的优化策略查看 '磁盘使用调优'

### 查询速度调优
- 留出至少一半内存给文件系统缓存
>Elasticsearch重度依赖于操作系统文件缓存来提高搜索速度，确保操作系统一半的内存留给文件系统作为缓存。
- 使用更快的硬件
    - 使用SSD 使用RAID 0 阵列。数据冗余由集群内副本保证。
    - 一定要使用本地存储，不要使用NFS和SMB
    - 避免使用虚拟存储如Amazon’s Elastic Block Storage
- 优化文档模型
    - 避免连接
    - 嵌套对象会让查询慢几倍
    - 父子文档会让查询慢几百倍
- 尽可能以最少的字段搜索
>查询的字段越多越慢。如果确实需要对多个字段进行查询，则可以将这些字段的值复制到同一个字段中，然后使用此字段查询即可，这可以用<em>copy_to</em>实现。
- 提前索引数据
>可以在文档中存储一些辅助性数据来提高搜索速度。例如：一个查询需要根据price字段做range 聚合；则可以在索引文档时就将当前文档所属的range范围作为一个字段存入文档，这样就可以不再使用range聚合而是使用term查询。
- 映射
>有些字段是数字类型，但是不见得在索引文档中它也要存为数字类型。比如一些字段可以存为keyword，而不是存为integer或者long.
- 避免使用脚本
>如果不得不使用脚本，尽量使用painless和expressions引擎。
- 使用近似日期
>在日期查询中,由于now是一个每时每刻都不同的时间，所以使用<em>now</em>会导致查询无法有效使用缓存。可以使用<em>now/m</em>等近似日期，使得所有用户在同一分钟内的查询可以有效地利用缓存。
```sh
# 将查询分割为可以有效利用缓存的查询，但是在某些情况下由于bool查询的影响可能也会更慢
curl -X GET "localhost:9200/index/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "constant_score": {
      "filter": {
        "bool": {
          "should": [
            {
              "range": {
                "my_date": {
                  "gte": "now-1h",
                  "lte": "now-1h/m"
                }
              }
            },
            {
              "range": {
                "my_date": {
                  "gt": "now-1h/m",
                  "lt": "now/m"
                }
              }
            },
            {
              "range": {
                "my_date": {
                  "gte": "now/m",
                  "lte": "now"
                }
              }
            }
          ]
        }
      }
    }
  }
}
'
```
- 强制合并只读索引
>将只读索引段合并为大段可以更好地提高搜索速度。但是不要强制合并仍然在写入的段。<em>merged down to a single segment</em>.
- 预热全局顺排索引
>Global ordinals 是一种数据结构，它可以用于在keyword类型的数据上执行terms聚合。由于Elasticsearch不知道何时会使用它们，所以它们是懒加载到内存的。我们可以告诉Elasticsearch在索引刷新时提前加载Global ordinals:
```sh
# eager_global_ordinals 参数设置为true时会提前加载
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "mappings": {
    "_doc": {
      "properties": {
        "foo": {
          "type": "keyword",
          "eager_global_ordinals": true
        }
      }
    }
  }
}
'
```
- 预热文件系统缓存
><em>index.store.preload </em> 可以提示Elasticsearch提前加载一些索引到文件系统缓存中，当然这依赖于具体的操作系统。但是加载过多的索引或者文件到文件系统缓存中可能会导致查询更慢，需要谨慎使用此项。

- 将标识字段映射为 keyword
>keyword类型字段对term查询来说更优。而Number类型字段对Range查询更优。但是标识字段不会用作range查询，所以可以映射为keyword类型。

- 配置索引排序来加速连接查询
><em>index sorting</em>可以使连接查询速度更快。
- 使用preference优化缓存利用率
>有很多缓存，如文件系统缓存、请求缓存、查询缓存；所有这些缓存都是存在于某个节点上。如果请求执行的节点不同，则就不能很好地利用这些缓存。使用preference使用多次执行同一的查询在同一分片上执行，对提高缓存利用率大有裨益。
- 副本对吞吐量有益，但不总是有益
>文件系统缓存很可能是对Elasticsearch性能影响最大的因子。当每个分片有足够的文件缓存时，性能最高。如果单个节点分片过多，每个分片使用的文件缓存减小，则性能下降。

>正确的副本数 = max(max_failures, ceil(num_nodes / num_primaries) - 1)

    >注：
    - num_nodes 节点数
    - num_primaries 所有的索引主分片总数
    - max_failures 在保证数据安全的情况下，最大可异常节点数

- 打开自适应副本选择
>在存在多个复制集的情况下，Elasticsearch会动态根据一个<em>自适应副本选择标准(包括：响应时间、服务时间和队列大小等)</em>来选择请求执行的分片以提高吞吐量降低延迟。

### 使用性能分析接口调优
>注意：
- 性能分析接口会增加索引开销
- 性能分析接口的耗时不能作为正常查询耗时参考，但是可以作为不同查询结构的相对参考
- 可以用于相对性能分析，但是不能作为绝对性能标准
### 磁盘调优
- 关闭不必要的功能
>默认情况下Elasticsearch索引为大多数字段打开了doc-values功能，所以这些字段可以用于搜索和聚合。
```sh
#如果某些字段永远都不会用于过滤，则可以将index关闭
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "mappings": {
    "_doc": {
      "properties": {
        "foo": {
          "type": "integer",
          "index": false
        }
      }
    }
  }
}
'
# 如果一个字段仅用于match而不关系使用它来评分，则可以关闭norms
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "mappings": {
    "_doc": {
      "properties": {
        "foo": {
          "type": "text",
          "norms": false
        }
      }
    }
  }
}
'
# 如果一个字段不会用于短语搜索，则可以告诉Elasticsearch不用索引position
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "mappings": {
    "_doc": {
      "properties": {
        "foo": {
          "type": "text",
          "index_options": "freqs"
        }
      }
    }
  }
}
'
```

- 不要使用默认动态字符串映射
```sh
# 指定字符串字段映射类型，防止不必要的将字段映射为text
curl -X PUT "localhost:9200/index" -H 'Content-Type: application/json' -d'
{
  "mappings": {
    "_doc": {
      "dynamic_templates": [
        {
          "strings": {
            "match_mapping_type": "string",
            "mapping": {
              "type": "keyword"
            }
          }
        }
      ]
    }
  }
}
'
```

- 注意分片大小
>更大的分片在存储数据时更高效。为了增大分片大小，可以降低主分片数目。当然，分片更大也有弊端，比如恢复很慢。

- 关闭 _all
>_all占用了可观的空间，如果不必要就关闭此字段。
- 关闭 _source
>_source存储了文档原始JSON。如果不会使用_source，就关闭它。当然，update和reindex API就不能工作了。
- 使用 best_compression
>_source 和stored fields 使用了不可忽略的磁盘空间，可以使用 best_compression 来对它们进行压缩。
- 强制合并
>可以使用 _forcemerge API将分片段强制合并，每个分片最小可以合并为只有一个段(max_num_segments=1)。
- 对索引收缩
>使用Shrink API 可以减小索引的分片数。
- 使用可用的占空间最小的数字类型
- 使用索引排序来将相似文档放在一起
>Elasticsearch保存_source时，会将不同的文档进行压缩以提高压缩比。默认情况下文档以它们索引到索引中的顺序进行压缩。如果打开<em>idnex sorting</em>,则文档会按照排序后的顺序进行压缩，这样可以提高压缩比。
- 保持文档中字段顺序一致