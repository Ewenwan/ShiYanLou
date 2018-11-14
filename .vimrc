// 例子1
set nocompatible
set encoding=utf-8
set fileencodings=utf-8,chinese
set tabstop=4
set cindent shiftwidth=4
set backspace=indent,eol,start
autocmd Filetype c set omnifunc=ccomplete#Complete
autocmd Filetype cpp set omnifunc=cppcomplete#Complete
set incsearch
set number
set display=lastline
set ignorecase
syntax on
set nobackup
set ruler
set showcmd
set smartindent
set hlsearch
set cmdheight=1
set laststatus=2
set shortmess=atI
set formatoptions=tcrqn
set autoindent 
set tags=tags＃sudo apt-get install exuberant-ctags


// 例子2  注释
// Ubuntu 中 Vim 配置文件 ~/.vimrc，位于用户目录中.

syntax enable # 语法高亮
syntax on
colorscheme darkblue  # 设定配色方案

set encoding=utf-8 # 编码方式
set fileencodings=ucs-bom,utf-8,cp936
set fileencoding=utf-8
# set fenc=utf-8
# set fencs=utf-8,usc-bom,euc-jp,gbk,gb2312
# set enc=utf-8

set autoindent # 创建新行保持与上一行相同的缩进
set smartindent # 智能缩进

set tabstop=4 # 设置制表符(TAB)的宽度(以空格数量来表示)
set shiftwidth=4 # 设置移位操作 >> 或 << 的缩进长度
set expandtab # 使用空格代替制表符,可能会导致 Python 等依赖于制表符的编程语言出现问题.

set noexpandtab # 不要用空格代替制表符

set number # 显示行号
set autowrite # 自动保存
set ruler # 显示打开状态栏标尺
set cursorline # 突出显示当前行

set showmatch # 匹配光标所经过的括号等.
set showcmd # 命令行显示输入的命令
set showmode # 命令行显示vim当前模式

set hlsearch # 高亮搜索内容的所有匹配位置
set incsearch # 搜索过程中动态显示匹配内容
set ignorecase # 搜索时忽略大小写
set smartcase # 搜索对大小写敏感

set clipboard+=unnamed # 共享剪贴板  
set nocompatible # 不要使用vi的键盘模式，而是vim自己的

set laststatus=2 # vim 窗口底部显示永久状态栏，显示文件名，行号，列号等.
set spell spelllang=en_us # 英语拼写检查

set langmenu=zh_CN.UTF-8 # 语言设置
set helplang=cn

set nobackup # 禁止创建备份
set noswapfile # 禁止创建交换文件. 交换文件用于编辑器崩溃或冲突时，恢复文件.
set history=1000

set autochdir # 同一 vim 窗口编辑多个文件时，自动切换到正在编辑的文件.

set undofile # 文件关闭后也可以维护撤销历史记录,.un~扩展名保存的隐藏文件.
set errorbells # 错误信息响铃，只对错误信息有效.
set visualbell # 错误视觉提示

filetype plugin indent on # 允许插件
filetype on
filetype indent on

