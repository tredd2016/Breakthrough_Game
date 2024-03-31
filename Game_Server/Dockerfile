#  BUILD STAGE #

FROM alpine:3.19.1 as buildstage

RUN apk update && \
    apk add --no-cache \
        build-base \
        cmake \
        boost-dev

WORKDIR /breakthrough
COPY . .

WORKDIR /breakthrough/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 2

#  Image #

FROM alpine:3.19.1 as runstage

RUN apk update && \
    apk add --no-cache \
    libstdc++ \
    boost-program_options

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=buildstage \
    ./breakthrough/build/Breakthrough \
    ./app/

ENTRYPOINT [ "./app/Breakthrough" ]

EXPOSE 9999