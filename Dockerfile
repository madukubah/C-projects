FROM alpine:latest

USER root

RUN apk update 
RUN apk add --no-cache gcc musl-dev make
RUN apk add mandoc
RUN apk add man-pages

RUN apk add busybox-extras

WORKDIR /etc/app

CMD ["sh"]