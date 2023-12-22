#include "PIR_Client.h"

#include "AddShare.h"
#include "ShamirShare.h"
#include "utils.h"
#include "params.h"

PIRClient::PIRClient(
    std::string seed,
    std::vector<std::pair<const boost::asio::ip::address, uint16_t>>& sockets,
    size_t num_server, bool use_shamir, size_t privacy)
    : privacy(privacy),
      num_server(num_server),
      he(SealClient::create_params(params::MOD_DEGREE, params::PLAINTEXT_MOD)),
      use_shamir(use_shamir){
  if (sockets.size() != num_server)
    throw std::runtime_error("Wrong number of sockets");

  network.reserve(num_server);
  for (size_t i = 0; i < num_server; i++)
    network.push_back(std::make_unique<ClientNetworkAdapter>(
        ClientNetworkAdapter(utils::get_ciphertext_size(params::MOD_DEGREE),
                             sockets[i].first, sockets[i].second)));

  // set share
  if(use_shamir){
    share = std::make_unique<ShamirShare>(params::PLAINTEXT_MOD);
  }
  else {
    share = std::make_unique<AddShare>(params::PLAINTEXT_MOD);
  }

  share->seed_rng(seed);
  if(use_shamir){
    auto degree = num_server - this->privacy - 1;
    dynamic_cast<ShamirShare*>(share.get())->set_degree(degree);
  }
  share->preprocess(num_server);
}

// hint: shares queries and reciev response from the server and reconstruct here
void PIRClient::fetch_image(size_t index, size_t num_images,
                            std::string& out_filename) {
  if (index >= num_images) throw std::runtime_error("Wrong index provided");

  std::vector<std::vector<uint64_t>> sharesPerServer(num_server);
  for(int i = 0; i < num_images; i++)
  {
    std::map<uint64_t, uint64_t> shares;

    if(i == index){
      shares = share->share(htole64(1), num_server);
    }
    else{
      shares = share->share(htole64(0), num_server);
    }

    //auto reconstructedShare = share->reconstruct(shares);
    //std::cout<<"Reconstructed share is "<<reconstructedShare<<"\n";

    for(const auto& share : shares)
    {
      std::vector<uint64_t> shareVec(he.get_slots(), share.second);
      auto encryptedShare = he.encrypt(shareVec);
      network.at(share.first - 1)->send_ciphertext(encryptedShare);
    }
  }

  std::cout<<"Sent all shares\n";

  std::vector<uint64_t> image;
  
  std::vector<std::vector<uint64_t>> decryptedTexts;
  for(int i = 0; i < num_server; i++){
    auto response = network.at(i)->receive_ciphertext(he.get_context());
    //std::cout<<"Noise in response "<<he.get_noise(response)<<"\n";
    decryptedTexts.push_back(he.decrypt(response));
  }

  std::cout<<"Received all responses from server\n";

  if(use_shamir && decryptedTexts.size() <= num_server / 2)
  {
    throw std::runtime_error("Not enough responses received");
  }

  if(!use_shamir && decryptedTexts.size() < num_server)
  {
    throw std::runtime_error("Not enough responses received");
  }

  /*for(int i = 0; i < decryptedTexts.at(0).size(); i++)
  {
    std::cout<<htole64(decryptedTexts.at(0).at(i))<<", ";
  }*/

  //std::cout<<"ENDED\n";

  for(int i = 0; i < he.get_slots(); i++)
  {
    std::map<uint64_t, uint64_t> toReconstruct;
    for(int j = 0; j < decryptedTexts.size(); j++)
    {
      toReconstruct[j + 1] = htole64(decryptedTexts.at(j).at(i));
    }

    uint64_t word = share->reconstruct(toReconstruct);
    image.push_back(htole64(word));
  }
  

  /*for(int i = 0; i < image.size(); i++)
  {
    std::cout<<htole64(image.at(i))<<", ";
  }*/

  utils::write_image(out_filename, image, params::NUM_BYTES);
}
