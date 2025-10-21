/*
 * n-bus.c
 *
 *  Created on: 24-Oct-2021
 *      Author: nazar
 */

#include "n-bus.h"
#include "common.h"

#define SERVER_PATH  "/tmp/tpf_unix_sock.server"

#define CLIENT_PATH "/tmp/tpf_unix_sock.client"


int client_sock = -1;

nbus_callback_hdler shdler;


pthread_t rx_tid;



static void *rx_handler(void* param)
{
	int ret;
	char buf[256];

	while(1)
	{
		ret = recv(client_sock, buf, sizeof(buf),0);
		if (ret == -1) {
			printf("RECV ERROR = %d\n", errno);
			close(client_sock);
			client_sock=-1;

			return NULL;
		}
		else {
			printf("DATA RECEIVED = %s\n", buf);
		}
	}
}

static int init_socket(void)
{

	int rc, len,i;
	struct sockaddr_un server_sockaddr;
	struct sockaddr_un client_sockaddr;

	memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
	memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));

	/**************************************/
	/* Create a UNIX domain stream socket */
	/**************************************/
	client_sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client_sock == -1) {
		printf("SOCKET ERROR = %d\n", errno);
		return -1;
	}

	/***************************************/
	/* Set up the UNIX sockaddr structure  */
	/* by using AF_UNIX for the family and */
	/* giving it a filepath to bind to.    */
	/*                                     */
	/* Unlink the file so the bind will    */
	/* succeed, then bind to that file.    */
	/***************************************/
	client_sockaddr.sun_family = AF_UNIX;
	strcpy(client_sockaddr.sun_path, CLIENT_PATH);
	len = sizeof(client_sockaddr);

	unlink(CLIENT_PATH);
	rc = bind(client_sock, (struct sockaddr *) &client_sockaddr, len);
	if (rc == -1){
		printf("BIND ERROR: %d\n", errno);
		close(client_sock);
		client_sock=-1;
		return -1;
	}

	/***************************************/
	/* Set up the UNIX sockaddr structure  */
	/* for the server socket and connect   */
	/* to it.                              */
	/***************************************/
	server_sockaddr.sun_family = AF_UNIX;
	strcpy(server_sockaddr.sun_path, SERVER_PATH);
	rc = connect(client_sock, (struct sockaddr *) &server_sockaddr, len);
	if(rc == -1){
		printf("CONNECT ERROR = %d\n", errno);
		close(client_sock);
		client_sock=-1;
		return -1;
	}

	return 0;

}


static int send_data(int cmd_id,char *data,int len)
{
	int ret;
	nbus_cmd_frame_t frame;

	frame.cmd_id=cmd_id;
	frame.data_len=len;
	memcpy(frame.cmd_data,data,len);
	ret = send(client_sock, &frame, sizeof(nbus_cmd_frame_t), 0);
	if (ret == -1) {
		printf("SEND ERROR = %d\n", errno);
		close(client_sock);
		client_sock=-1;
		return -1;
	}

	return 0;

}

int nbus_connect(char *client_name,nbus_callback_hdler hdler)
{
	int ret;



	ret =init_socket();

	if(ret == -1)
	{
		printf("socket init failed\n");

		return -1;
	}

	printf("Sending data...\n");

	shdler=hdler;

	if (pthread_create(&rx_tid, NULL,
			rx_handler, NULL)
			!= 0)
	{

		// Error in creating thread
		printf("Failed to create thread\n");

		return -1;
	}
	ret=send_data(NBUS_CONNECT,client_name,strlen(client_name));

	if(ret == -1)
	{
		printf("send_datafailed\n");

		return -1;
	}

	return 0;
}


int nbus_sendmsg(int id,char * msg,int len)
{
	int ret;

	nbus_msg_t msg_fr;

	msg_fr.dst_id=id;

	msg_fr.msg_len=len;

	memcpy(msg_fr.msg_data,msg,len);

	ret=send_data(NBUS_SEND_MSG,&msg_fr,sizeof(nbus_msg_t ));

	if(ret == -1)
	{
		printf("send_datafailed\n");

		return -1;
	}

	return 0;
}


