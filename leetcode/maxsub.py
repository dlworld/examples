def lengthOfLongestSubstring(s: str) -> int:
    max = []
    cur = []

    for i in range(len(s)):
        c = s[i]
        if c not in cur:
            cur.append(c)
        else:
            if len(cur) > len(max):
                max = cur
            cur = [c]

    return len(max)

s = "abcabcbb"
print(lengthOfLongestSubstring(s))
s = "bbbb"
print(lengthOfLongestSubstring(s))
            
