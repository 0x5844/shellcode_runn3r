# shellcode_runn3r
Inline Shellcode Runner for macOS (ARM64) in C++

1. Compile the code:
```
g++ -o shellcode_runn3r shellcode_runn3r.cpp
shellcode_runn3r.cpp:37:5: warning: 'auto' type specifier is a C++11 extension [-Wc++11-extensions]
   37 |     auto shellcode_func = reinterpret_cast<void (*)()>(exec_mem);
      |     ^
1 warning generated.
```

2. Generate a shellcode file:
```
# this is a sample
echo -en '\x1F\x20\x03\xD5\x00\x00\x00\x14' > shellcode.bin
```

3. Run the compiled code with the generated shellcode file (observe the infinite loop as an example):
```
./shellcode_runn3r shellcode.bin
Executing shellcode...
^C
```
