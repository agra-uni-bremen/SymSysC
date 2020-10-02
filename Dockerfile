FROM klee/klee

RUN sudo apt update && sudo apt install time libboost-all-dev lld wget libssl-dev python-reportlab -y
# Damn old ubuntu with even older cmake without target link options
RUN sudo apt purge --auto-remove cmake -y
RUN cd && wget https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2.tar.gz && tar xzf cmake-3.18.2.tar.gz
RUN cd cmake-3.18.2 && ./bootstrap && make -j$(nproc) && sudo make install

RUN mkdir build && echo "cd build && cmake ../source && make -j$(nproc)" > make.sh && chmod +x make.sh
