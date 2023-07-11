FROM --platform=linux/amd64 debian:stable-slim

RUN apt-get update && apt-get upgrade -y

RUN apt-get -y install  \
    software-properties-common \
    libxml2-dev \ 
    python3-dev \
    build-essential \ 
    libssl-dev \
    libffi-dev \
    libxslt1-dev \
    zlib1g-dev \
    python3-pip \
    python3 \
    git \
    tox \
    cmake \
    openssl \
    patch \
    libffi-dev \
    curl \
    bzip2 \
    sqlite3 \
    libbz2-dev \
    pipx \
    libsqlite3-dev \
    libboost-all-dev \
    autotools-dev \
    libicu-dev \
    g++ \
    clang \
    libfmt-dev \
    libdocopt0 \
    nano \
    wget \
    ccache 


# Install OpenJDK-11
RUN apt-get update && \
    apt-get install -y default-jdk

ENV PYTHONPATH "/bin/python3"

ENV PATH ${PATH}:${PYTHONPATH}

RUN ln -s /usr/bin/python3 /usr/bin/python

RUN cd ~ && git clone https://github.com/tiagomunarolo/cpp-sas7bdat.git

RUN cd ~/cpp-sas7bdat && make pyenv-download && make pyenv-python

RUN ~/.pyenv/bin/pyenv virtualenv -p python3.8 3.8.12 venv38

RUN . /root/.pyenv/versions/venv38/bin/activate && \ 
    python3 -m pip install --upgrade pip && \
    pip3 install wheel setuptools gcovr==5.0 numpy cmaketools conan==1.53.0

ENV ACTIVATEPY /root/.pyenv/versions/venv38/bin/activate

RUN . ${ACTIVATEPY} && cd ~/cpp-sas7bdat && \
    make configure && make build
    