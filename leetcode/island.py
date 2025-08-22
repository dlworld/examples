import copy
from collections import deque

def numIslands_dfs(grid):
    if not grid or not grid[0]:
        return 0

    rows, cols = len(grid), len(grid[0])
    count = 0

    def dfs(r, c):
        if r < 0 or r >= rows or c < 0 or c >= cols:
            return

        if grid[r][c] != 1:
            return

        # mark visited
        grid[r][c] = 0

        dfs(r - 1, c)
        dfs(r + 1, c)
        dfs(r, c + 1)
        dfs(r, c - 1)

    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 1:
                count += 1
                dfs(r, c)

    return count


def numIslands_bfs(grid):
    if not grid or not grid[0]:
        return 0

    rows, cols = len(grid), len(grid[0])
    count = 0
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]

    def bfs(r, c):
        queue = deque([(r, c)])
        cur_r, cur_c = queue.popleft()
        for dr, dc in directions:
            new_r, new_c = cur_r + dr, cur_c + dc
            if 0 <= new_r < rows and 0 <= new_c < cols and grid[new_r][new_c] == '1':
                queue.append(new_r, new_c)
                grid[new_r][new_c] = '0'

    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == '1':
                bfs(r, c)
                count += 1

    return count


grid = [
    [1, 1, 0, 0, 0],
    [1, 1, 0, 0, 0],
    [0, 0, 1, 0, 0],
    [0, 0, 0, 1, 1]
]

print(numIslands_dfs(copy.deepcopy(grid)))
print(numIslands_bfs(copy.deepcopy(grid)))
