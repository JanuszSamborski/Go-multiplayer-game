#include<iostream>
#include"networking.h"

using namespace std;

int main()
{
	networkingClient Client("2001:db8:0:f101::1");
	tlv test, test2;
	test.type=0x1;
	test.length=2;
	test.value.x=10;
	test.value.y=20;

	Client.sendMessage(test);

	test2 = Client.receiveMessage();
	cout<<"type: " + to_string(test2.type)<<endl
		<<"length: " + to_string(test2.length)<<endl
		<<"value 1: " + to_string(test2.value.x)<<endl
		<<"value 2: " + to_string(test2.value.y)<<endl;



	return 0;
}
