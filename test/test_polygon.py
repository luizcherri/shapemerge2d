from pyshapemerge2d import Polygon,Vertex,vvector

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
        Vertex(10,10),Vertex(15,10),Vertex(15,15),Vertex(10,15)]))

