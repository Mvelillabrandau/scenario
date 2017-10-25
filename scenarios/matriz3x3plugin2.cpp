// ndn-grid-topo-plugin.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[]) // main
{
  CommandLine cmd;
  cmd.Parse(argc, argv);

  AnnotatedTopologyReader topologyReader("", 25); 
  topologyReader.SetFileName("extensions/topo-grid-3x3.txt"); // Se llama al archivo 
  topologyReader.Read(); // Lecura del archivo con la topologia.

  // Instancio el objeto consumerNodes
  NodeContainer nodo1, nodo2, nodo3, nodo4, nodo5, nodo6 ,nodo7, consumerNodes;

  // Se instala NDN stack en todos los nodos
  ndn::StackHelper ndnHelper;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Double", "MaxSize", "10000");

  nodo1.Add(Names::Find<Node>("Node1"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo1); // Se instala la politica de reemplazo en el nodo 1 (ROUTER)
  std::cout << "Instalando en el router 1 \n" << std::endl;
  nodo2.Add(Names::Find<Node>("Node2"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo2); // Se instala la politica de reemplazo en el nodo 2 (ROUTER)
  std::cout << "Instalando en el router 2 \n" << std::endl;
  nodo3.Add(Names::Find<Node>("Node3"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo3); // Se instala la politica de reemplazo en el nodo 3 (ROUTER)
  std::cout << "Instalando en el router 3 \n" << std::endl;
  nodo4.Add(Names::Find<Node>("Node4"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo4); // Se instala la politica de reemplazo en el nodo 4 (ROUTER)
  std::cout << "Instalando en el router 4 \n" << std::endl;
  nodo5.Add(Names::Find<Node>("Node5"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo5); // Se instala la politica de reemplazo en el nodo 5 (ROUTER)
  std::cout << "Instalando en el router 5 \n" << std::endl;
  nodo6.Add(Names::Find<Node>("Node6"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo6); // Se instala la politica de reemplazo en el nodo 6 (ROUTER)
  std::cout << "Instalando en el router 6 \n" << std::endl;
  nodo7.Add(Names::Find<Node>("Node7"));// Se busca el nodo segun nombre
  ndnHelper.Install(nodo7); // Se instala la politica de reemplazo en el nodo 7 (ROUTER)
  std::cout << "Instalando en el router 7 \n" << std::endl;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache"); // Se define la politica de NO CACHE
  consumerNodes.Add(Names::Find<Node>("Node0")); // Se busca el nodo segun nombre
  ndnHelper.Install(consumerNodes); // Se instala la politica de reemplazo en el nodo CONSUMIDOR
  std::cout << "Instalando en el router consumidor \n" << std::endl;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache"); // Se define la politica de NO CACHE
  Ptr<Node> producer = Names::Find<Node>("Node8");// Se busca el nodo segun nombre
  ndnHelper.Install(producer); // Se instala la politica de reemplazo en el nodo PRODUCTOR
  std::cout << "Instalando en el router PRODUCTOR \n" << std::endl;

  // Set BestRoute strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();


  // Se instalan las aplicaciones NDN
  std::string prefix = "/prefix";

  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr2"); // Se crea la instancia
  consumerHelper.SetPrefix(prefix); // Seteo del prefijo
  consumerHelper.SetAttribute("Frequency", StringValue("1")); // Seteo de la frecuencia en que enviara 
                                                              // los intereses (1 por segundo).
  consumerHelper.Install(consumerNodes); // Se installa la aplicacion en uno o mas nodos.

  ndn::AppHelper producerHelper("ns3::ndn::Producer"); // Se crea la instancia
  producerHelper.SetPrefix(prefix); //Seteo del prefijo
  producerHelper.SetAttribute("PayloadSize", StringValue("1024")); //
  producerHelper.Install(producer); // Se instala la aplicacion en una o mas nodos.

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins(prefix, producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(20.0));

  ndn::CsTracer::InstallAll("results/cs-trace.txt", Seconds(1)); // Poner cache 

  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}