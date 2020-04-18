import consts as c
from itertools import permutations, product
from operator import itemgetter
from copy import copy

rotors = ["EKMFLGDQVZNTOWYHXUSPAIBRCJ",
          "AJDKSIRUXBLHWTMCQGZNPYFVOE",
          "BDFHJLCPRTXVZNYEIWGAKMUSQO",
          "ESOVPZJAYQUIRHXLNFTGKDCMWB",
          "VZBRGITYUPSDNHLXAWMJQOFECK"]
rotors_ring = ['Q', 'E', 'V', 'J', 'Z']
reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT"
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

              
cycles = []
stack = []

def get_cycle(index, plaintext, ciphertext):
    
    for i in range(len(plaintext)):
        if index == 0:
            stack.append((i,plaintext[i]))
            get_cycle(index + 1, plaintext, ciphertext)
            stack.pop()
        elif ciphertext[stack[-1][0]] == plaintext[i]:
            if (i, plaintext[i]) not in stack:
                stack.append((i,plaintext[i]))
                if stack[0][1] == ciphertext[i]:
                    get_cycle(index + 1, plaintext, ciphertext)
                    cycles.append(copy(stack))
                elif index != len(plaintext) - 1:
                        get_cycle(index + 1, plaintext, ciphertext)
                stack.pop()
    return cycles

def remove_duplicate(cycles):
    rm_cycles = []
    for cycle in cycles:
        d = {key:value for key,value in cycle}
        d = sorted(d.items(), key = itemgetter(0))
        if d not in rm_cycles:
            rm_cycles.append(d)
    return rm_cycles

def check(r, sp, cycle, ch):
    e = ch
    new_sp = sp
    for offset, _ in cycle:
        new_sp = [sp[0], sp[1], chr(c.asciiA + (ord(new_sp[2]) - c.asciiA + offset) % 26)]
        e, _ = encrypt(e, r, new_sp, {})
    return e == ch

def encrypt(plaintext, rotor = [0, 1, 2], start_positions = ['A', 'A', 'A'], steckers = {}):
    cnt = [(ord(sp) - c.asciiA) for sp in start_positions]
    ring = [(ord(r) - c.asciiA) for r in rotors_ring]
    text = plaintext.upper()
    ciphertext = ""

    # print("encrypting:", plaintext, "...")
    # print("rotor:", rotor)
    # print("start positions:", start_positions)
    # print("steckers:", steckers)

    for s in text:
        v = [0] * 8
        out = ""
        # stecker
        if s in steckers:
            v[0] = ord(steckers[s]) - c.asciiA
        else:
            v[0] = ord(s) - c.asciiA
        # check the rotors
        if cnt[2] == ring[rotor[2]]:
            if cnt[1] == ring[rotor[1]]:
                cnt[0] = (cnt[0] + 1) % 26
            cnt[1] = (cnt[1] + 1) % 26
        cnt[2] = (cnt[2] + 1) % 26
        # rotor3
        v[1] = ord(rotors[rotor[2]][(v[0] + cnt[2]) % 26]) - c.asciiA
        # rotor2
        v[2] = ord(rotors[rotor[1]][(v[1] + cnt[1] - cnt[2]) % 26]) - c.asciiA
        # rotor1
        v[3] = ord(rotors[rotor[0]][(v[2] + cnt[0] - cnt[1]) % 26]) - c.asciiA
        # reflector
        v[4] = ord(reflector[(v[3] - cnt[0]) % 26]) - c.asciiA
        # rotor1
        v[5] = rotors[rotor[0]].index(chr((v[4] + cnt[0]) % 26 + c.asciiA)) 
        # rotor2
        v[6] = rotors[rotor[1]].index(chr((v[5] - cnt[0] + cnt[1]) % 26 + c.asciiA))
        # rotor3
        v[7] = rotors[rotor[2]].index(chr((v[6] - cnt[1] + cnt[2]) % 26 + c.asciiA))
        #stecker
        if v[7] in steckers:
            out = steckers[chr((v[7] - cnt[2]) % 26 + c.asciiA)]
        else:
            out = chr((v[7] - cnt[2]) % 26 + c.asciiA)
        ciphertext += out

    return ciphertext, cnt

def Rejewski_analyse(ciphertext):
    rotor = list(permutations([0, 1, 2], 3))
    start_positions = list(product(alphabet, alphabet, alphabet))

    print("Rejewski analyse...")

    for r in rotor:
        for sp in start_positions:
            text, _ = encrypt(ciphertext, list(r), list(sp), {})
            if text[0] == text[3] and text[1] == text[4] and text[2] == text[5]:
                print("Get!!!")
                print(r)
                print(sp)
                print(text)

def Turing_analyse(plaintext, ciphertext):
    rotor = list(permutations([0, 1, 2, 3, 4], 3))
    start_positions = list(product(alphabet, alphabet, alphabet))
    cycles = remove_duplicate(get_cycle(0, plaintext, ciphertext))

    print("Turing analyse...")

    for r in rotor:
        for sp in start_positions:
            cycle_true = True
            for cycle in cycles:
                right = False
                for c in alphabet:
                    if check(r, sp, cycle, c):
                        right = True
                        break
                if not right:
                    cycle_true = False
                    break
            if cycle_true:
                ans, _ = encrypt(plaintext, r, sp, {})
                if ans == ciphertext:
                    print("Get")
                    print(r)
                    print(sp)
                    return

if __name__ == "__main__":
    Turing_analyse("WETTERBERICHTADOLFHITLER", "DIGENINOZYSIPLXYKMIUHHQC")
    # Rejewski_analyse("HGABLE")