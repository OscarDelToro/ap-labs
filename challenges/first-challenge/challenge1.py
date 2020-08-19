L = ['a', 'b', 't',['cc', 'dad', ['eeoo', 'fiction']], 'g', 'hi']
b=[1,4,'b','hola',[[[1,5,8,[23,56,[1]]]]]]

def deplen(a):
    sum=0
    for x in a:
        if type(x) == tuple or type(x) == list:
            sum+=deplen(x)
        else:
            sum+=1
    return sum

print ("Length of L =", deplen(b));