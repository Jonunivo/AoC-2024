#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <tuple>
#include <map>
#include <queue>
#include <stack>
#include <array>
#include <unordered_map>

using namespace std;

class Solution
{
private:
    fstream fin;
    string data;

    void parse()
    {
        string line;
        while (getline(this->fin, line))
        {
            this->data += line;
        }
    }

    void fragmentation(vector<tuple<int64_t, int>> &nums, int pos)
    {
        for (int i = 0; i < pos; ++i)
        {
            if (get<0>(nums[i]) != -1)
            {
                continue;
            }

            if (get<1>(nums[i]) == get<1>(nums[pos]))
            {
                swap(nums[pos], nums[i]);

                return;
            }
            else if (get<1>(nums[i]) > get<1>(nums[pos]))
            {

                tuple<uint64_t, int> temp = {-1, get<1>(nums[i]) - get<1>(nums[pos])};
                tuple<uint64_t, int> temp2 = {-1, get<1>(nums[pos])};

                nums[i] = {get<0>(nums[pos]), get<1>(nums[pos])};
                nums[pos] = temp2;
                nums.insert(nums.begin() + i + 1, temp);

                return;
            }
            else
            {
                continue;
            }
        }
    }

    void combineFragmentations(vector<tuple<int64_t, int>> &nums)
    {
        int pos = 0;
        while (pos < nums.size() - 1)
        {
            if (get<0>(nums[pos]) == -1 && get<0>(nums[pos + 1]) == -1)
            {
                get<1>(nums[pos]) += get<1>(nums[pos + 1]);
                nums.erase(nums.begin() + pos + 1);
            }
            else
            {
                pos++;
            }
        }
    }

public:
    Solution(string fileName)
    {
        this->fin.open(fileName, ios::in);
        this->parse();
        this->fin.close();
    }

    auto part1() -> uint64_t
    {
        uint64_t sol = 0;
        vector<uint64_t> nums;
        uint64_t c = 0;

        for (int i = 0; i < this->data.size(); i++)
        {
            if (i % 2 == 0)
            {
                for (int j = 0; j < this->data[i] - '0'; j++)
                {
                    nums.push_back(c);
                }

                c++;
            }
            else
            {
                for (int j = 0; j < this->data[i] - '0'; j++)
                {
                    nums.push_back(-1);
                }
            }
        }

        int low = 0, high = nums.size() - 1;

        while (low < high)
        {
            if (nums[low] != -1)
            {
                low++;
            }
            else if (nums[high] == -1)
            {
                high--;
            }
            else
            {
                swap(nums[low], nums[high]);
                low++;
                high--;
            }
        }

        for (uint64_t i = 0; i < nums.size(); i++)
        {
            if (nums[i] != -1)
            {
                sol += (uint64_t)nums[i] * i;
            }
        }

        return sol;
    }
    auto part2() -> uint64_t
    {
        uint64_t sol = 0;
        vector<tuple<int64_t, int>> nums;
        uint64_t c = 0;

        for (int i = 0; i < this->data.size(); i++)
        {
            if (i % 2 == 0)
                nums.push_back({c++, this->data[i] - '0'});
            else
                nums.push_back({-1, this->data[i] - '0'});
        }

        for (int i = nums.size() - 1; i > 0; i--)
        {
            if (get<0>(nums[i]) == -1)
                continue;

            this->fragmentation(nums, i);
            this->combineFragmentations(nums);
        }

        uint64_t id = 0;
        for (int i = 0; i < nums.size(); i++)
        {
            if (get<0>(nums[i]) != -1)
                for (int j = 0; j < get<1>(nums[i]); j++)
                    sol += get<0>(nums[i]) * id++;
            else
            {
                id += get<1>(nums[i]);
            }
        }

        return sol;
    }
};

auto main() -> int
{

    Solution aoc = Solution("input.txt");
    //Solution test1 = Solution("test1.txt");
    //Solution test2 = Solution("test2.txt");

    //cout << "Part 1 test: " << test1.part1() << endl;
    cout << "Part 1: " << aoc.part1() << endl;
    cout << "--------------------------" << endl;
   // cout << "Part 2 test: " << test2.part2() << endl;
    cout << "Part 2: " << aoc.part2() << endl;

    return 0;
}