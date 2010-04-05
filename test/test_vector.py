from pyshapemerge2d import Vector


def test_add():
	sum=Vector(1,2)+Vector(2,3)
	print sum
	assert sum==Vector(3,5)
	
def test_taxilen():
	assert Vector(3,1).maxnorm()==3
	assert Vector(3,3).maxnorm()==3
	assert Vector(3,4).maxnorm()==4
	assert Vector(1,1).maxnorm()==1
	assert Vector(0,1).maxnorm()==1
	assert Vector(1,0).maxnorm()==1
	assert Vector(0,0).maxnorm()==0
	assert Vector(-3,1).maxnorm()==3
	assert Vector(3,-3).maxnorm()==3
	assert Vector(3,-4).maxnorm()==4
	assert Vector(-1,-1).maxnorm()==1
	assert Vector(0,1).maxnorm()==1
	assert Vector(-1,0).maxnorm()==1
	assert Vector(0,0).maxnorm()==0
	