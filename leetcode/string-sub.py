
test="word"
n=4
s=2

sub=[]

def traceback(start, k, cur_list):
    if k == 0:
        sub.append(tuple(cur_list))
        return

    print(cur_list)
    for i in range(start, n - k + 1):
        cur_list.append(test[i])
        traceback(i + 1, k - 1, cur_list)
        print(cur_list)
        cur_list.pop()



traceback(0, s, [])
print(sub)
