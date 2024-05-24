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

def minima(data):
    output = data[1]
    return output

def maxima(data):
    output = data[len(data)-1]
    return output
    
def median(data):
    if len(data) % 2 == 0:  # Check if the length of data is even
        middle_index1 = int(len(data) / 2) - 1  # Index of first middle element
        middle_index2 = int(len(data) / 2)  # Index of second middle element
        output = (data[middle_index1] + data[middle_index2]) // 2  # Calculate the average
    else:  # If length of data is odd
        middle_index = len(data) // 2  # Index of middle element
        output = data[middle_index]  # Middle element itself
    return output
    
def simpleSort(data):
    
    dummy = data[:]
    
    n = len(dummy)
    
    for i in range(n):
        for j in range(0, n-i-1):
            if dummy[j] > dummy[j+1]:
                dummy[j],dummy[j+1] = dummy[j+1],dummy[j]
    return dummy
    
while(b != 3): # replace w/ while 1 ... i dont want this online compiler to explode again
    #define grablength
    b = b+1
    pi_temp = []
    STM_temp = []
    for i in range(captures):
            #grab STM reading
            #RXread ### REPLACE NEXT LN With UART reading (STM)
            STM_temp.append(random.randint(1, 10))
            #grab digital reading
            #pi_temp[i] = random.randint(1, 10) ### REPLACE NEXT LN With DIGIAL reading
            pi_temp.append(random.randint(1, 10))
            #gpioread
    #sortpidata <<< Sort
    pi_temp = simpleSort(pi_temp)
    #sortSTMdata <<< Sort
    STM_temp = simpleSort(STM_temp)
    pi_min = pi_temp[1]
    pi_max = pi_temp[captures-1] # sUs indexing
    pi_median = median(pi_temp)
    
    STM_min = STM_temp[1]
    STM_max = STM_temp[captures-1]
    STM_median = median(STM_temp)
    #print("-" * 20) 
    print(f"Capture: {b}")
    print("=" * 20)
    print(f"Pi Min: {pi_min}")
    print(f"Pi MAX: {pi_max}")
    print("-" * 20)
    print(f"Pi Median: {pi_median}")
    print(f"Pi Average: {average(pi_temp)}")
    print("-" * 20)
    print(f"STM Min: {STM_min}")
    print(f"STM MAX: {STM_max}")
    print("-" * 20)
    print(f"STM Median: {STM_median}")
    print(f"STM Average: {average(STM_temp)}")
    print("_" * 20) 
    # wait x length
    # clear terminal
    
##print(f"rand Data:{pi_temp} of length: {len(pi_temp)}")
#end

