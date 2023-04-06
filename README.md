# Description

This program performs hashing on a set of files to compute a top hash for
a merkle hash tree.

it assumes you have `openssl` installed on your system.

To compile execute the following command:

```
gcc main.c -o main -lssl -lcrypto
```

once compiled you can invoke the program:

```
./main ./
```

Example output:

```
. is a directory
.. is a directory
File: list.c Hash:d88d75a90144773cc7541
File: README.md Hash:d3ae564035be9619ad009
File: list.h Hash:d88d75a90144773cc7541
File: main.c Hash:d88d75a90144773cc7541
.git is a directory
File: main Hash:16ba1f874d322e2d3bc49
Top Digest: 9d77d666e5abf373dd751

```