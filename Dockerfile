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

RUN adduser -D mdkh
USER mdkh
WORKDIR /home/mdkh
CMD ["sh"]
