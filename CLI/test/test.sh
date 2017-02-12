#!/usr/bin/env bash
../build/CLI/src/GI_CLI -o ../CLI/test/res.bmp test/CLITest.scm
diff ../CLI/test/res.bmp ../CLI/test/standard.bmp
