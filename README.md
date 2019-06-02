# Welcome

This is **minizen**, a very simple ticket searching system.

# Installation

This project uses the [GNU Build System][gnu-build-system] and so downloading,
building and installing the tool can be performed with the following steps:

```
$ git clone https://github.com/pdb/minizen.git
$ cd minizen
$ autoreconf -i
$ ./configure
$ make
$ make install
```

All standard GNU Build System options should work as expected (e.g.,
out-of-tree builds, custom installation prefixes, creation of distribution
tarballs, etc).

[gnu-build-system]: https://en.wikipedia.org/wiki/GNU_Build_System
