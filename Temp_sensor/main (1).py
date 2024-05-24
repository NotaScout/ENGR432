# Online Python compiler (interpreter) to run Python online.
# Write Python 3 code in this online editor and run it.

#start

import random

captures = 20

b = 0

pi_temp = []
STM_temp = []

def average(data):
    output = 0
    for i in range(captures):
        output = data[i] + output
        
    output = output/captures
    return output

while(b != 3): # replace w/ while 1 ... i dont want this online compiler to explode again
    #define grablength
    b = b+1
    pi_temp = []
    STM_temp = []
    for i in range(captures):
            #grab STM reading
            #RXread
            STM_temp.append(random.randint(1, 10))
            #grab digital reading
            #pi_temp[i] = random.randint(1, 10)
            pi_temp.append(random.randint(1, 10))
            #gpioread
    #sortpidata
    #sortSTMdata
    pi_min = pi_temp[1]
    pi_max = pi_temp[captures-1] # sUs indexing
    STM_min = pi_temp[1]
    STM_max = pi_temp[captures-1]
    #print("-" * 20) 
    print(f"Capture: {b}")
    print("=" * 20)
    print(f"Pi Min: {pi_min}")
    print(f"Pi MAX: {pi_max}")
    print("-" * 20) 
    print(f"STM Min: {STM_min}")
    print(f"STM MAX: {STM_max}")
    print("_" * 20) 

average = average(pi_temp)
print(f"Average: {average}")
print(f"rand Data:{pi_temp} of length: {len(pi_temp)}")
#end

