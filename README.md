# Welcome

This is **minizen**, a very simple ticket searching system.

# Usage

Once built and installed (see [Docker Quick Start](#docker-quick-start) or
[Manual Installation](#manual-installation) below) a search can be performed by
calling minizen with the following arguments:

<pre><code>$ minizen search <i>TABLE KEY VALUE</i>
</code></pre>

where:
* <code><i>TABLE</i></code> is one of `organizations`, `tickets` or `users`;
* <code><i>KEY</i></code> and <code><i>VALUE</i></code> describe the search
  criteria.

For example:

```
$ minizen search organizations _id 1
$ minizen search tickets priority high
$ minizen search users active true
```

Note that shell quoting or escaping may be necessary if any of the arguments
contain whitespace or any other special characters:

```
$ minizen search tickets subject \$100
$ minizen search users name "Bob Smith"
$ minizen search users email ""
```

The minizen tool will look for JSON files in the current working directory
unless a different directory is specified using the `--data-dir` option
_before_ the `search` command; e.g.:

<pre><code>$ minizen <b>--data-dir=<i>DIR</i></b> search <i>TABLE KEY VALUE</i>
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

<pre><code>$ docker run minizen search <i>TABLE KEY VALUE</i>
</code></pre>

Reference JSON files are preinstalled to `/var/db/minizen`, the container's
working directory, and so are queried by default. Alternate sets of JSON files
may be queried by mounting a local directory on `/var/db/minizen`; i.e.,

<pre><code>$ docker run <b>-v <i>DIR</i>:/var/db/minizen</b> minizen search <i>TABLE KEY VALUE</i>
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
<b>Lines executed:87.95% of 83</b>
Creating 'db.c.gcov'

File 'filter.c'
<b>Lines executed:85.71% of 35</b>
Creating 'filter.c.gcov'
</code></pre>

Coverage statistics are displayed when gcov runs (as shown above) and annotated
source files are created that highlight lines not executed. In the case above
the only lines not executed relate to malloc failures and [currently]
unreachable code.

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

# Code Challenge Points

## Specifications

### Language

The specification states:

> Use the language in which you are strongest.

and so this is in C (which is what most of my recent development has been in,
as I've talked about in my interviews and mentioned in my CV) though a
higher-level language may seem like a more natural choice for this project if
it was to be an ongoing concern and supported by others.

### README

I hope you're enjoying reading this as much as I've enjoyed writing it! :)

## Evaluation Criteria

### Extensibility

The decision to separate the data access functions into their own reusable
library, separate from the command line tool, is one way of showing a
separation of concerns and extensibility. As noted in `<minizen/db.h>` it would
be easy to add support in the library for any number of other backend databases
(whether traditional SQL databases or generic key-value stores) with this being
transparent to the command line tool.

Having a separate library also lets us easily re-use this code in other tools
(e.g., GUI applications, web applications, etc) while supporting independent
development of the library and those tools. The API could also be easily
extended in the future.

### Simplicity

The _code_ tries to be relatively simple (for C, at least); and this is a
reasonably small project: [SLOCCount](https://dwheeler.com/sloccount/) shows
~200 lines of source code for each of the library and the command line tool
in version 1.0.0:

```
$ sloccount lib src
...
SLOC	Directory	SLOC-by-Language (Sorted)
213     src             ansic=213
200     lib             ansic=200
...
```

The _design_ also tries to be simple, attempting to make as few assumptions
about the data as possible (for example, not assuming any uniqueness
constraints) and therefore treating all objects as opaque as possible. This
also helps keep things extensible for the future; though we could certainly
implement a more rigid schema if needed. We also only support a simple
key/value search at the moment, though more complex search queries could be
supported.

The _tool_ also aims to be simple for the user by taking all of its arguments
on the command line (though it could easily fall through to an interactive mode
in the future if no arguments were specified).

### Test Coverage

As shown in the [Test Coverage](#test-coverage) section, there are a number of
tests present for the minizen _library_. Arguably testing is a little more
complicated and intrusive in C than other languages; but it still has value
and proved useful in this exercise, as commit 8629d4a shows.

### Performance

As this basic implementation simply iterates over all objects in the table it
has O(_n_) performance for both:
* the first lookup in a table (which requires parsing the underlying JSON
  file); and
* subsequent cached lookups.

This can be measured empirically:

| Table Size (objects)             | 1,000 | 10,000 | 100,000 | 1,000,000 |
| :------------------------------- | ----: | -----: | ------: | --------: |
| Average _First_ Lookup (ms)      |  10.7 |   97.6 |   774.2 |   7,902.3 |
| Average _Subsequent_ Lookup (ms) |   0.1 |    3.1 |    45.7 |     452.0 |

These numbers come from running the special `minizen performance` command on
my laptop three times for each table size and averaging the results; e.g.:

```
$ for I in {1..3}; do
> minizen --data-dir=10000-users performance --lookups 10000 users _id 0
> done
10000 lookups in 32704.016 ms (average = 3.270 ms); first query = 100.256 ms
10000 lookups in 30680.144 ms (average = 3.068 ms); first query = 98.042 ms
10000 lookups in 30803.999 ms (average = 3.080 ms); first query = 94.604 ms
```

The input files for these tests were manually created by repeatedly cycling the
content of the reference `users.json` file (while ensuring the resulting file
was valid JSON).

We could get better performance by indexing well-known or often-queried keys in
the future.

### Robustness

The tool should handle and report all errors back to the user.

Internally error handling is very basic (with errors generally written to
stderr when they occur, and an error code propagated back to `main`) but with
the use of a shared library it would be nice to evolve this in the future.
Alternatives could include stashing the last error in `struct minizen_db`
(for retrieval by the caller when the API call finishes) or being able to set
an error callback.
