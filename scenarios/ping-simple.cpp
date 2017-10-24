/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Spyridon (Spyros) Mastorakis <mastorakis@cs.ucla.edu>
 *          Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"




namespace ns3 {

/**
 * This scenario simulates a very simple network topology:
 *
 *
 *      +----------+     1Mbps      +--------+     1Mbps      +----------+
 *      | consumer | <------------> | router | <------------> | producer |
 *      +----------+         10ms   +--------+          10ms  +----------+
 *
 *
 * Consumer requests data from producer with frequency 10 interests per second
 * (interests contain constantly increasing sequence number).
 *
 * For every received interest, producer replies with a data packet, containing
 * 1024 bytes of virtual payload.
 *
 * To run scenario and see what is happening, use the following command:
 *
 *     ./waf --run=ndn-cxx-simple-ping
 */

int
main(int argc, char *argv[])
{
  // Setting default parameters for PointToPoint links and channels
  // Se configuran los parametros por defecto para los links y canales (Channels) del Punto a punto
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  // Lectura optional para ejecutar el visualizador
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  // Se crean los nodos

  NodeContainer nodes; // Se declara la variable de tipo NodeContainer
  nodes.Create(3); // tres nodos creados 0,1,2

  // Connecting nodes using two links
  // Conexion de los nodos usando dos links

  PointToPointHelper p2p; // Declaracion de variable tipo PointToPointHelper
  p2p.Install(nodes.Get(0), nodes.Get(1)); // Conexion del nodo 0 con nodo 1
  p2p.Install(nodes.Get(1), nodes.Get(2)); // Conexion del nodo 1 con nodo 2


  // Install NDN stack on all nodes
  // Instalacion de NDN stack en todos los nodos, esto sirve para darte la configuracion a los nodos
  ndn::StackHelper ndnHelper; // Se declara la variable de tipo StackHelper

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache"); // Se define la politica de NO CACHE
  ndnHelper.Install(nodes.Get(0)); // Se instala en el nodo 0 (COMSUMIDOR)

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");
  std::cout << "Antes el router \n" << std::endl;
  ndnHelper.Install(nodes.Get(1)); // Se instala la politica de reemplazo en el nodo 1 (ROUTER)
  std::cout << "Despues el router \n" << std::endl;

  ndnHelper.SetOldContentStore("ns3::ndn::cs::Nocache");  // Se define la politica de NO CACHE
  ndnHelper.Install(nodes.Get(2)); // Se instala en el nodo 2 (PRODUCTOR)

  // Se define la estrategia de routeo
  ndn::StrategyChoiceHelper::InstallAll("/example", "/localhost/nfd/strategy/multicast"); // multicast

  // Installing global routing interface on all nodes
  // Se instala la interfaz de ruteo global en todo los nodos
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper; // Se define la variable
  ndnGlobalRoutingHelper.InstallAll();

  // Installing applications

  //Consumer
  ndn::AppHelper ConsumerHelper("ConsumerApp");
  ConsumerHelper.Install(nodes.Get(0))
    .Start(Seconds(5.0));
  std::cout << "Consumidor instalado nodo 0 \n" << std::endl;

  // Producer
  ndn::AppHelper ProducerHelper("ProducerApp");
  ProducerHelper.Install(nodes.Get(2))
    .Start(Seconds(5.0));
  std::cout << "Productor instalado nodo 2 \n" << std::endl;

  ndnGlobalRoutingHelper.AddOrigins("/example", nodes.Get(2));
  ndnGlobalRoutingHelper.AddOrigins("/imagenes", nodes.Get(2));
  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  Simulator::Stop(Seconds(50.0));

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
