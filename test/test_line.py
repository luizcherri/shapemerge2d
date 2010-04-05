from pyshapemerge2d import Line,Vertex,Vector,floor2,ceil2,FirstOctant
from visualize import draw_things,Square
import visualize
import random

def test_floor1():
    assert floor2(5,10)==0
    assert floor2(1,10)==0
    assert floor2(9,10)==0
    assert floor2(10,10)==1
    assert floor2(11,10)==1
    assert floor2(19,10)==1
    assert floor2(20,10)==2
    assert floor2(0,10)==0
    assert floor2(-1,10)==-1
    assert floor2(-9,10)==-1
    assert floor2(-10,10)==-1
    assert floor2(-11,10)==-2

def test_ceil1():
    assert ceil2(5,10)==1
    assert ceil2(1,10)==1
    assert ceil2(9,10)==1
    assert ceil2(10,10)==1
    assert ceil2(11,10)==2
    assert ceil2(19,10)==2
    assert ceil2(20,10)==2
    assert ceil2(0,10)==0
    assert ceil2(-1,10)==0
    assert ceil2(-9,10)==0
    assert ceil2(-10,10)==-1
    assert ceil2(-11,10)==-1


def test_revline1():
    l=Line(Vertex(0,0),Vertex(3,0))
    lrev=l.reversed()
    ge1=lrev.get(1)
    print ge1
    assert ge1==Vertex(2,0) 
    assert lrev.get(0)==Vertex(3,0) 
    assert lrev.get(2)==Vertex(1,0) 
    assert lrev.get(3)==Vertex(0,0)

    assert l.get(0)==Vertex(0,0) 
    assert l.get(1)==Vertex(1,0) 
    assert l.get(2)==Vertex(2,0) 
    assert l.get(3)==Vertex(3,0)
    
     

def test_line1():
    l=Line(Vertex(0,0),Vertex(10,0))
    assert l.get(0)==Vertex(0,0) 
    assert l.get(1)==Vertex(1,0) 
    assert l.get(10)==Vertex(10,0) 

    l=Line(Vertex(0,0),Vertex(10,1))
    assert l.get(0)==Vertex(0,0) 
    assert l.get(4)==Vertex(4,0) 
    assert l.get(5)==Vertex(5,1) 
    assert l.get(6)==Vertex(6,1) 
    assert l.get(10)==Vertex(10,1) 

def test_line2():
    l=Line(Vertex(0,0),Vertex(2,2))
    assert l.get(0)==Vertex(0,0)
    assert l.get(1)==Vertex(1,1)
    assert l.get(2)==Vertex(2,2)
def test_line3():
    l=Line(Vertex(0,0),Vertex(3,2))
    assert l.get(0)==Vertex(0,0)
    assert l.get(1)==Vertex(1,1)
    assert l.get(2)==Vertex(2,1)
    assert l.get(3)==Vertex(3,2)
def test_line4():
    l=Line(Vertex(0,0),Vertex(3,-2))
    assert l.get(0)==Vertex(0,0)
    assert l.get(1)==Vertex(1,-1)
    assert l.get(2)==Vertex(2,-1)
    assert l.get(3)==Vertex(3,-2)

def test_line5():
    l=Line(Vertex(0,0),Vertex(-3,2))
    assert l.get(0)==Vertex(0,0)
    assert l.get(1)==Vertex(-1,1)
    assert l.get(2)==Vertex(-2,1)
    assert l.get(3)==Vertex(-3,2)
    
def test_line6():
    l=Line(Vertex(0,0),Vertex(-2,3))
    assert l.get(0)==Vertex( 0,0)
    assert l.get(1)==Vertex(-1,1)
    assert l.get(2)==Vertex(-1,2)
    assert l.get(3)==Vertex(-2,3)

def test_line7():
    l=Line(Vertex(0,0),Vertex(-2,-3))
    assert l.get(0)==Vertex( 0,0)
    assert l.get(1)==Vertex(-1,-1)
    assert l.get(2)==Vertex(-1,-2)
    assert l.get(3)==Vertex(-2,-3)

def test_lines():
    for i in xrange(100):
        dx=random.randint(-1000,1000)
        dy=random.randint(-1000,1000)
        startx=random.randint(-1000,1000)    
        starty=random.randint(-1000,1000)
        l=Line(Vertex(startx,starty),Vertex(dx+startx,dy+starty))
        maxp=l.grid_maxnorm()    
        assert l.get(0)==Vertex(startx,starty)
        assert l.get(maxp)==Vertex(startx+dx,starty+dy)
        lastpos=None
        poss=[]
        for p in xrange(maxp+1):
            pos=l.get(p)
            poss.append(pos)
            if lastpos!=None:
                delta=pos-lastpos
                mn=delta.maxnorm()
                assert mn==1
            lastpos=pos
        lrev=l.reversed()
        poss_rev=list(([lrev.get(i) for i in xrange(maxp+1)]))
        assert list(reversed(poss))==poss_rev
        
def points(l):
    s=set()
    for p in xrange(0,l.grid_maxnorm()+1):
        s.add(l.get(p))
    return s
def test_pinch():
    l=Line(Vertex(0,0),Vertex(4,0))
    print l
    print "points l",points(l)
    l1,l2=list(l.pinch(Vertex(2,0)))    
    assert l1.v2()==l2.v1()
    assert l1.v1()==Vertex(0,0)
    assert l2.v2()==Vertex(4,0)
    print l1,l2
    print "l1+l2:",points(l1).union(points(l2))
def test_pinch2():
    random.seed(0)
    def r():
        return random.randint(0,40)
    for x in xrange(100):        
        l=Line(Vertex(r(),r()),Vertex(r(),r()))
        #print "-----------------------------------"
        #print "l: ",l
        for p in xrange(1,l.grid_maxnorm()):
            splitp=l.get(p)
            l1,l2=list(l.pinch(splitp))
            #print l1,l2
            #print "l1+l2:", sorted(points(l1).union(points(l2)))
            #print "l:",sorted(points(l))
            pl=points(l)
            pl1=points(l1)
            pl2=points(l2)
            assert pl1.union(pl2)==pl
            assert len(pl1.intersection(pl2))==1
            
def test_intersect():
    A=Line(Vertex(0,2),Vertex(10,2))            
    B=Line(Vertex(0,0),Vertex(10,5))
    isect,=list(A.intersection(B))
    print "isect:",points(isect)
    print "facit:",points(A).intersection(points(B))                
    assert points(isect)==points(A).intersection(points(B))
def test_intersect2():
    A=Line(Vertex(0,2),Vertex(10,2))            
    B=Line(Vertex(5,-5),Vertex(5,5))
    isect,=list(A.intersection(B))
    print "isect:",points(isect)
    print "facit:",points(A).intersection(points(B))                
    assert points(isect)==points(A).intersection(points(B))

def extremes(points):
    xs=[p.get_x() for p in points]
    ys=[p.get_y() for p in points]
    return (Vertex(min(xs),min(ys)),
            Vertex(max(xs),max(ys)))
def visualize_diff(apoints,bpoints,A,B,extras=[]):
    apoints=set(apoints)
    bpoints=set(bpoints)
    sqs=[]
    for p in apoints.intersection(bpoints):
        sqs.append(Square(p.get_x(),p.get_y(),p.get_x()+1,p.get_y()+1,'red'))
    for p in apoints.difference(bpoints):
        sqs.append(Square(p.get_x(),p.get_y(),p.get_x()+1,p.get_y()+1,'blue'))
    for p in bpoints.difference(apoints):
        sqs.append(Square(p.get_x(),p.get_y(),p.get_x()+1,p.get_y()+1,'green'))
    for line in [A,B]:
        sqs.append(visualize.Line(
            line.get_x1_inexact(),
            line.get_y1_inexact(),
            line.get_x2_inexact(),
            line.get_y2_inexact()
            ))
    sqs.extend(extras)
    draw_things(sqs)
    
def test_intersect3():
    random.seed(0)
    def r():
        return Vertex(random.randint(-10,10),
                      random.randint(-10,10))
    def r2A():
        return Vertex(random.randint(5,10),
                      random.randint(5,10))
    def r2B():
        return Vertex(random.randint(-10,10),
                      random.randint(-10,10))
    for x in xrange(100000):
        #print "#%d:============================================================"%x
        A=Line(r(),r())            
        B=Line(r(),r())
        facit=points(A).intersection(points(B))
        #print "A: %s, B: %s"%(A.floatrepr(),B.floatrepr())
        if len(facit)==0:
            continue #no intersect
        isects=list(A.intersection(B))
        #oct=FirstOctant(Line(Vertex(0,0),Vertex(10,1)))
        oct=FirstOctant(A)
        #assert 0
        if len(isects)==0:
            #print "Facit: %s, but found no isects"%(facit,)
            isect=Line()
        else:
            isect,=isects
        
        #print "Facit:",extremes(facit)        
        #print "Isect:",extremes(points(isect))
        extras=[]
        for e in extremes(facit):
            e=oct.transform_vertex(e)
            extras.append(
                    Square(e.get_x(),e.get_y(),e.get_x()+1,e.get_y()+1,'white'))
        for e in extremes(points(isect)):
            e=oct.transform_vertex(e)
            extras.append(
                    Square(e.get_x(),e.get_y(),e.get_x()+1,e.get_y()+1,'yellow'))

        if extremes(points(isect))!=extremes(points(A).intersection(points(B))):
            print "Assertion fail. Oct:",oct.__repr__()
            print "Should:",extremes(points(A).intersection(points(B)))
            print "Is:",extremes(points(isect))            
            visualize_diff(
            [oct.transform_vertex(x) for x in points(A)],
            [oct.transform_vertex(x) for x in points(B)],
            oct.transform_line(A),oct.transform_line(B),
            extras
            )
            assert 0


def test_intersect3a():
    #print "#%d:============================================================"%x
    A=Line(Vertex(1,3),Vertex(10,10))            
    B=Line(Vertex(-5,6),Vertex(4,4))            
    facit=points(A).intersection(points(B))
    #print "A: %s, B: %s"%(A.floatrepr(),B.floatrepr())
    isects=list(A.intersection(B))
    #oct=FirstOctant(Line(Vertex(0,0),Vertex(10,1)))
    oct=FirstOctant(A)
    #assert 0
    if len(isects)==0:
        print "Facit: %s, but found no isects"%(facit,)
        isect=Line()
    else:
        isect,=isects
    
    #print "Facit:",extremes(facit)        
    #print "Isect:",extremes(points(isect))
    extras=[]
    for e in extremes(facit):
        e=oct.transform_vertex(e)
        extras.append(
                Square(e.get_x(),e.get_y(),e.get_x()+1,e.get_y()+1,'white'))
    for e in extremes(points(isect)):
        e=oct.transform_vertex(e)
        extras.append(
                Square(e.get_x(),e.get_y(),e.get_x()+1,e.get_y()+1,'yellow'))
        
    visualize_diff(
            [oct.transform_vertex(x) for x in points(A)],
            [oct.transform_vertex(x) for x in points(B)],
            oct.transform_line(A),oct.transform_line(B),
            extras
            )
    assert extremes(points(isect))==extremes(points(A).intersection(points(B)))


def test_yrange_of_x1():    
    k=10
    m=0
    ik=1.0/k
    im=0    
    def f(x):
        return x*float(k)+m
    def fi(y):
        return y*float(ik)+im
    l=Line.exact_line(Vertex(0,0),Vertex(3,30))
    print sorted(points(l))
    hy=l.high_y_of_x(1)
    print "high y:", hy
    assert hy==19
    print "low: ",l.low_y_of_x(1)
    print "high: ",l.high_y_of_x(1)
    assert l.low_y_of_x(1)==10
    assert l.low_y_of_x(0)==0
    assert l.high_y_of_x(0)==9
    assert l.low_y_of_x(2)==20
    assert l.high_y_of_x(2)==29
    assert l.low_y_of_x(3)==30
    assert l.high_y_of_x(3)==30

def test_yrange_of_x1b():    

    l=Line(5,2,4,1,11,2,41,2)

    k=(11/2.0-4/1.0)/(41/2.0-4)
    m=4-k*5/2.0
    print "k:%f, m: %f"%(k,m)
    ik=1.0/k
    im=0    
    def f(x):
        return x*float(k)+m
    def fi(y):
        return y*float(ik)+im
           
    print "low 3: ",l.low_y_of_x(3)
    print "high 3: ",l.high_y_of_x(3)
    print "low 4: ",l.low_y_of_x(4)
    print "high 4: ",l.high_y_of_x(4)

    
def visualize_facit(A,B):
    visualize_diff(
                points(A),
                points(B),
                A,B)
        
def test_intersect4():
    A=Line(Vertex(0,2),Vertex(10,2))            
    B=Line(Vertex(5,0),Vertex(10,10))
    isect,=list(A.intersection(B))
    print "isect:",points(isect)
    visualize_facit(A,B)
    print "facit:",points(A).intersection(points(B))                
    assert points(isect)==points(A).intersection(points(B))


if __name__=='__main__':
    print "Wait"
    raw_input()
    A=Line(Vertex(0,2),Vertex(10,2))            
    B=Line(Vertex(5,0),Vertex(10,10))
    isect,=list(A.intersection(B))
