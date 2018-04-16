# TextEditor 一个简单的文本编辑器  

[![Build Status](https://travis-ci.org/linyacool/WebServer.svg?branch=master)](https://travis-ci.org/linyacool/WebServer)
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)

## 建仓库的目的

1. 熟悉常用的Qt类的使用，eg:`QWidget`,`QAction`,`QMainWindow`,`QStatusBar`,`QToolBar`以及其他Qt类的用法.

2. 完成一个与`Notepad++`相近的文本编辑器

## 运行环境说明

本项目的代码使用的是`QT 5.9.4`, IDE环境是`QT Creator 4.5.0`, 编译工具是`MinGW 5.3.0 32bit`. 

## TextEditor介绍

关于具体的TextEditor的运行界面，可以看running-pics文件夹！

### 一览  

![主界面](https://github.com/JosanSun/textEditor/blob/master/running-pics/mainInterface.png  "主界面")

主界面

![打开文件界面](https://github.com/JosanSun/textEditor/blob/master/running-pics/openAFile.png  "打开文件")

打开文件界面

![文件菜单项](https://github.com/JosanSun/textEditor/blob/master/running-pics/fileMenu.png  "文件菜单项")

文件菜单项


![about](https://github.com/JosanSun/textEditor/blob/master/running-pics/about.png "about窗口")

关于窗口

### 程序结构介绍  
![程序视图](https://github.com/JosanSun/textEditor/blob/master/running-pics/programView.png "程序结构图")  
TextEditor程序项目结构图

主程序包括`main.cpp`, `MainWindow`是程序的主骨架类，`TextEditor`类是`QTextEdit`的子类，是`MainWindow`中间主体窗口.  

附件程序包括FindDialog类，这是查找窗口类. MD5Widget, MD5FileWidget是关于程序中MD5工具使用的窗口类. myheaders则是个人常用的一些自定义宏名.  

## 功能日志  

### 已实现功能介绍  
1. 正常打开、保存、关闭文件。  
2. 常用的复制、剪切、粘贴、撤销、恢复文本功能。  
3. 对各个菜单项进行控制，使其符合正常书写逻辑。比如，刚打开的文件，保存按钮应该失效。  
4. 命令行打开程序，可识别file参数  
5. 实现查找功能功能，可以控制大小写，向上和向下查找。  
6. MD5功能可以根据文本或者文件产生相应的MD5值。  

### **TextEditor**更新计划  

详细的更新，请点击[此处](https://github.com/JosanSun/textEditor/blob/master/CHANGELOG "更新日志")  

- [x] 退出的时候，对于未保存的文件都是直接退出.  

- [x] 对于改动后的文本，显示的文件名应该带有[*].  
	
- [x] 读写文件操作  
	
- [x] 编辑菜单增加撤销，恢复，拷贝，剪切，全选.  

- [x] 编辑菜单，增加**删除项**的事件响应.    --20180326完成

- [x] 增加状态栏的显示，可以显示目前编码格式，可以显示是Insert还是OverType状态.  

- [x] MD5增加从文件生成校验码功能  

- [ ] 将单文档程序改为多文档程序  

- [ ] 增加状态栏编码方式的显示  

- [ ] 有效地打开显示大文件数据, 目标大小暂定为300MB.  

- [ ] 增加状态栏编码方式的显示  

- [ ] 增加文件拖拽，打开文件功能  

- [ ] 待修复bugs:  
	- [ ] 更新APP功能.  
	- [ ] save菜单项应该在保存文件后，不可点击  
	- [ ] Overwrite模式下面, 只对英文有效, 对于汉字没有效果。  
	- [ ] MD5工具可以出现多个窗口。理想状态，只出现一个窗口。  

## 讨论群

对TextEditor的完善工作感兴趣的，可以加QQ群:**676908519**.备注:Text.

## 参考资料

1. [C++ GUI Programming with QT 4](http://www-cs.ccny.cuny.edu/~wolberg/cs221/qt/books/C++-GUI-Programming-with-Qt-4-1st-ed.pdf )

2. [rich text Example](http://doc.qt.io/qt-5/examples-richtext.html  "官方例子")

3. [notepad](https://github.com/thunderobot/notepad)


## 致谢

感谢@[scienceasdf](https://github.com/scienceasdf)提供的MD5校验功能。
感谢@[Longxr](https://github.com/Longxr)对save菜单项和MD5工具的改进。
