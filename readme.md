# TextEditor 一个简单的文本编辑器

## 建仓库的目的

1. 熟悉常用的Qt类的使用，eg:`QWidget`,`QAction`,`QMainWindow`,`QStatusBar`,`QToolBar`以及其他Qt类的用法.

2. 完成一个与`Notepad++`相近的文本编辑器

## 调试环境说明

本项目的代码使用的是`QT 5.9.1`, IDE环境是`QT Creator 4.3.1`. 

## TextEditor介绍

关于具体的TextEditor的运行界面，可以看running-pics文件夹！

### 一览

![主界面](https://github.com/JosanSun/textEditor/blob/master/running-pics/mainInterface.png  "主界面")

主界面

![打开文件界面](https://github.com/JosanSun/textEditor/blob/master/running-pics/openAFile.png  "打开文件")

打开文件界面

![about](https://github.com/JosanSun/textEditor/blob/master/running-pics/about.png "about窗口")

关于窗口

现在已经完成TextEditor主体框架的搭建，但是具体菜单项的信号与槽的连接，还在进一步完善当中.

待完善：  

- [x] 退出的时候，对于未保存的文件都是直接退出.  

- [x] 对于改动后的文本，显示的文件名应该带有[*].  
	
- [x] 读写文件操作
	
- [x] 编辑菜单增加撤销，恢复，拷贝，剪切，全选。

- [*] 编辑菜单，增加**删除项**的事件响应。   --20180326完成

- [ ] 增加状态栏的显示，可以显示目前编码格式，可以显示是Insert还是OverType状态。

- [ ] MD5增加从文件生成校验码功能 

- [ ] 待修复bugs:  
	- [ ] 更新APP功能。


## 讨论群

对TextEditor的完善工作感兴趣的，可以加QQ群:**676908519**.备注:Text.

## 参考资料

1. [C++ GUI Programming with QT 4](http://www-cs.ccny.cuny.edu/~wolberg/cs221/qt/books/C++-GUI-Programming-with-Qt-4-1st-ed.pdf )

程序的主体代码是仿照第三章和第四章的`spreadsheet`以及第六章的`mdieditor`。



