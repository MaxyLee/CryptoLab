import random

def gen_num(filename, N):
    num = random.randint(128, 255)

    for i in range((N >> 3) - 1):
        num += (num << 8) + random.randint(0, 255)

    if num % 2 == 0:
        num += 1

    with open(filename, 'w') as f:
        f.write(str(num))
    


if __name__ == "__main__":
    gen_num("num1024.in", 1024)
    gen_num("num2048.in", 2048)