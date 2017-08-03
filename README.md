# SFMT-Database

## Download
```bash
$ git clone https://github.com/odanado/SFMT-Database
$ git submodule init
$ git submodule update
```

## build
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## preprocess
```bash
$ ./preprocess
step > 417
```

## search
```bash
$ search
417
8
1 9 9 2 0 2 0 7
```

## verify
``` bash
$ md5sum -c md5sum-0417.txt
```
