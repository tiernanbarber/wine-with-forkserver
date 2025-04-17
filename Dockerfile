FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and all required headers
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        gcc \
        g++ \
        make \
        autoconf \
        automake \
        libtool \
        pkg-config \
        git \
        linux-libc-dev \
        libc6-dev \
        libmount-dev \
        libudev-dev \
        libdbus-1-dev \
        libncurses5-dev \
        libncursesw5-dev \
        libx11-dev \
        libxext-dev \
        libfreetype6-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libxrandr-dev \
        libxinerama-dev \
        libxcursor-dev \
        libxi-dev \
        libxcomposite-dev \
        libxrender-dev \
        libxfixes-dev \
        libxinerama-dev \
        libxxf86vm-dev \
        libxss-dev \
        libxpm-dev \
        libxaw7-dev \
        libxmu-dev \
        libxft-dev \
        libxinerama-dev \
        libxkbfile-dev \
        libxshmfence-dev \
        linux-headers-generic \
        linux-source \
        wget \
        vim \
        sudo \
        ca-certificates

# Add a non-root user for building/running
RUN useradd -ms /bin/bash wineuser && echo "wineuser ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER wineuser
WORKDIR /home/wineuser

# (Optional) Clone your Wine fork and copy your custom files
RUN git clone https://github.com/tiernanbarber/wine-with-forkserver.git wine-with-forkserver
# COPY ./wine-with-forkserver /home/wineuser/wine-with-forkserver

# Set up build environment
ENV PATH="/home/wineuser/.local/bin:${PATH}"

# Default shell
CMD ["/bin/bash"]

# Commands to configure and make wine:
# ./configure --enable-forkserver --without-mingw --enable-win64 --without-x --without-opengl --disable-winemenubuilder --disable-win16 --disable-tests --without-wayland --without-vulkan

# make clean
# make -j$(nproc)