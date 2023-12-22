#include "PIR_Server.h"

#include "utils.h"
#include "params.h"

PIRServer::PIRServer(uint16_t port, size_t num_images, std::string db_path)
    : network(utils::get_ciphertext_size(params::MOD_DEGREE), port),
      he(SealServer::create_params(params::MOD_DEGREE, params::PLAINTEXT_MOD)),
      db_path(db_path) {
  read_database(num_images);
}

void PIRServer::read_database(size_t num_images) {
  for (size_t i = 0; i < num_images; i++)
    database.push_back(utils::read_image(
        db_path + "/img" + std::to_string(i) + ".jpg", params::NUM_BYTES));
}

void PIRServer::respond() {

  /*for(int i = 0; i < database.at(0).size(); i++)
  {
    std::cout<<database.at(0).at(i)<<", ";
  }

  std::cout<<"\n";*/

  seal::Ciphertext response;
  for(int i = 0; i < database.size(); i++)
  {
    seal::Ciphertext request = network.receive_ciphertext(he.get_context());
    //std::cout<<"Request received\n";

    if(i == 0)
    {
      he.multiply_plain_inplace(request, database.at(i));
      response = request;
      continue;
    }

    seal::Ciphertext dst;
    he.multiply_plain_inplace(request, database.at(i));
    he.add_inplace(response, request);
  }

  
  network.send_ciphertext(response);
}



uint64_t PIRServer::getMaxWordsPerImage()
{
  size_t maxSize = 0;

  for(int i = 0; i < database.size(); i++)
  {
    if(database.at(i).size() > maxSize){
      maxSize = database.at(i).size();
    }
  }

  return maxSize;
}
