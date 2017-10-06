#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/application.h"

#include "producer.hpp"

namespace ns3 {


// Class inheriting from ns3::Application
class ProducerApp : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("ProducerApp")
      .SetParent<Application>()
      .AddConstructor<ProducerApp>();

    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    //Create an instance of the app, and passing the dummy version of KeyChain (no real signing)
    m_instance.reset(new app::Producer(ndn::StackHelper::getKeyChain()));
    m_instance->run(); // can be omitted
  }

  virtual void
  StopApplication()
  {
    //Stop and destroy the instance of the app
    m_instance.reset();
  }

private:
  std::unique_ptr<app::Producer> m_instance;
};

} // namespace ns3
