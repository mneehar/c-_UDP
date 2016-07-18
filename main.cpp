// This file is for testing the udp server application
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include "fuze_udpacceptor.h"
using namespace std;
 
int main(int argc , char ** argv)
{
	// Best way to do this from the config file by making the config reader. But
	// taking short cut for time being.

	if ( argc != 2 )
	{
		cout << " please give port and ip" << endl;
		return -1;
	}

	int port = atoi(argv[1]);
	cout << " Input Port  :" << port  << endl;
 

        // Keeping it open for taking now as single thread. But later on we can 
	// expand it for multi thread
	//udpstream* stream = NULL;
	udpacceptor* acceptor = NULL;
	// as we are creating server application over here no need to have the IP we will 
	// connect as ANY
	acceptor = new udpacceptor(port);
	if ( acceptor != NULL )
	{
		acceptor->start();
	}
	return 0;

}
