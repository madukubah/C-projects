FROM alpine:latest

RUN apk update 

RUN apk add --no-cache gcc musl-dev make
RUN apk add mandoc
RUN apk add man-pages
RUN apk add busybox-extras
RUN apk add nano

# for building alpine packages
RUN apk add abuild build-base fakeroot

RUN apk add git
RUN apk add vim

RUN apk add linux-virt
RUN apk add linux-lts
RUN apk add openvpn
RUN apk add autoconf automake
RUN apk add libtool

RUN apk add samba-client

# RUN apk add nmap


RUN adduser -D mdkh
USER mdkh
WORKDIR /home/mdkh
CMD ["sh"]
