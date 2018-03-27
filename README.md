# LSI

[![Coverage Status](https://scan.coverity.com/projects/11270/badge.svg?flat=1)](https://scan.coverity.com/projects/p-ainters-drawing-board-alpha)  [![Build Status](https://travis-ci.org/Lw-Cui/LSI.svg?branch=v0.1-rc1)](https://travis-ci.org/Lw-Cui/LSI)

**L**iwei's **S**cheme **I**nterpreter.

## Build from scratch

Use LSI for painting!

```
git submodule update --init
mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release
make LSI_CLI
../build/Interface/CLI/src/LSI_CLI -o `dirname $0`/res.bmp cli_test/CLITest.scm -p stdlib
```


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


