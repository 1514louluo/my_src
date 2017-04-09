#ifndef _NET_H
#define _NET_H

#include "midware.h"

struct connect_fd_info
{
        int                      fd;
        uint64_t              uniqtag;
};

static struct connect_fd_info **hold_fds;

static int set_nonblock(int socketfd)
{
        int flags;

        flags = fcntl(socketfd, F_GETFL, 0);
        if(-1 == flags)
        {
                perror("fcntl, getfl");
                return flags;
        }

        flags |= O_NONBLOCK;
        flags = fcntl(socketfd, F_SETFL, flags);
        if(-1 == flags)
        {
                perror("fcntl, setfl");
                return flags;
        }

        return flags;
}

static inline void sockaddr_init(struct sockaddr_in *addr, const char *ip, unsigned  short  int port)
{
        memset(addr, 0, sizeof(struct sockaddr_in));
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = inet_addr(ip);
        addr->sin_port = htons(port);
}

int tcp_server_init(const char *ip, unsigned  short  int port,  int max_size)
{
        int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(-1 == listen_fd)
        {
                perror("socket");
                return listen_fd;
        }
        int reuse = 1;
        if(-1 == setsockopt(m_listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)))
        {
                perror("setsockopt, SO_REUSEADDR");
                listen_fd = -1;
                return listen_fd;
        }

        struct sockaddr_in addr;
        sockaddr_init(&addr, ip, port);

        if(-1 == bind(listen_fd, (const struct sockaddr*)&addr, (socklen_t)sizeof(addr)))
        {
                perror("bind");
                return listen_fd;
        }

        if(-1 == listen(listen_fd, SOMAXCONN))
        {
                perror("listen");
                return listen_fd;
        }

        hold_fds = (struct connect_fd_info **)P_MALLOC(sizeof(struct connect_fd_info *) * max_size);
        if(NULL == hold_fds)
        {
                close(listen_fd);
                listen_fd = -1;
        }
        else
        {
                int i = 0;
                for(; i < max_size; ++i)  hold_fds[i] = NULL;
        }

        return listen_fd;

}

void tcp_server_run(int listen_fd, int max_size)
{
        int epollfd = epoll_init(max_size);
        if(0 > epollfd) return;

        struct epoll_event *pepoll_events = epoll_event_init(max_size);
        if(NULL == pepoll_events) return;

        struct pollfd pfd_new;
        pfd_new.fd = listen_fd;

        struct pollfd **pfd_array = (struct pollfd **)malloc(sizeof(struct pollfd *)*1);
        if(pollfd_array_input(pfd_array, 1, &pfd_new)) return;

        pollfd_array_insert_sort(pfd_array, 1);

        while(1)
        {
                epoll_poll(pfd_array, pepoll_events,  1, max_size, epollfd,  50000,  1);
        }
}

int epoll_init(int max_size)
{
        return epoll_create(max_size);
}

void *epoll_event_init(int max_size)
{
        return  P_MALLOC(sizeof(struct epoll_event) * max_size);
}

int epoll_add(int epoll_fd,  int addfd,  uint32_t opt)
{
        struct epoll_event event;
        event.data.fd = addfd;
        event.events = opt;
        return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, addfd, &event); // -1 is error!
}

int epoll_del(int epoll_fd, int delfd)
{
        struct epoll_event event;
        event.data.fd = 0;
        event.events = 0;
        return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, delfd, &event); // -1 is error!
}

int epoll_mod(int epoll_fd,  int modfd,  uint32_t opt)
{
        struct epoll_event event;
        event.data.fd = modfd;
        event.events = opt;
        return epoll_ctl(epoll_fd, EPOLL_CTL_MOD, modfd, &event); // -1 is error!
}


struct pollfd
{
        int                   			    fd;
        typedef int (*handle_pollfd)(int pollfd, int fd);
};

int pollfd_array_input(struct pollfd **pfd_array, int pfd_array_len, struct pollfd *pfd_new)
{
        int i = 0;
        for(; i < pfd_array_len; ++i)
        {
                if(NULL == pfd_array[i])
                {
                        pfd_array[i] = pfd_new;
                        return 0;
                }
        }
        return -1;
}

void pollfd_array_insert_sort(struct pollfd **pfd_array, int pfd_array_len)
{
        struct pollfd * tmp = NULL;
        int i = 1;
        int j = 0;
        for(; i < pfd_array_len; ++i)
        {
                tmp = pfd_array[i];
                j = i - 1;
                while((0 <= j) && (tmp->fd < pfd_array[j]->fd))
                {
                        pfd_array[j+1] = pfd_array[j];
                        --j;
                }
                pfd_array[j+1] = tmp;
        }
}

int pollfd_array_binary_get(struct pollfd **pfd_array, int pfd_array_len, int fd)
{
        int left = 0;
        int right = pfd_array_len - 1;
        while(left < right)
        {
                int mid = (left + right)/2;
                if(pfd_array[mid]->fd > fd)
                {
                        right = mid - 1;
                }
                else if(pfd_array[mid]->fd < fd)
                {
                        left = mid + 1;
                }
                else
                        return mid;
        }
        return -1;
}

void epoll_poll(struct pollfd **pfd_array, struct epoll_event *pepoll_events,  int pfd_size, int max_size, int epoll_fd,  int timeout,  int flag)
{
        int fds = epoll_wait(epoll_fd, pepoll_events, max_size, timeout);
        if(fds == 0)
        {
                //             MY_LOGINFO("epoll wait timeout notify");
                continue;
        }

        if(fds == -1)
        {
                if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                {
                        continue;
                }
                MY_LOGERROR("epoll wait error, errno="<<errno<<" errstr="<<strerror(errno));
                return;
        }

        int i;
        for(i=0; i<fds; i++)
        {
                // check if new client connection
                int index = pollfd_array_binary_get(pfd_array, pfd_size, pepoll_events[i].data.fd);
                if(-1 != index)
                {
                        pfd_array[index]->handle_pollfd(epoll_fd, pfd_array[index]->fd);
                }
                else
                {
                        if(flag) // has tcp accept socket, recv data
                        {
                                // do recv data!
                                tcp_handle_readfd(pepoll_events[i].data.fd);
                        }
                }

        }
}

int tcp_handle_newfd(int epoll_fd, int listen_fd)
{
        struct sockaddr_in new_addr;
        memset(&new_addr, 0, sizeof(new_addr));
        socklen_t new_addr_len = sizeof(new_addr);
        int new_client_fd = accept(listen_fd, (struct sockaddr*)&new_addr, &new_addr_len);
        if(-1 == new_client_fd)
        {
                perror("accept");
                return -1;
        }
        int noblock_result = set_nonblock(new_client_fd);
        if(noblock_result == -1)
        {
                return -1;
        }

        // set sendbuffer;
        uint32_t sendbuf = 1024*32;
        int optresult = setsockopt(new_client_fd,SOL_SOCKET,SO_SNDBUF,(char*) &sendbuf, sizeof(uint32_t));
        if(-1 == optresult)
        {
                perror("setsockopt, SO_SNDBUF");
                return -1;
        }

        // add new client fd to epoll fd
        int add_result = epoll_add(epoll_fd, new_client_fd, EPOLLIN);
        if(add_result == -1)
        {
                perror("epoll_ctl, add");
                return -1;
        }
        return 0;
}

#define READBUF_BLOCK_SIZE 1024
struct readbuf
{
        struct readbuf *next;
        char block[READBUF_BLOCK_SIZE];
        int    capacity;
};

struct readbuf_head
{
        struct readbuf *next;
        struct readbuf *tail;
	 int                    info;
};

#define NEW_READBUF_NODE ({struct readbuf *node = (struct readbuf *)P_MALLOC(sizeof(struct readbuf)); node->next=NULL;node->capacity=0;node;})
#define NEW_READBUF_HEAD ({struct readbuf_head *head = (struct readbuf_head *)P_MALLOC(sizeof(struct readbuf_head)); head->next=head->tail=NEW_READBUF_NODE; head;})

#define READBUF_APPEND(head) do{struct readbuf *newnode = NEW_READBUF_NODE; (head)->tail->next=newnode; (head)->tail = newnode;}while(0);
#define READBUF_FREE(head) 	    do{while((head)){struct readbuf *node = (head);(head)=(head)->next;P_FREE(node);}}while(0);
//#define READBUF_FOREACH(head)

int tcp_handle_readfd(int epoll_fd, int readfd)
{
        struct readbuf_head *rhead = NEW_READBUF_HEAD;
        int offset = 0;
        int recvsize = 0;
        int status = 0;
        while(1)
        {
                recvsize = recv(readfd, rhead->tail->block + offset, READBUF_BLOCK_SIZE - offset, MSG_DONTWAIT);

                if(recvsize < 0) // error
                {
                        if(EAGAIN  == errno || EWOULDBLOCK  == errno || EINTR == errno)
                        {
                                perror("recv");
                                break;
                        }
                        else
                        {
                                status = -1;
                                perror("recv");
                                break;
                        }
                }
                else if(0 == recvsize)   // close
                {
                        return -1;
                        break;
                }
                else if(recvsize == READBUF_BLOCK_SIZE - offset) // full
                {
                        offset = 0;
                        rhead->tail->capacity = READBUF_BLOCK_SIZE;
                        READBUF_APPEND(rhead);
                }
                else // part
                {
                        offset += recvsize;
                        rhead->tail->capacity = offset;
                }
        }
        /*no data recv */
        if(rhead->next = rhead->tail && 0 == rhead->tail->capacity)
        {
                READBUF_FREE(rhead);
                return;
        }
        /*check if error*/
        if(-1 == status)
        {
        }
        /*put to the queue*/
	 rhead->info = readfd;
        unsigned int len = sizeof(void *);
        request_fifo_put((const unsigned char *)rhead, len);
}

int tcp_handle_writefd(void)
{
	
}

int tcp_handle_closefd(int fd)
{
	close(fd);
}
enum
{
        PIPE_CMD_CLOSE=1,
        PIPE_CMD_SEND=2,
};

/*pipe data: data[5]*/
int pipe_handle_readfd(int epoll_fd, int readfd)
{
        while(1)
        {
                char cmd;
                int pipe_result = read(readfd, &cmd, 1);
                if(pipe_result == -1)
                {
                        perror("read pipe");
                        break;
                }

                switch(cmd)
                {
                        case PIPE_CMD_SEND:
                        {
                                int send_result = tcp_handle_writefd();
                                if(send_result == -1)
                                {
                                        tcp_handle_closefd(optfd);
                                }
                                break;
                        }

                        case PIPE_CMD_CLOSE:
                        {
                                tcp_handle_closefd(optfd);
                                break;
                        }

                        default:
                                break;
                }
        }

}

int pipe_handle_writefd(int writefd,   char cmd)
{
	return write(writefd, &cmd, 1);
}

#endif //_NET_H




