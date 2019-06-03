# Welcome

This is **minizen**, a very simple ticket searching system.

# Installation

## Dependencies

This project uses the [GNU Build System][gnu-build-system] and depends on the
following GNU tools:
* [Autoconf](https://www.gnu.org/software/autoconf/)
* [Automake](https://www.gnu.org/software/automake/)
* [GCC](https://www.gnu.org/software/gcc/)
* [GNU Make](https://www.gnu.org/software/make/)

This project also uses the following third party libraries and tools:
* [Check](https://libcheck.github.io/check/)
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

In addition, [Git](https://git-scm.com/) is needed if cloning this repository.

The following table shows the command required to install these dependencies on
some common Linux distributions:

| Distribution | Command                                                                        |
| ------------ | ------------------------------------------------------------------------------ |
| Alpine       | ```# apk add autoconf automake build-base check-dev git```                     |
| Arch Linux   | ```# pacman -S base-devel check git```                                         |
| CentOS       | ```# yum install autoconf automake check-devel gcc git make```                 |
| Ubuntu       | ```# apt-get install autoconf automake build-essential check git pkg-config``` |

## Building and Installing

Downloading, building, testing and installing the tool can be performed with
the following steps:

```
$ git clone https://github.com/pdb/minizen.git
$ cd minizen
$ autoreconf -i
$ ./configure
$ make
$ make check
$ make install
```

All standard GNU Build System options should work as expected (e.g.,
out-of-tree builds, custom installation prefixes, creation of distribution
tarballs, etc).

[gnu-build-system]: https://en.wikipedia.org/wiki/GNU_Build_System
