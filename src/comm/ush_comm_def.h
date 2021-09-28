#ifndef USH_COMM_DEF_H
#define USH_COMM_DEF_H


#define USH_COMM_MSG_ALIGNMENT __attribute__((aligned(1)))

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


#endif // USH_COMM_DEF_H
