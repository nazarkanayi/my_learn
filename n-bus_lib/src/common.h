/*
 * common.h
 *
 *  Created on: 24-Oct-2021
 *      Author: nazar
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#define MAX_CMD_DATA 100
#define MAX_MSG_DATA 90

#define NBUS_CONNECT  1

#define NBUS_DISCONNECT 2

#define NBUS_CONNECT_RES 3

#define NBUS_SEND_MSG 4

#define NBUS_GET_ID 5

#define NBUS_GET_ID_RES 6



typedef struct nbus_cmd_frame_s
{

 int cmd_id;
 int data_len;
 char cmd_data[MAX_CMD_DATA];


}nbus_cmd_frame_t;



typedef struct nbus_msg_s
{
 int dst_id;
 int msg_len;
 char msg_data[MAX_CMD_DATA];

}nbus_msg_t;




#endif /* SRC_COMMON_H_ */
