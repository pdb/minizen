FROM alpine AS build

RUN apk add --no-cache \
	autoconf \
	automake \
	build-base \
	check-dev \
	json-c-dev \
	libtool

COPY . /minizen

RUN cd /minizen && \
	autoreconf -i && \
	./configure && \
	make && \
	make check && \
	make install

FROM alpine AS release

RUN apk add --no-cache json-c

COPY --from=build /usr/local /usr/local/

ENTRYPOINT ["minizen"]
