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
$ ./search
417
8
1 9 9 2 0 2 0 7
```

## verify
``` bash
$ md5sum -c md5sum-0417.txt
```

## For AWS Lambda
### create DB
```bash
$ ./create_db
step > 477
```

### split DB
```bash
$ ./split_db
step > 477
stage > 1
$ ./split_db
step > 477
stage > 2
$ ./split_db
step > 477
stage > 3
$ ./split_db
step > 477
stage > 4
```

### sort DB
```bash
$ ./sort_db
step > 477
stage > 4
```
