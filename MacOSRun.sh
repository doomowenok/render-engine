#!/bin/bash

# Build Docker Image
docker build -t render-engine .

# Allow local Docker access to X11
xhost +local:docker

# Run Docker Container with X11 forward 
docker run -e DISPLAY=$DISPLAY -e XDG_RUNTIME_DIR=/tmp/runtime -v /tmp/.X11-unix:/tmp/.X11-unix render-engine