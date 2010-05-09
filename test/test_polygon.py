from pyshapemerge2d import Polygon,Vertex,vvector,Line2

def test_poly_eq():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    polyb=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    polyc=Polygon(vvector([
        Vertex(0,1),Vertex(0,0),Vertex(1,0),Vertex(1,1)]))
    polyd=Polygon(vvector([
        Vertex(0,1),Vertex(0,0),Vertex(1,0),Vertex(1,2)]))
    assert polya==polyb
    assert polya==polyc
    assert not (polya==polyd)
    
def test_is_ccw():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    assert polya.is_ccw()
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(0,1),Vertex(1,1),Vertex(1,0)]))
    assert not polya.is_ccw()

def test_merge_straights():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(5,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    polya.merge_straight_sections()
    assert polya==Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))

def test_remove_loops1():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    polyb=polya.remove_loops()
    print polyb
    assert polyb==Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
        
def test_remove_loops2():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(10,15),Vertex(15,15),Vertex(15,10),Vertex(10,10),Vertex(0,10)]))
    polyb=polya.remove_loops()
    print polyb
    assert polyb==Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
            
def test_remove_loops3():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(10,15),Vertex(15,15),Vertex(15,10),Vertex(0,10)]))
    polyb=polya.remove_loops()
    print polyb
    assert polyb==Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
            
def test_remove_loops4():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,15),Vertex(15,15),Vertex(15,10),Vertex(0,10)]))
    polyb=polya.remove_loops()
    print polyb
    assert polyb==Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
            
def test_remove_loops5():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(80,0),Vertex(100,0),Vertex(80,0),Vertex(0,2)]))
    polyb=polya.remove_loops()
    assert polyb==Polygon(vvector([
        Vertex(0,0),Vertex(60,0),Vertex(0,2)]))
            
def test_remove_loops6():
    polya=Polygon(vvector([
        Vertex(15,15),Vertex(15,10),Vertex(0,10),Vertex(0,0),Vertex(10,0),Vertex(10,15)]))
    polyb=polya.remove_loops()
    print polyb
    assert polyb==Polygon(vvector([
        Vertex(10,10),Vertex(10,15),Vertex(15,15),Vertex(15,10)]))

def test_inside_poly1():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    assert not polya.is_inside(Vertex(5,11))
    
    assert polya.is_inside(Vertex(5,5))
    assert polya.is_inside(Vertex(0,5))
    assert polya.is_inside(Vertex(5,10))
    assert not polya.is_inside(Vertex(-1,5))
    assert not polya.is_inside(Vertex(11,5))
def test_inside_poly2():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(0,5),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    assert polya.is_inside(Vertex(5,5))
def test_inside_poly3():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(5,5),Vertex(0,10)]))
    assert polya.is_inside(Vertex(7,5))
    assert polya.is_inside(Vertex(5,5)) #on edge
    assert polya.is_inside(Vertex(3,5))
    assert not polya.is_inside(Vertex(5,7))
    
def test_polygon_intersect_line1():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,5),Vertex(15,5))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(10,5))]
def test_polygon_intersect_line2():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,0),Vertex(15,0))))
    print ls    
    assert ls==[Line2(Vertex(0,0),Vertex(10,0))]
def test_polygon_intersect_line3():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(5,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,0),Vertex(15,0))))
    print ls
    assert ls==[Line2(Vertex(0,0),Vertex(10,0))]
def test_polygon_intersect_line4():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(5,0),Vertex(10,0),Vertex(5,0),Vertex(1,0),Vertex(0,1)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,0),Vertex(15,0))))
    print ls
    assert ls==[Line2(Vertex(0,0),Vertex(10,0))]
def test_polygon_intersect_line5():
    polya=Polygon(vvector([
        Vertex(0,-1),Vertex(10,0),Vertex(5,0),Vertex(0,0)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,0),Vertex(15,0))))
    print ls
    assert ls==[Line2(Vertex(0,0),Vertex(10,0))]

def test_polygon_intersect_line6():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(15,5),Vertex(-5,5))))
    print ls
    assert ls==[Line2(Vertex(10,5),Vertex(0,5))]

def test_polygon_intersect_line7():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(5,5),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,7),Vertex(15,7))))
    print ls
    assert ls==[Line2(Vertex(0,7),Vertex(4,7)),
                Line2(Vertex(6,7),Vertex(10,7))]
                
def test_polygon_intersect_line8():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(5,5),Vertex(15,5))))
    print ls
    assert ls==[Line2(Vertex(5,5),Vertex(10,5))]
                
def test_polygon_intersect_line9():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,5),Vertex(5,5))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(5,5))]

def test_polygon_intersect_line10():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(2,5),Vertex(8,5))))
    print ls
    assert ls==[Line2(Vertex(2,5),Vertex(8,5))]
                
def test_polygon_intersect_line11():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(2,15),Vertex(8,15))))
    print ls
    assert ls==[]

def test_polygon_intersect_line12():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(0,5),Vertex(10,5))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(10,5))]

def test_polygon_intersect_line13():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(5,15),Vertex(-5,5))))
    print ls
    assert ls==[Line2(Vertex(0,10),Vertex(0,10))]

def test_polygon_intersect_line14():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(10,15),Vertex(10,-5))))
    print ls
    assert ls==[Line2(Vertex(10,10),Vertex(10,0))]

def test_polygon_intersect_line15():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(10,-15),Vertex(10,25))))
    print ls
    assert ls==[Line2(Vertex(10,0),Vertex(10,10))]

def test_polygon_intersect_line16():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(10,-15),Vertex(10,5))))
    print ls
    assert ls==[Line2(Vertex(10,0),Vertex(10,5))]
def test_polygon_intersect_line17():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(10,7),Vertex(10,5))))
    print ls
    assert ls==[Line2(Vertex(10,7),Vertex(10,5))]
def test_polygon_intersect_line18():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(10,10),Vertex(15,15))))
    print ls
    assert ls==[Line2(Vertex(10,10),Vertex(10,10))]

def test_polygon_intersect_line19():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(5,5),Vertex(15,15))))
    print ls
    assert ls==[Line2(Vertex(5,5),Vertex(10,10))]

def test_polygon_intersect_line20():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,5),Vertex(5,5),Vertex(5,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5,5),Vertex(15,5))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(10,5))]
    
def test_polygon_intersect_line21():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,5),Vertex(5,5),Vertex(5,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(0,5),Vertex(10,4))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(10,4))]
def test_polygon_intersect_line22():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,5),Vertex(5,5),Vertex(5,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(0,5),Vertex(10,6))))
    print ls
    assert ls==[Line2(Vertex(0,5),Vertex(5,6))]

def test_polygon_intersect_line22():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10,0),Vertex(10,5),Vertex(5,5),Vertex(5,10),Vertex(0,10)]))
    ls=list(polya.intersect_line(Line2(Vertex(2,7),Vertex(2,7))))
    print ls
    assert ls==[Line2(Vertex(2,7),Vertex(2,7))]
    
def test_polygon_intersect_line23():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10000,0),Vertex(10000,10000),Vertex(0,10000)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5000,5000),Vertex(15000,5000))))
    print ls
    assert ls==[Line2(Vertex(0,5000),Vertex(10000,5000))]
def test_polygon_intersect_line23():
    polya=Polygon(vvector([
        Vertex(0,0),Vertex(10000000,0),Vertex(10000000,10000000),Vertex(0,10000000)]))
    ls=list(polya.intersect_line(Line2(Vertex(-5000000,5000000),Vertex(15000000,5000000))))
    print ls
    assert ls==[Line2(Vertex(0,5000000),Vertex(10000000,5000000))]

    
    
    
    

