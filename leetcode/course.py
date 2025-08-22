from collections import deque

def findOrder(num, prereq):
    # init graph
    graph = {}
    indegree = [0] * num
    for i in range(num):
        graph[i] = []

    for a, b in prereq:
        graph[b].append(a)
        indegree[a] += 1
    
    queue = deque()
    for i in range(num):
        if indegree[i] == 0:
            queue.append(i)

    order = []
    while queue:
        node = queue.popleft()
        order.append(node)
        for i in graph[node]:
            indegree[i] -= 1
            if indegree[i] == 0:
                queue.append(i)

    if len(order) == num:
        return order

    return []

num=4
prereq = [[1, 0], [2, 0], [3, 1], [3, 2]]
print(findOrder(num, prereq))
