FROM python:2

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -qqy && apt-get install -qqy \
  make \
  clang \
  curl \
  gcc \
  g++ \
  libgsl23 \
  libgsl-dev

RUN pip install numpy pandas scipy pytest pybind11 dit sklearn

#COPY tests/requirements.txt /requirements.txt
#RUN python3 -m pip install -r /requirements.txt

COPY . /APSingleCodebase

#WORKDIR /APSingleCodebase
#RUN make clean && make

WORKDIR /APSingleCodebase
ENTRYPOINT ["/usr/local/bin/pytest", "-s", "--github"]
