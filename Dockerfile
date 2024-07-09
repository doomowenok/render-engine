FROM ubuntu:latest
RUN apt-get update && apt-get install -y build-essential cmake libsdl2-dev
COPY . /app
WORKDIR /app
RUN cmake .
RUN make
CMD ["./RenderEngine"]