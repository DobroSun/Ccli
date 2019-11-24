
# Dependensies
-----
```sh
sudo apt-get install libreadline6-dev
sudo apt-get install libgtest-dev
```

Building Clang and LLVM from source
[Here](http://clang.llvm.org/get_started.html)
```sh
export PATH="$(pwd)/bin:$PATH" # from /llvm-project/build/bin/
cd ../../clang/include/clang/
cp -r * /usr/local/bin/
```
