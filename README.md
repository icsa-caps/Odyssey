<<<<<<< HEAD
# Kite
=======
#Kite
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a

Kite is a replicated, RDMA-enabled Key-Value Store that enforces available Release Consistency.
Kite implements a read/write/RMW API an uses:
1. Eventual Store for relaxed reads  & writes
2. MW-ABD (simply called ABD) for releases & acquires (i.e. linearizable reads/writes)
3. Basic Paxos for RMWs.

## Eventual Store (ES)
<<<<<<< HEAD
Eventual Store is a protocol that provides per-keu Sequential consistency.
ES implements reads locally and incurs a broadcast round for writes.


## ABD Algorithm (Attiya, Bar-Noy, Dolev )
=======
Eventual Store is aprotocol that provides per-keu Sequential consistency.
ES implements reads locally and incurs a broadcast round for writes.


## ABD Algorithm(Attiya, Bar-Noy, Dolev )
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a
ABD is an algorithm that implements linearizable reads and writes
Writes incur 2 broadcast rounds and reads incur 1 broadcast round with an conditional second round.

## Paxos
Paxos is implemented as such:
<<<<<<< HEAD
* Basic Paxos (leaderless with 3 rtts)
* Key-granularity: Paxos commands to different keys do not interact
* With both release and acquire semantics
* Compare And Swaps can be weak: they can fail locally if the comparison fails locally
=======
* Basic Paxos (leaderless with 3rtts)
* Key-granularity: Paxos commands to differnt keys do not interact
* With both release and acquire semantics
* Compare And Swaps can be weak: they can fail lcaolly if the comparison fails locally
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a

## Kite API
Kite API contains two flavours (a blocking and a nonblocking) of the following commands
1. read()
2. write()
3. release()
4. acquire()
5. CAS_strong()
6. CAS_weak()
7. FAA()

The Kite API can be sued by the client threads. 
./src/client.c already contains implementations of
* The Treiber Stack
* Michael & Scott Queues
<<<<<<< HEAD
* Harris and Michael Lists
=======
* Harris and Micahel Lists
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a
* A circular producer consumer pattern

Also a User Interface to issue requests from the Command Line is available.


## Optimizations
The protocol is implemented over UD Sends and Receives.
All messages are batched.


## Repository Contains
1. A modified version of MICA that serves as the store for Kite
2. A layer that implements the protocols that run over MICA

<<<<<<< HEAD
## Requirements
RDMA capable NICs and Infiniband switch
=======
## Requirments
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a

### Dependencies
1. numactl
1. libgsl0-dev
1. libnuma-dev
1. libatmomic_ops
1. libmemcached-dev
1. MLNX_OFED_LINUX-4.1-1.0.2.0

### Settings
1. Run subnet-manager in one of the nodes: '/etc/init.d/opensmd start'
<<<<<<< HEAD
2. On every node apply the following:
3. echo 8192 | tee /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages > /dev/null
4. echo 10000000001 | tee /proc/sys/kernel/shmmax /proc/sys/kernel/shmall > /dev/null
 * Make sure that the changes have been applied using cat on the above files
 * The following changes are temporary (i.e. need to be performed after a reboot)
 
## How to run Kite
In kite/src/drf-sc there is a script "run-drf-sc.sh"
To run Kite:
1. Modify the script to contain the ip-addresses of the machines that will run Kite
2. Run the script in all machines. 
    * The script must be run first in the machine that has its ip first in the list
    * Then it can be run in whatever order in the rest of he machines
=======
1. On every node apply the following:
 1. echo 8192 | tee /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages /sys/devices/system/node/node1/hugepages/hugepages-2048kB/nr_hugepages > /dev/null
 1. echo 10000000001 | tee /proc/sys/kernel/shmmax /proc/sys/kernel/shmall > /dev/null
 * Make sure that the changes have been applied using cat on the above files
 * The following changes are temporary (i.e. need to be performed after a reboot)
>>>>>>> 981ca3cc793d69cad9416f16c45fcf7cbacfaa9a

## Tested on
* Infiniband cluster of 5 inter-connected nodes, via a Mellanox MSX6012F-BS switch, each one equiped with a single-port 56Gb Infiniband NIC (Mellanox MCX455A-FCAT PCIe-gen3 x16).
* OS: Ubuntu 18.04.1 LTS (Kernel: 4.15.0-55-generic) 
