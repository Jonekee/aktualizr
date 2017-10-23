FROM ubuntu:xenial
LABEL Description="Aktualizr testing dockerfile"

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update
RUN apt-get -y install debian-archive-keyring software-properties-common apt-transport-https
RUN echo "deb http://httpredir.debian.org/debian jessie-backports main contrib non-free" > /etc/apt/sources.list.d/jessie-backports.list
RUN apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D
RUN apt-add-repository 'deb https://apt.dockerproject.org/repo ubuntu-xenial main'

# It is important to run these in the same RUN command, because otherwise
# Docker layer caching breaks us
RUN apt-get update && apt-get -y install docker-engine liblzma-dev bison e2fslibs-dev libgpgme11-dev libglib2.0-dev gcc g++ make cmake git psmisc dbus python3-dbus python3-gi python3-openssl libdbus-1-dev libjansson-dev libgtest-dev libssl-dev autoconf automake pkg-config libtool libexpat1-dev libboost-program-options-dev libboost-test-dev libboost-random-dev libboost-regex-dev libboost-dev libboost-system-dev libboost-date-time-dev libboost-thread-dev libboost-log-dev libjsoncpp-dev curl libcurl4-openssl-dev lcov clang clang-format-3.8
RUN apt-get update && apt-get -y install ostree libostree-dev libsodium-dev libarchive-dev python3-venv python3-dev valgrind libp11-dev softhsm2 opensc libengine-pkcs11-openssl
WORKDIR aktualizr
ADD . src
