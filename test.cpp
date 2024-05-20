#include <iostream>
#include <vector>
#include <cmath>


int compare(std::string s1, std::string s2)
{
    size_t len_s1 = s1.size();
    size_t len_s2 = s2.size();
    int i = 0;
    int count = 0;
    while (len_s1 && len_s2)
    {
        if(s1[i] == s2[i])
            count += 1;

        i += 1;
        len_s1 -= 1;
        len_s2 -= 1;
    }
    return (count);
}


int get_pos(std::vector<int> data, int value)
{
    for (size_t i = 0; i < data.size(); i += 1)
    {
        if (data[i] == value)
            return(i);
    } 
    return(0);
}

void you_mean(std::vector<std::string> data, std::string key)
{
    std::vector<int> values;

    for (size_t i = 0; i < data.size(); i += 1)
    {
        values.push_back(compare(data[i], key));
    }
    std::cout << "Did you mean " << data[get_pos(values, *max_element(values.begin(), values.end()))] 
        << ", instead of " << key << std::endl;

}


int main()
{
    std::vector<std::string> data;
    data.push_back("hello");
    data.push_back("girl");
    data.push_back("boy");

    you_mean(data, "boohs");
}



