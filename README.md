# PrivaGram
Private Information Retrieval protocol

<p> The application implements the private information retrieval protocol, which allows the user to fetch an item from a database without the database server knowing which item the user wants to retrieve. In this specific implementation, the server contains images and the user sends the index of the desired image.</p>

<p>The procol can be instantiated with both Additive and Shamir secret sharing. These schemes divide the secret (index of the image) into multiple shares to send to the servers, which ensures that no server can know the true value of the index.
In order to reconstruct the original secret, we need a certain number of shares (threshold) and this step will be conducted on the client side after receiving results from the server.</p>

<p>The program uses Homomorphic Encryption, which enables computations on encrypted data without decrypting it first. For this step, we use the BFV cryptosystem and its implementation in the <a href="https://github.com/microsoft/SEAL" target="_blank">SEAL library</a>.</p>


