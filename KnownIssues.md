#### `llvm-clang` 链接失败

`g++-5`在链接时：

```shell
/usr/lib/gcc/x86_64-linux-gnu/5/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/5/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/5/lto-wrapper -plugin-opt=-fresolution=/tmp/ccDhApi0.res -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc --sysroot=/ --build-id --eh-frame-hdr -m elf_x86_64 --hash-style=gnu --as-needed -export-dynamic -dynamic-linker /lib64/ld-linux-x86-64.so.2 -z relro -o GI /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/5/crtbegin.o -L/usr/lib/gcc/x86_64-linux-gnu/5 -L/usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/5/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/5/../../.. CMakeFiles/GI.dir/main.cpp.o libContorller.a ../../Interpreter/src/libinterpreter.a -lsfml-graphics -lsfml-window -lsfml-system -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-linux-gnu/5/crtend.o /usr/lib/gcc/x86_64-linux-gnu/5/../../../x86_64-linux-gnu/crtn.o
```

`llvm-3.7`在链接时：

```shell
 "/usr/bin/ld" -export-dynamic -z relro --hash-style=gnu --build-id --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o GI /usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/../../../x86_64-linux-gnu/crt1.o /usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/../../../x86_64-linux-gnu/crti.o /usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/crtbegin.o -L/home/travis/build/P-ainters/Drawing-Board-Alpha/deps/llvm-3.8.1/install/lib -L/usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1 -L/usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/../../../x86_64-linux-gnu -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/../../.. -L/usr/lib/llvm-3.8/bin/../lib -L/lib -L/usr/lib -lc++ -lc++abi CMakeFiles/GI.dir/main.cpp.o libContorller.a ../../Interpreter/src/libinterpreter.a -lsfml-graphics -lsfml-window -lsfml-system -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/crtend.o /usr/bin/../lib/gcc/x86_64-linux-gnu/5.4.1/../../../x86_64-linux-gnu/crtn.o
```

后者会出现链接失败的问题。