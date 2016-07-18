#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "fuze_udpacceptor.h" 
udpacceptor::udpacceptor(int port)
{
    // Lets initialize.
	m_sd = 0;
	m_port = port;
    	m_seq = 0;
	m_eseq = 0;
	m_swrong ="";
	m_fp = NULL;


}

udpacceptor::~udpacceptor()
{
    	if (m_sd > 0) 
	{
        	close(m_sd);
		m_sd = 0;

    	}
}
int udpacceptor::start()
{
	int istat = 1;
	struct sockaddr_in si_ime, si_iother;
	// creating udp socket here , if it fails then we will have to return from here
	if ((m_sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    	{
        	cout << " Socket Initialization failed" << endl;
		istat = 0;
		return istat;
    	}
	// Now binding the socket
	memset((char *) &si_ime, 0, sizeof(si_ime));
    	si_ime.sin_family = AF_INET;
    	si_ime.sin_port = htons(m_port);
    	si_ime.sin_addr.s_addr = htonl(INADDR_ANY);
	// Now bind the socket to port 
	if( bind(m_sd , (struct sockaddr*)&si_ime, sizeof(si_ime) ) == -1)
    	{	
        	cout << " Failed to bind... No need to go ahead" <<endl;
		return -1;
    	}
	socklen_t slen= sizeof(si_iother);
	// definiging BUFLEN = 2048 IN HEADER FILE
	char buf [BUFLEN];
	
	while(1)
	{
		sleep(9);
		//memset(buf,0,BUFLEN);
		cout << " waiting for heart beat" << endl;
		if ( recvfrom(m_sd, buf, BUFLEN, 0, (struct sockaddr *) &si_iother, &slen)== -1)
		{
			cout << " receivefrom failed" << endl;
			istat = -1;
			break;
		}
		// Now process the payload and get the buffer which need to be sent to client
		// refer to udpacceptor file for protocol details
		string cload = processpayload(buf);
		//memset(buf,0,BUFLEN);
		//strcpy(buf,cload.c_str());
		if (m_eseq == -999)
		{
			cout << " something has gone wrong " << endl;
			continue;
		}
		if (sendto(m_sd, cload.c_str(), cload.length(), 0, (struct sockaddr*) &si_iother, slen) == -1)
        	{
            		cout << " Communication can not happen" << endl;
			istat = -1;
			break;
        	}
		if ( m_eseq == -100)
		{
			cout << " end of the file ... closing everything" << endl;
			break;
		}

	}

}
	// first heart beat communication 
        // sequence num 0 is for heart beat
        // sequence number -1 is for getting the file name
        // sequence number -10 is for end of transmission communication
        // sequence number -100 is for checksum communication
/*
Lets keep the payload structure is as below
int sequence_number
client_data

server payload
only sequence number;

*/
/*
Payload processig should be scalable in multiple threads so that we can achieve performance 

*/
string udpacceptor::getstringsequence(string s)
{
	cout << " get string sequence " << endl;
	string ir = "-999"; // Initializing by default sequence .
	int pos = s.find (m_delim);
	if ( pos == string::npos )
	{
		cout << " does not have the delimiter returning with default" << endl;
		return ir;
	}
	cout << " got the position number : " << pos << endl;
	ir = s.substr(0,pos);
	cout << " string sequence number is : " << ir << endl;
	return ir;
}

string udpacceptor::getclientpayload(string s)
{
	cout << " Inside getclientpayload()" << endl;
	string sret = "";
	int iret = -999;
	int pos = s.find (m_delim);
	if ( pos == string::npos)
	{
		cout << "need to return here " <<endl;
		return sret;
	}
	sret = s.substr( pos+3);
	return sret;
}
	
string udpacceptor::processpayload (string s)
{
	string sp ="";
	cout << " inside processpayload()" << endl;
	// Now atleast in each package the delimiter should be there.
	// First get the string version of sequence number 
	string sseq = getstringsequence(s);
	if (sseq.compare("-999") == 0)
	{
		m_swrong = "-999";
		cout << " No sequence ignore the message " <<endl;
		return sseq +m_delim;
	}
	string spload = getclientpayload(s);
	m_seq = stringtoint(sseq);
	cout << " Got the sequence number :" << m_seq << endl;
	// Now see what is the expected sequence numberit should be just 0, -1 , -100 or greater than 1
	if ( m_seq != m_eseq )
	{
		sp = inttostring(m_eseq) + m_delim;
		cout << " Expected sequence is :::" << m_eseq << endl;
		return sp;
	}
	
	if (m_seq == 0)
	{
		cout << " Got the heart beat message" << endl;
		// now ask the client to give the file name
		m_eseq  = -1;// expected sequence which we think should be -1 after heart beat
		sp =inttostring(m_eseq)+m_delim;
		return sp;
		
	}
	else if (m_seq == -1)
	{
		cout << " got the file name" <<spload  << endl;
		int sq = -1;
		sp = inttostring(sq);
		cout << sp << endl;
		m_eseq = 1;
		// Now opening the file
		m_fp = fopen(sp.c_str(),"w");
		if (m_fp == NULL)
		{
			
			cout << " File open failed:: can not proceed need to terminate" << endl;
			m_eseq = -100;
			sp = inttostring(m_eseq) + m_delim;
			return sp;
		}
		cout << " So far so good going ahead " << endl;
		m_eseq = 1;
		sp = inttostring(m_eseq) + m_delim;
		return sp;
	}
	else if (m_seq == -100)
	{
		cout << " Now got the end of file sequence take care" <<endl;
		m_eseq = -100;
		sp = inttostring(m_eseq) + m_delim;
		return sp;
	}
	else
	{
	
		cout << " Processing pay load for sequence number" << m_seq << endl;
		fwrite( spload.c_str(),1,spload.length(),m_fp);
		m_eseq = m_seq+1;
		sp = inttostring(m_eseq) +m_delim;
		cout << " expecting the next sequence as ::::" << m_eseq << endl;
		return sp;
		
	}
	// all ogical returning points are covered . Just preparing a dummy return here
	sp = "-999" +m_delim;	
	return sp;
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

