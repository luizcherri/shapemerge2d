from pyshapemerge2d import Line2,Vertex,Vector,BooleanOp,Polygon,vvector,Shape,lvector

from visualize import draw_things
import visualize
from math import sin,cos
vorig=Vertex
def test_edge_sort():
    b=BooleanOp()
    v=vorig(0,0)
    for x in xrange(100):
        if x==0:
            ang=0
        else:
            ang=x/6.0
        def Vertex(a,b):
            x=a*cos(ang)-b*sin(ang)
            y=b*cos(ang)+a*sin(ang)
            return vorig(int(x),int(y))
        for side in [0,1]: 
            l=Line2(Vertex(-10,0),Vertex(0,0))
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,0),Vertex(0,10)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,0),Vertex(0,10))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(0,0),Vertex(-10,10)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(0,0),Vertex(-10,10))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                
            tosort=lvector([
                    Line2(Vertex(-10,10),Vertex(0,0)),
                    Line2(Vertex(0,10),Vertex(0,0)),
                    Line2(Vertex(0,0),Vertex(10,10))])
            out=list(b.dbg_step5_sort_edges(v,l,tosort,side))
            if side==0:
                assert out[0]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[2]==Line2(Vertex(0,0),Vertex(10,10))
            else:
                assert out[2]==Line2(Vertex(-10,10),Vertex(0,0))
                assert out[1]==Line2(Vertex(0,10),Vertex(0,0))
                assert out[0]==Line2(Vertex(0,0),Vertex(10,10))
                

class NondirLine(object):
    def __init__(self,v1,v2):
        self.v1,self.v2=v1,v2
    def __hash__(self):
        return self.v1.get_x()+self.v2.get_x()+\
                self.v1.get_y()+self.v2.get_y()
    def __eq__(self,o):
        if self.v1==o.v1 and self.v2==o.v2:
            return True
        if self.v1==o.v2 and self.v2==o.v1:
            return True
        return False
    def get_v1(self):
        return self.v1
    def get_v2(self):
        return self.v2
    def __repr__(self):
        return "NondirLine(%s,%s)"%(self.v1,self.v2)
    
def test_add_lines():
    #print "Press any key when debugger ready"
    #raw_input()
    bo=BooleanOp()
    poly_a=Polygon(vvector([
        Vertex(0,0),Vertex(2,0),
        Vertex(2,2),Vertex(0,2)]))
    poly_b=Polygon(vvector([
        Vertex(1,1),Vertex(3,1),
        Vertex(3,3),Vertex(1,3)]))
    shape_a=Shape("shape_a",poly_a)
    shape_b=Shape("shape_b",poly_b)
    print "Shape_a,shape_b: ",shape_a,shape_b
    bo.step1_add_lines(shape_a,shape_b)
    bo.step2_intersect_lines()
    lines=list(bo.dbg_step2_get_split_lines())
    print "The found, split lines:"
    print "\n".join(str(x) for x in lines)
    assert len(lines)==12
    assert Line2(Vertex(0,0),Vertex(2,0)) in lines
    assert Line2(Vertex(2,0),Vertex(2,1)) in lines
    assert Line2(Vertex(2,1),Vertex(2,2)) in lines
    assert Line2(Vertex(2,2),Vertex(1,2)) in lines
    assert Line2(Vertex(1,2),Vertex(0,2)) in lines
    assert Line2(Vertex(0,2),Vertex(0,0)) in lines
    assert Line2(Vertex(1,1),Vertex(2,1)) in lines
    assert Line2(Vertex(2,1),Vertex(3,1)) in lines    
    assert Line2(Vertex(3,1),Vertex(3,3)) in lines
    assert Line2(Vertex(3,3),Vertex(1,3)) in lines
    assert Line2(Vertex(1,3),Vertex(1,2)) in lines
    assert Line2(Vertex(1,2),Vertex(1,1)) in lines
    
    bo.step3_create_edges()
    edges=bo.dbg_step3_and_4_get_edges()    
    edgelines=dict((Line2(edge.get_v1(),edge.get_v2()),edge) for edge in edges)
    
    assert set(edgelines.keys())==set(lines) 
    for key,edge in edgelines.items():
        if key.get_v1().get_y()==key.get_v2().get_y():
            assert edge.get_k_approx()==0.0
            assert edge.get_is_vertical()==False
        else:
            #vertical
            assert edge.get_k_approx()==0.0 #also for vertical but:
            assert edge.get_is_vertical()==True
    
    bo.step4_eliminate_deadends()
    edges=bo.dbg_step3_and_4_get_edges()    
    edgelines=dict((Line2(edge.get_v1(),edge.get_v2()),edge) for edge in edges)
    
    assert set(edgelines.keys())==set(lines) 
    for key,edge in edgelines.items():
        if key.get_v1().get_y()==key.get_v2().get_y():
            assert edge.get_k_approx()==0.0
            assert edge.get_is_vertical()==False
        else:
            #vertical
            assert edge.get_k_approx()==0.0 #also for vertical but:
            assert edge.get_is_vertical()==True

            
    bo.step5_create_cells()
    cells=list(bo.dbg_step5_get_cells())
    print "Cells:\n","\n".join(str(x) for x in cells)
    def lineset(*vertices):
        s=set()
        for v1,v2 in zip(vertices,vertices[1:]+vertices[:1]):
            s.add(NondirLine(v1,v2))
        return s
            
    facit_cells=[
                 lineset(Vertex(0,0),Vertex(2,0),Vertex(2,1),Vertex(3,1),
                         Vertex(3,3),Vertex(1,3),Vertex(1,2),Vertex(0,2)),
                 lineset(Vertex(1,1),Vertex(2,1),Vertex(2,2),Vertex(1,2)),
                 lineset(Vertex(2,1),Vertex(3,1),Vertex(3,3),Vertex(1,3),
                         Vertex(1,2),Vertex(2,2)),
                 lineset(Vertex(0,0),Vertex(2,0),Vertex(2,1),Vertex(1,1),
                         Vertex(1,2),Vertex(0,2))
                 ]
    print "Facit cells","\n".join(str(x) for x in facit_cells)
    assert len(cells)==4
    
    for cell in cells:
        edges=cell.dbg_get_edges()
        edgelines=set(NondirLine(edge.get_v1(),edge.get_v2()) for edge in edges)
        
        if not (edgelines in facit_cells):
            for facitnr in xrange(len(facit_cells)):
                print "============================================="
                print "Comparing against facit #%d"%(facitnr,)
                facit=facit_cells[facitnr]
                print "Extra:",edgelines.difference(facit)
                print "Missing:",facit.difference(edgelines)
            
            
            sqs=[]
            for line in edgelines:
                sqs.append(visualize.Line(
                    line.get_v1().get_x(),
                    line.get_v1().get_y(),
                    line.get_v2().get_x(),
                    line.get_v2().get_y(),                       
                    (255,0,0)))
            draw_things(sqs)
    bo.step6_determine_cell_cover()
    