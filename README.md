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

In addition, [Git](https://git-scm.com/) is needed if cloning this repository.

The following table shows the command required to install these dependencies on
some common Linux distributions:

| Distribution | Command                                            |
| ------------ | -------------------------------------------------- |
| Alpine       | ```# apk add autoconf automake build-base git```   |
| Arch Linux   | ```# pacman -S base-devel git```                   |
| CentOS       | ```# yum install autoconf automake gcc git make``` |
| Ubuntu       | ```# apt-get install build-essential git```        |

## Building and Installing

Downloading, building and installing the tool can be performed with the
following steps:

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
