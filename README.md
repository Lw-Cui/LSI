# 画板 Alpha

## 名字起源

这是一款名为“画板Alpha“的桌面绘图应用，使用`C++`开发，支持通过编写`Scheme`代码来生成图像。 至于为什么要叫Alpha，因为它有两个兄弟：[画板Beta](https://github.com/P-ainters/Drawing-Board-Beta) 和 [画板Segma](https://github.com/P-ainters/Drawing-Board-Sigma)……（BTW. 正如`LLVM`就是`LLVM`的意思一样，Alpha 就是Alpha，不是版本号)

## 效果展示

 ![Overview](picture/overview.png)



这里演示了绘制一个谢尔宾斯基三角形和`Y-Combinator`的使用。

由于画板仅支持画点，因此在 [Scheme/GUI.scm](./Scheme/GUI.scm) 中实现了直线绘制、三角形绘制和谢尔宾斯基三角形绘制的算法，程序启动时会自动加载这个文件。之后就可以`load`一个写好参数的文件直接展示效果了。

`Y-Combinator`是通过（自动）加载 [Scheme/Base.scm](./Scheme/Base.scm) 后得到了`Y` 函数来实现的。这个文件同时实现了诸如`=` `and` `map` 等常见函数。