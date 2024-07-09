FROM ubuntu:latest
RUN apt-get update && apt-get install -y build-essential cmake libsdl2-dev x11-apps
COPY . /app
WORKDIR /app

# Set up XDG_RUNTIME_DIR
ENV XDG_RUNTIME_DIR=/tmp/runtime
RUN mkdir -p $XDG_RUNTIME_DIR && chmod 0700 $XDG_RUNTIME_DIR

RUN cmake .
RUN make

# Set DISPLAY environment variable before running the application
ENV DISPLAY=:0

CMD ["./RenderEngine"]