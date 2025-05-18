FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    libpthread-stubs0-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Add compiler flags for OpenSSL (pass via environment variable)
ENV CFLAGS="-Wall -O2 -DUSE_OPENSSL"
ENV LDFLAGS="-lpthread -lssl -lcrypto"

RUN make

EXPOSE 8080
CMD ["./webmenu"]
