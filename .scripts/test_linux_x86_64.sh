#!/bin/bash

# Build the docker image
docker build -t gateway_sphere:x86_64.0.0.0 -f .docker/test/linux/x86_64/Dockerfile .

# Run the docker image
docker run gateway_sphere:x86_64.0.0.0