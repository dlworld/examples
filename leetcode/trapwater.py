
def trap(height):
    n = len(height)
    if n <= 2:
        return 0

    max_left = height[0]
    max_right = height[n - 1]
    left = 1
    right = n - 1
    water = 0
    while left <= right:
        if max_left < max_right:
            if height[left] > max_left:
                max_left = height[left]
            else:
                water += max_left - height[left]
                print(f"left {left} {water}")
            left += 1
        else:
            if height[right] > max_right:
                max_right = height[right]
            else:
                water += max_right - height[right]
                print(f"right {right} {water}")
            right -= 1

    return water

height=[0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]
print(trap(height))
