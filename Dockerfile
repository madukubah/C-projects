FROM alpine:latest

USER root

RUN apk update 
RUN apk add --no-cache gcc musl-dev make
RUN apk add mandoc
RUN apk add man-pages

RUN apk add busybox-extras

RUN mkdir -p /var/www/html/public

# RUN cd /var/www/html/public
# RUN echo "<p>hello world</p>" > /var/www/html/public/index.html

WORKDIR /etc/app
CMD ["sh"]