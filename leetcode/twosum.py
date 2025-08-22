def twosum(nums, target):
    lens = len(nums)
    for i in range(0, lens - 1):
        for j in range(i + 1, lens):
            if nums[i] + nums[j] == target:
                return [i, j]

    return []

nums = [2,7,11,15]
target = 9
print(twosum(nums, target))

nums = [3,2,4]
target = 6
print(twosum(nums, target))
