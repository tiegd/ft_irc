#include "Channel.hpp"

int main()
{
	Client *client_1 = new Client(1);
	client_1->setNickname("jocelyn");
	client_1->setUsername("petit zob");
	std::cout << "Nickname = " << client_1->getNickname() << "\nUsername = " << client_1->getUsername() << std::endl;

	Client *client_2 = new Client(1);
	client_2->setNickname("gaducurt");
	client_2->setUsername("gros zob");

	std::string name = "zizi";

	Channel channel_1(name, client_1);
	std::cout << "name = " << channel_1.getName() << std::endl;
	channel_1.getOperators();
	channel_1.getUsers();
	std::cout << "\n" << std::endl;
	
	channel_1.addUser(client_2);
	channel_1.getOperators();
	channel_1.getUsers();
	std::cout << std::endl;

	std::cout << "Has password = " << channel_1.getHasPassword() << std::endl;
	std::cout << "password = " << channel_1.getPassword() << std::endl;
	channel_1.setPassword("zizikekette69");
	std::cout << "password = " << channel_1.getPassword() << std::endl;
	std::cout << "Has password = " << channel_1.getHasPassword() << std::endl;
	channel_1.rmPassword();
	std::cout << "password = " << channel_1.getPassword() << std::endl;
	std::cout << "Has password = " << channel_1.getHasPassword() << std::endl;
	std::cout << std::endl;

	std::cout << "nb clients = " << channel_1.getNbMembers() << "\nnb Operators = " << channel_1.getNbOp() << std::endl;
	channel_1.ejectClient(client_1, client_2);
	channel_1.getOperators();
	channel_1.getUsers();
	std::cout << "nb clients = " << channel_1.getNbMembers() << "\nnb Operators = " << channel_1.getNbOp() << std::endl;
	std::cout << std::endl;

	channel_1.addOperator(client_2);
	channel_1.getOperators();
	std::cout << "nb clients = " << channel_1.getNbMembers() << "\nnb Operators = " << channel_1.getNbOp() << std::endl;
	std::cout << std::endl;

	channel_1.rmOperator(client_1);
	channel_1.ejectClient(client_1, client_2);
	std::cout << "nb clients = " << channel_1.getNbMembers() << "\nnb Operators = " << channel_1.getNbOp() << std::endl;
	channel_1.getOperators();
	channel_1.getUsers();
	std::cout << std::endl;

	channel_1.setTopic("speak about tacos 5viandes");
	std::cout << channel_1.getTopic() << std::endl;
	channel_1.rmTopic();
	std::cout << channel_1.getTopic() << std::endl;

	delete client_1;
}