FROM ubuntu:latest

RUN apt update -y && apt install -y \
    build-essential autotools-dev   \
    libboost-all-dev cmake

WORKDIR /home

COPY Makefile /home
COPY bd.cc /home
COPY CMakeLists.txt /home

ENTRYPOINT [ "make" ]