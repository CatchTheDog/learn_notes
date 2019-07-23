# Elasticsearch 查询
## 轻量级搜索，使用Query-string
```curl
curl -X GET "http://localhost:9200/employee/_search?q=last_name:Smith +name:(mary john) +date:>2014-09-10 +(aggregations geo)"

GET /_search?sort=date:desc&sort=_score&q=search
```
注：表达式中：+ 前缀表示必须与查询条件匹配。类似地， - 前缀表示一定不与查询条件匹配。没有 + 或者 - 的所有其他条件都是可选的——匹配的越多，文档就越相关。

- _all字段，是string类型；可以使用copy_to替换，copy_to支持更加复杂的查询
- 不要轻易使用轻量级搜索，尽量使用request body 查询API
## 查询表达式
```curl
curl -X GET "localhost:9200/megacorp/employee/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match" : {
            "last_name" : "Smith"
        }
    }
}
'
```

## 全文搜索
>Elasticsearch 默认按照相关性得分排序，即按照每个文档与查询的匹配程度排序。
```curl
curl -X GET "localhost:9200/megacorp/employee/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match" : {
            "about" : "rock climbing"
        }
    }
}
'
```
## 短语搜索
>如果需要精确匹配一系列单词或短语，就需要使用短语搜索;如下搜索仅匹配同时包含“rock"和"climbing"并且二者以短语"rock climbing"的形式紧挨着的文档。
```curl
curl -X GET "localhost:9200/megacorp/employee/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match_phrase" : {
            "about" : "rock climbing"
        }
    }
}
'
```
## 高亮搜索
>高亮搜索用于在搜索结果中提示某个文档符合查询条件的原因
```curl
curl -X GET "localhost:9200/megacorp/employee/_search" -H 'Content-Type: application/json' -d'
{
    "query" : {
        "match_phrase" : {
            "about" : "rock climbing"
        }
    },
    "highlight": {
        "fields" : {
            "about" : {}
        }
    }
}
'
```
## 聚合分析
>类似于SQL中的GROUP BY 
```curl
curl -X GET "localhost:9200/megacorp/employee/_search" -H 'Content-Type: application/json' -d'
{
  "aggs": {
    "all_interests": {
      "terms": { "field": "interests" }
    }
  }
}
'
```

## 数据变动冲突解决
- 悲观并发控制
  假定有变更冲突可能发生，所以在操作前先将其锁住，在修改后再释放锁
- 乐观并发控制
  Elasticsearch中使用此方法；假定冲突时不可能发生的，并且不会阻塞正在尝试的操作。如果源数据在读写过程中被修改，则操作失败，由程序控制如何解决冲突，例如：重试、或者使用新的数据再操作、或者报告客户。

- Elasticsearch 中每个文档都有一个_version,当文档被修改时版本号会递增；Elasticsearch 使用这个 _version 号来确保变更以正确顺序得到执行。如果旧版本的文档在新版本之后到达，它可以被简单的忽略；
   * 使用内部版本号时，文档修改请求中的版本号与文档当前版本号相同时，操作才能成功。
- 通过外部系统使用版本控制
  Elasticsearch 中通过增加version_type=external到查询字符串重用外部版本号，版本号必须是大于0的整数。
   * 外部版本号的处理方式和我们之前讨论的内部版本号的处理方式有些不同， Elasticsearch 不是检查当前 _version 和请求中指定的版本号是否相同， 而是检查当前 _version 是否 小于 指定的版本号。 如果请求成功，外部的版本号作为文档的新 _version 进行存储。
  ```curl
    curl -X PUT "localhost:9200/website/blog/2?version=5&version_type=external" -H 'Content-Type: application/json' -d'
        {
            "title": "My first external blog entry",
            "text":  "Starting to get the hang of this..."
        }
        '
  ```
- retry_on_conflict 指定发生版本冲突后重试次数

## 将多个查询请求合并成一个
>mget API将多个请求合并成一个请求，可以一次取回多个文档，提高请求速度。mget API 要求有一个 docs 数组作为参数，每个 元素包含需要检索文档的元数据， 包括 _index 、 _type 和 _id 。如果你想检索一个或者多个特定的字段，那么你可以通过 _source 参数来指定这些字段的名字：
```curl
curl -X GET "localhost:9200/_mget" -H 'Content-Type: application/json' -d'
{
   "docs" : [
      {
         "_index" : "website",
         "_type" :  "blog",
         "_id" :    2
      },
      {
         "_index" : "website",
         "_type" :  "pageviews",
         "_id" :    1,
         "_source": "views"
      }
   ]
}
'
```
- 可以省略_index _type 
- 可以传入 ids

## 批量操作
>bulk API允许在单个步骤中进行多次create/index/update/delete请求。
请求体格式如下：
```curl
{ action: { metadata }}\n
{ request body        }\n
{ action: { metadata }}\n
{ request body        }\n
...
```
>action/metadata 行指定 哪一个文档 做 什么操作 。

- action 必须是以下选项之一:

    * create
        > 如果文档不存在，那么就创建它。详情请见 创建新文档。
    * index
        > 创建一个新文档或者替换一个现有的文档。详情请见 索引文档 和 更新整个文档。
    * update
        > 部分更新一个文档。详情请见 文档的部分更新。
    * delete
        > 删除一个文档。详情请见 删除文档。
-  metadata 应该 指定被索引、创建、更新或者删除的文档的 _index 、 _type 和 _id 。

- 可以在请求路径中指定_index和_type,也可以在特定操作中覆盖此参数
- 批量请求大小：一个好的批量大小在开始处理后所占用的物理大小约为 5-15 MB。

## 深度分页
使用scroll替换深度分页，提高查询性能。
>scroll 查询特点：
- 先做初始化，然后批量拉取结果
- 查询结果是在查询初始化时的快照数据，查询开始后索引上的任何变化都不会出现在查询结果中。
- 采用字段_doc排序
- 每次查询超时时间会自动刷新，保持游标需要资源，尽可能在不用时清除游标
- 每次批量拉取的数据量，可能会超过分页参数size的限制，实际返回的文档数量最大值为：size * number_of_primary_shards
  ```curl
  GET /old_index/_search?scroll=1m 
    {
        "query": { "match_all": {}},
        "sort" : ["_doc"], 
        "size":  1000
    }
  
  GET /_search/scroll
    {
        "scroll": "1m", 
        "scroll_id" : "cXVlcnlUaGVuRmV0Y2g7NTsxMDk5NDpkUmpiR2FjOFNhNnlCM1ZDMWpWYnRROzEwOTk1OmRSamJHYWM4U2E2eUIzVkMxalZidFE7MTA5OTM6ZFJqYkdhYzhTYTZ5QjNWQzFqVmJ0UTsxMTE5MDpBVUtwN2lxc1FLZV8yRGVjWlI2QUVBOzEwOTk2OmRSamJHYWM4U2E2eUIzVkMxalZidFE7MDs="
    }
  ```


## 映射和分析
### 精确值 与 全文
- 精确值指可以精确匹配的数据  查询含义是：这个文档匹配查询条件吗？
- 全文指文本数据 查询含义是：这个文档匹配查询的程度有多高？

>为了支持全文搜索，Elasticsearch首先对文档进行分析，然后根据分析结果建立倒排索引。

### 倒排索引
>Elasticsearch 使用一种称为 倒排索引 的结构，它适用于快速的全文搜索。一个倒排索引由文档中所有不重复词的列表构成，对于其中每个词，有一个包含它的文档列表。
- 分词
- 标准化
>倒排会保存每一个词项出现过的文档总数， 在对应的文档中一个具体词项出现的总次数，词项在文档中的顺序，每个文档的长度，所有文档的平均长度，等等，这些信息会用于计算文档相关性。

>倒排索引特点：
- 倒排索引一旦生成，永远不会被修改。
- 通过增加新的补充索引来反映索引最近的数据修改，而不是直接重写整个倒排索引，每个倒排索引都会被轮流查询到——从最早的开始——查询完后再对结果进行合并。
- Luncene引入了按段搜索的概念；每个段本身都是一个倒排索引，索引在Luncene中除了表示所有段(倒排索引)的集合外，还包括所有倒排索引的提交点——一个列出了所有已知段的文件。
- 逐段搜索流程：
   * 新文档被收集到内存索引缓存
   * 不时地, 缓存被 提交 ：
        * 一个新的段--一个追加的倒排索引--被写入磁盘。
        * 一个新的包含新段名字的 提交点 被写入磁盘。
        * 磁盘进行 同步 — 所有在文件系统缓存中等待的写入都刷新到磁盘，以确保它们被写入物理文件。
   * 新的段被开启，让它包含的文档可见以被搜索。
   * 内存缓存被清空，等待接收新的文档。
- refresh 写入和打开新段的操作被称为刷新；默认情况下每个分片会每秒自动刷新一次。可在写大量文档前先关闭自动刷新或者降低刷新频率，然后在写完之后再恢复刷新频率。
```curl
PUT /my_logs
{
  "settings": {
    "refresh_interval": "30s" 
  }
}
# 关闭刷新
PUT /my_logs/_settings
{ "refresh_interval": -1 } 
```
- flush 这个执行一个提交并且截断 translog 的行为在 Elasticsearch 被称作一次 flush 。 分片每30分钟被自动刷新（flush），或者在 translog 太大的时候也会刷新。在重启节点或关闭索引之前执行 flush 有益于你的索引。当 Elasticsearch 尝试恢复或重新打开一个索引， 它需要重放 translog 中所有的操作，所以如果日志越短，恢复越快。
```curl
POST /blogs/_flush 
POST /_flush?wait_for_ongoing 
```
- translog 默认 translog 是每 5 秒被 fsync 刷新到硬盘， 或者在每次写请求完成之后执行(e.g. index, delete, update, bulk)。这个过程在主分片和复制分片都会发生。最终， 基本上，这意味着在整个请求被 fsync 到主分片和复制分片的translog之前，你的客户端不会得到一个 200 OK 响应。
  - 但是对于一些大容量的偶尔丢失几秒数据问题也并不严重的集群，使用异步的 fsync 还是比较有益的。比如，写入的数据被缓存到内存中，再每5秒执行一次 fsync 。这个选项可以针对索引单独设置，并且可以动态进行修改。如果你决定使用异步 translog 的话，你需要 保证 在发生crash时，丢失掉 sync_interval 时间段的数据也无所谓。
    ```curl
    PUT /my_index/_settings
    {
        "index.translog.durability": "async",
        "index.translog.sync_interval": "5s"
    }
    ```
  - 如果你不确定这个行为的后果，最好是使用默认的参数（ "index.translog.durability": "request" ）来避免数据丢失。
- optimize API用于强制将索引段数目合并到max_sum_segments参数指定的大小以提高搜索性能。
  ```curl
  POST /logstash-2014-10/_optimize?max_num_segments=1 
  ```
### 分析与分析器
分析包含以下过程：
- 首先，将一块文本分成适合于倒排索引的独立的词条
- 之后，将这些词条统一化为标准格式以提高它们的“可搜索性”，或者 recall

>分析器执行上面的工作，分析器封装以下三个功能：
- 字符过滤器
>首先，字符串按顺序通过每个 字符过滤器 。他们的任务是在分词前整理字符串。一个字符过滤器可以用来去掉HTML，或者将 & 转化成 `and`。一个分析器可能有0个或者多个字符过滤器。
- 分词器
>其次，字符串被 分词器 分为单个的词条。一个简单的分词器遇到空格和标点的时候，可能会将文本拆分成词条。一个分析器 必须 有一个唯一的分词器。
- Token过滤器
>最后，词条按顺序通过每个 token 过滤器 。这个过程可能会改变词条（例如，小写化 Quick ），删除词条（例如， 像 a`， `and`， `the 等无用词），或者增加词条（例如，像 jump 和 leap 这种同义词）。

Elasticsearch内置了一些分析器，也可以自定义组合字符过滤器、分词器和Token过滤器以形成定制分析器。

内置分析器有：
- 标准分析器 Elasticsearch默认分析器，根据Unicode联盟定义的单词编辑划分文本，删除绝大部分标调，最后将词条小写
- 简单分析器 简单分析器在任何不是字母的地方分隔文本，将词条小写。
- 空格分析器 空格分析器在空格的地方划分文本。
- 语言分析器 基于特定语言提取词条并生成词干

>在对全文字段进行搜索时，查询字符串将会通过与该字段相同的分析过程，然后使用分析后的结果进行搜索。
>查询精确字段时，查询字符串不会被分析，会使用精确值进行匹配。


>可以使用如下方式测试分析器：
```curl
curl -X GET "localhost:9200/_analyze" -H 'Content-Type: application/json' -d'
{
  "analyzer": "standard",
  "text": "Text to analyze"
}
'
curl -X GET "localhost:9200/gb/_analyze" -H 'Content-Type: application/json' -d'
{
  "field": "tweet",
  "text": "Black-cats" 
}
'
```
>默认情况下，Elasticsearch检测到字符字段时，会自动使用标准分析器对该字段进行分析，要改变默认情况，需要在映射中为字段指定类型和分析器。

### 映射
Elasticsearch支持如下简单域类型：
- 字符串: string
- 整数 : byte, short, integer, long
- 浮点数: float, double
- 布尔型: boolean
- 日期: date
Elasticsearch 支持的复杂核心域类型如下：
null值，数组和对象

当在索引一个包含新字段的文档时，Elasticsearch会使用动态映射，自动通过JSON基本数据类型尝试猜测数据类型。
|JSON type| Elasticsearch type|
|--|--|
|布尔型: true 或者 false|boolean|
|整数: 123|long|
|浮点数: 123.45|double|
|字符串，有效日期: 2014-09-15|date|
|字符串: foo bar|string|

- 查看映射：
```curl
curl -X GET "http://localhost:9200/gb/_mapping/tweet"
```

- 动态映射可能会生成错误的映射数据类型，可自定义配置映射，支持参数如下：
    * type
    * index 控制怎样生成字符串
        - analyzed 首先分析字符串，然后索引它。换句话说，以全文索引这个域。
        - not_analyzed 索引这个域，所以它能够被搜索，但索引的是精确值。不会对它进行分析。
        - no 不索引这个域。这个域不会被搜索到。
    * analyzer 指定在搜索和索引时使用的分析器
    * fields 对单一字段按照多种方式进行分析
    * 已经存在的映射的字段的映射不能被修改，可以为映射新增字段映射
  
- 复杂核心域类型存储
    * 数组在Elasticsearch 被存储为多值字段，可以搜索但是是无序的。
    * 空值 Luncene中是不能存储null值的。所以存在null值的域为空域：
     以下三种域被认为是空的，它们不会被索引：
     ```
     "null_value":               null,
     "empty_array":              [],
     "array_with_null_value":    [ null ]
     ```
    * 对象 type为object,其内部还可以包含内部对象的映射：
    ```JSON
    {
    "gb": {
        "tweet": { 
        "properties": {
            "tweet":            { "type": "string" },
            "user": { 
            "type":             "object",
            "properties": {
                "id":           { "type": "string" },
                "gender":       { "type": "string" },
                "age":          { "type": "long"   },
                "name":   { 
                "type":         "object",
                "properties": {
                    "full":     { "type": "string" },
                    "first":    { "type": "string" },
                    "last":     { "type": "string" }
                }
                }
            }
            }
        }
        }
    }
    }
    ```
    Luncene不理解内部对象，将会把内部对象转换为如下进行存储：
    ```json
    {
        "tweet":            [elasticsearch, flexible, very],
        "user.id":          [@johnsmith],
        "user.gender":      [male],
        "user.age":         [26],
        "user.name.full":   [john, smith],
        "user.name.first":  [john],
        "user.name.last":   [smith]
    }
    ```
    >内部域 可以通过名称引用（例如， first ）。为了区分同名的两个域，我们可以使用全 路径 （例如， user.name.first ） 或 type 名加路径（ tweet.user.name.first ）。
    * 内部对象数组会被扁平化处理，如：
    ```JSON
    {
        "followers": [
            { "age": 35, "name": "Mary White"},
            { "age": 26, "name": "Alex Jones"},
            { "age": 19, "name": "Lisa Smith"}
        ]
    }
    ```
    将会被存储为：
    ```json
    {
        "followers.age":    [19, 26, 35],
        "followers.name":   [alex, jones, lisa, smith, mary, white]
    }
    ```
>注：在扁平化处理后，文档内部属性的关联性消失了。如果需要在查询时使用内部对象属性的关联性，需要使用嵌套对象。


## 请求体查询
Query DSL:查询表达式是Elasticsearch使用的一种查询语言。其结构如下：
```json
{
    QUERY_NAME: {
        ARGUMENT: VALUE,
        ARGUMENT: VALUE,...
    }
}

# 针对特定字段时如下
{
    QUERY_NAME: {
        FIELD_NAME: {
            ARGUMENT: VALUE,
            ARGUMENT: VALUE,...
        }
    }
}
```
### 合并查询语句
>一条复合语句可以将多条语句 — 叶子语句和其它复合语句 — 合并成一个单一的查询语句
```json
{
    "bool": {
        "must":     { "match": { "tweet": "elasticsearch" }},
        "must_not": { "match": { "name":  "mary" }},
        "should":   { "match": { "tweet": "full text" }},
        "filter":   { "range": { "age" : { "gt" : 30 }} }
    }
}
```

### 查询与过滤
DSL可以在以下两种情况下使用：
- 过滤 filtering context 查询不评分，检测文档是否与查询匹配，只有YES或者NO，查询速度较快，结果会缓存在内存中；过滤的目标是减少哪些需要通过评分查询进行检查的文档
- 查询 query context 查询会对文档根据与查询的相关性评分，评分(_score)反映文档与查询的匹配程度有多好,并且按照_score对文档进行评分，查询较慢

>使用查询和过滤的原则：除需要使用相关性的搜索外，都使用过滤

#### 重要的查询
- match_all 匹配所有文档，在没有指定查询方式时，它是默认的查询。经常与filter结合使用，所有文档评分均为1。
- match 在任何字段进行全文搜索或者精确查询时都可使用：在全文字段上使用match,查询条件中的字符串会被分析然后再与文档进行匹配；若在精确值字段上使用match，则会进行精确值查询。
- multi_match 可以在多个字段上执行相同的match查询
- range 查询找出那些落在指定区间内的数字或者时间
- term 用于匹配精确值，这些精确值可能是数字、时间、布尔或者那些 not_analyzed 的字符串;term 查询对于输入的文本不 分析 ，所以它将给定的值进行精确查询。
- terms terms 查询和 term 查询一样，但它允许你指定多值进行匹配。如果这个字段包含了指定值中的任何一个值，那么这个文档满足条件；和 term 查询一样，terms 查询对于输入的文本不分析。它查询那些精确匹配的值（包括在大小写、重音、空格等方面的差异）。
- exists 用于查找指定字段有值的文档，与SQL 中的NOT IS_NULL()类似。
#### 组合多个查询
可使用bool查询组合多查询，它可以将以下查询组合在一起：
- must
文档 必须 匹配这些条件才能被包含进来。
- must_not
文档 必须不 匹配这些条件才能被包含进来。
- should
如果满足这些语句中的任意语句，将增加 _score ，否则，无任何影响。它们主要用于修正每个文档的相关性得分。
- filter
必须 匹配，但它以不评分、过滤模式来进行。这些语句对评分没有贡献，只是根据过滤标准来排除或包含文档。

>相关性得分：
每一个子查询都独自计算文档的相关性得分，bool查询将子查询的得分进行合并并返回一个代表整个查询的得分。
```json
{
    "bool": {
        "must":     { "match": { "title": "how to make millions" }},
        "must_not": { "match": { "tag":   "spam" }},
        "should": [
            { "match": { "tag": "starred" }},
            { "range": { "date": { "gte": "2014-01-01" }}}
        ]
    }
}
# 如果没有must子查询，则需要至少匹配一个should语句，但是如果存在至少一#个must子查询，则should语句的匹配没有要求(匹配越多得分越高)
```
>使用filter包含查询后，该查询不参与评分，只起到过滤作用：
```json
{
    "bool": {
        "must":     { "match": { "title": "how to make millions" }},
        "must_not": { "match": { "tag":   "spam" }},
        "should": [
            { "match": { "tag": "starred" }}
        ],
        "filter": {
          "bool": { 
              "must": [
                  { "range": { "date": { "gte": "2014-01-01" }}},
                  { "range": { "price": { "lte": 29.99 }}}
              ],
              "must_not": [
                  { "term": { "category": "ebooks" }}
              ]
          }
        }
    }
}
```
- constant_score 将一个不变的常量评分应用于所有匹配的文档，它被经常用于只需要执行一个filter而没有其他查询(如评分查询)的情况，用于提高查询简洁性和清晰度：
```json
{
    "constant_score":   {
        "filter": {
            "term": { "category": "ebooks" } 
        }
    }
}
```

#### 验证查询
使用validate-query API验证查询的合法性：
```JSON
GET /gb/tweet/_validate/query?explain
{
   "query": {
      "tweet" : {
         "match" : "really powerful"
      }
   }
}
```
#### 多值字段排序：
```json
"sort": {
    "dates": {
        "order": "asc",
        "mode":  "min"  # 模式 min/max/avg/sum
    }
}
```

## 相关性
每个文档都有相关性评分，用一个正浮点数_score表示，_score越大，评分越高，相关性越好。

>评分的计算方式取决于查询类型,通常所说的relevance是用来计算全文本字段的值对于全文本检索词的相似度的算法。

>Elasticsearch 的相似度算法 被定义为检索词频率/反向文档频率 ，TF/IDF,包含以下内容：
- 检索词频率
检索词在该字段出现的频率？出现频率越高，相关性也越高。 字段中出现过 5 次要比只出现过 1 次的相关性高。
- 反向文档频率
每个检索词在索引中出现的频率？频率越高，相关性越低。检索词出现在多数文档中会比出现在少数文档中的权重更低。
- 字段长度准则
字段的长度是多少？长度越长，相关性越低。 检索词出现在一个短的 title 要比同样的词出现在一个长的 content 字段权重更大。

>可使用explain 查询语句查看评分依据：
```JSON
GET /_search?explain&format=yaml
{
   "query"   : { "match" : { "tweet" : "honeymoon" }}
}
```

>查询结果解释：
- 检索词频率:
检索词 `honeymoon` 在这个文档的 `tweet` 字段中的出现次数。
- 反向文档频率:
检索词 `honeymoon` 在索引上所有文档的 `tweet` 字段中出现的次数。
- 字段长度准则:
在这个文档中， `tweet` 字段内容的长度 -- 内容越长，值越小。

>可使用explain检查某一文档被匹配到的原因：
```curl
curl -X GET "localhost:9200/us/tweet/12/_explain" -H 'Content-Type: application/json' -d'
{
   "query" : {
      "bool" : {
         "filter" : { "term" :  { "user_id" : 2           }},
         "must" :  { "match" : { "tweet" :   "honeymoon" }}
      }
   }
}
'
```

## Doc Values
倒排索引在文档检索时很有用，但是在文档排序时不理想；
在Elasticsearch 中，Doc Values就是一种列式存储结构，默认情况下每个字段的Doc Values都是激活的，Doc Values 在字段索引时创建。

>Elasticsearch 中Doc Values 的应用场景如下：
- 对一个字段进行排序
- 对一个字段进行聚合
- 某些过滤，比如地理位置过滤
- 某些与字段相关的脚本计算

## 分布式检索执行过程
- query_then_fetch
  - 查询阶段：
     - 协调节点在收到查询请求后，将请求转发到所有相关分片的主分片或者副分片
    - 每个主分片或者副分片在本地执行查询，然后生成满足查询条件的记录优先队列(大小为 from+size)，并向协调节点返回优先队列中的所有文档的ID和排序值。
    - 协调节点将这些分片级的结果合并到自己的有序优先队列里
  - 取回阶段：
    - 协调节点辨别出哪些文档需要被取回并向相关的分片提交多个 GET 请求
    - 每个分片加载并 丰富 文档，如果有需要的话，接着返回文档给协调节点
    - 一旦所有的文档都被取回了，协调节点返回结果给客户端
- 影响查询过程的参数
  - preference 允许用来控制由哪些分片或节点来处理请求，它接受像 _primary, _primary_first, _local, _only_node:xyz, _prefer_node:xyz, 和 _shards:2,3 这样的值。
    - 让同一用户使用同一个值，可以避免  bouncing results 问题。
  - 超时问题 单一分片慢会影响整个查询性能，timeout参数指定分片允许处理数据的最大时间，若在超时范围内未处理完数据，则分片可以返回部分数据或者空结果。
  - routing
  - 搜索类型  默认搜索类型是 query_then_fetch;在某些情况下，可能需要设置搜索类型为dfs_query_then_fetch来改善相关精确度：
  ```curl
  GET /_search?search_type=dfs_query_then_fetch
  ```

## 索引设置
### 创建索引
```JSON
PUT /my_index
{
    "settings": { ... any settings ... },
    "mappings": {
        "type_one": { ... any mappings ... },
        "type_two": { ... any mappings ... },
        ...
    }
}
```
可以通过在config/elasticsearch.yml关闭索引自动创建：
```yaml
action.auto_create_index: false
```
可以通过配置项关闭通过指定_all或通配符删除索引：
```yaml
action.destructive_requires_name: true
```

### 索引设置
- number_of_shards 每个索引的主分片数，默认值是5，在索引创建后不可修改
- number_of_replicas 每个主分片的副本数，默认值1 ，可随时修改

### 配置分析器
```curl
curl -X PUT "localhost:9200/spanish_docs" -H 'Content-Type: application/json' -d'
{
    "settings": {
        "analysis": {
            "analyzer": {
                "es_std": {
                    "type":      "standard",
                    "stopwords": "_spanish_"
                }
            }
        }
    }
}
'
```

### 创建自定义分析器
```curl
PUT /my_index
{
    "settings": {
        "analysis": {
            "char_filter": { ... custom character filters ... },
            "tokenizer":   { ...    custom tokenizers     ... },
            "filter":      { ...   custom token filters   ... },
            "analyzer":    { ...    custom analyzers      ... }
        }
    }
}

curl -X PUT "localhost:9200/my_index" -H 'Content-Type: application/json' -d'
{
    "settings": {
        "analysis": {
            "char_filter": {
                "&_to_and": {
                    "type":       "mapping",
                    "mappings": [ "&=> and "]
            }},
            "filter": {
                "my_stopwords": {
                    "type":       "stop",
                    "stopwords": [ "the", "a" ]
            }},
            "analyzer": {
                "my_analyzer": {
                    "type":         "custom",
                    "char_filter":  [ "html_strip", "&_to_and" ],
                    "tokenizer":    "standard",
                    "filter":       [ "lowercase", "my_stopwords" ]
            }}
}}}
'
```

### 动态映射
```curl
curl -X PUT "localhost:9200/my_index" -H 'Content-Type: application/json' -d'
{
    "mappings": {
        "my_type": {
            "dynamic_templates": [
                { "es": {
                      "match":              "*_es", 
                      "match_mapping_type": "string",
                      "mapping": {
                          "type":           "string",
                          "analyzer":       "spanish"
                      }
                }},
                { "en": {
                      "match":              "*", 
                      "match_mapping_type": "string",
                      "mapping": {
                          "type":           "string",
                          "analyzer":       "english"
                      }
                }}
            ]
}}}
'
```

```curl
# 指定默认映射
curl -X PUT "localhost:9200/my_index" -H 'Content-Type: application/json' -d'
{
    "mappings": {
        "_default_": {
            "_all": { "enabled":  false }
        },
        "blog": {
            "_all": { "enabled":  true  }
        }
    }
}
'
```

### 重新索引数据
使用Reindex API