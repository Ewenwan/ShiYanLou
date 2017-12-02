#-*- coding:etf-8 -*-
'''
停车场 车队重排序
从一个列表   转换到另一个列表 所需的次数
'''
# [1,2,3,0,4],
# where 1,2,3,4 are different cars, and 0 is the empty spot.

# And the final state is

# [0,3,2,1,4].
# We can swap 1 with 0 in the initial array to get [0,2,3,1,4] and so on.
# Each step swap with 0 only.
# Edited by cyberking-saga


def garage(initial, final):
    steps = 0
    while initial != final:
        zero = initial.index(0)
        if zero != final.index(0):
            car_to_move = final[zero]
            pos = initial.index(car_to_move)
            initial[zero], initial[pos] = initial[pos], initial[zero]
        else:
            for i in range(len(initial)):
                if initial[i] != final[i]:
                    initial[zero], initial[i] = initial[i], initial[zero]
                    break
        steps += 1
    return steps

if __name__ == "__main__":
    initial = [1, 2, 3, 0, 4]
    final = [0, 3, 2, 1, 4]
    print("initial:", initial)
    print("final:", final)
    print(garage(initial, final))
