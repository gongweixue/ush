#ifndef USH_COMM_DEF_H
#define USH_COMM_DEF_H


#define USH_COMM_MSG_ALIGNMENT __attribute__((aligned(1)))

////////////////////////////       Touch       /////////////////////////////////
#define USH_COMM_TOUCH_Q_PATH "/FAW-HQ-COS-USH-TOUCH_PIPE"
#define USH_COMM_TOUCH_Q_MSG_MAX_CNT  (10)
#define USH_COMM_TOUCH_Q_MSG_MAX_LEN (128)




////////////////////////////      Listener     /////////////////////////////////
#define USH_COMM_LISTENER_Q_PATH_PREFIX "/FAW-HQ-COS-USH-LISTENER-"
#define USH_COMM_LISTENER_Q_MSG_MAX_CNT  (10)
#define USH_COMM_LISTENER_Q_MSG_MAX_LEN (128)
#define USH_COMM_LISTENER_Q_NAME_LEN_MAX (64)


#endif // USH_COMM_DEF_H
