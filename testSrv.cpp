#include<iostream>
#include"networking.h"

using namespace std;

int main()
{
	networkingServer Server;
	tlv test,test2;


	test = Server.receiveMessage();
	cout<<"type: " + to_string(test.type)<<endl
		<<"length: " + to_string(test.length)<<endl
		<<"value 1: " + to_string(test.value.x)<<endl
		<<"value 2: " + to_string(test.value.y)<<endl;


		test2.type=0x1;
		test2.length=2;
		test2.value.x=10;
		test2.value.y=30;

		Server.sendMessage(test2);


	return 0;
}
