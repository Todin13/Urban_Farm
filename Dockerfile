# Use the latest Ubuntu base image
FROM ubuntu:latest

# Install necessary tools
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    autoconf \
    libtool

# Clone and bootstrap vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg
RUN /opt/vcpkg/bootstrap-vcpkg.sh -disableMetrics

# Install dependencies using vcpkg, including necessary Boost components
RUN /opt/vcpkg/vcpkg install cpprestsdk:x64-linux libpqxx:x64-linux
RUN /opt/vcpkg/vcpkg install boost-system:x64-linux boost-regex:x64-linux boost-date-time:x64-linux boost-thread:x64-linux boost-random:x64-linux boost-filesystem:x64-linux boost-chrono:x64-linux boost-atomic:x64-linux
RUN /opt/vcpkg/vcpkg install libpqxx:x64-linux
RUN apt-get install -y libpq-dev

# Set environment variables
ENV CMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
ENV CPPRESTSDK_DIR=/opt/vcpkg/installed/x64-linux/share/cpprestsdk
ENV ZLIB_DIR=/opt/vcpkg/installed/x64-linux/share/zlib
ENV ZLIB_LIBRARY=/opt/vcpkg/installed/x64-linux/lib/libz.a
ENV ZLIB_INCLUDE_DIR=/opt/vcpkg/installed/x64-linux/include
ENV OPENSSL_CRYPTO_LIBRARY=/opt/vcpkg/installed/x64-linux/lib/libcrypto.a
ENV OPENSSL_INCLUDE_DIR=/opt/vcpkg/installed/x64-linux/include
ENV OPENSSL_ROOT_DIR=/opt/vcpkg/installed/x64-linux
ENV BOOST_INCLUDEDIR=/opt/vcpkg/installed/x64-linux/include
ENV LIBPQXX_DIR=/opt/vcpkg/installed/x64-linux/share/libpqxx


# Copy your project files into the Docker image
WORKDIR /usr/src/urbanfarm
COPY . .

RUN rm -rf build
# Create a build directory and configure the project
RUN mkdir -p build
# Configure the project
WORKDIR /usr/src/urbanfarm/build
RUN cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE \
          -Dcpprestsdk_DIR=$CPPRESTSDK_DIR \
          -DZLIB_LIBRARY=$ZLIB_LIBRARY \
          -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR \
          -DOPENSSL_ROOT_DIR=$OPENSSL_ROOT_DIR \
          -Dlibpqxx_DIR=$LIBPQXX_DIR \
          -DBOOST_INCLUDEDIR=$BOOST_INCLUDEDIR ..


# Build the project
RUN cmake --build .
