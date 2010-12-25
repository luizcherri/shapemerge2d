from pyshapemerge2d import Line,Vertex,Vector
from visualize import draw_things,Square
import visualize
import random

def points(l):
    return set(l.slow_all_vertices())
def visualize_diff(lines,extras=[]):
    points=dict()
    for line,col in lines:
        for point in list(line.slow_all_vertices()):
            points.setdefault(point,[]).append(col)
    sqs=[]
    for p,collist in points.items():
        R,G,B=0,0,0
        for r,g,b in collist:
            R+=r
            G+=g
            B+=b
        R/=len(collist)
        G/=len(collist)
        B/=len(collist)
        sqs.append(Square(p.get_x()+0.1,p.get_y()+0.1,p.get_x()+0.9,p.get_y()+0.9,(R,G,B)))
                    
    for line,col in lines:
        print "Drawing Line:",line.get_x1_inexact(),\
            line.get_y1_inexact(),\
            line.get_x2_inexact(),\
            line.get_y2_inexact(),line.taxilen()
        if line.taxilen()==0:
                sqs.append(Square(
                    line.get_v1().get_x()+0.4,
                    line.get_v1().get_y()+0.4,
                    line.get_v2().get_x()+0.6,
                    line.get_v2().get_y()+0.6,
                       tuple(tx/2 for tx in col)))            
                continue
        if (abs(line.get_x1_inexact()-
                line.get_x2_inexact())+
            abs(line.get_y1_inexact()-
                line.get_y2_inexact()))<=0.01:            
            sqs.append(visualize.Line(
                    line.get_v1().get_x()+0.5,
                    line.get_v1().get_y()+0.5,
                    line.get_v2().get_x()+0.5,
                    line.get_v2().get_y()+0.5,                       
                    tuple(tx/2 for tx in col)))
                #Square(line.get_x1_inexact()-0.1,
                #       line.get_y1_inexact()-0.1,
                #       line.get_x2_inexact()+0.1,
                #       line.get_y2_inexact()+0.1,                       
                #       tuple(tx/2 for tx in col)))            
        else:
            sqs.append(visualize.Line(
                line.get_x1_inexact(),
                line.get_y1_inexact(),
                line.get_x2_inexact(),
                line.get_y2_inexact(),
                tuple(tx/2 for tx in col)
                ))
    sqs.extend(extras)
    draw_things(sqs)

def showtest_vis():
    print "test vis"
    l=Line(Vertex(-5,-5),Vertex(5,5))
    visualize_diff([(l,(255,0,0))])

def test_split():
    l=Line(Vertex(0,0),Vertex(2,5))
    for splitpoint in [Vertex(0,0),
                       Vertex(1,3),
                       Vertex(2,5)
                       ]:
        la,lb=l.split(splitpoint)
        assert points(la).union(points(lb))==points(l)
                   
def test_downslope():
    l=Line(Vertex(0,5),Vertex(7,0))
    vs=list(l.slow_all_vertices())
    #visualize_diff([(l,(255,0,0))])
    assert(Vertex(0,5) in vs)
    assert(Vertex(7,0) in vs)
def test_downslope2():
    l=Line(Vertex(7,0),Vertex(0,5))
    vs=list(l.slow_all_vertices())
    #visualize_diff(vs,vs,l,l)
    assert(Vertex(7,0) in vs)
    assert(Vertex(0,5) in vs)
def single_overlap(a,b):
    assert len(a.intersection(b))==1 
    
def all_pairs(seq):
    l = len(seq)
    for i in range(l):
        for j in range(i+1, l):
            yield seq[i], seq[j]

    
def test_intersection_ex1():    
    la=Line(Vertex(5,5),Vertex(3,2))
    lb=Line(Vertex(4,1),Vertex(4,5))
    print "la: %s, lb: %s"%(la,lb)
    ls=list(la.intersection_points(lb))
    print "ls:",ls
    #visualize_diff([
    #                (la,(255,0,0)),
    #                (lb,(0,255,0)),
    #                ],[])
    ls=list(la.intersect2(lb))
    print "Ls:",ls
    #visualize_diff([
    #                (la,(255,0,0)),
    #                (lb,(0,255,0)),
    #                (ls[0],(0,0,255)),
    #                ],[])
def test_intersection_ex2():    
    la=Line(Vertex(0,2),Vertex(2,2))
    lb=Line(Vertex(1,1),Vertex(1,4))
    ls=list(la.intersection_points(lb))
    assert set(ls)==set([Vertex(1,2)])
def test_intersection_ex3():    
    la=Line(Vertex(0,2),Vertex(2,2))
    lb=Line(Vertex(1,4),Vertex(1,1))
    ls=list(la.intersection_points(lb))
    print "Actual: %s"%(ls,)
    assert set(ls)==set([Vertex(1,2)])

def test_adjacency():
    def r():
        return random.randint(-50,50)
    for x in xrange(1000):
        la=Line(Vertex(r(),r()),Vertex(r(),r()))
        lp=points(la)
        for p in lp:
            found=False
            for dx,dy in [(-1,0),(1,0),(0,-1),(0,1)]:
                if Vertex(p.get_x()+dx,p.get_y()+dy):
                    found=True
            assert(found)



def test_intersection2():    
    def r():
        return random.randint(-50,50)
    random.seed(0)
    for x in xrange(1000):
        la=Line(Vertex(r(),r()),Vertex(r(),r()))
        lb=Line(Vertex(r(),r()),Vertex(r(),r()))
        if not points(la).intersection(points(lb)):
            continue        
        ls=list(la.intersect2(lb))
        for a,b in all_pairs(ls):
            l2=list(a.intersect2(b))
            if len(l2)!=0:
                print "Problematic intersect: %s %s"%(la,lb)
                visualize_diff([
                                (la,(255,0,0)),
                                (lb,(0,255,0)),
                                (ls[0],(0,0,255)),
                                (ls[4],(0,0,255)),
                                ],[])
                visualize_diff([
                                (a,(255,0,0)),
                                (b,(0,255,0)),
                                (l2[0],(0,0,255)),
                                ],[])
                
            assert len(l2)==0
                        
def test_intersection():    
    def r():
        return random.randint(-50,50)
    random.seed(0)
    for x in xrange(1000):
        la=Line(Vertex(r(),r()),Vertex(r(),r()))
        lb=Line(Vertex(r(),r()),Vertex(r(),r()))
        if not points(la).intersection(points(lb)):
            continue
        try:
            ls=list(la.intersect(lb))
            have_split=False
            assert len(ls)>0    
            middle=ls[0]
            a,b,c,d=ls[1:]
            have_split=True
            sqs=[]
            sm=points(middle)
            sa=points(a)
            sb=points(b)
            sc=points(c)
            sd=points(d)
            sla=points(la)
            slb=points(lb)
            aplusb=points(la).union(points(lb))
            assert sm.intersection(aplusb)==sm
            assert sa.intersection(sla)==sa
            assert sb.intersection(slb)==sb
            assert sc.intersection(sla)==sc
            assert sd.intersection(slb)==sd
            splitted_union=sa.union(sc).union(sm).union(sb).union(sd)
            superf=splitted_union.difference(sla.union(slb))
            assert len(superf)==0
            #Note that not all points in la and lb are in m+a+b+c+d.            
        except Exception,cause:
            print "Oops:",cause
            if have_split:
                visualize_diff([
                                (la,(255,0,0)),
                                (lb,(0,255,0)),
                                ],[])
                visualize_diff([
                                (la,(255,0,0)),
                                (lb,(0,255,0)),
                                (middle,(0,0,255))
                                ],sqs)
                
                visualize_diff([
                                (a,(255,0,0)),
                                (b,(128,0,0)),
                                (c,(0,0,255)),
                                (d,(0,0,128)),
                                (middle,(0,255,0))
                                ],[])
            else:
                print "Visualize two lines"
                #raise
                visualize_diff([
                                (la,(255,0,0)),
                                (lb,(0,255,0))
                                ])
            raise
def test_side_of1():
    l=Line(Vertex(0,0),Vertex(10,0))    
    assert l.side_of_extrapolated_line(Vertex(5,5))==-1
    assert l.side_of_extrapolated_line(Vertex(5,-5))==1
    assert l.side_of_extrapolated_line(Vertex(15,5))==-1
    assert l.side_of_extrapolated_line(Vertex(15,-5))==1
    assert l.side_of_extrapolated_line(Vertex(-5,5))==-1
    assert l.side_of_extrapolated_line(Vertex(-5,-5))==1
    assert l.side_of_extrapolated_line(Vertex(-5,0))==0
    assert l.side_of_extrapolated_line(Vertex(5,0))==0
    assert l.side_of_extrapolated_line(Vertex(15,0))==0
    r=Line(Vertex(10,0),Vertex(0,0))    
    assert r.side_of_extrapolated_line(Vertex(5,5))==1
    assert r.side_of_extrapolated_line(Vertex(5,-5))==-1
    
def test_side_of2():    
    l=Line(Vertex(0,0),Vertex(1,10))
    assert l.side_of_extrapolated_line(Vertex(0,10))==-1
    assert l.side_of_extrapolated_line(Vertex(1,0))==1
    assert l.side_of_extrapolated_line(Vertex(1,8))==0
    assert l.side_of_extrapolated_line(Vertex(0,2))==0

def test_side_of3():    
    l=Line(Vertex(0,0),Vertex(1,1000000))
    assert l.side_of_extrapolated_line(Vertex(1000000,0))==1
    assert l.side_of_extrapolated_line(Vertex(-1000000,0))==-1
    assert l.side_of_extrapolated_line(Vertex(0,100000))==0
    assert l.side_of_extrapolated_line(Vertex(1000000,2000000))==1

def test_side_of4():
    l=Line(Vertex(0,0),Vertex(0,10))    
    assert l.side_of_extrapolated_line(Vertex(5,5))==1
    assert l.side_of_extrapolated_line(Vertex(-5,5))==-1
    assert l.side_of_extrapolated_line(Vertex(0,5))==0
    r=Line(Vertex(0,10),Vertex(0,0))    
    assert r.side_of_extrapolated_line(Vertex(5,5))==-1
    assert r.side_of_extrapolated_line(Vertex(-5,5))==1
    assert r.side_of_extrapolated_line(Vertex(0,5))==0

def test_line_closest():
    l=Line(Vertex(0,0),Vertex(10,0))    
    assert l.approx_closest(Vertex(5,0))==Vertex(5,0)
    assert l.approx_closest(Vertex(-5,0))==Vertex(0,0)
    assert l.approx_closest(Vertex(15,0))==Vertex(10,0)
    assert l.approx_closest(Vertex(5,5))==Vertex(5,0)
    assert abs(l.approx_dist(Vertex(5,5))-5)<1e-5
    
    l=Line(Vertex(1127808,639680),Vertex(1120704,642752))
    p=Vertex(1141685,587443)
    dist=l.approx_dist(p)
    print "Dist: ",dist
    assert abs(dist)<1141685
    

