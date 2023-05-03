#include <iostream>
#include <map>
#include <vector>

int main ()
{
//     std::map<Client, Channel> database;
//     std::map<Client, Channel>::iterator databaseIt = database.begin();
//     std::vector<Clinet>::iterator clientIt  = _clinet.begind(); 
//     database.insert(make_pair(client, ))
//     std::vector<Channel> a()
//    while(_clinets.size() > i )
//    {
//     databaseIt->fisrt = clientIt;
//     clientIt++;
//     databaseIt++;
//    }
//     /***************/

//     // part #test => Not Joined Yet!
//     channel c = channnelMap[namechannel]; 
//     c.user[gdhgfd]

    std::map<std::string, std::string> data;

    data["C++"] = "val1";
    data["Java"] = "val2";
    data["Python"] = "val3";
    data["JavaScript"] = "val4";
    data["Haskell"] = "val5";

    std::cout << data["Java"] << std::endl;
    // int i =  (data["c++"] != data.end().) ? 1 : 0;
    std::string val = data["c++"];
    if ( val != "" )
        std::cout << "okhh" << std::endl;
    else
        std::cout << "okhh2" << std::endl;

}