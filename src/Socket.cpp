#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/tcp.h>

#include "../include/Socket.h"
#include "../include/Logger.h"
#include "../include/InetAddress.h"


Socket::~Socket(){
   ::close(sockfd_);
}

void Socket::bindAddress(const InetAddress &localaddr){

  if (0 != ::bind(sockfd_, (sockaddr *)localaddr.getSockAddr(), sizeof(sockaddr_in)))
  {
      LOG_FATAL("bind sockfd:%d fail\n", sockfd_);
  }   
  
}