#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <iostream>


namespace app {

class Producer
{
public:
  Producer(ndn::KeyChain& keyChain)
    : m_keyChain(keyChain)
    , m_faceProducer(m_faceConsumer.getIoService())
    , m_scheduler(m_faceConsumer.getIoService())
  {

      m_faceProducer.setInterestFilter("/example/testApp",
                         std::bind(&Producer::onInterest, this, _1, _2),
                         ndn::RegisterPrefixSuccessCallback(),
                         std::bind(&Producer::onRegisterFailed, this, _1, _2));

      m_faceProducer.setInterestFilter("/imagenes/santiago",
                         std::bind(&Producer::onInterest, this, _1, _2),
                         std::bind([]{}),
                         std::bind(&Producer::onRegisterFailed, this, _1, _2));

  }

  void
  run()
  {
    m_faceProducer.processEvents(); // ok (will not block and do nothing)
    // m_faceConsumer.getIoService().run(); // will crash
  }

private:
  void
  onInterest(const ndn::InterestFilter& filter, const ndn::Interest& interest)
  {
    std::cout <<"EL INTERES A SIDO ENVIADO Y RESPONDIDO"<< "\n" << std::endl;
    std::cout << "<< Interes:  " << interest << "\n" <<std::endl;

    // Create new name, based on Interest's name
    /*ndn::Name dataName(interest.getName());
    dataName
      .append("testApp") // add "testApp" component to Interest name
      .appendVersion();  // add "version" component (current UNIX timestamp in milliseconds)*/

    static const std::string content = "HELLO KITTY";

    // Create Data packet
    std::shared_ptr<ndn::Data> data = std::make_shared<ndn::Data>();
    data->setName(interest.getName());
    data->setFreshnessPeriod(ndn::time::seconds(2));
    data->setContent(reinterpret_cast<const uint8_t*>(content.c_str()), content.size());

    /*auto data = std::make_shared<ndn::Data>(interest.getName());
    data->setFreshnessPeriod(ndn::time::milliseconds(1000));
    data->setContent(std::make_shared< ::ndn::Buffer>(1024));*/
    //ndn::StackHelper::getKeyChain().sign(*data);

    // Sign Data packet with default identity
    m_keyChain.sign(*data);
    // m_keyChain.sign(data, <identityName>);
    // m_keyChain.sign(data, <certificate>);

    // Return Data packet to the requester
    std::cout << ">> Dato: " << *data << std::endl;
    m_faceProducer.put(*data);
  }
  void
  onRegisterFailed(const ndn::Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix \""
              << prefix << "\" in local hub's daemon (" << reason << ")"
              << std::endl;
    m_faceProducer.shutdown();
  }

private:
  ndn::KeyChain& m_keyChain;
  ndn::Face m_faceConsumer;
  ndn::Face m_faceProducer;
  ndn::Scheduler m_scheduler;
};

} // namespace app
