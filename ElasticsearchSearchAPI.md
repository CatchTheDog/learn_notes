><b>此文档基于版本6.3</b>
# Search API
>除<em>Explain API</em>外，其他的所有<em>Search API</em>都支持多索引。
```sh
GET /kimchy,elasticsearch/_search?q=tag:wow
```
- routing参数
- 自动选择最佳分片
  ```sh
  curl -X PUT "localhost:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
    {
        "transient": {
            "cluster.routing.use_adaptive_replica_selection": true
        }
    }
    '
  ```
- 统计分组
- 全局超时时间设置
- 查询取消
- 查询并发和并行化处理

## Mutil Index Search
>除<em>Explain API</em>外，其他的所有<em>Search API</em>都支持多索引。
```sh
GET /kimchy,elasticsearch/_search?q=tag:wow
```

## URI Search
```sh
curl -X GET "localhost:9200/twitter/_search?q=user:kimchy"
```
### search_type
>Elasticsearch 为分布式系统，在进行数据搜索时，协调节点会向所有有可能存在数据的分片(存在于不同节点上)发送搜索请求，各个节点收到搜索请求后，在当前分片中获取符合搜索条件的记录返回，协调节点将各个节点返回的所有数据进行整合，然后将满足条件的数据返回给客户端。有两个问题：
- 数量问题
  >若客户端提供的分页参数为：from=10,size=15;则每个分片都需要返回自己分片内满足条件的前150条数据供协调节点排序，协调节点对收集到的数据进行重新排序，只保留重新排序后第135-150之间的数据返回给客户端。
- 评分问题
  >每个分片返回的文档的评分都是基于当前分片内的数据计算所得；而最终返回给客户端的评分需要基于索引内所有分片中的数据计算获取；所以应该先在所有分片中获取计算评分所需的信息，进行统一计算，然后将整体数据发送给各个分片，各个分片使用整体数据计算出当前分片内各个文档的评分。

>为了让用户进行取舍，Elasticsearch提供了search_type参数
- query_then_fetch 默认
   - 第一步，先向所有的shard发出请求，各分片只返回排序和排名相关的信息（注意，不包括文档document)，然后按照各分片返回的分数进行重新排序和排名，取前size个文档。
   - 第二步，去相关的shard取document。这种方式返回的document与用户要求的size是相等的。
- query_and_fetch
  >向索引的所有分片（shard）都发出查询请求，各分片返回的时候把元素文档（document）和计算后的排名信息一起返回。这种搜索方式是最快的。因为相比下面的几种搜索方式，这种查询方法只需要去shard查询一次。但是各个shard返回的结果的数量之和可能是用户要求的size的n倍。
- dfs_query_then_fetch
  >比第一种方式多了一个初始化散发(initial scatter)步骤。
- dfs_query_and_fetch
  >这种方式比第二种方式多了一个初始化散发(initial scatter)步骤，有这一步，据说可以更精确控制搜索打分和排名。

  >初始发散(initial scatter)
  >> 初始化散发其实就是在进行真正的查询之前，先把各个分片的词频率和文档频率收集一下，然后进行词搜索的时候，各分片依据全局的词频率和文档频率进行搜索和排名。显然如果使用DFS_QUERY_THEN_FETCH这种查询方式，效率是最低的，因为一个搜索，可能要请求3次分片。但，使用DFS方法，搜索精度应该是最高的。
  >>从性能考虑QUERY_AND_FETCH是最快的，DFS_QUERY_THEN_FETCH是最慢的。从搜索的准确度来说，DFS要比非DFS的准确度更高。

  ## 请求体查询
  请求体查询支持使用GET/POST方法。请求体中需要使用<em>Query DSL</em>.
  - from + size 不能大于 index.max_result_window 设定值，默认值10000
    >翻页可以使用scroll 或者 search after替代。与scroll 使用查询生成瞬时快照不同，search after查询是实时操作，所以如果在前一次查询与后一次查询之间索引内文档发生了新增、删除和修改，则排序结果有可能会发生变化。
    ```sh
    # 首次查询
    curl -X GET "localhost:9200/twitter/_search" -H 'Content-Type: application/json' -d'
        {
            "size": 10,
            "query": {
                "match" : {
                    "title" : "elasticsearch"
                }
            },
            "sort": [
                {"date": "asc"},
                {"_id": "desc"}
            ]
        }
        '
    # 后续查询
        curl -X GET "localhost:9200/twitter/_search" -H 'Content-Type: application/json' -d'
        {
            "size": 10,
            "query": {
                "match" : {
                    "title" : "elasticsearch"
                }
            },
            "search_after": [1463538857, "654323"],
            "sort": [
                {"date": "asc"},
                {"_id": "desc"}
            ]
        }
        '
    ```

    ## 排序
    ```sh
    curl -X GET "localhost:9200/my_index/_search" -H 'Content-Type: application/json' -d'
    {
        "sort" : [
            { "post_date" : {"order" : "asc"}},
            "user",
            { "name" : "desc" },
            { "age" : "desc" },
            "_score"
        ],
        "query" : {
            "term" : { "user" : "kimchy" }
        }
    }
    '
    ```
>若按照_score排序，则默认顺序为：desc;按照其他字段排序则默认顺序为：asc。

### 排序模式选项
>Elasticsearch 支持按照多值字段或者多个字段进行排序；<em>mode</em>选项值如下：

|mode|含义|
|---|---|
|min|以最小值排序|
|max|以最大值排序|
|sum|以总和排序(仅对值是数字的数组字段有用)|
|avg|以平均值排序(仅对值是数字的数组字段有用)|
|median|以中值排序(仅对值是数字的数组字段有用)|

```sh
curl -X PUT "localhost:9200/my_index/_doc/1?refresh" -H 'Content-Type: application/json' -d'
{
   "product": "chocolate",
   "price": [20, 4]
}
'
curl -X POST "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
   "query" : {
      "term" : { "product" : "chocolate" }
   },
   "sort" : [
      {"price" : {"order" : "asc", "mode" : "avg"}}
   ]
}
'
```
### 对嵌套对象排序
>对嵌套对象排序由<em>nested</em> 排序选项支持，它有以下属性：
- path
    指定在哪个嵌套对上排序，排序字段必须在此对象内
- filter
    对嵌套对象进行过滤，仅满足此过滤条件的内嵌对象才会参与排序，默认情况下无筛选
- nested
    与顶层nested相同，但是应用于嵌套对象内的嵌套对象

eg:
```sh
curl -X POST "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
   "query": {
      "nested": {
         "path": "parent",
         "query": {
            "bool": {
                "must": {"range": {"parent.age": {"gte": 21}}},
                "filter": {
                    "nested": {
                        "path": "parent.child",
                        "query": {"match": {"parent.child.name": "matt"}}
                    }
                }
            }
         }
      }
   },
   "sort" : [
      {
         "parent.child.age" : {
            "mode" :  "min",
            "order" : "asc",
            "nested": {
               "path": "parent",
               "filter": {
                  "range": {"parent.age": {"gte": 21}}
               },
               "nested": {
                  "path": "parent.child",
                  "filter": {
                     "match": {"parent.child.name": "matt"}
                  }
               }
            }
         }
      }
   ]
}
'
```
### 无值字段
<em>missing</em> 参数指定在排序时如何处理排序字段没有值的文档，有效值：
- _last
- _first
- 自定义值，该值会被用作无值文档的排序字段值用来排序
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "sort" : [
        { "price" : {"missing" : "_last"} }
    ],
    "query" : {
        "term" : { "product" : "chocolate" }
    }
}
'
```

### 未指定映射字段
默认情况下，对未指定映射字段的搜索会失败；<em>unmapped_type</em> 用于解决在某些未指定映射字段上排序问题。
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "sort" : [
        { "price" : {"unmapped_type" : "long"} }
    ],
    "query" : {
        "term" : { "product" : "chocolate" }
    }
}
'
```
### 地理坐标距离排序
### 基于脚本排序
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "term" : { "user" : "kimchy" }
    },
    "sort" : {
        "_script" : {
            "type" : "number",
            "script" : {
                "lang": "painless",
                "source": "doc[\u0027field_name\u0027].value * params.factor",
                "params" : {
                    "factor" : 1.1
                }
            },
            "order" : "asc"
        }
    }
}
'
```
### 跟踪评分
>在使用其他字段排序时，默认情况下不会计算评分，<em>track_scores</em> 选项用于开启评分：
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "track_scores": true,
    "sort" : [
        { "post_date" : {"order" : "desc"} },
        { "name" : "desc" },
        { "age" : "desc" }
    ],
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```
### 排序对内存影响
排序时相关字段值需要全部加载到内存，所以对字符型字段排序，该字段必须是精确值字段；对于数字型字段，该字段类型必须是一个明确的类型，如：short/integer/float.

## 字段过滤
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "_source": {
        "includes": [ "obj1.*", "obj2.*" ],
        "excludes": [ "*.description" ]
    },
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```
## fields
<em>stored_fields</em> 参数 默认值为false且不建议开启；使用<b>字段过滤</b>即可。

>stored_fields 与 source区别：
- stored_fields与_source在索引字段映射生成时指定(store/_source)
- 如果_source未关闭(默认开启),不论是否指定store=true，各个字段值都会被存储在_source中
- store=true,是将开启stored_fields的字段额外在与_source平级的结构中存储了一份，此结构中仅存储store=true的字段
- 开启stored_fields会增大磁盘开销(存储数据多了)，且在获取stored_fields时会增加磁盘读取开销，所以不建议使用stored_fields
- 除非文档关闭了_source，而只关心文档中的某几个字段，可以将这些字段存储在stored_fields中
- stored_fields不能返回对象节点，仅能返回叶子节点

```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "stored_fields" : ["user", "postDate"],
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```
## 脚本字段
```sh
# 1
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match_all": {}
    },
    "script_fields" : {
        "test1" : {
            "script" : {
                "lang": "painless",
                "source": "doc[\u0027price\u0027].value * 2"
            }
        },
        "test2" : {
            "script" : {
                "lang": "painless",
                "source": "doc[\u0027price\u0027].value * params.factor",
                "params" : {
                    "factor"  : 2.0
                }
            }
        }
    }
}
'
# 2
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
    {
        "query" : {
            "match_all": {}
        },
        "script_fields" : {
            "test1" : {
                "script" : "params['_source']['message']"
            }
        }
    }
'
```

### 区别
<em>doc['my_field'].value</em> 与 <em>params['_source']['my_field']</em>区别：
- doc 会将检测字段加载到内存中(使用缓存)，执行更快，但是消耗更多内存；并且doc 这种方式仅能返回简单值字段，不能返回内嵌对象字段，也只能对精确值字段进行处理。
- params 这种方式非常低效
- 建议使用doc方式

## Doc value Fields
如果指定字段不是doc_value字段，则会将此字段加载到内存，此查询会增加内存消耗。
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match_all": {}
    },
    "docvalue_fields" : ["test1", "test2"]
}
'
```

## Post filter
在基于查询结果的聚合计算完成之后，<em>post_filter</em> 可以将查询结果中的一部分数据去除(比如，用户需要查看颜色是红色，品牌是gucci的服装；但是我们同时也需要产生品牌是gucci的服装的统计数据；所以我们可以先查询所有的品牌是gucci的服装，在查询结果集上计算统计结果；在统计结果计算完成之后，再从查询结果中筛选出颜色是红色的服装信息，作为查询结果；将查询结果与聚合结果合并返回。)
```sh
curl -X GET "localhost:9200/shirts/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "filter": {
        "term": { "brand": "gucci" } 
      }
    }
  },
  "aggs": {
    "colors": {
      "terms": { "field": "color" } 
    },
    "color_red": {
      "filter": {
        "term": { "color": "red" } 
      },
      "aggs": {
        "models": {
          "terms": { "field": "model" } 
        }
      }
    }
  },
  "post_filter": { 
    "term": { "color": "red" }
  }
}
'
```

## 高亮
>高亮不反映复杂的布尔查询逻辑，所以对于一些复杂查询，可能会出现一些不相关的字段被高亮。

>_all字段只能在它被指定store=true时才可以使用高亮

>Elasticsearch 支持三种类型的高亮：
- unified 默认的高亮类型，使用Luncene Unified Highlighter，支持对精确查询和非精确查询(fuzzy,prefix,regex)高亮
- plain 使用标准的Luncene 高亮器；它能够反映基于词重要性和词位置的短语查询中的match逻辑查询进行处理。
  >palin 高亮器 会在内存中对每个文档每个字段进行计算，如果对大量文档大量字段进行高亮计算，建议使用unified(使用posting或者term_vector 字段)
- fvh Fast vector highlighter 使用Luncene Fast Vector 高亮器。这个高亮器可以用于在字段映射中 term_vector设置为 <em>with_positions_offsets</em>的字段。它具有以下特性：
    - 可以使用<em>boundary_scanner</em>进行自定义
    - 需要字段 term_vector设置为 with_positions_offsets,但是这个设置会使得索引大小增长
    - 可以将不同字段的匹配合并到一个结果中 <em>matched_fields</em>
    - 可以为字段不同位置匹配赋予不同的权重 如<em>Boosting Query</em>

```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match": { "content": "kimchy" }
    },
    "highlight" : {
        "fields" : {
            "content" : {}
        }
    }
}
'
```

### 偏移策略
为了生成有用的查询片段，高亮器需要了解每个词在原始文档字段中的起始偏移量与结束偏移量。这些信息可以由以下方式生成：
- The posting list.如果在映射配置中 <em>index_options</em> 被设置为<em>offsets</em>,则 unified高亮器可以使用这些信息来生成高亮信息而不用再次对文档进行分析。如果对于大字段进行高亮时，设置此配置很重要。这种方式要比term_vectors少占用磁盘空间。
- Term verctors.如果在映射配置中配置 <em>term_vector</em>为<em>with_positions_offsets</em>,那么unified高亮器可以自动使用term_vector高亮字段。对于大字段这种方式非常高效。fvh高亮器也使用此信息。
- Plain highlighting.如果没有前两者提供信息，则unified会使用此种方式。这种方式会在内存中创建一个小索引然后重新运行原始查询获取当前文档的匹配信息。这种操作会对需要高亮的每个文档的每个字段逐个进行。palin 高亮器 使用此种方式。这种方式在面对大文本时可能会耗费巨额的内存和时间；为了避免这种情况发生，下个版本会使用index.highlight.max_analyzed_offset配置项限制可分析文本包含最大字符数。

### 设置参数
高亮参数可以在全局配置，然后在字段中可以覆盖全局配置。
- boundary_chars 分界字符，默认<em>.,!? \t\n</em>
- boundary_max_scan 默认值20
- boundary_scanner 指定如何分割高亮片段，仅对unified和fvh高亮器有用；对unified高亮器默认值是sentence，对fvh高亮器默认值是chars
    - chars 仅对fvh有效，使用boundary_chars指定的字符作为高亮边界；boundary_max_scan指定为达到高亮边界最大扫描距离。
    - sentence 将高亮片段在句子边界处分割。<em>boundary_scanner_locale</em> <em>fragment_size</em>
    - word 将高亮片段在词边界处分割。<em>boundary_scanner_locale</em>
- boundary_scanner_locale 控制哪种本地化环境用于搜索句子和单词边界。eg:"en-US","fr-FR","js-JP"等。
- encoder 标识片段是否需要使用HTML标签包裹，默认 default(无标签包裹)，html
- fields 指定哪些字段需要高亮，可以使用 <em>comment_*</em>对所有以comment_开头的text或者keyword类型的字段进行高亮。
    >通配符类名称只能用于高亮text和keyword字段，如果是其他类型的字段高亮，需要使用全字段名称指定。
- force_source 高亮基于_source,即使字段被存储在不同位置。默认值为false
- fragmenter 指定文本如何被分割成高亮片段,只对plain 高亮器有效，默认值为span：
   - simple 分割为相同大小的片段 
   - span 分割为相同大小的片段，但是避免在高亮词之间进行分割。
- fragment_offset 控制从何处起开始高亮，仅对fvh有效。
- fragment_size 高亮片段字符数大小，默认100。
- highlight_query 高亮使用query匹配而不是search.
- matched_fields 将多个字段的匹配合并用于高亮某一个字段。仅对fvh有效。
- no_match_size 如果没有匹配片段时需要返回的字段片段长度，默认为0
- number_of_fragments 返回片段的最大数量。默认值5
- order 如果有评分时按照评分对高亮片段进行排序。默认值是none,以出现在字段中的顺序输出。设置为score时，将会按照评分倒序排列。
- phrase_limit 用于控制最多计算的短语数目，方式fvh分析过多的短语导致内存使用过高。仅对fvh有效，默认256
- pre_tags 与post_tags配合使用，定义用来包裹高亮文档的标签。默认情况下使用<em></em>标签。
- post_tags
- require_field_match 默认情况下，只有使用query match的字段才会被高亮。如果设置为false,则所有字段都会被高亮。默认为true.
- tags_schema 设置内置标签的样式。
- type unified plain fvh 默认值 unified


### 高亮内部工作原理
>高亮器需要解决的问题：
- 怎样分割片段?
>plain 高亮器使用指定的分析器分析文本，生成token流，然后遍历整个tocken流，每当当前token结束位置偏移量超过了fragment_size*已经生成的片段数，就创建一个新的片段。uniifed和fvh高亮器只要fragment_size允许一个片段就是一个有效的句子。
- 怎样在整个片段中寻找到最好的片段?
>相关配置：number_of_fragments 
- 如何在查询结果字段片段中高亮查询短语?
>相关配置：pre-tags,post-tags

## 重新评分
>重新评分有助于对一些query 和post_filter查询返回的顶部文档(Top K)进行重新排序(使用比较耗时的算法)。重新评分在各个分片将文档返回给协调节点进行整合前就已完成。

>当使用<em>rescore</em>时，如果指定字段排序而非使用score排序，会抛出异常。
在使用翻页过程中，不要改变<em>window_size</em>。
### Query rescorer
>查询过程中每个分片返回的文档数用参数<em>window_size</em>控制，默认值为10；在默认情况下原始查询结果中文档评分与重新评分查询获取的评分将会被线性合并用来生成最终文档评分。原始查询结果中文档评分和重新评分查询获取的评分所占比重可以使用参数<em>query_weight</em> 和 <em>rescore_query_weight</em>控制。两者默认值都是1.
```sh
curl -X POST "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
   "query" : {
      "match" : {
         "message" : {
            "operator" : "or",
            "query" : "the quick brown"
         }
      }
   },
   "rescore" : {
      "window_size" : 50,
      "query" : {
         "rescore_query" : {
            "match_phrase" : {
               "message" : {
                  "query" : "the quick brown",
                  "slop" : 2
               }
            }
         },
         "query_weight" : 0.7,
         "rescore_query_weight" : 1.2
      }
   }
}
'
```
>评分合并的方式可以通过参数<em>score_mode</em>控制：

|模式|描述|
|--|--|
|total|将前后评分相加，默认模式|
|mutiply|将前后评分相乘，对 <em>function query</em> 很有用|
|avg|取两次评分平均值|
|max|取两次评分最大值|
|min|取两次评分最小值|

```sh
curl -X POST "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
   "query" : {
      "match" : {
         "message" : {
            "operator" : "or",
            "query" : "the quick brown"
         }
      }
   },
   "rescore" : [ {
      "window_size" : 100,
      "query" : {
         "rescore_query" : {
            "match_phrase" : {
               "message" : {
                  "query" : "the quick brown",
                  "slop" : 2
               }
            }
         },
         "query_weight" : 0.7,
         "rescore_query_weight" : 1.2
      }
   }, {
      "window_size" : 10,
      "query" : {
         "score_mode": "multiply",
         "rescore_query" : {
            "function_score" : {
               "script_score": {
                  "script": {
                    "source": "Math.log10(doc.likes.value + 2)"
                  }
               }
            }
         }
      }
   } ]
}
'
```

## Search Type
>控制在查询执行过程中协调节点从各个分片上取回文档的过程。
- query_then_fetch
- dfs_query_then_fetch

## scroll

### sliced scroll
为了能够支持并行处理，提供了<em>sliced scroll</em>,每个scroll都能独立处理。
```sh
curl -X GET "localhost:9200/twitter/_search?scroll=1m" -H 'Content-Type: application/json' -d'
{
    "slice": {
        "id": 0, 
        "max": 2 
    },
    "query": {
        "match" : {
            "title" : "elasticsearch"
        }
    }
}
'
curl -X GET "localhost:9200/twitter/_search?scroll=1m" -H 'Content-Type: application/json' -d'
{
    "slice": {
        "id": 1,
        "max": 2
    },
    "query": {
        "match" : {
            "title" : "elasticsearch"
        }
    }
}
'
```
>为了避免消耗内存激增，需要限制slice.max 不要大于索引分片数；
>为了避免消耗激增，可以使用一个字段作为切分依据，此字段需要满足以下要求：
- 此字段值是数字
- 此字段 doc_values开启
- 索引中每个文档此字段只含有一个值，若个文档此字段包含多个值，则第一个值将会被使用
- 索引中每个文档的此字段值在索引创建以后在初始化以后再也没有做过修改
- 包含此字段的文档数与索引中文档总数应该相近

```sh
curl -X GET "localhost:9200/twitter/_search?scroll=1m" -H 'Content-Type: application/json' -d'
{
    "slice": {
        "field": "date",
        "id": 0,
        "max": 10
    },
    "query": {
        "match" : {
            "title" : "elasticsearch"
        }
    }
}
'
```
>如果某个字段值是递增的时间戳，则该字段被用来切分scroll 应该是非常安全的
>默认情况下，scroll最大切分数是1024，可以使用 <em>index.max_slices_per_scroll</em> 来自行设置。

## 偏好
<em>preference</em>用于控制查询在哪些分片上执行，它有如下值：
- _only_local
- _local
- prefer_nodes:abc,xyz
- _shards:2,3
- _only_nodes:abc*,x*yz
- 自定义值
```sh
curl -X GET "localhost:9200/_search?preference=xyzabc123" -H 'Content-Type: application/json' -d'
{
    "query": {
        "match": {
            "title": "elasticsearch"
        }
    }
}
'
```
>不建议使用 _primary, _primary_first, _replica 和 _replica_first。有可能会导致前后不一致的查询结果且会对节点造成额外负载。

## Explain
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "explain": true,
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```

## Version
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "version": true,
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```

## Index Boost
>当数据来自于多个索引时，可以通过<em>Index Boost</em>控制不同索引的数据的评分相对权重。
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "indices_boost" : [
        { "alias1" : 1.4 },
        { "index*" : 1.3 }
    ]
}
'
```
>上面查询语句中使用了索引别名和通配符，如果文档在两个索引均返现了匹配的文档，则<em>boost value</em> 是 1.4。

## min_score
>排除文档评分小于该配置设定值的文档。
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "min_score": 0.5,
    "query" : {
        "term" : { "user" : "kimchy" }
    }
}
'
```
## Named Queries
>每个filter和query都在最顶层接受一个参数<em>_name</em>;仅对bool查询有效。
```sh
curl -X GET "localhost:9200/_search" -H 'Content-Type: application/json' -d'
{
    "query": {
        "bool" : {
            "should" : [
                {"match" : { "name.first" : {"query" : "shay", "_name" : "first"} }},
                {"match" : { "name.last" : {"query" : "banon", "_name" : "last"} }}
            ],
            "filter" : {
                "terms" : {
                    "name.last" : ["banon", "kimchy"],
                    "_name" : "test"
                }
            }
        }
    }
}
'
```
## Inner hits
