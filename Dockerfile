FROM klee/klee

RUN sudo apt update && sudo apt install libboost-all-dev lld wget libssl-dev -y
# Damn old ubuntu with even older cmake without target link options
RUN sudo apt purge --auto-remove cmake -y
RUN cd && wget https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2.tar.gz && tar xzf cmake-3.18.2.tar.gz
RUN cd cmake-3.18.2 && ./bootstrap && make -j$(nproc) && sudo make install

RUN cd && mkdir build && cd build && cmake ../source && make -j$(nproc)

WORKDIR /home/klee/build

#CMD klee -libcxx -libc=klee $(find -iname *.o | grep llvm | xargs echo)
