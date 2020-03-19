# Lab1 Report

## Enigma



## 习题1.5

用`lab1_5.py`实现了穷尽密钥方法对密文进行破解，观察发现当密钥K=10时，对应的明文为`look up in the air its a bird its a plane its superman`

## 习题1.16

逆置换如下：

|       x       |  1   |  2   |  3   |  4   |  5   |  6   |  7   |  8   |
| :-----------: | :--: | :--: | :--: | :--: | :--: | :--: | :--: | :--: |
| $\pi^{-1}(x)$ |  2   |  4   |  6   |  1   |  8   |  3   |  5   |  7   |

用`lab1_16.py`进行解密，得到明文`gentlemen do not read each others mail`

## 习题1.21

### 代换密码

代码见`lab1_26_1.py`

对词频进行统计，结果如下：

`A: 5 B: 0 C: 37 D: 8 E: 12 F: 9 G: 24 H: 5 I: 15 J: 7 K: 18 L: 7 M: 5 N: 13 O: 10 P: 6 Q: 1 R: 0 S: 20 T: 0 U: 14 V: 0 W: 5 X: 7 Y: 15 Z: 13 `

其中频数不少于10的如下：

`C: 37 G: 24 S:20 K: 18 I: 15 Y: 15 U:14 N:13 Z:13 E:12 O:10`

注意到C的频数远高于其他字母，因此猜测$d_k(C)=e$，且已知$d_k(F)=w$，对密文进行代换，结果如下：

`--------e---e-----w---we-----------e-------e------------e---e--e------e----e---e-e------e-------e-----e---------------------------------w-ee------w---e------e-------------e--w------e-----e--e--e---ew-ee------w------e--ew-ee-e--e----e--w--------e--e------e-`

注意到其中w-ee出现次数较多，与密文进行对照后发现这些位置均为FZCCN，且Z、N均属于频数较高对部分，因此猜测它们是t、a、o、i、n、s、h、r中的的一部分，而其中合理的组合为$d_k(Z)=h,d_k(N)=l$，即为单词wheel，在该假设之下，代换所得明文为：

`--------e--le-----w-l-we-----------e-------e------------e--le--e--l---e--h-e---e-e------e------hel----e------------------------------h--wheel-----w--hel----le----------h--e-lw---l--e-----e--e--e--hewheel-----w-----he--ewheele--eh--le--wh--h----e--e------e-`

观察发现其中并没有出现矛盾的情况，并且注意到其中一部分为-hewheel，从语法角度推测he前面的字母极有可能为t，因此假设$d_k(U)=t$，得到：

`------t-e--let----w-l-we----t------e-------e----t-------e--le--e--l---e--h-e---e-e------e------hel----e--------------------t---------ht-wheel-----wt-hel----le------t---h--e-lw---l--e-----e--e-te-thewheel-----w-t--the--ewheele--eh--le--wh--h----e--e-t---te-`

再对剩余的高频词进行大量的假设与验证后，最终得到了合理的密钥如下：

```python
sub = {
    'A':'v', 'B':'x', 'C':'e', 'D':'b', 'E':'i',
    'F':'w', 'G':'a', 'H':'f', 'I':'d', 'J':'c',
    'K':'s', 'L':'y', 'M':'m', 'N':'l', 'O':'n',
    'P':'u', 'Q':'j', 'R':'k', 'S':'o', 'T':'z',
    'U':'t', 'V':'q', 'W':'g', 'X':'p', 'Y':'r',
    'Z':'h'
    }
```

解密后得到明文：

`i may not be able to grow flowers but my garden produces just as many dead leaves old over shoes pieces of rope and bushels of dead grass as anybodys and today i bought a wheel barrow to help in clearing it up i have always loved and respected the wheel barrow it is the one wheeled vehicle of which i am perfect master`

### 维吉尼亚密码

代码见`lab1_26_2.py`

首先使用Kasiski测试法。在密文中，密文串HJV出现在五个位置，起始位置分别为107、125、263、317、329，其距离分别为18、156、210、222。这四个数的最大公因数为6，因此猜测密钥字的长度很可能是6。

再使用重合指数法进行确认。当m=1时，重合指数为0.041；m=2时，重合指数为0.038、0.04712004562303963；m=3时，重合指数为0.055、0.048、0.048；m=4时，重合指数为0.037、0.043、0.038、0.049；m=5时，重合指数为0.043、0.04296698326549073、0.033、0.035、0.043；m=6时，重合指数为0.063、0.084、0.049、0.065、0.043、0.073。可见只有m=6时重合指数值与0.065比较接近。

下面假设m=6，对于$1\le i \le 6$，分别计算其对应的$M_g$，得

```shell
i: 0
M[0]: 0.030 M[1]: 0.035 M[2]: 0.064 M[3]: 0.039 M[4]: 0.034 M[5]: 0.041 M[6]: 0.036 M[7]: 0.031 M[8]: 0.042 M[9]: 0.046 M[10]: 0.025 M[11]: 0.033 M[12]: 0.037 M[13]: 0.042 M[14]: 0.038 M[15]: 0.044 M[16]: 0.036 M[17]: 0.040 M[18]: 0.042 M[19]: 0.032 M[20]: 0.030 M[21]: 0.039 M[22]: 0.044 M[23]: 0.034 M[24]: 0.042 M[25]: 0.033
i: 1 
M[0]: 0.038 M[1]: 0.039 M[2]: 0.048 M[3]: 0.041 M[4]: 0.039 M[5]: 0.036 M[6]: 0.045 M[7]: 0.030 M[8]: 0.026 M[9]: 0.035 M[10]: 0.044 M[11]: 0.030 M[12]: 0.035 M[13]: 0.047 M[14]: 0.040 M[15]: 0.032 M[16]: 0.035 M[17]: 0.070 M[18]: 0.036 M[19]: 0.029 M[20]: 0.029 M[21]: 0.035 M[22]: 0.029 M[23]: 0.037 M[24]: 0.045 M[25]: 0.036
i: 2
M[0]: 0.035 M[1]: 0.036 M[2]: 0.033 M[3]: 0.037 M[4]: 0.035 M[5]: 0.041 M[6]: 0.027 M[7]: 0.037 M[8]: 0.033 M[9]: 0.042 M[10]: 0.041 M[11]: 0.045 M[12]: 0.040 M[13]: 0.042 M[14]: 0.036 M[15]: 0.031 M[16]: 0.034 M[17]: 0.039 M[18]: 0.042 M[19]: 0.031 M[20]: 0.039 M[21]: 0.032 M[22]: 0.034 M[23]: 0.043 M[24]: 0.058 M[25]: 0.045
i: 3
M[0]: 0.045 M[1]: 0.038 M[2]: 0.043 M[3]: 0.037 M[4]: 0.036 M[5]: 0.037 M[6]: 0.031 M[7]: 0.033 M[8]: 0.038 M[9]: 0.037 M[10]: 0.036 M[11]: 0.051 M[12]: 0.040 M[13]: 0.031 M[14]: 0.034 M[15]: 0.066 M[16]: 0.037 M[17]: 0.029 M[18]: 0.039 M[19]: 0.040 M[20]: 0.025 M[21]: 0.035 M[22]: 0.041 M[23]: 0.033 M[24]: 0.034 M[25]: 0.044
i: 4
M[0]: 0.040 M[1]: 0.033 M[2]: 0.034 M[3]: 0.039 M[4]: 0.044 M[5]: 0.033 M[6]: 0.043 M[7]: 0.046 M[8]: 0.046 M[9]: 0.034 M[10]: 0.034 M[11]: 0.035 M[12]: 0.034 M[13]: 0.035 M[14]: 0.033 M[15]: 0.044 M[16]: 0.033 M[17]: 0.035 M[18]: 0.035 M[19]: 0.055 M[20]: 0.040 M[21]: 0.035 M[22]: 0.043 M[23]: 0.044 M[24]: 0.030 M[25]: 0.032
i: 5
M[0]: 0.041 M[1]: 0.038 M[2]: 0.036 M[3]: 0.042 M[4]: 0.038 M[5]: 0.026 M[6]: 0.033 M[7]: 0.039 M[8]: 0.036 M[9]: 0.034 M[10]: 0.047 M[11]: 0.034 M[12]: 0.024 M[13]: 0.036 M[14]: 0.070 M[15]: 0.042 M[16]: 0.031 M[17]: 0.032 M[18]: 0.038 M[19]: 0.032 M[20]: 0.040 M[21]: 0.041 M[22]: 0.035 M[23]: 0.037 M[24]: 0.039 M[25]: 0.048
```

由此得到密钥$K=(2,17,24,15,19,14)$，解密后得到明文如下：

`i learned how to calculate the amount of paper needed for a room when i was at school you multiply the square footage of the walls by the cubic contents of the floor and ceiling combined and double it you then allow half the total for openings such as windows and doors then you allow the other half for matching the pattern then you double the whole thing again to give a margin of error and then you order the paper`

### 仿射密码

代码见`lab1_26_3.py`

考虑到密钥空间只有312，因此尝试采用枚举法破解：遍历所有可能的密钥并解密出对应的明文，并使用类似上一题的方法计算重合指数，当指数接近0.065时则极有可能为正确的密钥。经过枚举法，得到了一个可能的密钥$K=(19, 4)$，对应的明文为：

`o canada `

`terre de nos aieux `

`ton front est ceint de faeurons glorieux `

`car ton bras sait porter lepee `

`il sait porter la croix `

`ton histoire est une epopee `

`des plus brillants exploits `

`et ta valeur de foi trempee `

`protegera nos foyers et nos droits`

注意到明文为法语，因此前面的分析方法是不严谨的，但是明文却符合英文字母的概率分布，这可能是一个巧合

### 未知具体密码

代码见`lab1_26_4.py`

首先统计词频如下：

`A: 17 B: 17 C: 18 D: 9 E: 21 F: 16 G: 16 H: 17 I: 16 J: 12 K: 13 L: 23 M: 21 N: 4 O: 7 P: 8 Q: 6 R: 15 S: 23 T: 12 U: 12 V: 21 W: 11 X: 9 Y: 22 Z: 7`

发现分布相对均匀，因此是代换密码的可能性较小。

假设是仿射密码，使用之间的枚举法尝试破解，结果没有成功。

再考虑是维吉尼亚密码，使用重合指数法发现当m=6时比较符合，之后用前述方法得到密钥$K=(19, 7, 4, 14, 17, 24)$

解密后得到明文：

`i grew up among slow talkers men in particular who dropped words a few at a time like beans in a hill and when i got to minneapolis where people took a lake wobegon comma to mean the end of a story i couldnt speak a whole sentence in company and was considered not too briaht so i enrolled in a speech couqse taught by orvilles and the founder of reflexive relaxology a self hypnotic technique that enabled a person to speak up to three hundred words per minute`

## 习题1.26

若已知m、n，则对密文矩阵求转置即可得到明文

