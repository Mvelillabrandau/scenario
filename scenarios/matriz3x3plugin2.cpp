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
  //NodeContainer consumerNodes;

  // Se instala la pila NDN en algunos nodos.
  ndn::StackHelper ndnHelper; 
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10");

  ndnHelper.Install(Names::Find<Node>("Node1")); 
  //std::cout << "Instalando en el router 1 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node2")); 
  //std::cout << "Instalando en el router 2 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node3")); 
  //std::cout << "Instalando en el router 3 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node4")); 
  //std::cout << "Instalando en el router 4 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node5")); 
  //std::cout << "Instalando en el router 5 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node6"));
  //std::cout << "Instalando en el router 6 \n" << std::endl;
  ndnHelper.Install(Names::Find<Node>("Node7"));
  //std::cout << "Instalando en el router 7 \n" << std::endl;
  
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache"); // Se define la politica de NO CACHE
  Ptr<Node> consumer = Names::Find<Node>("Node0");
  //consumerNodes.Add(Names::Find<Node>("Node0")); // Se busca el nodo segun nombre
  ndnHelper.Install(consumer); // Se instala la politica de reemplazo en el nodo CONSUMIDOR
  //std::cout << "Instalando en el router consumidor \n" << std::endl;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache"); // Se define la politica de NO CACHE
  Ptr<Node> producer = Names::Find<Node>("Node8");// Se busca el nodo segun nombre
  ndnHelper.Install(producer); // Se instala la politica de reemplazo en el nodo PRODUCTOR
  //std::cout << "Instalando en el router PRODUCTOR \n" << std::endl;

  std::string prefix = "/prefix";

  // Configura la estrategia de enrutamiento
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/multicast");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  
  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr2"); // Se crea la instancia
  consumerHelper.SetPrefix(prefix); // Seteo del prefijo
  consumerHelper.SetAttribute("Frequency", DoubleValue (1));
  //consumerHelper.SetAttribute("Randomize", StringValue("uniform")); // Seteo de la frecuencia en que enviara 
                                                              // los intereses (1 por segundo).
  consumerHelper.Install(consumer); // Se installa la aplicacion en uno o mas nodos.

  ndn::AppHelper producerHelper("ns3::ndn::Producer2"); // Se crea la instancia
  producerHelper.SetPrefix(prefix); //Seteo del prefijo
  producerHelper.SetAttribute("PayloadSize", StringValue("1024")); //
  producerHelper.Install(producer); // Se instala la aplicacion en una o mas nodos.



  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins(prefix, producer);
  //ndnGlobalRoutingHelper.AddOrigins(prefix1, producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(20.0));

  //ndn::CsTracer::InstallAll("results/cs-trace-T2.txt", Seconds(0.5));
  //ndn::L3RateTracer::InstallAll("results/rate-trace-T2.txt", Seconds(0.5));
  //L2RateTracer::InstallAll("results/drop-trace-T2.txt", Seconds(0.5));


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