#include <asm-generic/socket.h>
#include <cerrno>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

int main() {

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  addrinfo *res;
  // get an address to listen to on 6380
  int rv = getaddrinfo(nullptr, "6380", &hints, &res);
  if (rv != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // attach a socket to our new address
  int s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (s < 0) {
    perror("socket");
    return 1;
  }

  int optval = 1;
  // lets us restart quickly; see Beej !
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  // claim 6380 and listen for upto 24 clients
  if (bind(s, res->ai_addr, res->ai_addrlen) < 0) {
    perror("bind");
    return 1;
  }
  freeaddrinfo(res); // we dont need it anymore
  if (listen(s, 24) < 0) {
    perror("listen");
    return 1;
  }

  char buf[4096];
  /*
   *
   *MAIN APPLICATION LOOP:
   * wait for a client
   * accept their connection
   *
   *  PER-CLIENT LOOP:
   *   read() what they say - their n bytes
   *   n > 0 - send output (for now just echo, while sent < n)
   *   n == 0 - they closed connection, move onto another client
   *   n < 0 - error, perror(), stop talking
   *
   * now its safe to end the convo => close()
   * go back to waiting
   *
   * */
  for (;;) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    // blocks until we have a client to accept, accept their connection
    int client_fd = accept(s, (struct sockaddr *)&their_addr, &addr_size);
    if (client_fd == -1) {
      perror("accept");
      continue;
    }

    bool ohShitMyWriteFailed = false;
    // start the PER CLIENT LOOP
    while (ohShitMyWriteFailed == false) {
      // read()
      ssize_t n = read(client_fd, buf, sizeof(buf));
      // easy cases of n
      if (n == 0) {
        break;
      }
      if (n < 0) {
        perror("read");
        break;
      }
      // send their data
      ssize_t sent = 0;
      while (sent < n) {
        ssize_t w = write(client_fd, buf + sent, n - sent);
        if (w < 0) {
          perror("write");
          ohShitMyWriteFailed = true;
          break;
        }
        sent += w;
      }
    }
    // however, we got out of there, we now want to close the connection
    close(client_fd);
  }
}
