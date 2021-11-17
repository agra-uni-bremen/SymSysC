FROM klee/klee

#RUN sudo apt update && sudo apt install time libboost-all-dev lld wget libssl-dev python-reportlab nano less -y
RUN sudo apt update && sudo apt install time libboost-all-dev wget libssl-dev python-reportlab nano less python-tabulate -y
# Damn old ubuntu with even older cmake without target link options
RUN sudo apt purge --auto-remove cmake -y
RUN cd && wget https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2.tar.gz && tar xzf cmake-3.18.2.tar.gz
RUN cd cmake-3.18.2 && ./bootstrap && make -j$(nproc) && sudo make install
RUN sudo ln -s /home/klee/klee_src/include/ /include	# This is for shitty clang include problem not honoring LIBRARY_PATH
RUN mkdir build build_native && echo "cd build && cmake -DBUILD=bytecode ../source && make -j$(nproc) && cd ../build_native && cmake ../source && make -j$(nproc)" > make.sh && chmod +x make.sh
RUN rm cmake-3.18.2.tar.gz
RUN pip install wllvm
