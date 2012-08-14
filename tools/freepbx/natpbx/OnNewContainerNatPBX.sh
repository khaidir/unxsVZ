#!/bin/bash

#FILE
#	/usr/sbin/OnNewContainerNatPBX.sh

#NatPBX is a special type of container that is built in a single group and once all in
#	the group have been started. A special group install script is run. This
#	script uses the unxsNATPBX commmand mostly.
#	Since the group install script handles many functions on name change all we need to do
#	is monitoring and dns changes. These can be dived in to two groups cleanup (delete
#	entries for previous hostname) and the provisioning/registration of new items
#	like new DNS SRV RRs for the new hostname.

#We could check to see of the complete group has been started and then run from here the
#GroupInstallNatPBX.sh script (sample.sh)