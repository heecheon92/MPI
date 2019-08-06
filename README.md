##Basic Setup on Master cluster:

Install mpich
```
sudo apt-get update && sudo apt-get install mpich
```

Configure /etc/hosts file. (Master node must have all the list of ip addresses and identity of slave nodes and master node itself) // mpirun or mpiexec can load this file to execute a program in parallel.

Add a user “mpiuser” (All nodes having mpiuser is preferred to reduce possible confusion) // sudo adduser mpiuser

Make mpiuser a sudoer
```
    sudo usermod -a -G sudo mpiuser
```
Setup an ssh server so that the master and slave clusters can interchange messages. 
```
    sudo apt-get install openssh-server”
```

In order for mpirun / mpiexec to execute a program in parallel smoothly, a passwordless ssh environment must be set up. Setup steps are as follows:
```
    su - mpiuser
    ssh-keygen -t rsa		*  or -t dsa
    ssh-copy-id client 		** ip-address may also be used.
    eval `ssh-agent`
    ssh-add ~/.ssh/id_rsa

*  you might have to use -t dsa if your system is outdated. Like linux before 2000.
** client is defined in machine file or /etc/hosts
	i.e. cat /etc/hosts
	      192.168.x.x	client
	      127.0.0.1		localhost
```
If done correctly with all the steps above, the master should be able to ssh to the slave nodes without a password.
If it is not working, rm ~/.ssh/id* and redo the steps above <br />


Once a passwordless ssh channel is created, a shared directory (nfs in this case), where a message can be interchanged, needs to be created. Passwordless ssh must be available to proceed to this step. Steps are as follows:
```    
    sudo apt-get install nfs-kernel-server
```
create a folder name “cloud” under mpiuser’s home directory
add an entry to /etc/exports with “home/mpiuser/cloud *(rw,sync,no_root_squash,no_subtree_check)”
```
    echo “home/mpiuser/cloud *(rw,sync,no_root_squash,no_subtree_check)” >> /etc/exports

```

Reapply /etc/exports by running 
```
    exportfs -a
    sudo service nfs-kernel-server restart
```
A shared directory must be configured on each slave cluster as well. Steps are as follows:
```
    sudo apt-get install nfs-common
```
create a folder name “cloud” under mpiuser’s home directory (slave node)
```
    sudo mount -t nfs master:/home/mpiuser/cloud ~/cloud
```
The step above requires that slave cluster configure /etc/hosts with ip address and identity of the master cluster and itself. <br/>

Add the following entry to /etc/fstab so that the mounted shared folder does not get unmounted on reboot. “master:/home/mpiuser/cloud /home/mpiuser/cloud nfs”
```
    echo “master:/home/mpiuser/cloud /home/mpiuser/cloud nfs” >> /etc/fstab

```

Once the basic environments are setup, the mpi program can be executed. Methods are as follows:
```    
    mpirun -np 1 (no. of cores) -hosts master,client (identities in /etc/hosts) ./mpi_executable
    mpirun -np 1 (no. of cores) ./mpi_executable # Running on the master cluster only.
    mpirun -np 1 (no. of cores) --hostfile /etc/hosts ./mpi_executable
```

Following is current /etc/hosts file on the master cluster:
```
    cat /etc/hosts
    127.0.0.1    localhost
    #127.0.1.1  master_identity
    192.168.0.31    master
    192.168.0.32    client
    192.168.0.33    client2
```
