#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h> //strdup()

/* change the word "define" to "undef" to try the (insecure) SNMPv1 version */
// #define DEMO_USE_SNMP_VERSION_3

// #ifdef DEMO_USE_SNMP_VERSION_3
// const char *our_v3_passphrase = "The Net-SNMP Demo Password";
// #endif
//不用版本3，注释

int main(int argc, char **argv)
{
    /* Initialize the SNMP library */
    init_snmp("snmpdemoapp"); //建立所有结构体  参数为任意字符串

    netsnmp_session session, *ss; //有通讯的套接字   使用sesson建立连接，但连接建立后使用的是ss，不知原因

    /* Initialize a "session" that defines who we're going to talk to */
    snmp_sess_init(&session); /* set up defaults */ //清空结构体
    session.peername = strdup("211.71.149.251");    //存服务端IP地址

    /* set up the authentication parameters for talking to the server */

#ifdef DEMO_USE_SNMP_VERSION_3 //不用版本3

    /* Use SNMPv3 to talk to the experimental server */

    /* set the SNMP version number */
    session.version = SNMP_VERSION_3;

    /* set the SNMPv3 user name */
    session.securityName = strdup("MD5User");
    session.securityNameLen = strlen(session.securityName);

    /* set the security level to authenticated, but not encrypted */
    session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;

    /* set the authentication method to MD5 */
    session.securityAuthProto = usmHMACMD5AuthProtocol;
    session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol) / sizeof(oid);
    session.securityAuthKeyLen = USM_AUTH_KU_LEN;

    /* set the authentication key to a MD5 hashed version of our
       passphrase "The Net-SNMP Demo Password" (which must be at least 8
       characters long) */
    if (generate_Ku(session.securityAuthProto,
                    session.securityAuthProtoLen,
                    (u_char *)our_v3_passphrase, strlen(our_v3_passphrase),
                    session.securityAuthKey,
                    &session.securityAuthKeyLen) != SNMPERR_SUCCESS)
    {
        snmp_perror(argv[0]);
        snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
        exit(1);
    }

#else /* we'll use the insecure (but simplier) SNMPv1 */  //用版本2c
    /* set the SNMP version number */
    // session.version = SNMP_VERSION_1; /*SNMP_VERSION_2c
    session.version = SNMP_VERSION_2c; //指定使用版本
    /* set the SNMPv1 community name used for authentication */
    session.community = "bjfu18"; //指定共同体
    session.community_len = strlen(session.community);
#endif                                                    /* SNMPv1 */
    ss = snmp_open(&session); /* establish the session */ //初始化连接
    if (!ss)
    {
        snmp_sess_perror("ack", &session); //错误信息
        exit(1);
    }

    //取CPU————————————————————
    netsnmp_pdu *pdu;

    /* Create the PDU for the data for our request. 1) We're going to GET the system.sysDescr.0 node.*/
    pdu = snmp_pdu_create(SNMP_MSG_GET); //创建PDU,指定类型

    oid anOID[MAX_OID_LEN]; //是char数组 表示MIT树
    size_t anOID_len;

    anOID_len = MAX_OID_LEN;
    // if (!snmp_parse_oid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len)) //指定对象
    // {
    //     snmp_perror(".1.3.6.1.2.1.1.1.0");
    //     exit(1);
    // }
    read_objid("HOST-RESOURCES-MIB::hrSWRunPerfCPU.1", anOID, &anOID_len); //指定对象
    //只能一个一个指定

#if OTHER_METHODS //在预定义没有该宏，是给予提示的
    /* These are alternatives to the 'snmp_parse_oid' call above,e.g. specifying the OID by name rather than numerically.*/
    read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
    get_node("sysDescr.0", anOID, &anOID_len);
    read_objid("system.sysDescr.0", anOID, &anOID_len);
#endif

    snmp_add_null_var(pdu, anOID, anOID_len); //变量绑定的值初始化为NULL

    netsnmp_pdu *response; //响应报文
    int status;            //操作状态

    status = snmp_synch_response(ss, pdu, &response); /* Send the Request out.*/ //阻塞模式发送并等待响应报文

    netsnmp_variable_list *vars; //变量绑定区
    int count = 1;               //所取对象计数

    /* Process the response.*/ //处理响应报文
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)
    {
        /* SUCCESS: Print the result variables*/ //打印对象名、值
        for (vars = response->variables; vars; vars = vars->next_variable)
            print_variable(vars->name, vars->name_length, vars);
        /* manipuate the information ourselves */ //打印对象值  自己按需处理值
        // for (vars = response->variables; vars; vars = vars->next_variable)
        // {
        //     if (vars->type == ASN_OCTET_STR)
        //     {
        //         char *sp = (char *)malloc(1 + vars->val_len);
        //         memcpy(sp, vars->val.string, vars->val_len);
        //         sp[vars->val_len] = '\0';
        //         printf("value #%d is a string: %s\n", count++, sp);
        //         free(sp);
        //     }
        //     else
        //         printf("value #%d is NOT a string! Ack!\n", count++);
        // }
    }
    else
    {
        /* FAILURE: print what went wrong!*/
        if (status == STAT_SUCCESS)
            fprintf(stderr, "Error in packet\nReason: %s\n", snmp_errstring(response->errstat));
        else if (status == STAT_TIMEOUT)
            fprintf(stderr, "Timeout: No response from %s.\n", session.peername);
        else
            snmp_sess_perror("snmpdemoapp", ss);
    }

    if (response)
        snmp_free_pdu(response); //释放PDU
    //每请求创建、释放一个PDU。未释放、未创建而直接在原来的PDU赋值，可能出错

    //取Mem————————————————————
    /* Create the PDU for the data for our request. 1) We're going to GET the system.sysDescr.0 node.*/
    pdu = snmp_pdu_create(SNMP_MSG_GET); //创建PDU,指定类型

    anOID_len = MAX_OID_LEN;
    // if (!snmp_parse_oid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len)) //指定对象
    // {
    //     snmp_perror(".1.3.6.1.2.1.1.1.0");
    //     exit(1);
    // }
    read_objid("HOST-RESOURCES-MIB::hrSWRunPerfMem.1", anOID, &anOID_len);

#if OTHER_METHODS //在预定义没有该宏，是给予提示的
    /* These are alternatives to the 'snmp_parse_oid' call above,e.g. specifying the OID by name rather than numerically.*/
    read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
    get_node("sysDescr.0", anOID, &anOID_len);
    read_objid("system.sysDescr.0", anOID, &anOID_len);
#endif

    snmp_add_null_var(pdu, anOID, anOID_len); //变量绑定的值初始化为NULL

    status = snmp_synch_response(ss, pdu, &response); /* Send the Request out.*/ //阻塞模式发送并等待响应报文

    /* Process the response.*/ //处理响应报文
    if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)
    {
        /* SUCCESS: Print the result variables*/ //打印对象名
        for (vars = response->variables; vars; vars = vars->next_variable)
            print_variable(vars->name, vars->name_length, vars);
        /* manipuate the information ourselves */ //打印对象值
        // for (vars = response->variables; vars; vars = vars->next_variable)
        // {
        //     if (vars->type == ASN_OCTET_STR)
        //     {
        //         char *sp = (char *)malloc(1 + vars->val_len);
        //         memcpy(sp, vars->val.string, vars->val_len);
        //         sp[vars->val_len] = '\0';
        //         printf("value #%d is a string: %s\n", count++, sp);
        //         free(sp);
        //     }
        //     else
        //         printf("value #%d is NOT a string! Ack!\n", count++);
        // }
    }
    else
    {
        /* FAILURE: print what went wrong!*/
        if (status == STAT_SUCCESS)
            fprintf(stderr, "Error in packet\nReason: %s\n", snmp_errstring(response->errstat));
        else if (status == STAT_TIMEOUT)
            fprintf(stderr, "Timeout: No response from %s.\n", session.peername);
        else
            snmp_sess_perror("snmpdemoapp", ss);
    }

    snmp_close(ss); //关闭连接
    return (0);
} /* main() */