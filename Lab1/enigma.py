import consts as c
from itertools import permutations, product

rotors = ["EKMFLGDQVZNTOWYHXUSPAIBRCJ",
          "AJDKSIRUXBLHWTMCQGZNPYFVOE",
          "BDFHJLCPRTXVZNYEIWGAKMUSQO",
          "ESOVPZJAYQUIRHXLNFTGKDCMWB",
          "VZBRGITYUPSDNHLXAWMJQOFECK"]
rotors_ring = ['Q', 'E', 'V', 'J', 'Z']
reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT"
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


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
                print("get!!!")
                print(r)
                print(sp)
                print(text)

def Turing_analyse(ciphertext):
    rotor = list(permutations([0, 1, 2, 3, 4], 3))
    start_positions = list(product(alphabet, alphabet, alphabet))

    print("Turing analyse...")




if __name__ == "__main__":
    # print(c.asciiA)
    Rejewski_analyse("HGABLE")
    