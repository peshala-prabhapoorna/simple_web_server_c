# Minimalist Web Server
a simple minimalist web server in C on Linux. made by following the tutorial  
by Nir Lichman - [Making Minimalist Web Server in C on Linux | Nir Lichtman](https://www.youtube.com/watch?v=2HrYIl6GpYg)
  
compile the web server:
```
make web_server.c
```
run the web server:
```
./web_server
```
test the web server:
```
curl localhost:8000/FILE
```
replace FILE with an html file in the root directory.  
  
>[!WARNING]
>it may take some time to free the port binded to the web server. therefore  
>re-running the web server immediately might not work. wait for a little  
>while before running the web server again.
>
>run the web server with `strace` to see if the port is active.  
>`strace ./web_server`
