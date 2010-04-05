import random

def f(x,y):
    random.seed(x+13*y)
    return random.random()*100

def ip1(a,b):
    f1=(1.0-a)*f(0,0)+a*f(1,0)
    f2=(1.0-a)*f(0,1)+a*f(1,1)
    f3=(1.0-b)*f1+b*f2
    return f3
    

def ip2(a,b):
    f1=(1.0-b)*f(0,0)+b*f(0,1)
    f2=(1.0-b)*f(1,0)+b*f(1,1)
    f3=(1.0-a)*f1+a*f2
    return f3
   
def test():
    for i in xrange(1000000):
        x,y=random.random(),random.random()
        v1=ip1(x,y)
        v2=ip2(x,y)
        assert(abs(v2-v1)<1e-10)
        if i%1000==0: print "#%d, diff"%i,v2-v1

    
