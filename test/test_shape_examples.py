from test_boolean_op import *

def test_verify_shape1():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCC
D         B
D         B
D         B
D         B
D         B
AAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output
    
def test_verify_shape2():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCCCC
D           B
D           B
D           B
D           B
D           B
AAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape3():
    input1=pyshape("leftbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
DCCCCCCCCCCCCC
D            B
D            B
D            B
D            B
D            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_poly_cleanup():
    input1=pyshape("leftbox",\
"""
     DCC
     D  CC 
     D    CCCC    
     D       C
AAAAAXBBBBBBBC
G    E
G    E
GFFFFE
""",mapmulti=dict(X=['A','D']))    
    #print asciart(input1)
    #print input1
    
    real_output=tidy_up_polygons(input1)
    real_str=asciart(real_output)
    facit=\
"""                   
      F
      HF
      H FF
      H   FF
 AEEEEHGGGGGGG
 A    D
 A   C
 BBBBC
"""
    print "real:\n",(real_str)
    print "facit:\n",(facit)
    print "real: ",repr(real_str.strip())
    print "facit:",repr(facit.strip())
    assert real_str.strip()==facit.strip() 
    


def test_verify_shape4():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape5():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCCCCCC
   D         B
   D         B
   D         B
   D         B
   D         B
   D         B
   D         B
   AAAAAAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
   HGGGGGGGGGG
   H         F
JIII         FEEEEEE
J                  D
J                  D
J            CCCCCCD
J            B
J            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape6():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCC
   D     B
   D     B
   AAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    assert should_output==real_output


def test_verify_shape7():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""
   DCCCCCC
   D     B
   D     B
   AAAAAAB
""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print asciart(real_output)
    assert should_output==real_output


def test_verify_shape8():
    input1=pyshape("leftbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    input2=pyshape("rightbox",\
"""

   DCCCCCC
   D     B
   D     B
   AAAAAAB

""")
    
    should_output=pyshape("resultbox",\
"""
FEEEEEEEEEEEEEEEEEEE
F                  D
F                  D
F            CCCCCCD
F            B
F            B
AAAAAAAAAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print "---------------------------------------------"
    print asciart(real_output)
    print "---------------------------------------------"
    assert should_output==real_output
        
        

def test_verify_shape9():
    input1=pyshape("leftbox",\
"""

 DCCCC
 D   B
 AAAAB
""")
    input2=pyshape("rightbox",\
"""
DCCCCCC
D     B
D     B
D     B
AAAAAAB
""")        
    should_output=pyshape("resultbox",\
"""
DCCCCCC
D     B
D     B
D     B
AAAAAAB
""")
    real_output=merge_shapes(input1,input2,False)    
    print asciart(real_output)
    assert should_output==real_output
    
    
def test_verify_shape10():
    input1=pyshape("leftbox",(
"""
DCCCCCCCC
D       B
D       B
D       B
D       B
D       B
AAAAAAAAB
""","solid"),
                   (
"""


    DCCCCCC
    D     B
    AAAAAAB

""","hole"))
    input2=pyshape("rightbox",\
"""
DCCCCCCC
D      B
D      B
D      B
D      B
D      B
AAAAAAAB
""")        
    should_output=pyshape("resultbox",\
"""
AHHHHHHHH
A       G
A       G
A      FG
A      E
A      ED
BBBBBBBBC
""")
    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    

    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    print "============================"
    print asciart(should_output)
    print "============================"
    assert real_output==should_output
    

def test_verify_shape11():
    input1=pyshape("leftbox",(
"""
DCCCCCCCC
D       B
D       B
D       B
D       B
D       B
AAAAAAAAB
"""),(
"""


  DCCCC
  D   B
  AAAAB


""","hole")
)
    
    input2=pyshape("rightbox",\
"""


    DCCCCC
    D    B
    AAAAAB                      


""")        
    should="""
 AHHHHHHHH
 A       G
 A ILL   GF
 A I K    E
 A JJK   DE
 A       C
 BBBBBBBBC    
"""    

    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert asci.strip()==should.strip()



def test_verify_shape12():
    input1=pyshape("leftbox",(
"""
DCCCCCCCCCCCC
D           B
D           B
D           B
D           B
D           B
D           B
D           B
D           B
D           B
AAAAAAAAAAAAB
"""),(
"""




  DCCCCCCC
  D      B
  D      B
  D      B
  D      B
  D      B
  AAAAAAAB
 

""","hole"),(
"""



    DCC
    D B
    AAB



""")
)
    
    input2=pyshape("rightbox",(
"""


    DCCCCCCCCC
    D        B
    AAAAAAAAAB                      


""","hole"))     
    should="""
 ADDDDDDDDDDDD
 A           C
 A EHHHHHHH  C
 A E      G  C
 A E      G  C
 A E LKK  G  C
 A E L J  G  C
 A E IIJ  G  C
 A FFFFFFFG  C
 A           C
 BBBBBBBBBBBBC
"""    

    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert asci.strip()==should.strip()
    
def test_verify_shape13():
    input1=pyshape("leftbox",(
"""
DCCCCCCCCCCCC
D           B
D           B
D           B
D           B
D           B
D           B
D           B
D           B
D           B
AAAAAAAAAAAAB
"""),(
"""




  DCCCCCCC
  D      B
  D      B
  D      B
  D      B
  D      B
  AAAAAAAB
 

""","hole"),(
"""



    DCC
    D B
    AAB



""")
)
    
    input2=pyshape("rightbox",(
"""
  DCCCCCCCCC
  D        B
  AAAAAAAAAB                      
"""))
         
    should=""" 
 ADDDDDDDDDDDD
 A           C
 A EHHHHHHH  C
 A E      G  C
 A E      G  C
 A E ILL  G  C
 A E I K  G  C
 A E JJK  G  C
 A FFFFFFFG  C
 A           C
 BBBBBBBBBBBBC

"""    

    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert asci.strip()==should.strip()

def test_verify_shape14():
    input1=pyshape("leftbox",(
"""
DCCC
D  B
D  B
AAAB



"""),(
"""



   DCCC
   D  B
   D  B
   AAAB
""")
)
    
    input2=pyshape("rightbox",(
"""
   DCCC
   D  B
   D  B
   AAAB



"""),(
"""



DCCC
D  B
D  B
AAAB
""")
)         
    should=pyshape("result",\
""" 
DCCCCCC
D     B
D     B
D     B
D     B
D     B
AAAAAAB
""")    

    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert real_output==should

def test_verify_shape15():
    input1=pyshape("leftbox",(
"""

 DCCC
 D  B
 D  B
 AAAB




""","hole"),(
"""




    DCCC
    D  B
    D  B
    AAAB

""","hole"),(
"""
ADDDDDDDDD
A        C
A        C
A        C
A        C
A        C
A        C
A        C
BBBBBBBBBC
""")
)
    
    input2=pyshape("rightbox",(
"""

       DCCC
       D  B
       D  B
       AAAB




"""),(
"""




 DCCC
 D  B
 D  B
 AAAB

""")
)         
    should=""" 
 AHHHHHHHHH
 ALLLL    GF
 AI  K     E
 AI  K     E
 AJJJPPPP DE
 A   M  O C
 A   M  O C
 A   NNNO C
 BBBBBBBBBC
"""    

    print "Merging shapes: ",input1,input2
    real_output=merge_shapes(input1,input2,False)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert asci.strip()==should.strip()

def test_verify_shape15():
    input1=pyshape("leftbox",(
"""
DC
AB
""",
"""
 DC
 AB
"""
))
    input2=pyshape("rightbox",(
"""
ADD
BBC
"""))      
    should=pyshape("result",\
""" 
ADD
BBC
""")
    real_output=shape_union(input1,input2)    
    asci=asciart(real_output)
    print "============================"
    print asci
    print "============================"
    assert real_output==should
