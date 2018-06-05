# LSI

[![Coverage Status](https://scan.coverity.com/projects/11270/badge.svg?flat=1)](https://scan.coverity.com/projects/p-ainters-drawing-board-alpha)   [![Build Status](https://travis-ci.org/Lw-Cui/LSI.svg?branch=v0.1-rc1)](https://travis-ci.org/Lw-Cui/LSI)

**L**iwei's **S**cheme **I**nterpreter — A Scheme interpreter with geometric figure generation.

View [demo video](https://www.dropbox.com/s/fche4xurlhohbcf/LSI.mp4?dl=0) & [presentation(In Chinese)](https://www.dropbox.com/s/84d3bo0gav6qthv/%E6%AF%95%E4%B8%9A%E8%AE%BE%E8%AE%A1%E7%AD%94%E8%BE%A9.pptx?dl=0) now!

## Feature

1.  Implemented a closure support interpreter with tail call optimization
2. Enabled user-defined Scheme libraries including Y-combinator,  geometric figure drawing and  coordinate transformation
3.  Added logger, unit test, continuous integration, coverage test and static analysis

## Build from scratch

```
git submodule update --init
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
make LSI_CLI
./Interface/CLI/src/LSI_CLI ../Scheme/demo/painter3.scm -p ../Scheme/stdlib
```

(Wait about 20s…)

And find picture in `Scheme/demo`!

![painter3.png](./Doc/IMG/painter3.bmp)

## License

```
MIT License

Copyright (c) 2016 Liwei Cui

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


