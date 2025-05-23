FROM debian:bullseye

RUN apt-get update && apt-get install -y build-essential libssl-dev

WORKDIR /app

# Copy all CivetWeb files including civetweb.c, civetweb.h, md5.inl, and your main.c
COPY . .

# Build with Makefile that links -ldl to fix dlopen/dlsym/dlclose errors
RUN make

EXPOSE 8080

CMD ["./webmenu"]
