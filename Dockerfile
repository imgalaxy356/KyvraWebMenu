FROM debian:bullseye

# Install build tools and libraries required for compilation
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    libpthread-stubs0-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source files into the container
COPY . .

# Build the project using make
RUN make

# Expose port 8080 for the app
EXPOSE 8080

# Run the compiled binary
CMD ["./webmenu"]
