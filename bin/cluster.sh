
OD_HOME="/users/akats/odyssey"

HOSTS=(
### TO BE FILLED: Please provide all cluster IPs
# Node w/ first IP (i.e., "manager") must run script before the rest of the nodes
# (instantiates a memcached to setup RDMA connections)
#
   10.0.3.1
   10.0.3.2
   10.0.3.3
   10.0.3.4
   10.0.3.5
  )


# Cloudlab
allIPs=(
	10.0.3.1
	10.0.3.2
	10.0.3.3
	10.0.3.4
	10.0.3.5
	10.0.3.6
	10.0.3.7
	)

# Informatics cluster
#allIPs=(192.168.8.4 #houston
#        192.168.8.6 #austin
#        192.168.8.5 #sanantonio
#        192.168.8.3 #indianapolis
#        192.168.8.2 #philly
#        192.168.5.11
#        192.168.5.13 )


### TO BE FILLED: Modify to get the local IP of the node running the script (must be one of the cluster nodes)
#cloudlab
LOCAL_IP=$(ip addr | grep 'state UP' -A2 | grep 'inet 10.0.3'| awk '{print $2}' | cut -f1  -d'/')
#Informatics
#localIP=$(ip addr | grep 'infiniband' -A2 | sed -n 2p | awk '{print $2}' | cut -f1  -d'/')

### Fill the RDMA device name (the "hca_id" of the device when executing ibv_devinfo)
NET_DEVICE_NAME="mlx4_0"  # cloudlab
#NET_DEVICE_NAME="mlx5_0" # informatics

##########################################
### NO NEED TO CHANGE BELOW THIS POINT ###
##########################################

REMOTE_IPS=${HOSTS[@]/$LOCAL_IP}
REMOTE_HOSTS=${HOSTS[@]/$LOCAL_IP}
localIP=${LOCAL_IP}

