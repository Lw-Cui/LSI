# 画板 Alpha

## 介绍

这是一款开发代号为画板Alpha 的`Scheme` 解释器，使用`C++`开发，带有图形绘制支持。至于为什么要叫这个名字，因为它有两个兄弟：[画板Beta](https://github.com/P-ainters/Drawing-Board-Beta) 和 [画板Segma](https://github.com/P-ainters/Drawing-Board-Sigma) :)

BTW. 正如`LLVM`就是`LLVM`的意思一样，Alpha 就是Alpha，不是版本号哦

## 使用

本项目使用`Git` 和`CMake`构建：

```shell
git submodule update --init
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 命令行交互

#### 构建

命令行交互依赖 [cxxopts](https://github.com/jarro2783/cxxopts)。

```shell
make GI_CLI
```

#### 使用

将可执行文件放入  [Scheme](./Scheme) 文件夹下（因为需要 [Scheme/stdlib](./Scheme/stdlib)），之后执行：

```shell
./GI_CLI test/CLITest.scm -o output.bmp Test.scm
```

即可依次读取并执行 test/CLITest.scm 和 Test.scm文件，并将生成的图片储存为 outout.bmp。

更多的demo 见 [gallary](./gallary) 

### 图形交互

#### 构建

构建图形交互的话需要 [SFML](https://www.sfml-dev.org/) 对图形的支持。本项目不包含它的源码，请见下文"SFML的安装"。

```shell
make GI
```

#### 使用

##  ![Overview](picture/overview.png)

这里演示了谢尔宾斯基三角形的绘制和`Y-Combinator`的使用。

`Y-Combinator`的实现在 [Scheme/stdlib](./Scheme/stdlib) 中。见下文“库”。

## 核心部分

### 解释器

完整支持闭包的Scheme 解释器。

### 库

命令行交互和图形交互中都会加载以下库文件：

* [Scheme/stdlib/Base.scm](./Scheme/stdlib/Base.scm) 中实现了基础的函数。

* [Scheme/stdlib/Shape.scm](./Scheme/stdlib/Shape.scm) 中实现了直线绘制、三角形绘制和谢尔宾斯基三角形绘制的算法。
* [Scheme/stdlib/Frame.scm](./Scheme/stdlib/Frame.scm) 中实现了图形的变换。

### 测试

拉取所有的代码：

```shell
git submodule update --init
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug
```

#### 单元测试 && Mock 测试

同样需要将可执行文件放入  [Scheme](./Scheme) 文件夹下。

* `make LexersTest` 构建词法分析器测试

* `make parserTest` 构建语法分析器测试

* `make lispLibTest` 构建库测试

* `make GUITest` 构建GUI 测试


#### 覆盖率测试

如果使用Debug 模式构建并执行了某些程序，并且安装了 [LCOV](http://ltp.sourceforge.net/coverage/lcov.php)，那么可以执行[./CoverageTestResult.sh](./CoverageTestResult.sh) 来获得覆盖率测试的分析结果。

#### 静态分析

[![Coverage Status](https://scan.coverity.com/projects/11270/badge.svg?flat=1)](https://scan.coverity.com/projects/p-ainters-drawing-board-alpha)

#### 持续集成

[![Build Status](https://travis-ci.org/P-ainters/Drawing-Board-Alpha.svg?branch=v0.1-Beta)](https://travis-ci.org/P-ainters/Drawing-Board-Alpha)




## SFML 安装

构建图形交互前，需要安装`SFML`库：

* `MacOS` 可以直接 `brew install sfml`完成 2.x 版本的安装
* `Linux(Ubuntu)` 则需要首先安装依赖 [DependencyInstall.sh](./DependencyInstall.sh) ，之后下载 `SFML 2.x` 源码进行编译安装。


如果使用`libc++` 标准库，请一定从源码编译安装。见  [KnownIssues.md](./KnownIssues.md) 


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


