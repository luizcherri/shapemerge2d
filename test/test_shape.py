from pyshapemerge2d import Shape,Polygon,Vertex,vvector,pvector

def test_shape_eq(): 
    assert 0
    polya1=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    polya2=Polygon(vvector([
        Vertex(2,0),Vertex(3,0),Vertex(3,1),Vertex(2,1)]))
    polyb1=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    polyb2=Polygon(vvector([
        Vertex(2,1),Vertex(2,0),Vertex(3,0),Vertex(3,1)]))
    polyc1=Polygon(vvector([
        Vertex(0,0),Vertex(1,0),Vertex(1,1),Vertex(0,1)]))
    polyc2=Polygon(vvector([
        Vertex(2,1),Vertex(2,0),Vertex(4,0),Vertex(3,1)]))
    
    shape_a=Shape("a",pvector([polya1,polya2]))
    shape_b=Shape("b",pvector([polyb1,polyb2]))
    shape_c=Shape("c",pvector([polyc1,polyc2]))

    assert shape_a==shape_b
    assert not (shape_a==shape_c)
    
