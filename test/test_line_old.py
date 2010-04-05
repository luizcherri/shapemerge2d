from pyshapemerge2d import Line,Vertex,LinePointGenerator,Vector 
import random

def test_simple_line_gen1():
    #print l
    l=\
        [
         Vertex(0,0),
         Vertex(1,0),
         Vertex(1,1),
         Vertex(2,1),
         Vertex(2,2),
         Vertex(3,2),
         Vertex(3,3),
         Vertex(4,3),
         Vertex(4,4)
         ]
    lineobj=Line.between(Vertex(0,0),Vertex(4,4))
    fw=LinePointGenerator(lineobj)
    for i in xrange(9):
        print fw.get_by_param(i)
        assert fw.get_by_param(i)==l[i]

def test_simple_line_intersect():
    a=Line.between(Vertex(0,0),Vertex(10,0))
    b=Line.between(Vertex(5,-5),Vertex(5,5))
    print "Intersect:",a.ideal_intersection(b)
    assert a.ideal_intersection(b)==Vertex(5,0)

def test_simple_line_reverse1():
    l=Line.between(Vertex(0,0),Vertex(4,0))
    lg=LinePointGenerator(l)
    rl=lg.reversed()
    assert rl.get_v1()==Vertex(4,0)        
    assert rl.get_v2()==Vertex(0,0)


    
def test_simple_line_reverse2():
    l=Line.between(Vertex(0,0),Vertex(4,1))
    lg=LinePointGenerator(l)
    norm=[lg.get_by_param(i) for i in xrange(9)]
    print "norm",norm
    l=lg.reversed()
    lg=LinePointGenerator(l)
    norm_r=[lg.get_by_param(i) for i in xrange(9)]
    print "norm-r",norm_r
    #print sorted(norm),sorted(norm_r)
    assert sorted(norm)==sorted(norm_r)

    print
    
    l=Line.between(Vertex(0,0),Vertex(4,1),3)
    lg=LinePointGenerator(l)
    off=[lg.get_by_param(i) for i in xrange(9)]
    print "off:",off
    l=lg.reversed()
    lg=LinePointGenerator(l)
    off_r=[lg.get_by_param(i) for i in xrange(9)]
    print "off_r:",off_r
    assert sorted(off)==sorted(off_r)

    for i in xrange(1000):
        try:
            l=Line.between(Vertex(
                    random.randint(-100,100),random.randint(-100,100)
                ),Vertex(
                    random.randint(-100,100),random.randint(-100,100)
                    ),random.randint(1,50))
            lg=LinePointGenerator(l)
            off=[lg.get_by_param(i) for i in xrange(lg.get_num_steps()+1)]
            #print "off:",off
            l=lg.reversed()
            lg=LinePointGenerator(l)
            off_r=[lg.get_by_param(i) for i in xrange(lg.get_num_steps()+1)]
            #print "off_r:",off_r
            assert sorted(off)==sorted(off_r)
        except:
            print l
            print lg
            raise
        
def test_simple_line_intersect_integer1():
    a=Line.between(Vertex(0,0),Vertex(10,2))
    b=Line.between(Vertex(5,-5),Vertex(5,5))
    print "Intersect:",Line.plausible_integer_intersection(a,b)

def get_first_p_brute(lg,x):
    for i in xrange(lg.get_num_steps()):
        v=lg.get_by_param(i)
        if (v.get_x()==x):
            return i;     
    raise Exception("Didn't find x=%d"%(x,))       
    
def test_get_first_p_check():
    l=Line.between(Vertex(0,0),Vertex(4,1))
    lg=LinePointGenerator(l)
    p=lg.get_first_p_for(2)
    print "p:",p
    assert p==3
    assert get_first_p_brute(lg,2)==3
def test_get_first_p_check2():
    for iy in xrange(100):
        l=Line.between(Vertex(0,0),Vertex(4,iy))
        lg=LinePointGenerator(l)
        p=lg.get_first_p_for(2)
        brutep=get_first_p_brute(lg,2)
        print "Brute-force p:",brutep
        print "p:",p
        assert brutep==p
        