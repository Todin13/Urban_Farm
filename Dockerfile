FROM ubuntu:latest

# Install C++ build environment and required libraries
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libcpprest-dev \
    libpqxx-dev

# Copy the source code
COPY . /app

# Build the application
WORKDIR /app
RUN cmake . && make

# Expose the port the REST server listens on
EXPOSE 8080

# Run the application
CMD ["./plant_monitoring_service"]