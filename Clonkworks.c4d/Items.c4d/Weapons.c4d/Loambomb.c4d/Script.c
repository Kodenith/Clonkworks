/*--- Flint ---*/

#strict

public func ExplodeSize() 
{ 
  return(18); 
}

protected func Hit()
{
  LoamFill(ExplodeSize(),GetX(),GetY(),"Earth-Smooth");
}

func LoamFill(Size,x,y,Mat){
	//for now it just uses the function below, later it will ingore all solid and liquid materials
	LoamCircle(ExplodeSize(),GetX(),GetY());
	
	CastParticles("FSpark", 5,50, 0,0, 50, 200, RGBa(179,116,54,177), RGBa(179,116,54,177));
	CastParticles("FSpark", 5,50, 0,0, 50, 200, RGBa(97,59,23,0), RGBa(97,59,23,177));
	Sound("Loambomb*");
	RemoveObject(this());
}

global func DrawMaterialCircle(string szMaterial,int iX,int iY,int iRad,bool sub)
{
for(var x=iX-iRad;x<iX+iRad;++x)
for(var y=iY-iRad;y<iY+iRad;++y)
if((x-iX)**2 + (y-iY)**2 < iRad**2)
DrawMaterialQuad(szMaterial,x,y,x+1,y,x+1,y+1,x,y+1,sub);
return true;
}

public func LoamCircle(int strength, int iX, int iY)
{
  var spray = [];
  var ObjInArea = [];
  //drawing circle, not overriding any other material besides walls and liquid
  for(var y = iY-strength; y <= iY+strength; y++)
    for(var x = iX-strength; x <= iX+strength; x++)
      if(AbsX(x)**2 + AbsY(y)**2 <= strength**2)
      {
		if(GBackSolid(x-GetX(),y-GetY())) continue;
		if(GBackLiquid(x-GetX(),y-GetY())){
			ArrayAdd(spray,MaterialName(GetMaterial(x-GetX(),y-GetY())),false);
		}
		DrawMaterialQuad("Earth",x,y,x,y+1,x,y,x,y,1);
		
		for(var obj in FindObjects(Find_Exclude(this()),Find_NoContainer(),Find_Category(C4D_Object),Find_AtPoint(x-GetX(),y-GetY()))){
			ArrayAdd(ObjInArea,obj,true);
		}
      }
	  
  //replacing all lost liquid	  
  for(var i in spray){
	  //DebugLog(i);
	  CastPXS(i,1,10,0,-(ExplodeSize()+1));
  }
  
  //pushing away objects
  for(var j in ObjInArea){
	  while(GBackSolid(AbsX(GetX(j)),AbsY(GetY(j))) || ObjectDistance(j,this()) <= ExplodeSize()){
		  if(ObjectDistance(j,this()) > ExplodeSize()) break;
		  var RotRef = Angle(GetX(j),GetY(j),GetX(),GetY());
		  RotRef= (RotRef + 180) % 360 - 180;
		  var iXDir = Sin(RotRef, 1);
		  var iYDir = -Cos(RotRef, 1);
		  SetX(GetX(j)-iXDir,j);
		  SetY(GetY(j)-iYDir,j);
	  }
  }
	
  return true;
}

/* Kann in der Chemiefabrik hergestellt werden */
public func IsChemicalProduct() { return(1); }