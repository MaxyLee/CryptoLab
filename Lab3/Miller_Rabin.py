import random

def Miller_Rabin(p, round):
    if p == 2 or p == 3:
        return True
    
    if (p & 1) == 0 or p <= 1:
        return False
    
    s = 0
    d = p - 1

    while (d & 1) == 0:
        s += 1
        d >>= 1

    for i in range(round):
        print("round", i)
        a = random.randint(2, p - 1)
        x = pow(a, d, p)
        if x != 1 and x != p - 1:
            j = 1
            while j < s and x != p - 1:
                x = pow(x, 2, p)
                if x == 1:
                    return False
                j += 1
            if x != p - 1:
                return False
    
    return True
        

if __name__ == "__main__":
    p0 = 0
    p1 = 0

    with open("num1024.in", 'r') as f:
        s = f.read()
        p0 = int(s)

    with open("num2048.in", 'r') as f:
        s = f.read()
        p1 = int(s)

    if(Miller_Rabin(p0, 10)):
        print("num1024 is a prime number")
    else:
        print("num1024 is not a prime number")

    if(Miller_Rabin(p1, 10)):
        print("num2048 is a prime number")
    else:
        print("num2048 is not a prime number")

    # print(len(str(p0)))