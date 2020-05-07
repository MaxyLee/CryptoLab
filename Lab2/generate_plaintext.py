import random

if __name__ == "__main__":
    length = 1 << 14
    print(length)
    with open("random.in", 'w') as f:
        for _ in range(length):
            f.write(str(random.randint(0, 1)))

