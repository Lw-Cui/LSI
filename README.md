# 画板 Alpha

## 名字起源

这是一款名为“画板Alpha“的桌面绘图应用，使用`C++`开发，支持通过编写`Scheme`代码来生成图像。 至于为什么要叫Alpha，因为它有两个兄弟：[画板Beta](https://github.com/P-ainters/Drawing-Board-Beta) 和 [画板Segma](https://github.com/P-ainters/Drawing-Board-Sigma) :)

BTW. 正如`LLVM`就是`LLVM`的意思一样，Alpha 就是Alpha，不是版本号哦

## 功能

 ![Overview](picture/overview.png)



这里演示了谢尔宾斯基三角形的绘制和`Y-Combinator`的使用。

画板只对外界暴露画点的操作，因此 [Scheme/GUI.scm](./Scheme/GUI.scm) 中实现了直线绘制、三角形绘制和谢尔宾斯基三角形绘制的算法。程序启动时解释器会加载这个文件。之后就可以`load` [Scheme/demo/triangle.scm](./Scheme/demo/triangle.scm) 来展示结果了。

`Y-Combinator`是通过（自动）加载 [Scheme/Base.scm](./Scheme/Base.scm) 后得到了`Y` 函数来实现的。这个文件实现了诸如`=` `and` `map` 等常见函数。

## 依赖

画板Alpha 依赖`Goolge Test`测试库，`easylogging++`日志库和`SFML`图形库和正确支持`C++11`或以上的编译器。

因此运行测试前，需要

```shell
git submodule update --init
```

拉取测试库的源码。

运行画板前，需要安装`SFML`库：

* `MacOS` 可以直接 `brew install sfml`完成 2.x 版本的安装
* `Linux(Ubuntu)` 则需要首先安装依赖 [DependencyInstall.sh](./DependencyInstall.sh) ，之后下载 `SFML 2.x` 源码进行编译安装。

## License

```
MIT License

Copyright (c) 2016 lwCui

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```


