# PETS Exercise 2
## Private Information Retrieval
This is the framework for the third (and final!) exercise in PETS.
It is recommended to first look at the examples provided by SEAL to understand what is provided.

## Dependencies
- boost 1.7
- [Microsoft SEAL 3.7](https://github.com/Microsoft/SEAL)

## Building the project
Building SEAL:
```shell
cd SEAL
mkdir build && cd build
cmake ..
make
cd ..
```

Building the framework:
```shell
mkdir build && cd build
cmake ..
make
```

## Testing
From the build directory:
### start _n_ servers
Start e.g. three of them locally using distinct ports (-p) , e.g.:

```shell
cd build
./server -p 11111 -d ../database/ -n 40
./server -p 22222 -d ../database/ -n 40
./server -p 33333 -d ../database/ -n 40
```
The option -d describes the path to the database, -n the number of entries in the database (you may also test with less). Execute `./server -h` for more information on how to run the server.

### retrieve an image
To retrieve e.g. the 7th image (-i) from the database, execute

```shell
./client -i 7 -o out.jpg -n 40 --servers 127.0.0.1:11111 127.0.0.1:22222 127.0.0.1:33333
```

You have to provide the servers explicitly. You can configure Shamir secret sharing with privacy against 2 servers with the additional flags `--shamir=1 -t 2`. Execute `./client -h` for more information on how to run the client.

### Test script
Additionally, we provide a testscript in the tests folder, which fetches all 40 images from a configurable number of servers.

To execute the protocol with 2 servers and additive secret sharing, invoke:
```shell
python3 pir_test.py -n 2
```

To execute the protocol with 3 servers, Shamir sharing and a privacy of up to 2 colluding servers, invoke:
```shell
python3 pir_test.py -n 3 --shamir=1 -t 2
```
