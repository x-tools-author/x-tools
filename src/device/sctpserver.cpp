/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sctpserver.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
#include <usrsctp.h>

#define PORT 13
#define DAYTIME_PPID 40
#define SLEEP 1

SctpServer::SctpServer(QObject *parent)
    : Socket(parent)
{
    connect(this, &SctpServer::finished, this, &Socket::closed);
}

SctpServer::~SctpServer() {}

void SctpServer::run()
{
    struct socket *sock, *conn_sock;
    struct sockaddr_in addr;
    struct sctp_udpencaps encaps;
    socklen_t addr_len;
    char buffer[80];
    time_t now;
    struct sctp_sndinfo sndinfo;

    usrsctp_init(m_serverPort, NULL, NULL);
#ifdef SCTP_DEBUG
    usrsctp_sysctl_set_sctp_debug_on(SCTP_DEBUG_NONE);
#endif
    usrsctp_sysctl_set_sctp_blackhole(2);
    usrsctp_sysctl_set_sctp_no_csum_on_loopback(0);

    if ((sock = usrsctp_socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP, NULL, NULL, 0, NULL)) == NULL) {
        emit errorOccurred(QStringLiteral("usrsctp_socket failed"));
        return;
    }

    memset(&encaps, 0, sizeof(struct sctp_udpencaps));
    encaps.sue_address.ss_family = AF_INET;
    encaps.sue_port = htons(m_serverPort);
    if (usrsctp_setsockopt(sock,
                           IPPROTO_SCTP,
                           SCTP_REMOTE_UDP_ENCAPS_PORT,
                           (const void *) &encaps,
                           (socklen_t) sizeof(struct sctp_udpencaps))
        < 0) {
        emit errorOccurred(QStringLiteral("usrsctp_setsockopt failed"));
        return;
    }
    memset((void *) &addr, 0, sizeof(struct sockaddr_in));
#ifdef HAVE_SIN_LEN
    addr.sin_len = sizeof(struct sockaddr_in);
#endif
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (usrsctp_bind(sock, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
        emit errorOccurred(QStringLiteral("usrsctp_bind failed"));
        return;
    }

    if (usrsctp_listen(sock, 1) < 0) {
        emit errorOccurred(QStringLiteral("usrsctp_listen failed"));
        return;
    }

    emit opened();
    qInfo() << "SCTP server is listening on port" << m_serverPort;
    while (1) {
        if (isInterruptionRequested()) {
            break;
        }

        addr_len = 0;
        if ((conn_sock = usrsctp_accept(sock, NULL, &addr_len)) == NULL) {
            continue;
        }
        time(&now);
#ifdef _WIN32
        if (_snprintf(buffer, sizeof(buffer), "%s", ctime(&now)) < 0) {
#else
        if (snprintf(buffer, sizeof(buffer), "%s", ctime(&now)) < 0) {
#endif
            buffer[0] = '\0';
        }
        sndinfo.snd_sid = 0;
        sndinfo.snd_flags = 0;
        sndinfo.snd_ppid = htonl(DAYTIME_PPID);
        sndinfo.snd_context = 0;
        sndinfo.snd_assoc_id = 0;
        if (usrsctp_sendv(conn_sock,
                          buffer,
                          strlen(buffer),
                          NULL,
                          0,
                          (void *) &sndinfo,
                          (socklen_t) sizeof(struct sctp_sndinfo),
                          SCTP_SENDV_SNDINFO,
                          0)
            < 0) {
            perror("usrsctp_sendv");
        }
        usrsctp_close(conn_sock);
    }
}