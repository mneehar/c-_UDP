#ifndef __FUZE_UDPACCEPTOR_H
#define __FUZE_UDPACCEPTOR_H

#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include<sstream> 
using namespace std;

#define BUFLEN 2048

struct client_payload
{
	int sequence_number;
	char client_data[BUFLEN];
};
 
class udpacceptor
{
	int    m_sd;
	int    m_port;
	int  m_seq;
	int m_eseq;
	string m_swrong;
	FILE* 	m_fp;	
	string m_delim;
 

public:
    	udpacceptor(int port);
    	~udpacceptor();
	int start();
private:
	udpacceptor() {}
	string processpayload (string s);
	string inttostring(int s);
	string getclientmessage(string cmessage);
	int getsequencenumber( string message);
	int stringtoint( string s );
	//string inttostring(int a);
	string getstringsequence( string s);
	string getclientpayload( string s);
};

#endif
