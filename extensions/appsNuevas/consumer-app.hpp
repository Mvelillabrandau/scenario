#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/application.h"

#include "consumer.hpp"

#include "ns3/ndnSIM-module.h"
#include "ns3/integer.h"
#include "ns3/string.h"

namespace ns3 {


// Class inheriting from ns3::Application
class ConsumerApp : public Application
{
public:
  static TypeId
  GetTypeId()
  {
    static TypeId tid = TypeId("ConsumerApp")
      .SetParent<Application>()
      .AddConstructor<ConsumerApp>();

    return tid;
  }

protected:
  // inherited from Application base class.
  virtual void
  StartApplication()
  {
    //Create an instance of the app, and passing the dummy version of KeyChain (no real signing)
    m_instance.reset(new app::Consumer);
    //m_instance->setNPings(m_nPings);
    m_instance->run(); // can be omitted

  }

  virtual void
  StopApplication()
  {
    //Stop and destroy the instance of the app
    m_instance.reset();
  }

private:
  std::unique_ptr<app::Consumer> m_instance;
  //uint32_t m_nPings;
};

} // namespace ns3
