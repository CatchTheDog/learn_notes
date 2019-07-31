# 聚合
>聚合返回基于query 查询的结果进行聚合计算后的数据。有以下四大类：
- 桶
>支持脚本。一些列构造桶的聚合，每个桶都与一个key和一个文档分类标准相关联。当聚合执行时，所有的文档分类标准(依据)对每个文档进行判断，将其分类到其满足的桶中。此类聚合执行的结果是：我们得到了桶的集合，每个桶内都含有满足该桶分类依据的文档。
>桶聚合可以有子聚合(桶聚合或者指标聚合)
- 指标
>支持脚本。能够跟踪聚合轨迹且在一系列文档上计算指标的聚合类型。
- 矩阵
>不支持脚本。可以在从请求文档内提取的多个字段值上操作并生成矩阵结果。
- 管道
>对其他聚合的结果和与其关联的指标进行聚合处理。

>聚合可以嵌套，可以在桶聚合中嵌套指标聚合，这是聚合的真正威力所在。
>聚合操作用于对double类型数据进行处理。因此，如果聚合操作用于绝对值大于2^53 的long型数据，聚合结果可能是近似值。

>聚合的结构
```sh
"aggregations" : {
    "<aggregation_name>" : {
        "<aggregation_type>" : {
            <aggregation_body>
        }
        [,"meta" : {  [<meta_data_body>] } ]?
        [,"aggregations" : { [<sub_aggregation>]+ } ]?
    }
    [,"<aggregation_name_2>" : { ... } ]*
}
```

>子聚合只能够在桶聚合中使用。

>Values Source
- 脚本
- 对没有mapping的字段，使用<em>value_type</em>.

## 指标聚合
>指标聚合根据从文档某个字段中提取出的值或者使用脚本对文档字段中的值进行处理后的值计算出指标。

>数字指标聚合是指标聚合中比较特殊的类型，它只返回数字。有些聚合返回单个数字值(如<em>avg</em>)而有些聚合返回多个数字值(如<em>stats</em>).这些指标聚合在作为桶排序的直接子聚合时扮演重要角色(某些聚合要求将桶排序聚合的结果按照几个字段值的统计值排序返回)。

### 平均数聚合
>计算文档某字段值平均值，这些字段值可以是文档字段值，也可以是脚本生成的值。
```sh
curl -X POST "localhost:9200/exams/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs" : {
        "avg_grade" : {
            "avg" : {
                "script" : {
                    "source" : "doc.grade.value"
                }
            }
        }
    }
}
'
curl -X POST "localhost:9200/exams/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs" : {
        "avg_grade" : {
            "avg" : {
                "script" : {
                    "id": "my_script",
                    "params": {
                        "field": "grade"
                    }
                }
            }
        }
    }
}
'
curl -X POST "localhost:9200/exams/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs" : {
        "avg_corrected_grade" : {
            "avg" : {
                "field" : "grade",
                "script" : {
                    "lang": "painless",
                    "source": "_value * params.correction",
                    "params" : {
                        "correction" : 1.2
                    }
                }
            }
        }
    }
}
'

```

>对 missing value 的文档处理：默认情况下此文档将会被忽略，我们可以为聚合字段缺失值的文档指定一个固定的字段值。
```sh
curl -X POST "localhost:9200/exams/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs" : {
        "grade_avg" : {
            "avg" : {
                "field" : "grade",
                "missing": 10 
            }
        }
    }
}
'
```
### Cardinality Aggregation
>一个获取某个字段内唯一值的近似数目的单值指标聚合。用于统计的值既可以来自于某个字段也可以来自于脚本生成。
```sh
curl -X POST "localhost:9200/sales/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs" : {
        "type_count" : {
            "cardinality" : {
                "field" : "_doc",
                "precision_threshold": 100 
            }
        }
    }
}
'
```
><em>precision_threshold</em>参数用于近似精度控制，此参数指定一个值，在此值之外，计数会有近似。此参数支持的最大值为：40000，设置大于40000的值的效果与设置40000的效果一样，默认值是3000。此选项是以内存换精度，即精度提高，则意味着聚合消耗内存增加。

#### 关于计数近似
>如果要计算精确值，就需要将所有值加载到HashSet中，然后获取集合size即可。但是这种方式在对大量数据或者大值字段进行处理时在内存消耗和分片之间的信息交换上耗费巨大资源。

>Cardinality 聚合基于 <em>HyperLogLog++</em>算法，此算法具有以下属性：
- 可配置的精度，以此来决定怎样用内存换精度
- 在低基数数据集聚合是精度非常高
- 无论数据量多大，内存占用量仅由精度决定;如果我们设置精度是c,则内存使用量为 c * 8 字节。

>注：对<em>大数据量</em>的string字段来说，提前将字段的hash值存在文档中，然后使用此hash值进行Cardinality 聚合无疑是最快的。可以使用Mapper Murmur3 Plugin来完成生成hash并存入文档的工作。

### Extended Stats Aggregation
>对Stats Aggregation 的扩展，增加了:
-  sum_of_squares  平方和
-  variance 方差
-  std_deviation 标准差
-  std_deviation_bounds 标准差范围(上下界)

>标准差范围，可以使用参数 sigma
```sh
curl -X GET "localhost:9200/exams/_search" -H 'Content-Type: application/json' -d'
{
    "size": 0,
    "aggs" : {
        "grades_stats" : {
            "extended_stats" : {
                "field" : "grade",
                "sigma" : 3 
            }
        }
    }
}
'
```

### Geo Bounds Aggregation
>返回一个包含所有地理坐标点的边界矩形。

### Geo Centroid Aggregation
>根据地理坐标数据获取所有地理坐标点的中心位置点。

### Max Aggregation
>获取最大值

### Min Aggregation
>获取最小值

### Percentiles Aggregation
>根据数值字段生成一个或者多个百分位数值。默认情况下，百分位数聚合会生成[1,5,25,50,75,95,99]百分位的数值。我们也可以自定义返回的百分位数位置。
```sh
curl -X GET "localhost:9200/latency/_search" -H 'Content-Type: application/json' -d'
{
    "size": 0,
    "aggs" : {
        "load_time_outlier" : {
            "percentiles" : {
                "field" : "load_time",
                "percents" : [95, 99, 99.9] 
            }
        }
    }
}
'
```

#### Keyed Response
>在默认情况下，<em>keyed</em>被设置为<em>true</em>,每个桶都与一个唯一的key值关联且返回的结果是一个hash而不是数组。可以将<em>keyed</em>设置为<em>false</em>来使返回结果称为数组。
```sh
# default keyed true
{
    ...

   "aggregations": {
      "load_time_outlier": {
         "values" : {
            "1.0": 5.0,
            "5.0": 25.0,
            "25.0": 165.0,
            "50.0": 445.0,
            "75.0": 725.0,
            "95.0": 945.0,
            "99.0": 985.0
         }
      }
   }
}
# keyed false
{
    ...

    "aggregations": {
        "load_time_outlier": {
            "values": [
                {
                    "key": 1.0,
                    "value": 5.0
                },
                {
                    "key": 5.0,
                    "value": 25.0
                },
                {
                    "key": 25.0,
                    "value": 165.0
                },
                {
                    "key": 50.0,
                    "value": 445.0
                },
                {
                    "key": 75.0,
                    "value": 725.0
                },
                {
                    "key": 95.0,
                    "value": 945.0
                },
                {
                    "key": 99.0,
                    "value": 985.0
                }
            ]
        }
    }
}
```

#### 百分位数通常都是近似值
>Elasticsearch在计算百分位数时使用<em>TDigest</em>算法：
- 精确度与q*(1-q)成比例，这意味着极端百分位数比非极端百分位数更精确
- 对少量数据，百分位数非常精确
- 在桶内数据量变化时，百分位数计算结果会是近似值。
  
#### Compression
>近似算法需要平衡内存使用率和预估精度。Elasticsearch使用参数<em>Compression</em>来做平衡：
```sh
curl -X GET "localhost:9200/latency/_search" -H 'Content-Type: application/json' -d'
{
    "size": 0,
    "aggs" : {
        "load_time_outlier" : {
            "percentiles" : {
                "field" : "load_time",
                "tdigest": {
                  "compression" : 200 
                }
            }
        }
    }
}
'
```
><em>TDigest</em> 使用‘nodes’的数量来计算近似百分位数；nodes 的个数越多，与数据量相关的精确则越高，相应地占用的内存也就越高。<em>compression</em>参数限制nodes 数目的最大值为 20 * compression.

>所以，设置更大的compression参数可以提高精确度，但是同时也增大了内存占用和算法执行耗时。compression默认值是100.

>一个nodes 大概使用32 字节内存。所以默认情况下，可能会占用64KB内存。实践证明，数据分布越随机，TDigest占用内存越小。


### HDR Histogram
>高动态范围直方图。


### Percentile Ranks Aggregation
>百分位等级聚合，用于对数字值计算一个或者多个百分位等级。即它计算某个值在所有观察值中的百分比排名，比如如果一个值大于或者等于95%的观察值，则我们说这个值在第95的百分位等级上。
```sh
curl -X GET "localhost:9200/latency/_search" -H 'Content-Type: application/json' -d'
{
    "size": 0,
    "aggs" : {
        "load_time_ranks" : {
            "percentile_ranks" : {
                "field" : "load_time", 
                "values" : [500, 600]
            }
        }
    }
}
'
# 55%的数据小于或者等于500；64%的数据小于或者等于600
{
    ...

   "aggregations": {
      "load_time_ranks": {
         "values" : {
            "500.0": 55.00000000000001,
            "600.0": 64.0
         }
      }
   }
}
```

### Scripted Metric Aggregation
>该功能是实验性质的，有可能在未来改变或者删除。
```sh
# 从销售计算利润
curl -X POST "localhost:9200/ledger/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match_all" : {}
    },
    "aggs": {
        "profit": {
            "scripted_metric": {
                "init_script" : "params._agg.transactions = []",
                "map_script" : "params._agg.transactions.add(doc.type.value == 'sale' ? doc.amount.value : -1 * doc.amount.value)", 
                "combine_script" : "double profit = 0; for (t in params._agg.transactions) { profit += t } return profit",
                "reduce_script" : "double profit = 0; for (a in params._aggs) { profit += a } return profit"
            }
        }
    }
}
'
curl -X POST "localhost:9200/ledger/_search?size=0" -H 'Content-Type: application/json' -d'
{
    "aggs": {
        "profit": {
            "scripted_metric": {
                "init_script" : {
                    "id": "my_init_script"
                },
                "map_script" : {
                    "id": "my_map_script"
                },
                "combine_script" : {
                    "id": "my_combine_script"
                },
                "params": {
                    "field": "amount", 
                    "_agg": {}        
                },
                "reduce_script" : {
                    "id": "my_reduce_script"
                }
            }
        }
    }
}
'
```

### Stats Aggregation
>返回多个指标的聚合，返回指标包括：
- min
- max
- sum
- count
- avg
  
### Sum Aggregation
>返回求和。

### Top Hits Aggregation
>作为子聚合，返回每个桶内与查询最相关的几个文档。

>选项：
- from
- size 默认值 3
- sort 桶内文档排序方式。默认通过_score排序。

>每个hit支持以下功能：
- 高亮
- Explain
- Named filters and queries
- Source filtering
- stored fileds
- scripted fields
- Doc value fields
- include versions

>terms 聚合中不能使用 top_hits 作为排序选项值。
```sh
curl -X POST "localhost:9200/sales/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match": {
      "body": "elections"
    }
  },
  "aggs": {
    "top_sites": {
      "terms": {
        "field": "domain",
        "order": {
          "top_hit": "desc"
        }
      },
      "aggs": {
        "top_tags_hits": {
          "top_hits": {}
        },
        "top_hit" : {
          "max": {
            "script": {
              "source": "_score"
            }
          }
        }
      }
    }
  }
}
'
```