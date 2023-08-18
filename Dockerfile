FROM --platform=linux/amd64 debian:stable-slim

RUN apt-get update && apt-get upgrade -y

# Essential dev libs
RUN apt-get -y install  \
    software-properties-common \
    build-essential \ 
    libxml2-dev \ 
    libffi-dev \
    libxslt1-dev \
    zlib1g-dev \
    libffi-dev \
    libbz2-dev \
    libsqlite3-dev \
    libboost-all-dev \
    autotools-dev \
    libicu-dev 

# Open SSL
RUN apt-get -y install  \
    libssl-dev \
    openssl

# Python libs 
RUN apt-get -y install  \
    python3-dev \
    python3-pip \
    python3 \
    pipx 

# C compiler + dependencies
RUN apt-get -y install  \
    cmake \
    g++ \
    clang 

# Useful dependencies
RUN apt-get -y install  \
    git \
    patch \
    curl \
    bzip2 \
    nano \
    wget 

ENV PYTHONPATH "/bin/python3"

ENV PATH ${PATH}:${PYTHONPATH}

RUN ln -s /usr/bin/python3 /usr/bin/python

RUN cd ~ && git clone https://github.com/tiagomunarolo/cpp-sas7bdat.git

RUN cd ~/cpp-sas7bdat && make pyenv-download && make pyenv-python

RUN ~/.pyenv/bin/pyenv virtualenv 3.8 venv38

RUN . /root/.pyenv/versions/venv38/bin/activate && \ 
    python3 -m pip install --upgrade pip && \
    pip3 install wheel setuptools gcovr==5.0 numpy cmaketools conan==1.53.0

ENV ACTIVATEPY /root/.pyenv/versions/venv38/bin/activate

RUN . ${ACTIVATEPY} && cd ~/cpp-sas7bdat && \
    make configure && make build
    