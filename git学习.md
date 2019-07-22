# git学习笔记
## 关于git
git 是由Linus开发的分布式版本控制系统.

## 分布式版本控制系统与集中式版本控制系统
### 集中式版本控制系统
> 版本库集中存放在中央服务器，各个开发者需要先从中央服务器获取最新版本，完成开发后将代码提交到中央服务器。

> 缺点：
- 必须联网
- 提交/拉取速度受限于网速

### 分布式版本控制系统
>分布式版本控制系统没有中央服务器，各个开发者本地都有一个完整的版本库，各个开发者提交代码时需要将本地文件的修改推送给其他开发者。

>优点
- 无需联网即可运行
- 安全性更好

## 安装git
>自行百度

## 配置git
在命令行中执行如下命令：
```git
git config --global user.name "CatchTheDog"
git config --global user.email "1924528451@qq.com"
```

## 创建版本库
>版本库(repository),是一个被git管理的目录，此目录下所有文件的新增、删除和修改，git都能跟踪。

- 在本地创建一空目录
- 在命令行中，进入该目录，执行如下命令：
```git
git init
```
- 在该目录下创建文件 readme.txt
- 在命令行中执行如下命令
```git
git add readme.txt
```
- 执行如下命令将文件提交到仓库
```git
git commit -m "wrote a readme file."
```

## 已提交文件修改
修改readme.txt文件内容

- 执行命令查看当前仓库状态
```git
git status
```
- 执行如下命令查看文件未提交内容与仓库差异
```git
git diff readme.txt
```
- 执行如下命令添加提交内容并提交
```git
git add readme.txt
git commit -m "add distributed"
```

## 版本回退
- 使用如下命令查看文件提交历史
```git
git log --pretty=oneline
```
- 在git中，用HEAD 表示当前版本，用HEAD^表示当前版本上一版本，用HEAD^^表示当前版本上上一个版本，...，用HEAD~100表示当前版本往前100个版本。回退版本可以使用git reset命令；使用如下命令将readme.txt回退到 add distributed 版本：
```git
git reset --hard HEAD^
```
- 版本回退后无法看到此版本之后的提交，如果想要回到此版本后提交的版本，则需要使用版本号进行前推，使用如下命令回到 append GPL 版本：
```git
git reset --hard f1058810
```
- 可以使用上述命令回到 append GPL 的前提是我们知道 append GPL 版本的版本号(前几位)，如果忘记了append GPL 版本号，则需要使用如下命令查看此前每次版本变更操作记录获取版本号，以便重新回到‘未来版本’：
```git
git reflog
```

## 工作区和暂存区
### 工作区 working directory
> 工作区就是存放 readme.txt 的目录 learngit

### 版本库 repository
> 工作区中有一个隐藏目录.git,是Git的版本库。版本库中存放了暂存区(stage/index),还有git自动创建的分支master,以及指向master的一个指针HEAD.
> git add 命令就是将工作区的文件修改添加到暂存区
> git commit 命令就是把暂存区的所有内容提交到当前分支

## 撤销修改
## 当前修改未放入暂存区
>在修改文件后，可以使用如下命令撤销本地修改：
```git
git checkout -- readme.txt
```
>上述命令执行的操作是：把readme.txt 文件在工作区的修改全部撤销，有以下两种情况：
- readme.txt自修改后还没有被放到暂存区，撤销修改后就回到和版本库的最新状态
- readme.txt在添加到了暂存区然后做了修改(当前修改尚未提交到暂存区)，撤销修改后就回到添加到暂存区的状态
>命令中 -- 很重要，没有此参数，则此命令为切换分支命令

### 当前修改已放入暂存区
>在修改文件后，将修改提交到了暂存区，可使用如下命令可以将暂存区的修改撤销掉，重新放回工作区：
```git
git reset HEAD readme.txt
```
>git reset 命令 既可以回退版本，也可以将暂存区的修改回退到工作区
>上述命令中HEAD表示最新版本

>撤销暂存区修改并放回工作区后，可以再修改，或者使用git checkout -- readme.txt 命令撤销工作区修改

## 删除文件
- 使用rm 命令删除文件
- 若要从版本库中删除该文件，执行如下命令即可：
```git
git rm test.txt
git commit -m "remove test.txt"
```
- 若删错了，想要恢复文件，则执行如下命令：
```git
git checkout -- test.txt
```
> git rm 用于删除一个文件，如果一个文件已经被提交到版本库，删除该文件后，可以恢复文件到最新版本，但是会丢失最近一次提交后修改的内容。

## 远程仓库
>由于本地Git仓库和远程仓库(如github)之间使用SSH传输，所以要配置SSH:
- 创建SSH key
> 创建SSH Key。在用户主目录下，看看有没有.ssh目录，如果有，再看看这个目录下有没有id_rsa和id_rsa.pub这两个文件，如果已经有了，可直接跳到下一步。如果没有，打开Shell（Windows下打开Git Bash），创建SSH Key：
```git
 ssh-keygen -t rsa -C "youremail@example.com"
```
- 登陆GitHub，打开“Account settings”，“SSH Keys”页面：
然后，点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容,点“Add Key”.
- 登陆GitHub，然后，在右上角找到“Create a new repo”按钮
- 在Repository name填入learngit，其他保持默认设置，点击“Create repository”按钮
- 在本地仓库下运行如下命令将本地仓库与远程仓库关联：
```git
git remote add origin https://github.com/CatchTheDog/learngit.git
```
- 执行如下命令将本地库内容推送到远程库上
```git
git push -u origin master
```
- 此后只要本地做了修改，可以使用如下命令提交变更：
```git
git push origin master
```
- 可以使用如下命令将远程库clone到本地：
```git
git clone https://github.com/CatchTheDog/learngit.git
```

## 分支管理
- 分支：Git把每次提交按照提交时间组织起来，就构成了分支
- 在Git中，主分支称为master分支
- HEAD指向分支，分支指向提交
- 使用如下命令创建分支：
```git
git checkout -b dev
```
> -b 参数 表示创建并切换分支，相当于如下两条命令执行：
```git
git branch dev #创建分支
git checkout dev #切换分支
```
- 用如下命令查看分支：
```git
git branch
```
- 在分支修改文件内容并提交后，可用如下命令将分支修改合并到master
```git
git merge dev
```
> git merge 命令用于将指定分支的修改合并到当前分支
- 将分支修改合并到主分支后，可以使用如下命令删除分支：
```git
git branch -d dev
```
- 在分支修改文件后提交，在主分支修改文件后提交；然后将分支修改合并到主分支时(执行命令 git merge feature1)，发生冲突，无法直接合并;可以使用git status命令查看冲突情况；使用vim查看发生冲突的问题如下：

Git is a distributed version control system.</br>
Git is free software distributed under the GPL.</br>
Git has a mutable index called stage.</br>
Git tracks changes.</br>
Git tracks changes of files.</br>
Creating a new branch is quick.</br>
<<<<<<< HEAD
Creating a new branch is quick & simple.</br>
=======</br>
Creating a new branch is quick AND simple.</br>
>>>>>>> feature1</br>

> Git用<<<<<<<，=======，>>>>>>>标记出不同分支的内容;编辑readme.txt 为如下内容后保存：

Git is a distributed version control system.</br>
Git is free software distributed under the GPL.</br>
Git has a mutable index called stage.</br>
Git tracks changes.</br>
Git tracks changes of files.</br>
Creating a new branch is quick.</br>
Creating a new branch is quick and simple.</br>

提交修改，冲突即可解决。然后删除feature1分支即可

- 使用如下命令可以查看分支合并情况：
```git
git log --graph --pretty=oneline --abbrev-commit
```
结果如下：
*   0329bf9 (HEAD -> master) conflict fixed
|\
| * ce09296 (feature1) AND simple
* | f605ca6 & simple
|/  
* dd86f9c branch dev test
* 340f775 (origin/master) remove test.txt
* 119958b add test.txt
* a9a7b39 git tracks changes of files
* 94e62aa git tracks changes
* 9137b6c understand how stage works
* f105881 append GPL
* 00c202b add distributed
* c9f8283 wrote a readme file.

### 分支管理策略
>默认情况下，如果可以，Git会使用Fast forward模式，但在此模式下，删除分支后，会丢掉分支信息。

>使用如下命令可以禁用 Fast forward，Git会在merge时生成一个新的commit，就可以保留分支信息：
```git
git merge --no-ff -m "merge with no-ff" dev
```
>实际应用用，建议分支管理原则如下：
- master分支应该稳定，仅用来发布新版本，不要在master分支上做开发
- 在dev分支上开发，开发完成后发布前再将dev分支修改合并到master分支

### bug分支
>临时需要创建分支修改bug但是本地开发任务尚未完成(若直接创建分支，分支内会包含尚在开发状态的修改)，将所有新增文件都使用git add 放入暂存区，然后可以使用如下命令暂存当前开发分支修改:
```git
git stash
```
> 然后在主分支创建分支修复bug,合并bug修复分支到主分支后，删除bug修复分支
> 
> 切换到dev分支，执行如下命令查看暂存的修改列表：
```git
git stash list
```

>可以有如下两种方式恢复暂存内容：
- 使用如下命令，但是暂存内容不会删除，使用 git stash list 命令仍然会看到：
```git
git stash apply stash@{0} #此命令可多次执行
```
- 使用如下命令，暂存区内容会删除：
```git
git stash drop stash@{0}
```

### feature 分支
>开发新功能时可新建分支开发，如果需要删除一个没有被合并过的分支，可通过如下命令完成：
```git
git branch -D <name>
```

### 远程推送
>当从远程仓库克隆时，实际上Git自动把本地的master分支和远程的master分支对应起来了，并且，远程仓库的默认名称是origin。
- 使用如下命令查看 远程库信息：
```git
git remote -v
```
- 若需要把本地分支推送到远程分支，使用如下命令：
```git
git push origin dev
```
- 在使用命令 git clone 从远程仓库获取资源后，默认情况下只能看到master分支，可使用如下命令切换到dev分支：
```git
git checkout -b dev origin/dev
```
- 若在推送代码时提示代码已被其他人修改，存在冲突，无法推送，可使用如下命令解决：
  * 指定本地dev分支与远程库dev分支连接
    ```git
    git branch --set-upstream-to=origin/dev dev
    ```
  * 拉取远程库中最新版本
    ```git
    git pull
    ```
  * 手动合并冲突
  * 提交，再次push
    ```git
    git commit -m "fix env conflict"
    git push origin dev
    ```

  所以在多人协作开发时，工作模式如下：
  - 首先，可以试图用git push origin <branch-name>推送自己的修改；
  - 如果推送失败，则因为远程分支比你的本地更新，需要先用git pull试图合并；
  - 如果合并有冲突，则解决冲突，并在本地提交；
  - 没有冲突或者解决掉冲突后，再用git push origin <branch-name>推送就能成功！

如果git pull提示no tracking information，则说明本地分支和远程分支的链接关系没有创建，用命令git branch --set-upstream-to <branch-name> origin/<branch-name>。

## rebase 变基
多人在同一分支协作开发时，分支图形看起来比较凌乱，使用rebase可以将分支图形变成一条干净的直线：
```git
git rebase
```

## 标签
- 标签用于标识分支，git中tag指向分支的某个提交。
- 使用如下命令为分支打标签：
```git
# 1.切换到需要打标签的分支上
# 2.执行如下命令
git tag tag_name
```
- 默认情况下标签是打在当前分支的最新提交上
- 可以使用如下命令对指定版本打标签
```git
git tag -a v0.1 -m "version 0.1 released" 1094adb
```
- 可以使用如下命令查看当前分支上的所有标签
```git
git tag
```
- 可以使用如下命令查看指定标签信息
```git
git show 测试1
```
- 如果标签打错了，可以使用如下命令删除
```git
git tag -d tagName
```
- 可以使用如下命令推送标签到远程
```git
git push origin tagName
```
- 可以使用如下命令推送所有标签到远程
```git
git push origin --tags
```

- 如果要删除已经推送远程的标签，可以使用如下命令:
```git
git tag -d tagName # 先删除本地标签
git push origin :refs/tags/tagName  # 然后删除远程标签
```
### 使用GitHub
- fork 开源项目到自己账号下
- git clone 自己账号下fork过来的开源项目(必须clone自己账号的项目，不然没有权限推送)
- 在本地修改bug或者开发，然后推送到远程；
- 发起一个pull request
- 等待对方拒绝或者接受pull request

### 使用码云
- 自己百度

## 自定义git
>git有很多配置项，如命令颜色、忽略一些文件(编写.gitignore文件，使用git check-ignore 命令检查文件是否被添加到忽略文件中)、为命令配置别名(类似快捷命令)
- 当前文件夹 git 配置项内容放在.git/config文件中，如别名
- 全局git 配置项  放在用户主目录下的.gitconfig文件中

### 搭建git服务器
- 自己百度
