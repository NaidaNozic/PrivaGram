# PrivaGram
<h2>Private Information Retrieval protocol</h2>

<p> The application implements the <strong>private information retrieval protocol</strong>, which allows the user to fetch an item from a database without the database server knowing which item the user wants to retrieve. In this specific implementation, the server contains images and the user sends the index of the desired image.</p>

<p>The procol can be instantiated with both <strong>Additive</strong> and <strong>Shamir secret sharing</strong>. These schemes divide the secret (index of the image) into multiple shares to send to the servers, which ensures that no server can know the true value of the index.
In order to reconstruct the original secret, we need a certain number of shares (threshold) and this step will be conducted on the client side after receiving results from the server.</p>

<p>The program uses <strong>Homomorphic Encryption</strong>, which enables computations on encrypted data without decrypting it first. For this step, we use the BFV cryptosystem and its implementation in the <a href="https://github.com/microsoft/SEAL" target="_blank">SEAL library</a>.</p>

## Dependencies
- boost 1.7
- [Microsoft SEAL 3.7](https://github.com/Microsoft/SEAL)

## Building the project
Building SEAL:
```shell
cd PIR
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
Additionally, there is a testscript in the tests folder, which fetches all 40 images from a configurable number of servers.

To execute the protocol with 2 servers and additive secret sharing, invoke:
```shell
python3 pir_test.py -n 2
```

To execute the protocol with 3 servers, Shamir sharing and a privacy of up to 2 colluding servers, invoke:
```shell
python3 pir_test.py -n 3 --shamir=1 -t 2
```


