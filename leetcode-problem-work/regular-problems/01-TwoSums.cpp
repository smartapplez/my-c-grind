#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class Solution {
public:
  vector<int> onepass_twoSum(vector<int> &nums, int target) {
    // This will essentially be the map of complements
    unordered_map<int, int> numMap;
    int n = nums.size();

    for (int i = 0; i < n; i++) {
      int complement = target - nums[i];
      // Check to see if the map contains the complement of the target
      if (numMap.contains(complement))
        return {numMap[complement], i};

      // Record the {key:nums_val,value:index} pair
      // This happens afterward because we have already recorded this index
      // value
      numMap[nums[i]] = i;
    }
    return {};
  }

  vector<int> twopass_twoSum(vector<int> &nums, int target) {
    // This will be the map of nums, we will look if the complements are within
    // that map
    unordered_map<int, int> numMap;
    int n = nums.size();

    // Storing all of the nums in the unordered_map
    for (int i = 0; i < nums.size(); i++) {
      numMap[nums[i]] = i;
    }

    // Checking if the complement is in the unordered_map that doesn't return
    // the same index value
    for (int i = 0; i < nums.size(); i++) {
      int complement = target - nums[i];
      if (numMap.count(complement) && numMap[complement] != i) {
        return {numMap[complement], i};
      }
    }
    return {};
  }
};

int main() {
  Solution soln;
  vector<int> nums = {2, 7, 11, 15};
  vector<int> twoSums = soln.onepass_twoSum(nums, 9);
  for (int i = 0; i < twoSums.size(); i++) {
    cout << "nums[" << i << "] = " << twoSums[i] << endl;
  }

  nums = {3, 2, 4};
  twoSums = soln.twopass_twoSum(nums, 6);
  for (int i = 0; i < twoSums.size(); i++) {
    cout << "nums[" << i << "] = " << twoSums[i] << endl;
  }
}
