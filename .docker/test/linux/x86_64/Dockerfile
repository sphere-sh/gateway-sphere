FROM alpine:3.22 AS test_runner

WORKDIR /app

# Install build tools
#
RUN apk add --no-cache gcc g++ make git

# Install cmake 4
#
RUN apk add --no-cache cmake
RUN apk add --upgrade cmake --repository=http://dl-cdn.alpinelinux.org/alpine/edge/main

# Copy the source code respecting .dockerignore
#
COPY . .

# Build the project using CMake
#
RUN mkdir build && cd build && cmake .. && make

WORKDIR /app/build

# Run tests
#
CMD ["ctest", "--output-on-failure"]
