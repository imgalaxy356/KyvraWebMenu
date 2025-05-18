FROM debian:bullseye

RUN apt-get update && apt-get install -y build-essential

WORKDIR /app
COPY . .

RUN make

EXPOSE 8080
CMD ["./webmenu"]
