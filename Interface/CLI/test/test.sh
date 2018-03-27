#!/usr/bin/env bash
../build/Interface/CLI/src/LSI_CLI -o `dirname $0`/res.bmp cli_test/CLITest.scm -p stdlib
diff ../Interface/CLI/test/res.bmp ../Interface/CLI/test/standard.bmp
