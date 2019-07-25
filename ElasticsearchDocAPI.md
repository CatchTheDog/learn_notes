# Elasticsearch 深入查询
## Index API
### 索引自动创建
索引自动创建通过配置项: <em>action.auto_create_index</em> 控制,此配置项默认是true,即可自动创建索引。允许自动创建的索引名称模式可以配置在此配置项下(正则表达式，使用+/-表示允许和禁止)；可以通过将此项配置为false禁止索引创建。
```sh
# 仅允许自动创建 twitter,index10,不允许创建index1*
curl -X PUT "localhost:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
    "persistent": {
        "action.auto_create_index": "twitter,index10,-index1*,+ind*" 
    }
}
'
# 禁止自动创建任何索引
curl -X PUT "localhost:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
    "persistent": {
        "action.auto_create_index": "false" 
    }
}
'
# 允许自动创建所有索引
curl -X PUT "localhost:9200/_cluster/settings" -H 'Content-Type: application/json' -d'
{
    "persistent": {
        "action.auto_create_index": "true" 
    }
}
'
```
### op_type
op_type 参数可以指定为<em>create</em>来强制执行创建操作，此操作是不存在则创建，如果指定id的文档已经存在，则此操作失败；也可以通过_create指定此值。
```sh
PUT twitter/_doc/1?op_type=create
{
    "user" : "kimchy",
    "post_date" : "2009-11-15T14:12:12",
    "message" : "trying out Elasticsearch"
}
PUT twitter/_create/1
{
    "user" : "kimchy",
    "post_date" : "2009-11-15T14:12:12",
    "message" : "trying out Elasticsearch"
}
```
### 等待活跃分片数量达到规定数
为了保证在索引文档时有足够的活跃分片数，可配置参数<em>index.write.wait_for_active_shards</em>作为索引文档时必须处于活跃状态的分片数，活跃分片数量低于此，则重试直至超时或者活跃分片数达到要求操作成功。在请求中也可指定参数 wait_for_active_shards来指定当前操作对活跃分片数的要求。
>此配置项的合法值为：all、正整数n<= number_of_replicas + 1

>此配置项能够尽可能地保证请求在足够多的分片上执行，但是也不能完全避免在某些活跃分片上操作执行失败的可能性——操作执行成功的分片数有可能小于此配置项指定的最小活跃分片数。

### 空更新检测
如果使用index API更新一个已经存在的文档，则会自动创建新的文档即使新老文档并没有差异。可以使用_update API配合 detect_noop=true来检测空操作。关于何时使用空操作并没有硬性规定。

### 超时
默认情况下，如果被分配执行index操作的主分片不可用时，操作在失败并返回响应之前会等待一分钟；可以使用timeout参数改变默认等待时长：
```sh
curl -X PUT "localhost:9200/twitter/_doc/1?timeout=5m" -H 'Content-Type: application/json' -d'
{
    "user" : "kimchy",
    "post_date" : "2009-11-15T14:12:12",
    "message" : "trying out Elasticsearch"
}
'
```

## Get API
Get API用于从索引中获取指定ID的文档，也可以使用HEAD检测指定ID的文档是否存在。
### 实时可见
默认情况下，GET API的操作是实时的，它不会被刷新频率所影响。若在执行GET API请求时，有文档已经修改但是尚未刷新(搜索可见)，GET API会立即触发一次刷新操作使得修改可见。如果要关闭实时可见，可以设置参数 readtime=false.

### 返回文档字段过滤
```sh
curl -X GET "localhost:9200/twitter/_doc/0?_source=false"
curl -X GET "localhost:9200/twitter/_doc/0?_source_include=*.id&_source_exclude=entities"
curl -X GET "localhost:9200/twitter/_doc/0?_source=*.id,retweeted"
```
### stored fileds
```sh
curl -X GET "localhost:9200/twitter/_doc/1?stored_fields=tags,counter"
```
### 获取_source
```sh
curl -X GET "localhost:9200/twitter/_doc/1/_source?_source_include=*.id&_source_exclude=entities'"
```
### Refresh
### 执行请求分片偏好设置
配置参数：<em>preference</em>
- _primary
- _local
- custom value
### routing
```sh
GET twitter/_doc/2?routing=user1
```
### 文档版本控制
可指定<em>version</em>参数值，只有文档版本等于此值时，文档才会返回。

## Delete by Query API
对满足查询条件的所有文档执行删除操作：
```sh
curl -X POST "localhost:9200/twitter/_delete_by_query" -H 'Content-Type: application/json' -d'
{
  "query": { 
    "match": {
      "message": "some message"
    }
  }
}
'
```
<em>_delete_by_query</em> 使用内部版本号删除在查询操作执行时刻索引快照中满足条件的记录；如果在快照产生和删除操作执行之间，有满足条件的文档发生了变化，则会产生版本冲突异常。

>0 不是合法的内部版本号，所以版本号是0的记录，是不能被<em>_delete_by_query</em>删除的。

>在执行查询和删除的批量操作过程中，如果某个批量操作失败，则会按照指数退避算法进行重试，最大重试次数为10次。如果在执行中发生失败且重试达到最大重试次数，则返回失败，已经执行完成的操作不会回滚。

>如果不想版本冲突引起操作失败，可以在请求中加入参数：<em>conflicts=proceed</em> 或者在请求体重加入：<em>"conflicts": "proceed"</em>
```sh
curl -X POST "localhost:9200/twitter/_doc/_delete_by_query?conflicts=proceed" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match_all": {}
  }
}
'
```

><em>_delete_by_query</em> 使用scroll，分页大小为1000,可以在请求连接中传入 scroll_size参数改变默认值。

### URL参数
<em>_delete_by_query</em>除了支持标准的参数如 pretty外，还支持 refresh,wait_for_active_shards,timeout,wait_for_completion,scroll=10m,scroll_size=5000.
- refresh 传入此参数会导致所有被当前请求涉及到的分片全部进行索引刷新，delete API 的refresh参数仅会引起收到delete请求的分片刷新。
- wait_for_completion=false  Elasticsearch会执行一些前置检查、然后加载请求、最后生成一个task(可以使用task API取消或者获取任务状态，并生成.tasks/task/${taskID}文件)
- requests_per_second  每秒执行请求的数量 默认值-1

### task API
- 使用task API查看任务状态：
```sh
curl -X GET "localhost:9200/_tasks?detailed=true&actions=*/delete/byquery"
```
- 取消任务：
```sh
curl -X POST "localhost:9200/_tasks/r1A2WoRbTwKZ516z6NEs5A:36619/_cancel"
```
- rethrottling
- slicing _delete_by_query 支持sliced scroll 来并行执行删除操作。
- slice


## Update API
### 脚本更新
### 部分文档更新
在既有脚本更新，又有部分文档更新时，部分文档更新将被忽略。
### 空更新操作
detect_noop：false
### upsert
- upsert
```sh
# 如果数据不存在，则会执行upsert 内容；如果数据已经存在，script内容将被执行。
curl -X POST "localhost:9200/test/_doc/1/_update" -H 'Content-Type: application/json' -d'
{
    "script" : {
        "source": "ctx._source.counter += params.count",
        "lang": "painless",
        "params" : {
            "count" : 4
        }
    },
    "upsert" : {
        "counter" : 1
    }
}
'
```
- scripted_upsert
```sh
# 如果不需要运行upsert部分，不论在文档存在或者不存在情况下都运行脚本部#分，则设置"scripted_upsert":true
curl -X POST "localhost:9200/sessions/session/dh3sgudg8gsrgl/_update" -H 'Content-Type: application/json' -d'
{
    "scripted_upsert":true,
    "script" : {
        "id": "my_web_session_summariser",
        "params" : {
            "pageViewEvent" : {
                "url":"foo.com/bar",
                "response":404,
                "time":"2014-01-01 12:32"
            }
        }
    },
    "upsert" : {}
}
'
```
- doc_as_upsert
```sh
curl -X POST "localhost:9200/test/_doc/1/_update" -H 'Content-Type: application/json' -d'
{
    "doc" : {
        "name" : "new_name"
    },
    "doc_as_upsert" : true
}
'
```
> update API不支持外部版本号。

## Update By Query API
与<em>Delete By Query API</em> 类似。
```sh
curl -X POST "localhost:9200/twitter/_update_by_query?conflicts=proceed"

curl -X POST "localhost:9200/twitter/_update_by_query?conflicts=proceed" -H 'Content-Type: application/json' -d'
{
  "query": { 
    "term": {
      "user": "kimchy"
    }
  }
}
'
curl -X POST "localhost:9200/twitter/_update_by_query" -H 'Content-Type: application/json' -d'
{
  "script": {
    "source": "ctx._source.likes++",
    "lang": "painless"
  },
  "query": {
    "term": {
      "user": "kimchy"
    }
  }
}
'
curl -X POST "localhost:9200/twitter,blog/_doc,post/_update_by_query?routing=1&scroll_size=100"
```
### pipeline
>支持igest Node属性,需要指定pipeline
```curl
curl -X PUT "localhost:9200/_ingest/pipeline/set-foo" -H 'Content-Type: application/json' -d'
{
  "description" : "sets foo",
  "processors" : [ {
      "set" : {
        "field": "foo",
        "value": "bar"
      }
  } ]
}
'
curl -X POST "localhost:9200/twitter/_update_by_query?pipeline=set-foo"
```
### URL 参数
- pretty
- refresh
- wait_for_active_shards
- wait_for_completion
- timeout
- scroll

### 使用Task API 
>同<em>Delete By Query API</em>类似，可以使用<em>Task API</em>

### Slicing
>支持使用<em>Sliced Scroll</em>来并行化更新操作。

### 启用新字段
对已经没有指定动态索引的字段，索引数据后无法使用此字段搜索。可以使用<em>update by query</em> 实现该字段可搜索。

## Muti Get
```sh
curl -X GET "localhost:9200/test/_doc/_mget?stored_fields=field1,field2" -H 'Content-Type: application/json' -d'
{
    "docs" : [
        {
            "_id" : "1" 
        },
        {
            "_id" : "2",
            "stored_fields" : ["field3", "field4"] 
        }
    ]
}
'
```

## Bulk API
```sh
curl -X POST "localhost:9200/_bulk" -H 'Content-Type: application/json' -d'
{ "update" : {"_id" : "1", "_type" : "_doc", "_index" : "index1", "retry_on_conflict" : 3} }
{ "doc" : {"field" : "value"} }
{ "update" : { "_id" : "0", "_type" : "_doc", "_index" : "index1", "retry_on_conflict" : 3} }
{ "script" : { "source": "ctx._source.counter += params.param1", "lang" : "painless", "params" : {"param1" : 1}}, "upsert" : {"counter" : 1}}
{ "update" : {"_id" : "2", "_type" : "_doc", "_index" : "index1", "retry_on_conflict" : 3} }
{ "doc" : {"field" : "value"}, "doc_as_upsert" : true }
{ "update" : {"_id" : "3", "_type" : "_doc", "_index" : "index1", "_source" : true} }
{ "doc" : {"field" : "value"} }
{ "update" : {"_id" : "4", "_type" : "_doc", "_index" : "index1"} }
{ "doc" : {"field" : "value"}, "_source": true}
'
```

## Reindex API
><em>Reindex API</em>不会创建索引，所以需要在执行前创建好索引。
<em>Reindex API</em>会将源索引数据拷贝到目标索引。
```sh
curl -X POST "localhost:9200/_reindex" -H 'Content-Type: application/json' -d'
{
  "size":10,
  "source": {
    "index": ["twitter", "blog"],
    "type": ["_doc", "post"],
    "sort": { "date": "desc" },
    "_source": ["user", "_doc"],
    "query": {
      "term": {
        "user": "kimchy"
      }
    }
  },
  "dest": {
    "index": "new_twitter",
    "version_type": "external"
  },
  "script": {
    "source": "if (ctx._source.foo == 'bar') {ctx._version++; ctx._source.remove('foo')}",
    "lang": "painless"
  }
}
'
```
## Term Vectors
```sh
curl -X GET "localhost:9200/twitter/_doc/1/_termvectors"
```

## ?refresh
Index/Update/Delete/Bulk API支持使用refresh参数何时当前操作对搜索可见，允许的参数值如下：
- 空 或者 true
- wait_for
- false——默认值