#include <stdio.h>
#include <string.h>
#include<unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "fuze_udpacceptor.h"

#define SERVER_LENGTH 2048
 
udpacceptor::udpacceptor(int port, const char* address,string filename)
{
    // Lets initialize.
        m_sd = 0;
        m_port = port;
        m_address = address;
        m_seq = 0;
	m_filepath = filename;
        m_filename = "";
	FILE* m_fp = NULL;        
	m_payload = "";
	m_delim = ";;;";

}

udpacceptor::~udpacceptor()
{
    	if (m_sd > 0) 
	{
        	close(m_sd);
		m_sd = 0;

    	}
	if (m_fp != NULL)
	{
		fclose(m_fp);
	}

}
		

int udpacceptor::client_start()
{
	cout << "client_start" <<endl;
	int istatus =1;
	struct sockaddr_in si_other;

	m_sd = socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
    	if ( m_sd == -1)
    	{
        	cout << " Not able to create socket" << endl;
        	return 0;
    	}


	memset((char *) &si_other, 0, sizeof(si_other));
    	si_other.sin_family = AF_INET;
    	si_other.sin_port = htons(m_port);

    	if (inet_aton(m_address.c_str() , &si_other.sin_addr) == 0)
    	{
        	cout << "inet_aton failed" <<endl;
        	return -1;
    	}
        socklen_t slen= sizeof(si_other);
	char buf[SERVER_LENGTH];
	while(1)
	{
		sleep(10);
		cout << " As per Protocol send Heart beat message" << endl;
		string message = getclientmessage();
		if (sendto(m_sd, message.c_str(), strlen(message.c_str()) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        	{
            		cout << "sendto failed" << endl;
			istatus = -1;
			break;
        	}
		memset(buf,0,SERVER_LENGTH);
		if (recvfrom(m_sd, buf, SERVER_LENGTH, 0, (struct sockaddr *) &si_other, &slen) == -1)
        	{
            		cout << " receive from failed" << endl;
			istatus = -1;
			break;
        	}
		processserverpayload(buf);// it is for only sequence update


	}
	return istatus;
}
/*

 // first heart beat communication 
        // sequence num 0 is for heart beat
        // sequence number -1 is for getting the file name
        // sequence number -10 is for end of transmission communication
        // sequence number -100 is for checksum communication



*/

string udpacceptor::getclientmessage()
{
	cout << "getclientmessage" <<endl;
	//char buf[CLIENT_LENGTH];
	//memset(buf,0,CLIENT_LENGTH);
	//client_payload p;
	// first we need to send heart beat message
	string pl = ";;;";
	if ( m_seq == 0)
	{
		//p.client_sequence =m_seq;
		//p.client_data ="";
		//memcpy(buf,&p,sizeof(client_payload));
		//return buf;
		// returning  a string of payyload
		pl = inttostring(m_seq) + m_delim;
		return pl; 
	}
	else if ( m_seq == -1)
	{
		//p.client_sequence = m_seq;
		//strcpy(p.client_data ,getfilename().c_str());
		//memcpy(buf,&p,sizeof(client_payload));
		string fn = getfilename();
		if ( fn != "" )
		{
		
			pl = inttostring(m_seq) + m_delim + getfilename();
		}
		else
		{
			cout << " file name is not set : setting sequence = -100" << endl;
			m_seq =100;
			pl = inttostring (m_seq) + m_delim ;
		}
		
		return pl;
	}
	else
	{
		// Now we need to do the file transefer . Because all the initialization done.
		// already m_seq =1 set by file name communication
		string s ="";
		if (m_fp == NULL)
		{
			cout << " Opening the file :" <<m_filepath << endl;
			m_fp = fopen(m_filepath.c_str(),"r");
		}
		//if (m_fp != NULL)
		//{
		/*	if( !feof(m_fp))
			{
				fscanf(m_fp,"%s",pl);
				//p.client_sequence = m_seq;
				//memcpy(buf,&p,sizeof(client_payload));
				pl = inttostring(m_seq) + m_delim + s;
				m_seq ++;
				
			
			}
			else
			{
				// It is end of file
				m_seq = -10;
				//p.client_sequence = m_seq;
				//memcpy(buf,&p,sizeof(client_payload));
				pl = inttostring(m_seq) + m_delim;
				fclose(m_fp);	

			}*/
		//	return pl;
		//}
	

	}
	cout << " finished with get client message" <<endl;
	return pl;
	

}
			
string udpacceptor:: getfilename()
{
	cout << " get file name" << endl;
	string s = "";
	string delim = "/";
	int pos = m_filepath.rfind (delim);
	if ( pos != string::npos )
	{
		m_filename = m_filepath.substr(pos , strlen(m_filepath.c_str()));
	}
	if (m_filepath != "")
	{
		m_filename = m_filepath;
	}
	else
	{
		cout << " file name is not set in configuration" << endl;
		m_filename = "";
	}
	cout << " File Name : :::::" << m_filename << endl;
	return m_filename;
	
}
int  udpacceptor::getsequencenumber( string s)
{
	cout << " inside getsequencenumber" << endl;
	int seq = -999; //invalid number
	int f  = s.find(m_delim);
	string ss = "";
	if ( f != string ::npos )
	{

		ss = s.substr(0,f);
		seq  = stringtoint(ss);
	}
	else
		seq = -999;
	cout << " GOT M_SEQ :::" << f  << "   string:::" << ss << "   position:::" << f << endl;
	return seq;
}
			

void udpacceptor:: processserverpayload(string s)
{
	cout << " Inside processserverpayload" <<endl;
	//client_payload p;
	//char buff [CLIENT_LENGTH];
	//memset(buff, 0, CLIENT_LENGTH);
	//strcpy(buff,s.c_str());
	// find out the acknowledged sequence number 
	//memcpy(&p,buff,strlen(s.c_str()));
	m_seq = getsequencenumber(s);


	cout << " sequence from server : " << m_seq << endl;
	//m_seq = p.client_sequence;
	//cout << "testing" << m_seq << endl;
	//m_seq = htons( p.client_sequence);
	//cout << "testing 111" << m_seq << endl;
}
string udpacceptor::inttostring( int number)
{

        ostringstream oss;
        oss << number;
        return oss.str();
}
int udpacceptor:: stringtoint ( string s )
{
	int i = -1000;
	stringstream stream1;
	stream1.str("");
	stream1.str(s);
	stream1>>i;
	return i;
	
}
