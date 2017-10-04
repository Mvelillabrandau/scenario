#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>



#include <iostream>
#include <fstream>


namespace app {

class Consumer
{
public:
  Consumer()
  :m_faceConsumer(m_ioService) // Create face with io_service object
  , m_scheduler(m_ioService)
  {
  }

  void
  run()
  {
    std::ifstream ficheroEntrada;
    std::string frase;
    ficheroEntrada.open ("extensions/consultas.txt");
    //Aplicacion interna del nodo consumidor
    getline (ficheroEntrada,frase); //Obtengo la primera linea
    ndn::Interest interest(frase);
    interest.setInterestLifetime(ndn::time::seconds(5)); //Tiempo de vida en PIT
    interest.setMustBeFresh(true);
    m_faceConsumer.expressInterest(interest,
                          std::bind(&Consumer::onData, this,  _1, _2),
                          std::bind(&Consumer::onNack, this, _1, _2),
                          std::bind(&Consumer::onTimeout, this, _1)); //Mando el primer interes

    // Schedule a new event, Programos los distintos eventos
    int contador = 1;
    while (! ficheroEntrada.eof() ) {
      if ( frase.size( ) > 0 ) std::cout << "Leido: " << frase  << std::endl;
      getline (ficheroEntrada,frase);
      m_scheduler.scheduleEvent(ndn::time::seconds(contador),
                              std::bind(&Consumer::delayedInterest, this, frase)); //Cada 2 segundo se enviara un interes nuevo
      contador = contador + 1;

      }

      std::cout << "\n" << " >> Enviando el siguiente interes: " << interest  << "\n" << std::endl;

    // m_ioService.run() will block until all events finished or m_ioService.stop() is called
    m_ioService.run();
    ficheroEntrada.close();
    // Alternatively, m_face.processEvents() can also be called.
    // processEvents will block until the requested data received or timeout occurs.
    //m_faceConsumer.processEvents();

  }

public:

  void
  onData(const ndn::Interest& interest, const ndn::Data& data)
  {
    //std::cout << data << "\n" << std::endl;
  }
  void
  onNack(const ndn::Interest& interest, const ndn::lp::Nack& nack)
  {
    //std::cout << "received Nack with reason: " << nack.getReason()
      //        << " for interest " << interest << "\n" << std::endl;
    std::cout << " << EL INTERES HA SIDO ENVIADO, PERO NO HA SIDO RESPONDIDO" << "\n" << std::endl;
  }
  void
  onTimeout(const ndn::Interest& interest)
  {
    std::cout << "Timeout \n" << interest << "\n" <<  std::endl;
  }
  void
  delayedInterest(std::string frase)
  {
  std::cout << "=========================================================" << std::endl;
  std::cout << "Un interes mas, de la fucion delayedInterest de scheduler" << std::endl;

  ndn::Interest interest(frase);
  interest.setInterestLifetime(ndn::time::seconds(5)); //Tiempo de vida en PIT
  interest.setMustBeFresh(true);

  m_faceConsumer.expressInterest(interest,
                         std::bind(&Consumer::onData, this, _1, _2),
                         std::bind(&Consumer::onNack, this, _1, _2),
                         std::bind(&Consumer::onTimeout, this, _1));

  std::cout << " >> Enviando el siguiente interes: " << interest  << "\n" << std::endl;

  }

private:
  ndn::Face m_faceConsumer;
  ndn::Scheduler m_scheduler;
  ndn::Face m_faceProducer;
  boost::asio::io_service m_ioService;
  std::string frase;
};

} // namespace app
