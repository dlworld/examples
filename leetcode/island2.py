
from collections import deque

def bfs(land, i, j):
    queue = deque()
    queue.append((i, j))
    print(queue)
    while queue:
        i, j = queue.popleft()

        directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
        for (d_i, d_j) in directions:
            new_i = i + d_i
            new_j = j + d_j

            if (new_i >= 0 and new_j >= 0 and new_i < len(land) and new_j < len(land[0]) and land[new_i][new_j] == 1):
                queue.append((new_i, new_j))
                print(i, j, queue)
                land[new_i][new_j] = -1
            
def land_bfs(land):
    rows = len(land)
    cols = len(land[0])
    counts = 0

    for i in range(rows):
        for j in range(cols):
            if (land[i][j] == 1):
                counts += 1
                bfs(land, i, j)

    return counts


def dfs(land, i, j):
    if (i < 0 or j < 0 or i >= len(land) or j >= len(land[0])):
        return

    if (land[i][j] != 1):
        return

    land[i][j] = -1

    dfs(land, i, j + 1)
    dfs(land, i, j - 1)
    dfs(land, i + 1, j)
    dfs(land, i - 1, j)

def land_dfs(land):
    rows = len(land)
    cols = len(land[0])
    counts = 0

    for i in range(rows):
        for j in range(cols):
            if (land[i][j] == 1):
                counts += 1
                dfs(land, i, j)

    return counts


grid = [
    [1, 1, 0, 1, 1],
    [1, 0, 0, 0, 0],
    [0, 0, 0, 0, 1],
    [1, 1, 0, 1, 1]
]
#print(land_dfs(grid))
print(land_bfs(grid))
