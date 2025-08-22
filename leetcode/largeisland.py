from collections import deque

directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
def bfs(grid, x, y, cur_id):
    n = len(grid)
    records = deque([(x, y)])
    grid[x][y] = cur_id
    size = 1

    while records:
        x, y = records.popleft()
        for i, j in directions:
            x_n = x + i
            y_n = y + j
            if x_n >= 0 and x_n < n  and y_n >= 0 and y_n < n and grid[x_n][y_n] == 1:
                records.append([(x_n, y_n)])
                size += 1
                grid[x_n][y_n] = cur_id

    return size


def big_island(grid):
    n = len(grid)
    island_sizes = {}
    cur_id = 2

    for x in range(n):
        for y in range(n):
            if grid[x][y] == 1:
                size = bfs(grid, x, y, cur_id)
                island_sizes[cur_id] = size
                cur_id += 1

    print(island_sizes)
    max_size = max(island_sizes.values()) if island_sizes else 0

    for x in range(n):
        for y in range(n):
            if grid[x][y] == 0:
                neighbors = set()
                for i, j in directions:
                    x_n = x + i
                    y_n = y + j
                    if x_n >= 0 and x_n < n  and y_n >= 0 and y_n < n and grid[x_n][y_n] >= 2:
                        neighbors.add(grid[x_n][y_n])

                size = 1
                for i in neighbors:
                    size += island_sizes[i]
                if size > max_size:
                    max_size = size
    return max_size

grid = [[1, 0], [0,1]]
print(big_island(grid))
