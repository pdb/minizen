# Welcome

This is **minizen**, a very simple ticket searching system.

# Usage

The minizen tool will look for JSON files in the current working directory
unless a different directory is specified using the `--data-dir` option; e.g.:

```
$ minizen --data-dir=/tmp/minizen
```

# Installation

## Dependencies

This project uses the [GNU Build System][gnu-build-system] and depends on the
following GNU tools:
* [Autoconf](https://www.gnu.org/software/autoconf/)
* [Automake](https://www.gnu.org/software/automake/)
* [GCC](https://www.gnu.org/software/gcc/)
* [GNU Libtool](https://www.gnu.org/software/libtool/)
* [GNU Make](https://www.gnu.org/software/make/)

This project also uses the following third party libraries and tools:
* [Check](https://libcheck.github.io/check/)
* [JSON-C](https://github.com/json-c/json-c)
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

In addition, [Git](https://git-scm.com/) is needed if cloning this repository.

The following table shows the command required to install these dependencies on
some common Linux distributions:

| Distribution | Command                                                                                              |
| ------------ | ---------------------------------------------------------------------------------------------------- |
| Alpine       | ```# apk add autoconf automake build-base check-dev git json-c-dev libtool```                        |
| Arch Linux   | ```# pacman -S base-devel check git json-c```                                                        |
| CentOS       | ```# yum install autoconf automake check-devel gcc git json-c-devel libtool make```                  |
| Ubuntu       | ```# apt-get install autoconf automake build-essential check git libtool libjson-c-dev pkg-config``` |

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
