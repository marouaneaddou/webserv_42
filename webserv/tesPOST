int main() 
{ 
	// map declaration 
	map<int, Client*> mymap; 
	int new_socket = 3;
    mymap.insert(std::make_pair(new_socket, new Client(new_socket)));
    new_socket = 4;
    mymap.insert(std::make_pair(new_socket, new Client(new_socket)));
	// mapping strings to integers 
// 	mymap["geeksforgeeks"] = 1; 
// 	mymap["computer"] = 2; 
// 	mymap["science"] = 3; 
// 	mymap["portal"] = 4; 

	// accessing the value in a map using key
	cout << "Value at Key " << 4 
		<< " is: " << mymap.at(4)->request->method << std::endl;;
	mymap.at(4)->request->method = "sf kamalna";
	cout << "Value at Key " << 4 
		<< " is: " << mymap.at(4)->request->method << std::endl; ; 
	return 0; 
}
