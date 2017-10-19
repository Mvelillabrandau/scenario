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

  // Se instala NDN stack en todos los nodos
  ndn::StackHelper ndnHelper;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "10000");
  ndnHelper.InstallAll();

  // Set BestRoute strategy
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Getting containers for the consumer/producer
  Ptr<Node> producer = Names::Find<Node>("Node8");
  NodeContainer consumerNodes;
  consumerNodes.Add(Names::Find<Node>("Node0"));


  // Se instalan las aplicaciones NDN
  std::string prefix = "/prefix";

  ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr"); // Se crea la instancia
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

  //ndn::CsTracer::InstallAll("results/cs-trace.txt", Seconds(1)); // Poner cache 

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