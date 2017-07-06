FROM daocloud.io/learningtech/gstreamer_base:latest
MAINTAINER CC <chencong@learning-tech.com>

COPY * ./

WORKDIR build

RUN cmake ..
RUN make

WORKDIR ..

CMD ["./bin/webrtc2rtmp"]