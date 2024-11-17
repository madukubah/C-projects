## simple webserver
this project mimicking apache webserver at the minimum viable product (MVP)
## TODO
### main
- develop CGI (Common Gateway Interface) to run dynamic file such as .php, .js
- optimize parsing: develop DFA so it can parsing entire raw-request
### request_hanlder
- add more extention for images and other file ext
- develop regex or string internings
### routing
- add security/middleware for accescing forbidden path

## request example
```
GET / HTTP/1.1
Host: localhost:666
Connection: keep-alive
sec-ch-ua: "Chromium";v="128", "Not;A=Brand";v="24", "Google Chrome";v="128"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "macOS"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: en-US,en;q=0.9,id;q=0.8
Cookie: visitor_uuid=250c8be3314d4182bb188c40ca088245; frontend_lang=en_US; tz=Asia/Makassar; checkface1=false; checkface2=false; checkface3=false; doubleco=false; stop1=true; fileToken=dummy-because-api-expects-one; session_id=3db2bd37a71ab780750d129a41d86b28c67f327f
```

## run
```
sh build.sh
cd build/
./simple_httpd
```
