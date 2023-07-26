import math

def cosinesim(target, curr_class):
    magA = 0.0
    magB = 0.0
    dot = 0.0
    for i, elem in enumerate(curr_class):
        magA += elem**2
        magB += target[i]**2;
        dot += elem * target[i]
    return math.acos(dot/ (math.sqrt(magA) * math.sqrt(magB)) )

def similarity(target, classes):
    #assume classes are 2d-array
    closestClass = -1;
    smallest = 10.0
    curr = 0.0
    for i, curr_class in enumerate(classes):
        curr = cosinesim(target, curr_class)
        if (curr < smallest):
            smallest = curr
            closestClass = i
    return closestClass;

