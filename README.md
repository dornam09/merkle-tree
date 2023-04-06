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

You can make a modification to the filesystem and observe the top hash
change.  For example:

```

. is a directory
.. is a directory
File: README.md Hash:d88d75a90144773cc7541
File: main.c Hash:d88d75a90144773cc7541
File: test.txt Hash:d3ae564035be9619ad009
.git is a directory
File: main Hash:16ba1f874d322e2d3bc49
Top Digest: 530695937e0972ffab58d
```

In this case 2 files were deleted `list.h` and `list.c` and 1 file was added
`test.txt`.  The hashes from both runs do not match.