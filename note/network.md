
# build a network

it's about how to connect 2 nodes together, annd we have 3 issues to
consider:
1. encoding problem  
   how to represent 0 and 1
2. framing problem  
   how to represent a meaningful frame
3. access control problem
   how to share a link

## encoding problem
a good encoding scheme makes voltage change all the time. the receiver
calculate the average voltage and use it as a baseline to determine
'low voltage' and 'high voltage', a constant changing voltage makes
the average accurate, and also makes the receiver's clock synchronized
with the sender all the time.

Ethenet use 4B/5B + NRZI(Not Return Zero Inverted) encoding scheme.
the NRZI scheme uses a voltage jump to represent 1 and no-jump to
represent 0, and 4B/5B scheme makes sure the 5-bits encoding for
4-bits data do not create long sequence 0s. neat!

## framing problem
as for the framing problem, frame header + crc is the common
solution. it is possible(WIFI provide this) to use a retransmit
strategy to give a reliable transmission, often requiring a sliding
window to make the transmission efficient, plus the acknowkedge +
timeouts mechanism.

## media access control problem
a unique MAC address is assigned to each node.

the media access control strategy for Ethenet is called CSMA/CD,
Carrier Sense Multiple Access with Collision Detection. all nodes can
distinguish if the link is busy or idle(hense carrier sense), and when
the sender detects a collision, it retransmit the data after waitting
for some time(hence collision detection). in order for the collision
detection to work, the link can not be too long, because the frame
size has a upper limit, which means the max data emitting time has an
upper bound.

the media access control strategy for WIFI is called CSMA/CA, Carrier
Sense Multiple Access with Collision Avoidance. it's impossible for
the sender to detect all collision in WIFI, the sender need to wait
for a ACK frame from the receiver to make sure that the delivery
suceeds and no collision happens(hence collision avoidance).

nowdays Ethenet is more like a point-to-point link, the node links to
a switch directly. WIFI is by its nature a shared link for the nodes
and media access control is essential.

# build an internet

it's about how to connect 2 networks(same or different) together.

the core problems here are:
1. how to wire up the link
2. how to provide an unified service for all underlying different
   networks

## wire-up links
the device that wires up many links is called switch. a switch is a
multi-links in, multi-links out device and runs the appropriate data
link protocols for each different link, so that it can send frames
from one network to other networks. the data switching process is also
called forwarding.

in order for the forwarding to work, the switch must peek the frame
header, then consulting its forwarding table, which contains
(dest-addr, out-port) pairs, to choose a out link for the data.

the problem is how to configure the forwarding table for each
switches. manual configuration is tedious and error-prone. Ethernet
switch has a learning strategy: it broadcast the frame if it does not
know which out-port to use, and adds the (source-addr, in-port) for
each frame to its forwarding table. the learning switch needs the
spanning tree algorithm to make sure the frame do not loop forever.

using a Ethernet switch to connect many Ethernet, you can send frame
to a node in other Etnernet, assuming you know the Ethernet address of
that node. this is an example of internet that's not using IP. as you
see, IP is not the only solution to build a internet.

aside from the forwarding table, the switch also support VC(virtual
circuits) through the vc table, whose entry format is like:
(input-port, vc-id-1, output-port, vc-id-2).

## logical network abstraction
when we wire-up different kinds of networks, we wanna hide their
differences and give a simple and unified network service to the upper
applications. here the Internet Protocol comes into play.

the key idea is, we define a network service that EVERY network can
provide. every concrete network uses their specific capability to
support a unified network service, so that their implementation detail
does not matter to upper layers. all these different networks can be
seen as the same network providing the same service.

IP defines the mentioned 'unified service' as the unreliable(best
effort) datagram service. this is the magic. this service is so damn
simple that all networks so far we've built are qualified to provide!

for its datagram service, IP defines the variable-length IP
packet. this packet could be cut into fragments(called fragmentation)
if packed size exceeds the physical frame size of the undering
network. the fragments will be reassembled in the destination ndoe.

IP also defines a global addressing scheme, the IP address.

## ip forwarding
ip address are hierarchical. an ip address can be devided into a
network part, and a host part.

in the early days, id addesses are categoried as A, B, and C
classes. today it's totally obsolete and the ip addresses are
classless. this is called CIDR, or supernet, and the main motivation
is to save the forwarding table of routers. in the router's forward
table, some (addr-prefix, out-port) entries are stored. if the
incoming address matches multiple prefix, the longest one counts.

subnetting is useful for organizations. the network part is calculated
by address, and a network mask.

supernet and subnet are so widely used today. you can say that ip
forwarding is prefix based. if the destination address and the router
have the same prefix, the router thinks the destination node is in the
same network with router, no forwarding is needed. if not, the router
use the prefix to search the forward table, finding the output port.

the forward table building process is called routing. you can think of
routing as a distrubuted calculation process that calculate the
shortest paths between every two networks, and filling the forward
tables using the results.

## ip tunnel
conceptually, an ip tunnel is a point-to-point link of two nodes, in
the IP internet. the transmitting data inside the tunnel could be
anything.

the ip tunnel usecases include: VPN and IPv6 over IPv4.

# TCP
a cool transport protocol, providing a reliable, bi-directional,
flow-control/congestion-contorl enabled, byte stream transportation.

for the transportation in one directon, both sender and receiver
maintain a buffer for the sliding window algorithm. the sender us a sequence No to label each undering packet, and the receiver acknowledge the data received to sender. the receiver also advertise its current buffer capaticy to the sender, so that the sender do not 

## use three-way handshake to start a connection
the connection-establishing process is actually about the client and
the server exchanging their sliding-window parameters, including the
starting sequence no, and the buffer size.

because of the data exchanging, you need at least 3 messages.

## connection tear-down
unlike the connection-establishing process, both directions need a standalone tearing down process.

for each tearing down process, both the tearing-down initiator and the
tearing-down acceptor must send a FIN to the peer, and acknowledge the
receiving FIN, hence need 4 messages.

## TIME_WAIT state
the tearing-down initiator must wait for 120s * 2, in the `TIME_WAIT`
state, before transit to the `CLOSING` state.

this means the port will be locked for 240s for the tearing-down
initiator.

## Nagle's algorithm
the related question is: when do you send the buffered data into the
wire?

in order to fight the `silly window symdrome`(too many small packets), Nagle's algorithm runs as:

> When the application produces data to send
>   if both the available data and the window ≥ MSS
>     send a full segment
>   else
>     if there is unACKed data in flight
>       buffer the new data until an ACK arrives
>     else
>       send all the new data now

MSS means maximum transmission unit.

if the Nagle's altorithm is enabled, your buffer data might not be
sent to the network until and ACK arrives. this kind of delay is not good for interactive applications.

the socket interface allows the application to turn off Nagel’s
algorithm by setting the TCP NODELAY option. Setting this option means
that data is transmitted as soon as possible.

# UDP
put inside src/dst demultiplexer ports and a CRC checksum in your
payload, and sending it through IP packets, that's UDP. very simple.

