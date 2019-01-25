# git 常用操作学习

## git 编辑操作

    命令                                  意义

    git add filename                     保存当前修改
    git commit                           提交当前修改
    git commit -m "description"          提交当前修改，并附加“description”说明

## git分支操作

    命令                                  意义

    git status                           查看当前分支状态
    git branch                           显示本地所有分支以及当前所在分支（以*号标出）
    git checkout -b newBranch            创建并转移到新的分支
    git checkout branch1                 转移到已经存在的branch1分支
    git branch -d branch1                删除branch1分支（需要在master分之下）
    git branch -a                        显示远程仓库分支（红色标出）
    git push origin --delete branch1     删除远程分支
    git fetch                            从远程对应分支上更新本地没有的内容
    git pull url                         从url远程仓库上拉取本地没有的内容（更新）
    git merge branch1                    将branch1合并到当前分支
    git push origin master               将本地master分支推送到远程master分支
    git push                             将当前分支推送到远程对应分支（远程有对应分支）
    git push origin thisBranch           将当前分支推送到远程thisBranch分支，如果没有，则创建
    git log                              查看本项目的修改日志

TODO：Continue...

## PR提交方法以及常见的问题

这里讲述一般的情况，提交PR的流程。首先我们假定你想对别人的开源项目做一份贡献，提交自己的修改建议，而

你并不是这个项目发起人指定的合作者，也就是说你对项目没有可写权限.

### 在线编辑方式

1. Fork他人项目.  

  要想编辑别人的项目，首先需要将别人仓库中的文件拷贝到自己的仓库中去，这里用到的一个动作就是Fork.

  这时候就能看到自己的仓库中有了相同的一份拷贝.

2. 新建分支

  Fork他人项目之后，自己仓库中默认处在主分支 master 上. 一般来说，为了不影响主分支内容，我们需要新建

  分支对项目文件进行编辑，新分支名字最好能体现出修改意图,此处我们暂且取名 branch1 . 实际上，此时新

  的分支相当于是 master 分支的一个拷贝, 与master分支完全相同，只有当你做了修改之后两者才会有差异.

3. 修改分支文件

  此时可以任意选择项目中的文件进行修改，包括进行编辑、添加、删除文件.

  可以 上传文件到工作目录下，或者删除文件，这也是一种修改.

4. 提交修改

  修改完毕之后，可以将修改提交到自己相应的分支中去，一般来说，在那个分支修改的就提交到哪个分支，例如

  我们这里新建的branch1分支，我们就把修改提交到branch1中去. 当然，你也可以在提交时选择提交到新的分支.

  需要注意的是，每一次提交都会有提交记录，包括做了哪些修改以及修改的描述等等. 在提交修改时我们需要对

  本次提交有一个简短的描述，以便以后看到后可以很快知道此次提交所修改的内容，而不用去看修改的文件本身。

5. 将合并请求推送给项目拥有者

  在提交完所有修改到自己的分支中后，如果我们对自己的修改满意（不满意也能推送），就可以把修改后的分支

  推送给项目拥有者，发出合并请求pull-request. 在发出合并请求时，应该告诉源作者自己修改的意图以及修改

  的好处.

### 本地编辑方式

  本地可用git对仓库文件进行编辑大致流程如下：

1. fork 他人分支

  在GitHub上将别人的仓库fork到自己的远程仓库中去；

2. 克隆仓库到本地

  将自己远程仓库克隆到本地，输入 *git clone url*, 例如我将自己的远程仓库HelloWorld克隆到本地：  
    *git clone https://github.com/robin18532/HelloWorld*

3. 创建并切换到新的分支

  进入本地仓库目录，此处进入HelloWorld目录, *cd HelloWorld* ，创建新的分支 *git branch newBranch*

  并切换到新的分支 *git checkout newBranch*

4. 修改项目

  用 *ls* 指令查看文件列表找到所要编辑的文件打开编辑，或者添加、删除文件等等.

5. 保存修改

  修改完成之后，保存. 然后对文件修改进行进行保存 *git add filename*,注意，每次修改之后都要

  输入这条指令对文件修改进行跟踪. 如果不确定是否已经添加，可用 *git status* 查看当前分支中的文件修改

  状态, 是否有未保存的修改，是否有保存了但是没有提交的修改.

6. 提交修改

  修改完成之后，输入指令 *git commit* 将修改提交到当前分支，然后系统会提示输入修改描述. 完成之后

  即是提交成功. 也可以输入指令 *git commit -m "description of the change"* 来提交修改.

7. 合并分支

  切换到自己的主分支，将修改分支合并进来:   

    git checkout master  
    git merge newBranch

8. 推送到远程仓库

  修改提交之后，需要将自己修改的分支推送到远程仓库，注意，这里只能推送到自己有写入权限的仓库，比如

  这里我只能推送到自己仓库（自己仓库中fork别人项目也有写入权限），输入指令 *git push origin master*

9. 向源作者发出合并请求

  登陆自己的GitHub账户，可以看到自己新的推送，如果你希望自己所做的修改能够让项目源作者接受，

  可以给源作者发送合并请求，此处与上面在线方式相同.

  选择希望合并的分支——>发起合并请求（pull-request）——>添加合并理由——>发送

### 合并冲突问题

  经常的情况是，你向项目源作者提交分支合并请求时出现上述问题，无法自动合并. 这是因为你自己仓库中的修改

  与主分支上的修改发生了冲突。一种粗暴的解决方式是，直接将本地的仓库删除，然后再Fork一遍别人的仓库，这

  样就能保证自己所要提交的修改是最新的, 就可以避免发生合并冲突的问题. 但是这种方法会让自己做的修改随之

  丢失，因为我们所做的修改是在之前自己的仓库中做的，而我们为了拉取最新的仓库，把自己的仓库清空了，所以

  此种方法严重不推荐.

  最为方便的是本地编辑方法：

  首先从远程要合并的仓库(项目源仓库)拉取最新版本：
  
      git pull url
      
  可以看到自己更新了哪些文件. 拉取成功之后，再次推送本地仓库到自己的远程仓库, 然后再次发起合并请求即可

**从远程拉取特定分支**

  使用上面的命令，我们只能拉取仓库中特定的分支，一般为master分支，但是有时从我们需要从远程拉取特定的分支，

  这就需要指定分支分支名称，用如下命令：

		git pull -b branch_name url


1. 中断合并：
  
      git merge --abort
      
2. 解决由空白字符引起的合并冲突：
  
      git merge -Xignore-space-change whitespace
  
3. 手动合并：
  
  取出原版本，新版本，你修改的版本这3个文件：
       
      $ git show :1:hello.rb > hello.common.rb
      $ git show :2:hello.rb > hello.ours.rb
      $ git show :3:hello.rb > hello.theirs.rb
  
  手动修复如上文件
   
      git merge-file -p hello.ours.rb hello.common.rb hello.theirs.rb > hello.rb
      
  比较合并差异：
  
      git diff --ours
      git diff --theirs
      git diff --base
  
  清理合并中产生的三个额外文件：
  
      git clean -f

强制本地仓库完全同步于远程仓库:

    git reset --hard origin/master

即可将根底仓库分支强制跟远程仓库master分支同步
