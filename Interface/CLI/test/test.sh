#!/usr/bin/env bash
dirname=`dirname $0`
../build/Interface/CLI/src/LSI_CLI -o $dirname/res.bmp cli_test/CLITest.scm -p stdlib
diff ../Interface/CLI/test/res.bmp ../Interface/CLI/test/standard.bmp
