#ifndef __FUZE_UDPACCEPTOR_H
#define __FUZE_UDPACCEPTOR_H

#include <string>
#include <netinet/in.h>
#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <sstream>

 
using namespace std;

#define SIZE 1024
#define BUFLEN 2048
#define CLIENT_LENGTH 2048
struct client_payload 
{
	int client_sequence;
	char client_data[SIZE];
};
 
class udpacceptor
{
	int    m_sd;
	string m_address;
	int    m_port;
	unsigned int m_seq;
	string m_filename;
	map<int,string>m_seqmap;
	string m_filepath;
	FILE* m_fp;
	string m_payload;
	string m_delim;
	
 

public:
	udpacceptor(int port, const char* address,string filename);
    	~udpacceptor();
 
	int client_start();
 
private:
    	udpacceptor() {}
	string getfilename();
	string getclientmessage();
	void processserverpayload (string s);
	string inttostring ( int s);
	int getsequencenumber(string s);
	int stringtoint( string s);

};

#endif
