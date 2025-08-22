n = 4
board = {}
solutions = []
for i in range(n):
    board[i] = -1

def check(row, col):
    for i in range(row):
        if board[i] == col:
            return False

        if abs(row - i) == abs(board[i] - col):
            return False
    return True

def backtrace(row):
    if row == n:
        print(board)
        solutions.append(board)
        return

    for col in range(n):
        if check(row, col):
            board[row] = col
            backtrace(row + 1)
            board[row] = -1

backtrace(0)
