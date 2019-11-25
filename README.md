
# Dependensies
-----
```sh
sudo apt-get install libreadline6-dev
sudo apt-get install libgtest-dev

sudo apt-get install clang
sudo apt-get install llvm
sudo apt-get install libclang-dev
cd /usr/lib/llvm-6.0/include/
cp -r * /usr/local/include/
```

# should link with
`
LLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING=1
`
