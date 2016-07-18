#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "fuze_udpacceptor.h"
 
using namespace std;
 
int main(int argc, char** argv)
{
	if (argc != 4) 
	{
        	cout<< " port: ip : file name" <<endl;
        	exit(1);
    	}
	int port = atoi(argv[1]);
	string ip = argv[2];
	string filename(argv[3]);
	cout << port << "::" << ip << "::" << filename<<endl;
	udpacceptor * ac = NULL;
	ac = new udpacceptor( port, ip.c_str(),filename);
	if ( ac == NULL)
	{
		cout << " failed to initiate" << endl;
		exit (1);
	}
	ac->client_start(); 
 
    	exit(0);
}
