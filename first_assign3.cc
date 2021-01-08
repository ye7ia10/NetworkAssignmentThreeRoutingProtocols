
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"


using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("Point to Point Graph Simulation Assignment 3");

int main (int argc, char *argv[]) {
  
  // Allow the user to override any of the defaults 
  CommandLine cmd;  
  cmd.Parse (argc, argv);

  // for the simulation like in first.cc
  Time::SetResolution (Time::NS);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
	


  // Here, we will explicitly create four nodes.  In more sophisticated
  // topologies, we could configure a node factory.
  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes_container;
  nodes_container.Create (6);  // six nodes in the given graph and eight edges.
  NodeContainer n0_1 = NodeContainer (nodes_container.Get (0), nodes_container.Get (1));
  NodeContainer n0_2 = NodeContainer (nodes_container.Get (0), nodes_container.Get (2));
  NodeContainer n1_2 = NodeContainer (nodes_container.Get (1), nodes_container.Get (2));
  NodeContainer n1_3 = NodeContainer (nodes_container.Get (1), nodes_container.Get (3));
  NodeContainer n1_4 = NodeContainer (nodes_container.Get (1), nodes_container.Get (4));
  NodeContainer n2_4 = NodeContainer (nodes_container.Get (2), nodes_container.Get (4));
  NodeContainer n3_4 = NodeContainer (nodes_container.Get (3), nodes_container.Get (4));
  NodeContainer n4_5 = NodeContainer (nodes_container.Get (4), nodes_container.Get (5));
  

  //Creating and Installing Stack Helper for the nodes container.
  NS_LOG_INFO ("Create stack.");
  InternetStackHelper internet_stack;
  internet_stack.Install (nodes_container);

  //Creating Net Device Containers for the n's
  NS_LOG_INFO ("Create channels.");
  PointToPointHelper p2p;
  NetDeviceContainer d0_1 = p2p.Install (n0_1);
  NetDeviceContainer d0_2 = p2p.Install (n0_2);
  NetDeviceContainer d1_2 = p2p.Install (n1_2);
  NetDeviceContainer d1_3 = p2p.Install (n1_3);
  NetDeviceContainer d1_4 = p2p.Install (n1_4);
  NetDeviceContainer d2_4 = p2p.Install (n2_4);
  NetDeviceContainer d3_4 = p2p.Install (n3_4);
  NetDeviceContainer d4_5 = p2p.Install (n4_5);



  // Later, we add IP addresses and assign interfaces.
  Ipv4AddressHelper ipv4_helper;
  ipv4_helper.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i1 = ipv4_helper.Assign (d0_1);
  ipv4_helper.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4_helper.Assign (d0_2);
  ipv4_helper.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4_helper.Assign (d1_2);
  ipv4_helper.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i3 = ipv4_helper.Assign (d1_3);
  ipv4_helper.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i4 = ipv4_helper.Assign (d1_4);
  ipv4_helper.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i4 = ipv4_helper.Assign (d2_4);
  ipv4_helper.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer i3i4 = ipv4_helper.Assign (d3_4);
  ipv4_helper.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer i4i5 = ipv4_helper.Assign (d4_5);
  


  //printing the routing table
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  OutputStreamWrapper wrapper = OutputStreamWrapper (&std::cout);
  Ipv4GlobalRoutingHelper::PrintRoutingTableAllAt(Time(), &wrapper, Time::NS);   
  Ipv4GlobalRoutingHelper g;
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper>("assign3-routing-table.routes", std::ios::out);
  g.PrintRoutingTableAllAt(Seconds(12), routingStream);
  

  // server
  uint16_t port = 9;
  UdpEchoServerHelper server(port);
  ApplicationContainer serverApp = server.Install(nodes_container.Get(5));
  serverApp.Start(Seconds (1.0));
  serverApp.Stop(Seconds (10.0));
  // client
  UdpEchoClientHelper client(i4i5.GetAddress(1), port);
  client.SetAttribute("MaxPackets", UintegerValue(1));
  client.SetAttribute("Interval", TimeValue(Seconds (1.0)));
  client.SetAttribute("PacketSize", UintegerValue(1024));
  ApplicationContainer clientApp = client.Install(nodes_container.Get(0));
  clientApp.Start(Seconds (2.0));
  clientApp.Stop(Seconds (10.0));


  
  //creating the animation xml file
  AnimationInterface anim ("assign3_anim.xml");
  anim.SetConstantPosition(nodes_container.Get(0),10.0,50.0);
  anim.SetConstantPosition(nodes_container.Get(1),40.0,20.0);
  anim.SetConstantPosition(nodes_container.Get(2),40.0,80.0);
  anim.SetConstantPosition(nodes_container.Get(3),70.0,20.0);
  anim.SetConstantPosition(nodes_container.Get(4),70.0,80.0);
  anim.SetConstantPosition(nodes_container.Get(5),100.0,80.0);
  //run simulator
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
