#ifndef USH_COMM_DEF_H
#define USH_COMM_DEF_H


#define USH_COMM_MSG_PACKED __attribute__((packed))


#define USH_COMM_CONN_FULL_NAME_LEN_MAX            (64)
#define USH_COMM_CONN_SHORTNAME_LEN_MAX            (20)

////////////////////////////       Touch       /////////////////////////////////
#define USH_COMM_TCH_Q_PATH "/USH-TCH"
#define USH_COMM_TCH_Q_MSG_MAX_CNT  (10)
#define USH_COMM_TCH_Q_MSG_MAX_LEN (128)


///////////////////////////      Realm        //////////////////////////////////
#define USH_COMM_REALM_Q_MSG_MAX_CNT       (10)
#define USH_COMM_REALM_Q_MSG_MAX_LEN       (128)


////////////////////////////      Listener     /////////////////////////////////
#define USH_COMM_LSTNR_Q_MSG_MAX_CNT       (10)
#define USH_COMM_LSTNR_Q_MSG_MAX_LEN       (128)


#endif // USH_COMM_DEF_H
