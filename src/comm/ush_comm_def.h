#ifndef USH_COMM_DEF_H
#define USH_COMM_DEF_H


#define USH_COMM_MSG_PACKED __attribute__((packed))


#define USH_COMM_CONN_NAME_LEN_MAX            (64)

////////////////////////////       Touch       /////////////////////////////////
#define USH_COMM_TOUCH_Q_PATH "/USH-TOUCH"
#define USH_COMM_TOUCH_Q_MSG_MAX_CNT  (10)
#define USH_COMM_TOUCH_Q_MSG_MAX_LEN (128)




////////////////////////////      Listener     /////////////////////////////////
#define USH_COMM_LISTENER_Q_PATH_PREFIX "/USH-LSTNR-"

#define USH_COMM_LISTENER_Q_SHORTNAME_LEN_MAX (20)
// [/USH-LSTNR-][shortname][-timestamp-][cert]
//    11B           20B         11B       10B

#define USH_COMM_LISTENER_Q_MSG_MAX_CNT       (10)
#define USH_COMM_LISTENER_Q_MSG_MAX_LEN       (128)



typedef enum USH_COMM_PORT {
    USH_COMM_PORT_TOUCH,
    USH_COMM_PORT_LISTENER,
    USH_COMM_PORT_MAX
} USH_COMM_PORT;
typedef struct comm_port_desc {
    USH_COMM_PORT  port;
} USH_COMM_MSG_PACKED ush_comm_port_description;


#endif // USH_COMM_DEF_H
