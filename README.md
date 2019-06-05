# Welcome

This is **minizen**, a very simple ticket searching system.

# Usage

Once built and installed (see [Docker Quick Start](#docker-quick-start) or
[Manual Installation](#manual-installation) below) a search can be performed by
calling minizen with three arguments:

<pre><code>$ minizen <i>TABLE KEY VALUE</i>
</code></pre>

where:
* <code><i>TABLE</i></code> is one of `organizations`, `tickets` or `users`;
* <code><i>KEY</i></code> and <code><i>VALUE</i></code> describe the search
  criteria.

For example:

```
$ minizen organizations _id 1
$ minizen tickets priority high
$ minizen users active true
```

Note that shell quoting or escaping may be necessary if any of the arguments
contain whitespace or any other special characters:

```
$ minizen tickets subject \$100
$ minizen users name "Bob Smith"
$ minizen users email ""
```

The minizen tool will look for JSON files in the current working directory
unless a different directory is specified using the `--data-dir` option; e.g.:

<pre><code>$ minizen <b>--data-dir=<i>DIR</i></b> <i>TABLE KEY VALUE</i>
</code></pre>

# Docker Quick Start

If you have [Docker](https://www.docker.com/) installed, the following steps
will build and run a container that executes the minizen tool:

```
$ git clone https://github.com/pdb/minizen.git
$ cd minizen
$ docker build -t minizen .
$ docker run minizen
```

Any additional arguments passed to `docker run minizen` will be passed through
to the minizen tool (see [Usage](#usage) for more information) making the
typical invocation:

<pre><code>$ docker run minizen <i>TABLE KEY VALUE</i>
</code></pre>

Reference JSON files are preinstalled to `/var/db/minizen`, the container's
working directory, and so are queried by default. Alternate sets of JSON files
may be queried by mounting a local directory on `/var/db/minizen`; i.e.,

<pre><code>$ docker run <b>-v <i>DIR</i>:/var/db/minizen</b> minizen <i>TABLE KEY VALUE</i>
</code></pre>

# Manual Installation

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
* [JSON-C](https://github.com/json-c/json-c) (version 0.13.0 or later)
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)

In addition, [Git](https://git-scm.com/) is needed if cloning this repository.

The following table shows the command required to install these dependencies on
some common Linux distributions:

| Distribution | Command                                                                       |
| ------------ | ----------------------------------------------------------------------------- |
| Alpine       | ```# apk add autoconf automake build-base check-dev git json-c-dev libtool``` |
| Arch Linux   | ```# pacman -S base-devel check git json-c```                                 |

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

## Test Coverage

Test coverage can be measured using
[gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html).

This requires the following options to be used at compile time:

<pre><code>$ ./configure <b>CFLAGS=-coverage --disable-shared</b>
</code></pre>

after which `make check` can be run as usual.

Once the tests have completed, gcov can be run:

<pre><code>$ cd lib
$ gcov *.gcno
File 'db.c'
<b>Lines executed:86.75% of 83</b>
Creating 'db.c.gcov'

File 'filter.c'
<b>Lines executed:85.71% of 35</b>
Creating 'filter.c.gcov'
</code></pre>

Coverage statistics are displayed when gcov runs (as shown above) and annotated
source files are created that highlight lines not executed.

## Memory Leak Checks

Memory leaks can be checked using [Valgrind](http://www.valgrind.org/).

This works well for the minizen tool itself:

```
$ valgrind --leak-check=full minizen
```

but to check memory leaks in the _tests_ we need to run the tests from within
the `test` subdirectory (so tests can find their data files) and ideally in a
non-forking mode:

<pre><code>$ cd test
$ make check
$ <b>CK_FORK=no</b> valgrind --leak-check=full <b>./unit-tests</b>
</code></pre>

[gnu-build-system]: https://en.wikipedia.org/wiki/GNU_Build_System
