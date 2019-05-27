FROM ubuntu:18.04

WORKDIR /programm

RUN apt-get update && \
    apt-get install -y --no-install-recommends cmake g++ libssl-dev make \
    libboost-system-dev libboost-program-options-dev libboost-log-dev

COPY ./src/ ./
RUN cmake . && make 

CMD [ "./generate_signature" ]

